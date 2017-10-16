#ifndef __H_ENCODE_PROXY_HEADER_H__
#define __H_ENCODE_PROXY_HEADER_H__
/*
* 这个地方是个人程序写法的爱好习惯
* 这个用于代理,隔离jni和c++,其他库里面看了可能没有,但是会发现,我使用了类的静态方法可以直接调用
* 这里的jni虽然支持c++,但是由于c++实际一般是在visual studio平台开发的.
*/
#include "encode.h"
#ifdef __cplusplus
extern "C"{
#endif
using namespace libffmpeg;

/*
* 如果只是处理sdcard里面的或者指定路径下的资源
* 第三个参数 : 被编码的资源路径
*/
void ffmpeg_encode_local(int width,int height,const std::string& dir);

/*
* 如果是处理即时数据流,那么就要提供回调方法或者共享java层的buffer地址
* 共享地址可能需要加锁,不然数据可能随时被java层更新导致编码时数据出错
*/
/*
* 初始化,并且注册回调函数,如果不是回调就直接提供输出文件的路径
*/
void ffmpeg_encode_init(int width,int height,const std::string& out_dir);
void ffmpeg_encode_cb_init(int width,int height,OnCallBackFrame onframe);
void ffmpeg_encode_process(unsigned char* data);//包括了flush操作
void ffmpeg_encode_close();

#ifdef __cplusplus
}
#endif

#endif //__H_ENCODE_PROXY_HEADER_H__