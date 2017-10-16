#include "encodeproxy.h"
using namespace libffmpeg;

/*
* 两个全局变量
*/
OnCallBackFrame OnFrame;
encode mencode;

void ffmpeg_encode_local(int width,int height,const std::string& dir){

    /*
    * 初始化
    */
    mencode.init(width,height);

}

void ffmpeg_encode_init(int width,int height,const std::string& outpath){

    /*
    * 初始化
    */
    mencode.init_o(width,height,outpath);

}

void ffmpeg_encode_cb_init(int width,int height,OnCallBackFrame onframe){
    /*
    * 初始化
    */
    //mencode.init(width,height);



}

//包括了flush操作
void ffmpeg_encode_process(unsigned char* data){

    mencode.onframe(data);
    mencode.flush();
}

void ffmpeg_encode_close(){

    mencode.close();

}