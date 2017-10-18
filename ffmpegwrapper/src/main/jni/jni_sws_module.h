#ifndef __H_SWS_MODULE_H__
#define __H_SWS_MODULE_H__
#include<jni.h>
#include "jni_core_module.h"
#include "jni_utils_module.h"

int register_android_jni_sws_module(JNIEnv* env, jclass clazz);

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    swsprocess
 * Signature: (Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_swsprocess
        (JNIEnv * env, jclass clazz, jobject surface);

#endif