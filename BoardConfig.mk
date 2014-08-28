USE_CAMERA_STUB := true

# inherit from the proprietary version
-include vendor/zte/enterprise_U950/BoardConfigVendor.mk

#Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR ?= device/zte/enterprise_U950/bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true


#ARCH
TARGET_NO_RADIOIMAGE := true
TARGET_BOOTLOADER_BOARD_NAME := enterprise_U950
TARGET_NO_BOOTLOADER := true
TARGET_BOARD_PLATFORM := tegra
TARGET_TEGRA_VERSION := t30

TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := true
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_ARCH_VARIANT_CPU := cortex-a9
TARGET_CPU_VARIANT := cortex-a9

BOARD_KERNEL_CMDLINE := 
BOARD_KERNEL_BASE := 0x10000000
BOARD_KERNEL_PAGESIZE := 2048

# fix this up by examining /proc/mtd on a running device
TARGET_USERIMAGES_USE_EXT4 := true
BOARD_BOOTIMAGE_PARTITION_SIZE := 0x00800000
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 0x00800000
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 536870912
BOARD_USERDATAIMAGE_PARTITION_SIZE := 1832910848
BOARD_FLASH_BLOCK_SIZE := 1024

TARGET_PROVIDES_INIT_TARGET_RC := true
TARGET_PREBUILT_KERNEL := device/zte/enterprise_U950/kernel

BOARD_HAS_NO_SELECT_BUTTON := true

TARGET_SPECIFIC_HEADER_PATH := device/zte/enterprise_U950/include

#WIFI
#BOARD_WPA_SUPPLICANT_DRIVER := NL80211
#WPA_SUPPLICANT_VERSION      := VER_0_8_X
#BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
#BOARD_HOSTAPD_DRIVER        := NL80211
#BOARD_HOSTAPD_PRIVATE_LIB   := lib_driver_cmd_bcmdhd
#BOARD_WLAN_DEVICE           := bcmdhd
#WIFI_DRIVER_MODULE_PATH     := "/system/lib/modules/bcmdhd.ko"
#WIFI_DRIVER_MODULE_NAME	:= bcmdhd
#WIFI_DRIVER_FW_PATH_PARAM   := "/sys/module/bcmdhd/parameters/firmware_path"
#WIFI_DRIVER_FW_PATH_STA := "/system/vendor/firmware/bcm4330/fw_bcmdhd.bin"
#WIFI_DRIVER_FW_PATH_P2P := "/system/vendor/firmware/bcm4330/fw_bcmdhd_p2p.bin"
#WIFI_DRIVER_FW_PATH_AP := "/system/vendor/firmware/bcm4330/fw_bcmdhd_apsta.bin"
#BOARD_LEGACY_NL80211_STA_EVENTS := true


BOARD_WLAN_DEVICE := bcmdhd
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_bcmdhd
WPA_SUPPLICANT_VERSION := VER_0_8_X
WIFI_DRIVER_MODULE_PATH := "/system/lib/modules/bcmdhd.ko"
WIFI_DRIVER_MODULE_NAME := "bcmdhd"
#WIFI_EXT_MODULE_PATH := "/system/lib/modules/cfg80211.ko"
#WIFI_EXT_MODULE_NAME := "cfg80211"
WIFI_DRIVER_FW_PATH_STA := "/system/vendor/firmware/bcm4330/fw_bcmdhd.bin"
#WIFI_DRIVER_FW_PATH_P2P := "/system/vendor/firmware/bcm4330//fw_bcmdhd_p2p.bin"
WIFI_DRIVER_FW_PATH_AP := "/system/vendor/firmware/bcm4330/fw_bcmdhd_apsta.bin"
WIFI_DRIVER_FW_PATH_PARAM := "/sys/module/bcmdhd/parameters/firmware_path"
#WIFI_DRIVER_MODULE_ARG := "fw_path: /system/vendor/firmware/bcm4330/fw_bcmdhd.bin nvram_path: /system/etc/nvram_4330.txt"
#BOARD_LEGACY_NL80211_STA_EVENTS := true
WIFI_DRIVER_MODULE_ARG      := "firmware_path=/system/vendor/firmware/bcm4330/fw_bcmdhd.bin nvram_path=/system/etc/nvram.txt iface_name=wlan0"


#egl
BOARD_EGL_CFG := device/zte/enterprise_U950/egl.cfg
USE_OPENGL_RENDERER := true
BOARD_EGL_WORKAROUND_BUG_10194508 := true
#TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true
BOARD_USE_MHEAP_SCREENSHOT := true
BOARD_EGL_SKIP_FIRST_DEQUEUE := true
BOARD_NEEDS_OLD_HWC_API := true
BOARD_EGL_NEEDS_LEGACY_FB := true

#rild
TARGET_PROVIDES_LIBRIL := vendor/zte/enterprise_U950/proprietary/lib/libztetd-ril.so
BOARD_RIL_NO_CELLINFOLIST := true

##audio
COMMON_GLOBAL_CFLAGS += -DMR0_AUDIO_BLOB -DMR0_CAMERA_BLOB -DNEEDS_VECTORIMPL_SYMBOLS
#BOARD_HAVE_PRE_KITKAT_AUDIO_BLOB := true
#BOARD_USES_GENERIC_AUDIO := false
#BOARD_USES_ALSA_AUDIO := true
#BOARD_USES_LEGACY_ALSA_AUDIO := true
USE_PROPRIETARY_AUDIO_EXTENSIONS := true
BOARD_USES_GENERIC_AUDIO := false
BOARD_USES_ALSA_AUDIO := false
BOARD_USES_TINY_AUDIO_HW := false
BOARD_HAVE_PRE_KITKAT_AUDIO_BLOB := true
BOARD_HAVE_PRE_KITKAT_AUDIO_POLICY_BLOB := true


BOARD_CHARGER_ENABLE_SUSPEND := true

# for old Recovery
USE_SET_METADATA := false

TARGET_RECOVERY_FSTAB := device/zte/enterprise_U950/ramdisk/fstab.tegra_enterprise

