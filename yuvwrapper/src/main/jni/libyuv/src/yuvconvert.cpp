//
// Created by rd0348 on 2017/10/13 0013.
//

#include "yuvconvert.h"

/*
* YUV其存储格式是： 共大小为(720×480×3>>1)字节，
* 分为三个部分:Y,U和V
* Y分量：    (720×480)个字节
* U(Cb)分量：(720×480>>2)个字节
* V(Cr)分量：(720×480>>2)个字节
* 三个部分内部均是行优先存储，三个部分之间是Y,U,V 顺序存储
* YV12和I420基本上是一样的，就是UV的顺序不同
*/

int yuvconvert::yuv(unsigned char *Src_data, unsigned char *Dst_data,
                                       int src_width, int src_height,
                                       bool EnableRotate, bool EnableMirror,
                                       unsigned char *Dst_data_mirror, unsigned char *Dst_data_rotate,
                                       int rotatemodel){

    //src:NV12 video size
    int NV12_Size = src_width * src_height * 3 / 2;
    int NV12_Y_Size = src_width * src_height;

    //dst:YUV420 video size
    int I420_Size = src_width * src_height * 3 / 2;
    int I420_Y_Size = src_width * src_height;
    int I420_U_Size = (src_width >> 1) * (src_height >> 1);
    int I420_V_Size = I420_U_Size;

    // video format transformation process
    unsigned char *Y_data_Src = Src_data;
    unsigned char *UV_data_Src = Src_data + NV12_Y_Size;
    int src_stride_y = src_width;
    int src_stride_uv = src_width;

    unsigned char *Y_data_Dst = Dst_data;
    unsigned char *U_data_Dst = Dst_data + I420_Y_Size;
    unsigned char *V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;

    int Dst_Stride_Y = src_width;
    int Dst_Stride_U = src_width >> 1;
    int Dst_Stride_V = Dst_Stride_U;
    //NV12ToI420
    libyuv::NV21ToI420(Y_data_Src, src_stride_y,
                       UV_data_Src, src_stride_uv,
                       Y_data_Dst, Dst_Stride_Y,
                       U_data_Dst, Dst_Stride_U,
                       V_data_Dst, Dst_Stride_V,
                       src_width, src_height);

    // video mirror process
    unsigned char *Y_data_Dst_mirror = Dst_data_mirror;
    unsigned char *U_data_Dst_mirror = Dst_data_mirror + I420_Y_Size;
    unsigned char *V_data_Dst_mirror = Dst_data_mirror + I420_Y_Size + I420_U_Size;
    int Dst_Stride_Y_mirror = src_width;
    int Dst_Stride_U_mirror = src_width >> 1;
    int Dst_Stride_V_mirror = Dst_Stride_U_mirror;

    if (EnableMirror) {
        libyuv::I420Mirror(Y_data_Dst, Dst_Stride_Y,
                           U_data_Dst, Dst_Stride_U,
                           V_data_Dst, Dst_Stride_V,
                           Y_data_Dst_mirror, Dst_Stride_Y_mirror,
                           U_data_Dst_mirror, Dst_Stride_U_mirror,
                           V_data_Dst_mirror, Dst_Stride_V_mirror,
                           src_width, src_height);
    }

    //video rotate process
    if (EnableRotate) {
        int Dst_Stride_Y_rotate;
        int Dst_Stride_U_rotate;
        int Dst_Stride_V_rotate;
        unsigned char *Y_data_Dst_rotate = Dst_data_rotate;
        unsigned char *U_data_Dst_rotate = Dst_data_rotate + I420_Y_Size;
        unsigned char *V_data_Dst_rotate = Dst_data_rotate + I420_Y_Size + I420_U_Size;

        if (rotatemodel == libyuv::kRotate90 || rotatemodel == libyuv::kRotate270) {
            Dst_Stride_Y_rotate = src_height;
            Dst_Stride_U_rotate = src_height >> 1;
            Dst_Stride_V_rotate = Dst_Stride_U_rotate;
        }
        else {
            Dst_Stride_Y_rotate = src_width;
            Dst_Stride_U_rotate = src_width >> 1;
            Dst_Stride_V_rotate = Dst_Stride_U_rotate;
        }

        if (EnableMirror) {
            libyuv::I420Rotate(Y_data_Dst_mirror, Dst_Stride_Y_mirror,
                               U_data_Dst_mirror, Dst_Stride_U_mirror,
                               V_data_Dst_mirror, Dst_Stride_V_mirror,
                               Y_data_Dst_rotate, Dst_Stride_Y_rotate,
                               U_data_Dst_rotate, Dst_Stride_U_rotate,
                               V_data_Dst_rotate, Dst_Stride_V_rotate,
                               src_width, src_height,
                               (libyuv::RotationMode) rotatemodel);
        }
        else {
            libyuv::I420Rotate(Y_data_Dst, Dst_Stride_Y,
                               U_data_Dst, Dst_Stride_U,
                               V_data_Dst, Dst_Stride_V,
                               Y_data_Dst_rotate, Dst_Stride_Y_rotate,
                               U_data_Dst_rotate, Dst_Stride_U_rotate,
                               V_data_Dst_rotate, Dst_Stride_V_rotate,
                               src_width, src_height,
                               (libyuv::RotationMode) rotatemodel);
        }
    }
    return 0;

}

int yuvconvert::N21ToN12(unsigned char *Src_data, unsigned char *Dst_data,
                       int src_width, int src_height){

    // 参考:http://www.cnblogs.com/dwdxdy/p/3713968.html
    //src:NV12 video size
    int NV12_Size = src_width * src_height * 3 / 2;
    int NV12_Y_Size = src_width * src_height;

    //dst:YUV420 video size
    int I420_Size = src_width * src_height * 3 / 2;
    int I420_Y_Size = src_width * src_height;
    int I420_U_Size = (src_width >> 1) * (src_height >> 1);
    int I420_V_Size = I420_U_Size;

    // video format transformation process
    unsigned char *Y_data_Src = Src_data;
    unsigned char *UV_data_Src = Src_data + NV12_Y_Size;
    int src_stride_y = src_width;
    int src_stride_uv = src_width;

    unsigned char *Y_data_Dst = Dst_data;
    unsigned char *U_data_Dst = Dst_data + I420_Y_Size;
    unsigned char *V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;

    int Dst_Stride_Y = src_width;
    int Dst_Stride_U = src_width >> 1;
    int Dst_Stride_V = Dst_Stride_U;
    //NV12ToI420
    libyuv::NV21ToI420(Y_data_Src, src_stride_y,
                       UV_data_Src, src_stride_uv,
                       Y_data_Dst, Dst_Stride_Y,
                       U_data_Dst, Dst_Stride_U,
                       V_data_Dst, Dst_Stride_V,
                       src_width, src_height);

    return 0;
}

int yuvconvert::ARGBScale(unsigned char * src_argb, int src_stride_argb,
                         int src_width, int src_height,
                         unsigned char * dst_argb, int dst_stride_argb,
                         int dst_width, int dst_height,
                         int filtering){

    return 0;
}

int yuvconvert::ARGBToI420(unsigned char* src_frame, int src_stride_frame,
               unsigned char* dst_y, int dst_stride_y,
               unsigned char* dst_u, int dst_stride_u,
               unsigned char* dst_v, int dst_stride_v,
               int width, int height){

    libyuv::ARGBToI420(src_frame, src_stride_frame,
                                      dst_y, dst_stride_y,
                                      dst_u, dst_stride_u,
                                      dst_v, dst_stride_v,
                                      width, height);
    return 0;
}