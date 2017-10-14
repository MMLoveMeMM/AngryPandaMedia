#ifndef __H_ORG_MIRROR_HEADER_H__
#define __H_ORG_MIRROR_HEADER_H__
#include<stdlib.h>
#ifdef __cplusplus
namespace liborg{
extern "C"{
#endif

void Mirror(unsigned char* yuv_temp, int nw, int nh, int w,
        int h);

#ifdef __cplusplus
}
}
#endif
#endif //__H_ORG_MIRROR_HEADER_H__