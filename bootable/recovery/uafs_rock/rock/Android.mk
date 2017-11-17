
ROCK_GOTA_SUPPORT=yes

ifeq ($(ROCK_GOTA_SUPPORT),yes)
LOCAL_PATH := $(call my-dir)
# DMC Prebuild APK
# ============================================================
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := RockClient
#LINE <> <20150515> <updte the gata apk> panzaoyan
LOCAL_SRC_FILES := RockGota-micromax-01.03.03-release-noIcon.apk     
LOCAL_MODULE_CLASS := APPS
LOCAL_CERTIFICATE := platform
#WITH_DEXPREOPT := false
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)

include $(BUILD_PREBUILT)

endif

