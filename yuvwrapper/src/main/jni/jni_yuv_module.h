#ifndef __H_YUV_HEADER_HEADER_H__
#define __H_YUV_HEADER_HEADER_H__
#include<jni.h>
#include "jni_core_module.h"
int register_android_jni_yuv_module(JNIEnv* env, jclass clazz);
/*
 * Class:     com_pumpkin_org_yuvwrapper
 * Method:    Yuv
 * Signature: ([B[BIIZZ[B[BI)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_yuvwrapper_Yuv
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jint, jint, jboolean, jboolean, jbyteArray, jbyteArray, jint);

/*
 * Class:     com_pumpkin_org_yuvwrapper
 * Method:    yuvN21ToN12
 * Signature: ([B[BII)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_yuvwrapper_yuvN21ToN12
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jint, jint);

/*
 * Class:     com_pumpkin_org_yuvwrapper
 * Method:    rgbToYuvBylibyuv
 * Signature: (Landroid/graphics/Bitmap[BIIII)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_yuvwrapper_rgbToYuvBylibyuv
  (JNIEnv *, jobject, jobject, jbyteArray ,jint ,jint ,jint ,jint );

#endif //__H_YUV_HEADER_HEADER_H__