#ifndef __H_SWS_MODULE_H__
#define __H_SWS_MODULE_H__
#include <jni.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<android/log.h>
#include "jni_core_module.h"

#ifdef __cplusplus
extern "C"
{
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#endif

int register_android_jni_sws_module(JNIEnv* env, jclass clazz);
/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    swscale
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_swscale(
        JNIEnv *,jobject , jstring , jstring);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    wallpaper
 * Signature: (Ljava/lang/String;Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_wallpaper(
        JNIEnv *,jobject ,jstring, jobject);

#ifdef __cplusplus
}
#endif

#endif
/*
libswscale里面实现了各种图像像素格式的转换。

libswscale使用起来很方便，最主要的函数只有3个：
（1） sws_getContext()：使用参数初始化SwsContext结构体。
（2） sws_scale()：转换一帧图像。
（3） sws_freeContext()：释放SwsContext结构体。
其中sws_getContext()也可以用另一个接口函数sws_getCachedContext()取代。

初始化方法

初始化SwsContext除了调用sws_getContext()之外还有另一种方法，更加灵活，可以配置更多的参数。该方法调用的函数如下所示。
（1） sws_alloc_context()：为SwsContext结构体分配内存。
（2） av_opt_set_XXX()：通过av_opt_set_int()，av_opt_set()…等等一系列方法设置SwsContext结构体的值。在这里需要注意，SwsContext结构体的定义看不到，所以不能对其中的成员变量直接进行赋值，必须通过av_opt_set()这类的API才能对其进行赋值。
（3） sws_init_context()：初始化SwsContext结构体。
这种复杂的方法可以配置一些sws_getContext()配置不了的参数。比如说设置图像的YUV像素的取值范围是JPEG标准（Y、U、V取值范围都是0-255）还是MPEG标准（Y取值范围是16-235，U、V的取值范围是16-240）。

通过av_pix_fmt_desc_get()可以获得指定像素格式的AVPixFmtDescriptor结构体。
通过AVPixFmtDescriptor结构体可以获得不同像素格式的一些信息。例如av_get_bits_per_pixel()，通过该函数可以获得指定像素格式每个像素占用的比特数（Bit Per Pixel）。

图像拉伸
SWS_BICUBIC性能比较好；SWS_FAST_BILINEAR在性能和速度之间有一个比好好的平衡.
*/