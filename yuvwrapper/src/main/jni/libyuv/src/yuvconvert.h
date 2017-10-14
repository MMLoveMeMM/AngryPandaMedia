//
// Created by rd0348 on 2017/10/13 0013.
//

#ifndef __H_YUVCONVERT_HEADER_H
#define __H_YUVCONVERT_HEADER_H

#ifdef __cplusplus
extern "C"
{
#include "libyuv.h"
#endif

class yuvconvert {

public:
static int yuv(unsigned char *Src_data, unsigned char *Dst_data,
                       int src_width, int src_height,
                       bool EnableRotate, bool EnableMirror,
                       unsigned char *Dst_data_mirror, unsigned char *Dst_data_rotate,
                       int rotatemodel);

static int N21ToN12(unsigned char *Src_data, unsigned char *Dst_data,
                       int src_width, int src_height);

static int ARGBScale(unsigned char * src_argb, int src_stride_argb,
                         int src_width, int src_height,
                         unsigned char * dst_argb, int dst_stride_argb,
                         int dst_width, int dst_height,
                         int filtering);

static int ARGBToI420(unsigned char* src_frame, int src_stride_frame,
               unsigned char* dst_y, int dst_stride_y,
               unsigned char* dst_u, int dst_stride_u,
               unsigned char* dst_v, int dst_stride_v,
               int width, int height);

};

#ifdef __cplusplus
}
#endif

#endif //__H_YUVCONVERT_HEADER_H
