#ifndef __H_ORG_SCALE_HEADER_H__
#define __H_ORG_SCALE_HEADER_H__
#include<stdlib.h>
#ifdef __cplusplus
namespace liborg{
extern "C"{
#endif

int scale_yuv (unsigned char* data, unsigned char*dst, int width, int height,
        int goalwidth, int goalheight);

#ifdef __cplusplus
}
}
#endif

#endif //__H_ORG_SCALE_HEADER_H__