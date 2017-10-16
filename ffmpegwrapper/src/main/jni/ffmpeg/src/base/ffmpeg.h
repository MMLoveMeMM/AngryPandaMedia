#ifndef __H_FFMPEG_BASE_HEADER_H__
#define __H_FFMPEG_BASE_HEADER_H__

#include <stdio.h>
#include <time.h>
#include <string>

extern "C"{
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libswscale/swscale.h"
#include "include/libavutil/log.h"
#include "include/libavutil/time.h"
}
namespace libffmpeg{

using namespace std;

class ffmpeg{
public:
    ffmpeg(){};
    virtual ~ffmpeg(){}const;

    virtual void init(int width,int height); // 初始化编码,或者格式转换等
    virtual void init_o(int width,int height,const std::string& out_dir){}; // 初始化编码,或者格式转换等
    virtual void onframe(unsigned char* data){}; // 开始编码或者格式转换等
    virtual void flush(){}; // 刷新缓冲区
    virtual void close(); // 关闭操作

    // 全局变量
    AVFormatContext *ofmt_ctx;
    AVStream* video_st;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;
    AVPacket enc_pkt;
    AVFrame *pFrameYUV;

    // 日志路径
    std::string log_dir;

    int framecnt = 0;
    int yuv_width;
    int yuv_height;
    int y_length;
    int uv_length;
    int64_t start_time;
};
}
#endif //__H_FFMPEG_BASE_HEADER_H__