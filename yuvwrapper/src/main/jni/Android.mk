LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc

LOCAL_YUV_DIR_LIST :=$(wildcard $(LOCAL_PATH)/libyuv/source/*.cc)
LOCAL_YUV_FILES := $(LOCAL_YUV_DIR_LIST:$(LOCAL_PATH)/%=%)
$(warning $(LOCAL_YUV_FILES))
LOCAL_SRC_FILES := $(LOCAL_YUV_FILES)

# TODO(fbarchard): Enable mjpeg encoder.
#   source/mjpeg_decoder.cc
#   source/convert_jpeg.cc
#   source/mjpeg_validate.cc

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS += -DLIBYUV_NEON
    LOCAL_SRC_FILES += \
        source/compare_neon.cc.neon    \
        source/rotate_neon.cc.neon     \
        source/row_neon.cc.neon        \
        source/scale_neon.cc.neon
endif

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/libyuv/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libyuv/include

LOCAL_MODULE := libyuv
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)