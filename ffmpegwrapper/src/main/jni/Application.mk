APP_PLATFORM := android-21
APP_ABI := armeabi
NDK_TOOLCHAIN_VERSION := clang
APP_CFLAGS += -Wall
APP_STL := c++_static # 这个地方不能够使用gnustl_static, 否则与ffmpeg 库(libavutil)不兼容,编译将会出错,如果工程wrapper合并,需要使用这个!
#APP_CPPFLAGS += -frtti
APP_MODULES := libffmpeg
APP_CPPFLAGS += -std=c++11 -fexceptions -frtti -fpermissive