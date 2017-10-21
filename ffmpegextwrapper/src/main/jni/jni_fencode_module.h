#ifndef __H_FFMPEG_ENCODE_MODULE_H__
#define __H_FFMPEG_ENCODE_MODULE_H__
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

int register_android_jni_fencode_module(JNIEnv* env, jclass clazz);
/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    yuvToHxxx
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_yuvToHxxx(
        JNIEnv *,jobject , jstring , jstring);

#ifdef __cplusplus
}
#endif

#endif //__H_FFMPEG_ENCODE_MODULE_H__