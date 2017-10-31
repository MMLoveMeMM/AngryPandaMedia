#ifndef __H_MP4TOKTV_MODULE_H__
#define __H_MP4TOKTV_MODULE_H__
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
#endif

int register_android_jni_mp4ktv_module(JNIEnv* env, jclass clazz);
/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    mp4Toavi
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_mp4Toavi(
        JNIEnv *,jobject , jstring , jstring);

#ifdef __cplusplus
}
#endif

#endif // __H_MP4TOKTV_MODULE_H__

/*
I,P，B帧和PTS，DTS的关系

基本概念：

I frame ：帧内编码帧 又称intra picture，I 帧通常是每个 GOP（MPEG 所使用的一种视频压缩技术）的第一个帧，经过适度地压缩，做为随机访问的参考点，可以当成图象。I帧可以看成是一个图像经过压缩后的产物。

P frame: 前向预测编码帧 又称predictive-frame，通过充分将低于图像序列中前面已编码帧的时间冗余信息来压缩传输数据量的编码图像，也叫预测帧；

B frame: 双向预测内插编码帧 又称bi-directional interpolated prediction frame，既考虑与源图像序列前面已编码帧，也顾及源图像序列后面已编码帧之间的时间冗余信息来压缩传输数据量的编码图像，也叫双向预测帧；

PTS：Presentation Time Stamp。PTS主要用于度量解码后的视频帧什么时候被显示出来

DTS：Decode Time Stamp。DTS主要是标识读入内存中的ｂｉｔ流在什么时候开始送入解码器中进行解码。

在没有B帧存在的情况下DTS的顺序和PTS的顺序应该是一样的。

ＩＰＢ帧的不同：

I frame:自身可以通过视频解压算法解压成一张单独的完整的图片。

P frame：需要参考其前面的一个I frame 或者B frame来生成一张完整的图片。

B frame:则要参考其前一个I或者P帧及其后面的一个P帧来生成一张完整的图片。

两个I frame之间形成一个GOP，在x264中同时可以通过参数来设定bf的大小，即：I 和p或者两个P之间B的数量。

通过上述基本可以说明如果有B frame 存在的情况下一个GOP的最后一个frame一定是P.

DTS和PTS的不同：

DTS主要用于视频的解码,在解码阶段使用.PTS主要用于视频的同步和输出.在display的时候使用.在没有B frame的情况下.DTS和PTS的输出顺序是一样的.

思路：
程序包含了对两个文件的处理：读取输入文件和写入输出文件。然后使用了一个avcodec_copy_context()拷贝输入的AVCodecContext到输出的AVCodecContext。然后再从输出的AVCodecContext写进输出文件

关键函数：
输入文件操作：

avformat_open_input()：打开媒体的的过程开始于avformat_open_input。

av_read_frame()：从输入文件中读取一个AVPacket。
输出文件操作：

avformat_alloc_output_context2()：初始化输出视频码流的AVFormatContext。

avformat_new_stream()：创建输出码流的AVStream。

avcodec_copy_context()：拷贝输入视频码流的AVCodecContex的数值t到输出视频的AVCodecContext。

avio_open()：打开输出文件。

avformat_write_header()：写文件头（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）。

av_interleaved_write_frame()：将AVPacket（存储视频压缩码流数据）写入文件。

av_write_trailer()：写文件尾（对于某些没有文件头的封装格式，不需要此函数。比如说MPEG2TS）

*/


