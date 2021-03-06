LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := avcodec
LOCAL_SRC_FILES := xprebuilt/libavcodec.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avformat
LOCAL_SRC_FILES := xprebuilt/libavformat.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avutil
LOCAL_SRC_FILES := xprebuilt/libavutil.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swresample
LOCAL_SRC_FILES := xprebuilt/libswresample.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := swscale
LOCAL_SRC_FILES := xprebuilt/libswscale.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avfilter
LOCAL_SRC_FILES := xprebuilt/libavfilter.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := aac
LOCAL_SRC_FILES := xprebuilt/libfdk-aac.so
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
                    jni_fencode_module.cpp \
                    jni_mux_module.cpp \
                    jni_aac_module.cpp

LOCAL_LDLIBS += -llog -lz -landroid
LOCAL_LDLIBS += -lGLESv2 -ljnigraphics
LOCAL_MODULE := ffmpegextwrapper

LOCAL_C_INCLUDES += $(LOCAL_PATH)/xinclude ./

LOCAL_SHARED_LIBRARIES:= avcodec avformat avutil swresample swscale avfilter aac

include $(BUILD_SHARED_LIBRARY)