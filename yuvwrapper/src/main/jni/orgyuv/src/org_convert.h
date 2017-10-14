#ifndef __H_ORG_CONVERT_HEADER_H__
#define __H_ORG_CONVERT_HEADER_H__
#include<stdlib.h>
#ifdef __cplusplus
namespace liborg{
extern "C"{
#endif

#include "stdint.h"

void NV21ToI420(unsigned char* dstyuv,unsigned char* data, int imageWidth, int imageHeight);

#ifdef __cplusplus
}
}
#endif

#endif //__H_ORG_CONVERT_HEADER_H__