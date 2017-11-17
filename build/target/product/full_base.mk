#
# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This is a build configuration for a full-featured build of the
# Open-Source part of the tree. It's geared toward a US-centric
# build of the emulator, but all those aspects can be overridden
# in inherited configurations.

PRODUCT_PACKAGES := \
    libfwdlockengine \
    WAPPushManager

    #wangyanhui delete 20150710	
    #OpenWnn \
    #libWnnEngDic \
    #libWnnJpnDic \
    #libwnndict \

PRODUCT_PACKAGES += \
    Galaxy4 \
    HoloSpiralWallpaper \
    LiveWallpapers \
    LiveWallpapersPicker \
    MagicSmokeWallpapers \
    NoiseField \
    PhaseBeam \
    VisualizationWallpapers \
    PhotoTable

# Additional settings used in all AOSP builds
ifeq (true,$(L9010_WIKO_HLOS))
PRODUCT_PROPERTY_OVERRIDES := \
    ro.com.android.dateformat=dd-MM-yyyy \
    ro.config.notification_sound=Tambourine.ogg \
    ro.config.alarm_alert=Stream.ogg \
    ro.config.ringtone=Dazzle.ogg
else
     ifeq (true,$(L5510_WIKO_HLOS))
      PRODUCT_PROPERTY_OVERRIDES := \
           ro.com.android.dateformat=dd-MM-yyyy \
           ro.config.notification_sound=Tambourine.ogg \
           ro.config.alarm_alert=Stream.ogg \
           ro.config.ringtone=Dazzle.ogg \
           ro.config.ringtone_2=Dazzle.ogg
     else	 
        ifeq (true,$(L5320_WIKO_HLOS))
      PRODUCT_PROPERTY_OVERRIDES := \
           ro.com.android.dateformat=dd-MM-yyyy \
           ro.config.notification_sound=Tambourine.ogg \
           ro.config.alarm_alert=Stream.ogg \
           ro.config.ringtone=Dazzle.ogg \
           ro.config.ringtone_2=Dazzle.ogg
        else	 
         PRODUCT_PROPERTY_OVERRIDES := \
            ro.com.android.dateformat=MM-dd-yyyy \
            ro.config.notification_sound=Graceful.ogg \
            ro.config.alarm_alert=Stream.ogg \
            ro.config.ringtone=Neon.ogg
        endif
    endif
endif


# Put en_US first in the list, so make it default.
PRODUCT_LOCALES := en_US

# Include drawables for all densities
PRODUCT_AAPT_CONFIG := normal hdpi xhdpi xxhdpi

# Get some sounds
#wangyanhui delete for oem audio res
#$(call inherit-product-if-exists, frameworks/base/data/sounds/AllAudio.mk)

# Get the TTS language packs
$(call inherit-product-if-exists, external/svox/pico/lang/all_pico_languages.mk)

# Get a list of languages.
$(call inherit-product, $(SRC_TARGET_DIR)/product/locales_full.mk)

# Get everything else from the parent package
$(call inherit-product, $(SRC_TARGET_DIR)/product/generic_no_telephony.mk)