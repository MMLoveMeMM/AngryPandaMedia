#ifndef __H_FFMPEG_CODEC_HEADER_H__
#define __H_FFMPEG_CODEC_HEADER_H__

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <jni.h>
#include "jni_core_module.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    FfmpegInit
 * Signature: (IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_FfmpegInit
  (JNIEnv *, jobject, jint, jint, jstring);

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    FfmpegOnFrame
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_FfmpegOnFrame
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    FfmpegClose
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_FfmpegClose
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif

#endif