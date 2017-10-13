//
// Created by rd0348 on 2017/10/13 0013.
//

#ifndef TEDDJNI_YUVCONVERT_H
#define TEDDJNI_YUVCONVERT_H

#ifdef __cplusplus
extern "C"
{
#include "libyuv.h"
#endif

class yuvconvert {

public:
int yuv(unsigned char *Src_data, unsigned char *Dst_data,
                       int src_width, int src_height,
                       bool EnableRotate, bool EnableMirror,
                       unsigned char *Dst_data_mirror, unsigned char *Dst_data_rotate,
                       int rotatemodel);

};

#ifdef __cplusplus
}
#endif

#endif //TEDDJNI_YUVCONVERT_H
