LOCAL_PATH := $(call my-dir)
LOCAL_MAIN :=$(LOCAL_PATH)
include $(LOCAL_PATH)/x264/libs/Android.mk

include $(CLEAR_VARS)

LOCAL_PATH :=$(LOCAL_MAIN)

LOCAL_STATIC_LIBRARIES += libx264

LOCAL_MODULE := x264encoder

INCLUDE_YUV_PATH = $(LOCAL_PATH)/yuv

INCLUDE_X264_PATH = $(LOCAL_PATH)/x264/include \
                $(LOCAL_PATH)/x264/src \
                ./

LOCAL_C_INCLUDES := $(INCLUDE_YUV_PATH) \
                    $(INCLUDE_X264_PATH)

LOCAL_YUV_DIR_LIST :=$(wildcard $(LOCAL_PATH)/yuv/*.cpp)
LOCAL_YUV_FILES := $(LOCAL_YUV_DIR_LIST:$(LOCAL_PATH)/%=%)

LOCAL_X264_DIR_LIST := $(wildcard $(LOCAL_PATH)/x264/src/*.cpp)
LOCAL_X264_FILES := $(LOCAL_X264_DIR_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := $(LOCAL_YUV_FILES) \
                    $(LOCAL_X264_FILES)
LOCAL_SRC_FILES += jni_core_module.cpp \
                    jni_utils_module.cpp \
                    jni_callback_module.cpp \
                    jni_x264_module.cpp

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)