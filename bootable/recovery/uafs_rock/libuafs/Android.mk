ROCK_GOTA_SUPPORT=yes

ifeq ($(ROCK_GOTA_SUPPORT),yes)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libuafs
LOCAL_MODULE_SUFFIX := .a
#LOCAL_RAW_STATIC_LIBRARY: = true
LOCAL_SRC_FILES := libua_gmobi_v3.1.1.a
LOCAL_MODULE_CLASS := STATIC_LIBRARIES

include $(BUILD_PREBUILT)


endif
