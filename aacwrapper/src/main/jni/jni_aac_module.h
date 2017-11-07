#ifndef __H_AAC_MODULE_H__
#define __H_AAC_MODULE_H__
#include <jni.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<android/log.h>
#include "jni_core_module.h"

//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libavutil/log.h>
//#include <libavutil/mathematics.h>

#ifdef __cplusplus
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/log.h"
#include "libavutil/mathematics.h"
#endif

int register_android_jni_aac_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    initRecord
 * Signature: (Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_initRecord(
        JNIEnv *,jobject,jstring);

/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    writeAudioData
 * Signature: ([B)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_writeAudioData(
        JNIEnv *,jobject,jbyteArray);

/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    closeRecord
 * Signature: (V)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_closeRecord(
        JNIEnv *,jobject);

/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    pcm2Aac
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_pcm2Aac(
        JNIEnv *,jobject,jstring,jstring);
/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    pcm2AacDetail
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_pcm2AacDetail(
        JNIEnv *,jobject,jstring,jstring);

#ifdef __cplusplus
}
#endif
#endif //__H_AAC_MODULE_H__