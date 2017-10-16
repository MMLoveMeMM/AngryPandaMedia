#ifndef __H_FFMPEG_BASE_ENCODE_HEADER_H__
#define __H_FFMPEG_BASE_ENCODE_HEADER_H__

#include <stdio.h>
#include <time.h>
#include<string>
#include "ffmpeg.h"

#ifdef __cpluscplus
extern "C"{

#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libswscale/swscale.h"
#include "include/libavutil/log.h"
#include "include/libavutil/time.h"

#endif

namespace libffmpeg{
using namespace std;

/*
* 定义一个经过ffmpeg处理后回调函数到java层的函数
*/
typedef void (*OnCallBackFrame)(void* buffer,int len);

class encode : public ffmpeg{
public:
    encode();
    virtual ~encode();

    /*
    * ffmpeg 自定义日志
    */
    static void custom_log(void *ptr, int level, const char* fmt, va_list vl);

    void init(int width,int height);
    void init_o(int width,int height,const std::string& out_dir);
    void onframe(unsigned char* data);
    void onframecb(unsigned char* data,OnCallBackFrame onframe);
    void flush();
    void flushcb(OnCallBackFrame onframe);
    void close();

private:

};

}

#ifdef __cpluscplus
}
#endif
#endif //__H_FFMPEG_BASE_ENCODE_HEADER_H__