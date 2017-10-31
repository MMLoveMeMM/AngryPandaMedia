#ifndef __H_AAC_MODULE_H__
#define __H_AAC_MODULE_H__
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

#include <libavfilter/avfiltergraph.h>
#include <libavfilter/buffersrc.h>
#include <libavfilter/buffersink.h>
#include <libavutil/mathematics.h>
#endif

int register_android_jni_aac_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    initRecord
 * Signature: (Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_initRecord(
        JNIEnv *,jobject,jstring);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    writeAudioData
 * Signature: ([B)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData(
        JNIEnv *,jobject,jbyteArray);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    recordClose
 * Signature: (V)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_recordClose(
        JNIEnv *,jobject);

#ifdef __cplusplus
}
#endif
#endif //__H_AAC_MODULE_H__