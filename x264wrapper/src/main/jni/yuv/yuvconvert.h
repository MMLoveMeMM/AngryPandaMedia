#ifndef __H_YUV_CONVERT_HEADER_H__
#define __H_YUV_CONVERT_HEADER_H__
#include "stdint.h"
#include<stdlib.h>
class yuvconvert{
public:
    yuvconvert();
    ~yuvconvert();
//nv21 to yuvi420
void nv21Toi420(uint8_t* dstyuv,uint8_t* data, int imageWidth, int imageHeight);

};

#endif //__H_YUV_CONVERT_HEADER_H__