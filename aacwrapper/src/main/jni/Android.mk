LOCAL_PATH := $(call my-dir)
MAIN_PATH := $(LOCAL_PATH)
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

LOCAL_PATH := $(MAIN_PATH)

LOCAL_SHARED_LIBRARIES:= avcodec avformat avutil swresample swscale avfilter aac

LOCAL_SRC_LIST := $(wildcard $(LOCAL_PATH)/src/*.cpp)
LOCAL_SRC_CORE := $(LOCAL_SRC_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := jni_core_module.cpp \
                    jni_utils_module.cpp \
                    jni_aac_module.cpp

LOCAL_SRC_FILES += $(LOCAL_SRC_CORE)

LOCAL_LDLIBS 	:= -llog -ljnigraphics -lz -ldl -lgcc
LOCAL_LDLIBS += -llog -lz -landroid
LOCAL_LDLIBS += -lGLESv2 -ljnigraphics
LOCAL_MODULE := aacwrapper

LOCAL_C_INCLUDES := jni_core_module.h \
                    jni_utils_module.h \
                    jni_help_module.h \
                    jni_aac_module.h
LOCAL_C_INCLUDES += $(LOCAL_PATH)/xinclude ./ $(LOCAL_PATH)/src

//LOCAL_CFLAGS := -D__STDC_CONSTANT_MACROS -Wno-sign-compare -Wno-switch -Wno-pointer-sign -DHAVE_NEON=1 \
//      -mfpu=neon -mfloat-abi=softfp -fPIC -DANDROID

include $(BUILD_SHARED_LIBRARY)