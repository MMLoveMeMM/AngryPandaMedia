#ifndef _H_MP4_TO_YUV_H__
#define _H_MP4_TO_YUV_H__
#include <jni.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<android/log.h>
#include <jni.h>
#include "jni_core_module.h"

#ifdef __cplusplus
extern "C"
{
#include <libavutil/log.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#endif

int register_android_jni_mp4toyuv_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    avcodecinfo
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_avcodecinfo(
        JNIEnv *,jobject);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    decode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_decode(
        JNIEnv *,jobject , jstring , jstring);

#ifdef __cplusplus
}
#endif

#endif //_H_MP4_TO_YUV_H__