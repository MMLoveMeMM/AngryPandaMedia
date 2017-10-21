LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := prebuilt/libavcodec-57.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := prebuilt/libavformat-57.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := prebuilt/libavutil-55.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := prebuilt/libswresample-2.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := prebuilt/libswscale-4.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avfilter
LOCAL_SRC_FILES := prebuilt/libavfilter-6.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := jni_core_module.cpp \
                    jni_utils_module.cpp \
                    jni_mp4ToYuv_module.cpp \
                    jni_nativewindow_module.cpp \
                    jni_im_module.cpp \
                    jni_filter_module.cpp \
                    jni_sws_module.cpp \
                    jni_yuvTorgb_module.cpp \
                    jni_fencode_module.cpp

LOCAL_LDLIBS += -llog -lz -landroid
LOCAL_LDLIBS += -lGLESv2 -ljnigraphics
LOCAL_MODULE := ffmpegextwrapper

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include ./

LOCAL_SHARED_LIBRARIES:= avcodec avformat avutil swresample swscale avfilter

include $(BUILD_SHARED_LIBRARY)