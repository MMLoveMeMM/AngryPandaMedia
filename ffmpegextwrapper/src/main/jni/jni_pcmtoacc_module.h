#ifndef __H_PCM_TO_ACC_MODULE_H__
#define __H_PCM_TO_ACC_MODULE_H__
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
#include <libavutil/opt.h>
#endif

int register_android_jni_pcmtoacc_module(JNIEnv* env, jclass clazz);
/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    pcmToacc
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_pcmToacc(
        JNIEnv *,jobject , jstring , jstring);

#ifdef __cplusplus
}
#endif

#endif //__H_PCM_TO_ACC_MODULE_H__
/*
av_register_all()：注册FFmpeg所有编解码器。

avformat_alloc_output_context2()：初始化输出码流的AVFormatContext。

avio_open()：打开输出文件。

av_new_stream()：创建输出码流的AVStream。

avcodec_find_encoder()：查找编码器。

avcodec_open2()：打开编码器。

avformat_write_header()：写文件头（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）。

avcodec_encode_audio2()：编码音频。即将AVFrame（存储PCM采样数据）编码为AVPacket（存储AAC，MP3等格式的码流数据）。

av_write_frame()：将编码后的视频码流写入文件。

av_write_trailer()：写文件尾（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）
*/