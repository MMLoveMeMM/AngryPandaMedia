LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MAIN :=$(LOCAL_PATH)
include $(LOCAL_PATH)/libyuv/libs/Android.mk

include $(CLEAR_VARS)

LOCAL_PATH :=$(LOCAL_MAIN)
LOCAL_STATIC_LIBRARIES += libyuv

LOCAL_CPP_EXTENSION := .cc .cpp

LOCAL_YUV_DIR_LIST :=$(wildcard $(LOCAL_PATH)/libyuv/src/*.cpp)
LOCAL_YUV_FILES := $(LOCAL_YUV_DIR_LIST:$(LOCAL_PATH)/%=%)

LOCAL_ORG_DIR_LIST :=$(wildcard $(LOCAL_PATH)/orgyuv/src/*.cpp)
LOCAL_ORG_FILES := $(LOCAL_ORG_DIR_LIST:$(LOCAL_PATH)/%=%)

LOCAL_JNI_DIR_LIST := jni_core_module.cpp \
                        jni_utils_module.cpp \
                        jni_yuv_module.cpp

LOCAL_SRC_FILES :=  $(LOCAL_YUV_FILES) \
                    $(LOCAL_ORG_FILES) \
                    $(LOCAL_JNI_DIR_LIST)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/libyuv/include \
                    $(LOCAL_PATH)/libyuv/include/libyuv \
                    $(LOCAL_PATH)/libyuv/src \
                    $(LOCAL_PATH)/orgyuv/src \
                    $(LOCAL_PATH)/orgyuv \
                    ./

LOCAL_MODULE := libusingyuv
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_LDFLAGS += -ljnigraphics

include $(BUILD_SHARED_LIBRARY)