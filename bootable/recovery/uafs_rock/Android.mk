ROCK_GOTA_SUPPORT=yes

ifeq ($(ROCK_GOTA_SUPPORT),yes)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := uafs.cpp 
LOCAL_SRC_FILES += ../ui.cpp ../roots.cpp
#LOCAL_SRC_FILES += ../default_device.cpp
LOCAL_SRC_FILES += ../screen_ui.cpp
LOCAL_SRC_FILES += ../bootloader.cpp 



commands_uafs_local_path := $(LOCAL_PATH)

LOCAL_MODULE := uafs

LOCAL_FORCE_STATIC_EXECUTABLE := true

LOCAL_MODULE_TAGS := optional

LOCAL_STATIC_LIBRARIES:= 
LOCAL_STATIC_LIBRARIES += libminui libpng libpixelflinger_static liblog
LOCAL_STATIC_LIBRARIES += libfs_mgr liblog libselinux
LOCAL_STATIC_LIBRARIES += libcutils libflashutils_gmobi libimage_update_gmobi
LOCAL_STATIC_LIBRARIES += libuafs libbz libz
LOCAL_STATIC_LIBRARIES += libstdc++ libc libm libmtdutils



ifeq ($(DTARGET_BOARD_PLATFORM),rockchip)
	LOCAL_SRC_FILES += ../rkimage.cpp
	LOCAL_CFLAGS += -DTARGET_BOARD_PLATFORM=rockchip
	LOCAL_CFLAGS += -fpermissive
	LOCAL_STATIC_LIBRARIES += librk_emmcutils libmtdutils
endif


ifeq ($(TARGET_RECOVERY_UI_LIB),)
  LOCAL_SRC_FILES += ../default_device.cpp
else
  LOCAL_STATIC_LIBRARIES += $(TARGET_RECOVERY_UI_LIB)
endif

#libpixelflinger_static for x86 is using encoder under hardware/intel/apache-harmony
ifeq ($(TARGET_ARCH),x86)
LOCAL_STATIC_LIBRARIES += libenc
endif

LOCAL_C_INCLUDES := system/extras/ext4_utils kernel $(LOCAL_CUST_INC_PATH) $(LOCAL_PATH)/fota/include $(LOCAL_PATH)/fota mediatek/kernel/include/linux 
LOCAL_C_INCLUDES += system/core/fs_mgr/include
LOCAL_C_INCLUDES += mediatek/custom/$(TARGET_PRODUCT)/recovery/inc
LOCAL_C_INCLUDES += system/extras/ext4_utils bionic/libc/private
LOCAL_C_INCLUDES += bootable/recovery
LOCAL_C_INCLUDES += system/extras/ext4_utils/kernel-3.4
LOCAL_C_INCLUDES += system/extras/ext4_utils system/vold external/openssl/include
	               

include $(BUILD_EXECUTABLE)

include $(commands_uafs_local_path)/libuafs/Android.mk
include $(commands_uafs_local_path)/flashutils/Android.mk
include $(commands_uafs_local_path)/rock/Android.mk

commands_uafs_local_path := 
endif
