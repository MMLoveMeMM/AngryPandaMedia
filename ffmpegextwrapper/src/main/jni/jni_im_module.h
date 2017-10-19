#ifndef __H_IM_MODULE_H__
#define __H_IM_MODULE_H__
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
#endif

int register_android_jni_im_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    initencode
 * Signature: (II)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_initencode(
        JNIEnv *,jobject,jint ,jint);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    onframe
 * Signature: ([B)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_onframe(
        JNIEnv *,jobject,jbyteArray);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    flush
 * Signature: ()I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_flush(
        JNIEnv *,jobject);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    closeencode
 * Signature: ()I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_closeencode(
        JNIEnv *,jobject);

#ifdef __cplusplus
}
#endif

#endif //__H_IM_MODULE_H__