//
// Created by rd0348 on 2017/10/12 0012.
//

#include "x264encodeproxy.h"
#include "x264encode.h"

x264encode* _x264encode;
H264DataCallBack h264DataCallBackFunc;

void initX264Encode(int width,int height,int fps,int bite,H264DataCallBack h264DataCallBack){

    if(NULL!=_x264encode){

    }

    _x264encode=new x264encode();
    _x264encode->initX264Encode(width,height,fps,bite);
    h264DataCallBackFunc=h264DataCallBack;

}

void encoderH264(void* pdata, unsigned int datalen, long long time){
    if(_x264encode==NULL){
        return;
    }
    int i=0;
    char* bufdata=NULL;
    int buflen=-1;
    int isKeyFrame;
    _x264encode->startEncoder((uint8_t*)pdata,*&bufdata,*&buflen,*&isKeyFrame);
    if(buflen!=-1){
        if(NULL!=h264DataCallBackFunc){
            h264DataCallBackFunc(bufdata,buflen);
        }
        if(bufdata){
            delete [] bufdata;
        }
    }
}

void uninitX264Encode(){
    if(_x264encode){
        _x264encode->releaseEncoder();
        delete _x264encode;
        _x264encode=NULL;
    }
}