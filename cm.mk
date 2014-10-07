## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := enterprise_V985

TARGET_SCREEN_HEIGHT := 1280
TARGET_SCREEN_WIDTH := 720

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Enhanced NFC
$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)

# Inherit device configuration
$(call inherit-product, device/zte/enterprise_V985/device_enterprise_V985.mk)

TARGET_BOOTANIMATION_PRELOAD := true
TARGET_BOOTANIMATION_TEXTURE_CACHE := true

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := enterprise_V985
PRODUCT_NAME := cm_enterprise_V985
PRODUCT_BRAND := zte
PRODUCT_MODEL := ZTE V985
PRODUCT_MANUFACTURER := zte

# Enable Torch
PRODUCT_PACKAGES += Torch
