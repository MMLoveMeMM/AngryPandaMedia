LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := fullprebuilt/libavcodec-56.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := fullprebuilt/libavformat-56.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := fullprebuilt/libavutil-54.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := fullprebuilt/libswresample-1.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := fullprebuilt/libswscale-3.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avfilter
LOCAL_SRC_FILES := fullprebuilt/libavfilter-5.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := postproc
LOCAL_SRC_FILES := fullprebuilt/libpostproc-53.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := jni_core_module.cpp \
                    jni_utils_module.cpp \
                    jni_mp4ToYuv_module.cpp \
                    jni_nativewindow_module.cpp \
                    jni_im_module.cpp \
                    jni_filter_module.cpp

LOCAL_LDLIBS += -llog -lz -landroid
LOCAL_LDLIBS += -lGLESv2 -ljnigraphics
LOCAL_MODULE := ffmpegextwrapper

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include ./

LOCAL_SHARED_LIBRARIES:= avcodec avformat avutil swresample swscale avfilter postproc

include $(BUILD_SHARED_LIBRARY)