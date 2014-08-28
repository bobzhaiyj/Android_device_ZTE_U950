/*
 * Copyright (C) 2012 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "lights"

#include <cutils/log.h>

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>


#include <sys/ioctl.h>
#include <sys/types.h>

#include <hardware/lights.h>

static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_notification;
static struct light_state_t g_battery;
static struct light_state_t g_buttons;
static int g_backlight = 255;

char const*const BUTTON_FILE = "/sys/devices/platform/leds_pwm/leds/keybl/brightness";

/* -1: normal
	0: close
  >=1: blink

	1,1  ==> green->yellow->red
	1,-1 ==> yellow->red
	1,0  ==> green->green

  When RED_BLINK_FILE=-1 && GREEN_BLINK_FILE=-1  ==> yellow!!!
*/
char const*const RED_BLINK_FILE = "/sys/class/timed_output/led_red_zte/enable";
char const*const GREEN_BLINK_FILE = "/sys/class/timed_output/led_green_zte/enable";

char const*const LCD_BACKLIGHT_FILE = "/sys/devices/platform/tegra-pwm-bl/backlight/tegra-pwm-bl/brightness";

enum {
  LED_RED,
  LED_GREEN,
  LED_YELLOW,
  LED_BLANK,
};

enum {
  PULSE_LENGTH_VERY_SHORT = 250,
  PULSE_LENGTH_SHORT = 500,
  PULSE_LENGTH_NORMAL = 1000,
  PULSE_LENGTH_LONG = 2500,
  PULSE_LENGTH_VERY_LONG = 5000,
  PULSE_LENGTH_ALWAYS_ON = 1,
};

enum {
  BLINK_MODE_OFF = 0,
  BLINK_MODE_VERY_SHORT = 250,
  BLINK_MODE_SHORT = 500,
  BLINK_MODE_NORMAL =  1000,
  BLINK_MODE_LONG = 2500,
  BLINK_MODE_VERY_LONG = 5000,
};

static int write_int(const char* path, int value) {
  int fd;
  int bytes, written;
  char buffer[20];
  static int already_warned = 0;

  fd = open(path, O_RDWR);
  if (fd < 0) {
    if (already_warned == 0) {
      ALOGE("write_int failed to open %s\n", path);
      already_warned = 1;
    }
    return -errno;
  }

  bytes = snprintf(buffer, sizeof(buffer), "%d\n",value);
  written = write(fd, buffer, bytes);
  close(fd);

  return written == -1 ? -errno : 0;
}

void init_globals(void) {
  pthread_mutex_init (&g_lock, NULL);
}

static int is_lit(struct light_state_t const* state) {
  return state->color & 0x00ffffff;
}

static void set_speaker_light_locked(struct light_device_t *dev,
                                     struct light_state_t *state) {
  unsigned int colorRGB = state->color & 0xFFFFFF;
  unsigned int color = LED_BLANK;
  unsigned int blinkMode = BLINK_MODE_OFF;

  ALOGE("set_led_state colorRGB=%08X, %s\n", colorRGB, __FUNCTION__);

  if ((colorRGB >> 8) & 0xFF) color = LED_GREEN;
  if ((colorRGB >> 16) & 0xFF) color = LED_RED;
  if (((colorRGB >> 8) & 0xFF) && ((colorRGB >> 16) & 0xFF)) {ALOGE("LED_YELLOW=%08X", colorRGB);color = LED_YELLOW;}

  if(state->flashOnMS >= PULSE_LENGTH_LONG || state->flashOffMS >= BLINK_MODE_LONG) {
      blinkMode = BLINK_MODE_LONG;
  }

  if(state->flashOnMS == PULSE_LENGTH_ALWAYS_ON) {
      state->flashMode = LIGHT_FLASH_NONE;
  }

  switch (state->flashMode) {
    case LIGHT_FLASH_TIMED:
      switch (color) {
        case LED_RED:
          write_int(RED_BLINK_FILE, 1);
          write_int(GREEN_BLINK_FILE, 0);
          break;
        case LED_GREEN:
          write_int(GREEN_BLINK_FILE, 1);
          write_int(RED_BLINK_FILE, 0);
          break;
        case LED_YELLOW:
          write_int(GREEN_BLINK_FILE, 3);
          write_int(RED_BLINK_FILE, 3);
          break;
        case LED_BLANK:
          write_int(RED_BLINK_FILE, 0);
          write_int(GREEN_BLINK_FILE, 0);
          break;
        default:
          ALOGE("set_led_state colorRGB=%08X, unknown color\n", colorRGB);
          break;
      }
      break;
    case LIGHT_FLASH_NONE:
      write_int(RED_BLINK_FILE, 0);
      write_int(GREEN_BLINK_FILE, 0);
      switch (color) {
        case LED_RED:
          write_int(RED_BLINK_FILE, -1);
          write_int(GREEN_BLINK_FILE, 0);
          break;
        case LED_GREEN:
          write_int(RED_BLINK_FILE, 0);
          write_int(GREEN_BLINK_FILE, -1);
          break;
		case LED_YELLOW:
          write_int(RED_BLINK_FILE, -1);
          write_int(GREEN_BLINK_FILE, -1);
		  break;
        case LED_BLANK:
          write_int(RED_BLINK_FILE, 0);
          write_int(GREEN_BLINK_FILE, 0);
          break;
      }
      break;
    default:
      ALOGE("set_led_state colorRGB=%08X, unknown mode %d\n",
            colorRGB, state->flashMode);
  }
}

static void set_speaker_light_locked_dual(struct light_device_t *dev,
                                          struct light_state_t *bstate,
                                          struct light_state_t *nstate) {
  unsigned int bcolorRGB = bstate->color & 0xFFFFFF;
  unsigned int bcolor = LED_BLANK;
  unsigned int blinkMode = BLINK_MODE_LONG;

  if ((bcolorRGB >> 8) & 0xFF) bcolor = LED_GREEN;
  if ((bcolorRGB >> 16) & 0xFF) bcolor = LED_RED;

  /* The kernel driver only supports one dual blink mode and does not distinct
     between background colors. It is activated by writing a 1 to the amber
     blink file and then a 3 to the amber blink file.
  */

  switch (bcolor) {
    case LED_RED:
    case LED_GREEN:
      write_int (RED_BLINK_FILE, 1);
      write_int (GREEN_BLINK_FILE, 1);
      break;
    default:
      ALOGE("set_led_state (dual) unexpected color: bcolorRGB=%08x\n", bcolorRGB);
  }
}

static int set_light_buttons_locked(struct light_device_t* dev,
                             struct light_state_t const* state) {
  int err = 0;
  int on = is_lit(state);

  err = write_int(BUTTON_FILE, on ? 255 : 0);

  return err;
}

static int set_light_buttons(struct light_device_t* dev,
                             struct light_state_t const* state) {
  int err;
  pthread_mutex_lock(&g_lock);
  err = set_light_buttons_locked(dev, state);
  pthread_mutex_unlock(&g_lock);

  return 0;
}

static void handle_speaker_battery_locked(struct light_device_t *dev) {

  if (is_lit(&g_battery) && is_lit(&g_notification)) {
    set_speaker_light_locked_dual(dev, &g_battery, &g_notification);
  } else if (is_lit (&g_battery)) {
    set_speaker_light_locked(dev, &g_battery);
  } else {
    set_speaker_light_locked(dev, &g_notification);
  }
}

static int rgb_to_brightness(struct light_state_t const* state)
{
  int color = state->color & 0x00ffffff;
  return ((77 * ((color >> 16) & 0x00ff)) + (150 * ((color >> 8) & 0x00ff)) +
          (29 * (color & 0x00ff))) >> 8;
}

static int set_light_backlight(struct light_device_t* dev,
                               struct light_state_t const* state) {
  int err = 0;
  int brightness = rgb_to_brightness(state);
  ALOGV("%s brightness=%d color=0x%08x", __func__,brightness, state->color);
  pthread_mutex_lock(&g_lock);
  g_backlight = brightness;
  err = write_int(LCD_BACKLIGHT_FILE, brightness);
  pthread_mutex_unlock(&g_lock);
  return err;
}

static int set_light_battery(struct light_device_t* dev,
                             struct light_state_t const* state) {
  pthread_mutex_lock(&g_lock);
  g_battery = *state;
  handle_speaker_battery_locked(dev);
  pthread_mutex_unlock(&g_lock);

  return 0;
}

static int set_light_attention(struct light_device_t* dev,
                               struct light_state_t const* state) {
  return 0;
}

static int set_light_notifications(struct light_device_t* dev,
                                   struct light_state_t const* state) {
  pthread_mutex_lock(&g_lock);
  g_notification = *state;
  handle_speaker_battery_locked(dev);
  pthread_mutex_unlock(&g_lock);

  return 0;
}

static int close_lights(struct light_device_t *dev) {
  if (dev)
    free (dev);

  return 0;
}

static int open_lights(const struct hw_module_t* module, char const* name,
                       struct hw_device_t** device) {
  int (*set_light)(struct light_device_t* dev,
  struct light_state_t const* state);
  struct light_device_t *dev;

  if (0 == strcmp(LIGHT_ID_BACKLIGHT, name)) {
    set_light = set_light_backlight;
  } else if (0 == strcmp(LIGHT_ID_BUTTONS, name)) {
    set_light = set_light_buttons;
  } else if (0 == strcmp(LIGHT_ID_BATTERY, name)) {
    set_light = set_light_battery;
  } else if (0 == strcmp(LIGHT_ID_ATTENTION, name)) {
    set_light = set_light_attention;
  } else if (0 == strcmp(LIGHT_ID_NOTIFICATIONS, name))  {
    set_light = set_light_notifications;
  } else {
	ALOGE("module method name=%s, %s skipped\n", name, __FUNCTION__);
    return -EINVAL;
  }

  pthread_once(&g_init, init_globals);
  dev = malloc(sizeof(struct light_device_t));
  memset(dev, 0, sizeof(struct light_device_t));

  dev->common.tag = HARDWARE_DEVICE_TAG;
  dev->common.version = 0;
  dev->common.module = (struct hw_module_t*) module;
  dev->common.close = (int (*)(struct hw_device_t*)) close_lights;
  dev->set_light = set_light;

  *device = (struct hw_device_t*) dev;
  return 0;

}

static struct hw_module_methods_t lights_module_methods = {
  .open = open_lights,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
  .tag = HARDWARE_MODULE_TAG,
  .version_major = 1,
  .version_minor = 0,
  .id = LIGHTS_HARDWARE_MODULE_ID,
  .name = "ZTE U5 Lights Module",
  .author = "Focus.Lau <focus.lau@gmail.com>",
  .methods = &lights_module_methods,
};