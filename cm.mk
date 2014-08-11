## Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := enterprise_U950

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/zte/enterprise_U950/device_enterprise_U950.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := enterprise_U950
PRODUCT_NAME := cm_enterprise_U950
PRODUCT_BRAND := zte
PRODUCT_MODEL := enterprise_U950
PRODUCT_MANUFACTURER := zte
