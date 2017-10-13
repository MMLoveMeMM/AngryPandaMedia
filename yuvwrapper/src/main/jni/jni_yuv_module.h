#ifndef __H_YUV_HEADER_HEADER_H__
#define __H_YUV_HEADER_HEADER_H__
#include<jni.h>
#include "libyuv.h"
#include "jni_core_module.h"
int register_android_jni_yuv_module(JNIEnv* env, jclass clazz);
/*
 * Class:     com_protruly_im_core_IMCore
 * Method:    Yuv
 * Signature: ([B[BIIZZ[B[BI)I
 */
JNIEXPORT jint JNICALL Java_com_protruly_im_core_IMCore_Yuv
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jint, jint, jboolean, jboolean, jbyteArray, jbyteArray, jint);

#endif //__H_YUV_HEADER_HEADER_H__