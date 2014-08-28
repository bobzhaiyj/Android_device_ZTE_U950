## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := enterprise_U950

TARGET_SCREEN_HEIGHT := 800
TARGET_SCREEN_WIDTH := 480

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Enhanced NFC
$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)

# Inherit device configuration
$(call inherit-product, device/zte/enterprise_U950/device_enterprise_U950.mk)

TARGET_BOOTANIMATION_PRELOAD := true
TARGET_BOOTANIMATION_TEXTURE_CACHE := true

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := enterprise_U950
PRODUCT_NAME := cm_enterprise_U950
PRODUCT_BRAND := zte
PRODUCT_MODEL := ZTE U950
PRODUCT_MANUFACTURER := zte

# Enable Torch
PRODUCT_PACKAGES += Torch
