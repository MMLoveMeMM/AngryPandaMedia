LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MAIN :=$(LOCAL_PATH)

include $(LOCAL_MAIN)/ffmpeg/libs/Android.mk

include $(LOCAL_MAIN)/xlog/libs/Android.mk

include $(CLEAR_VARS)

LOCAL_PATH :=$(LOCAL_MAIN)
LOCAL_SHARED_LIBRARIES := avcodec avdevice avfilter avformat avutil postproc swresample swscale
LOCAL_SHARED_LIBRARIES += xlog #增加一个日志打印方式,cpp那边尽量不使用jni里面日志,保持和jni没有耦合性,方便跨平台

LOCAL_FFMPEG_DIR_LIST :=$(wildcard $(LOCAL_PATH)/ffmpeg/src/base/*.cpp)
LOCAL_FFMPEG_FILES := $(LOCAL_FFMPEG_DIR_LIST:$(LOCAL_PATH)/%=%)

LOCAL_FFMPEG_CODEC_DIR_LIST :=$(wildcard $(LOCAL_PATH)/ffmpeg/src/codec/*.cpp)
LOCAL_FFMPEG_CODEC_FILES := $(LOCAL_FFMPEG_CODEC_DIR_LIST:$(LOCAL_PATH)/%=%)

LOCAL_JNI_DIR_LIST := jni_core_module.cpp \
                        jni_utils_module.cpp \
                        jni_codec_module.cpp

LOCAL_SRC_FILES :=  $(LOCAL_FFMPEG_FILES) \
                    $(LOCAL_FFMPEG_CODEC_FILES) \
                    $(LOCAL_JNI_DIR_LIST)

LOCAL_C_INCLUDES +=  $(LOCAL_PATH)/ffmpeg/src/base \
                     $(LOCAL_PATH)/ffmpeg/src/codec \
                     $(LOCAL_PATH)/ffmpeg/include \
                     $(LOCAL_PATH)/ffmpeg \
                     $(LOCAL_PATH)/xlog/include \
                     ./

LOCAL_MODULE := libffmpeg
LOCAL_MODULE_TAGS := optional
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog -lz
LOCAL_LDLIBS += -latomic # 这个需要对应Application添加支持

include $(BUILD_SHARED_LIBRARY)