LOCAL_PATH :=$(call my-dir) # 当前路径

LOCAL_MODULE := libmarsxlog
LOCAL_SRC_FILES := libmarsxlog.so

include $(PREBUILT_SHARED_LIBRARY)