//
// Created by rd0348 on 2017/10/11 0011.
//
#include <jni.h>
#include "jni_core_module.h"
#include "x264encodeproxy.h"
#ifndef ANGRYPANDAAV_JNI_X264_MODULE_H
#define ANGRYPANDAAV_JNI_X264_MODULE_H
#ifdef __cplusplus
extern "C"
{
#endif

int register_android_jni_x264_module(JNIEnv* env, jclass clazz);

JNIEXPORT void JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_initX264Encode(JNIEnv *env, jobject instance,
                                                            jint width, jint height, jint fps,
                                                            jint bite);
JNIEXPORT jint JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_encoderH264(JNIEnv *env, jobject instance, jint length,
                                                         jlong time);

JNIEXPORT void JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_uninitX264Encode(JNIEnv *env, jobject instance);

#ifdef __cplusplus
}
#endif

#endif //ANGRYPANDAAV_JNI_X264_MODULE_H
