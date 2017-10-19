#ifndef __H_FILTER_H__
#define __H_FILTER_H__
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
#endif

int register_android_jni_filter_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    filter
 * Signature: (Ljava/lang/Object;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_filter(
        JNIEnv *,jobject , jobject , jstring);


#ifdef __cplusplus
}
#endif
#endif //__H_FILTER_H__