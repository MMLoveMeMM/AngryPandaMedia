#ifndef __H_NATIVEWINDOW_MODULE_H__
#define __H_NATIVEWINDOW_MODULE_H__
#include <jni.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
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

int register_android_jni_nativewindow_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    nativewindow
 * Signature: (Ljava/lang/Object;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_nativewindow(
        JNIEnv *,jobject , jobject , jstring);

#ifdef __cplusplus
}
#endif

#endif //