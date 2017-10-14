#ifndef __H_ORG_ROTATE_HEADER_H__
#define __H_ORG_ROTATE_HEADER_H__
#include<stdlib.h>
#ifdef __cplusplus
namespace liborg{
extern "C"{
#endif
// 顺时针
int rotateI420Degree270(unsigned char* dstyuv,unsigned char* srcdata, int imageWidth, int imageHeight);
int rotateI420Degree180(unsigned char* dstyuv,unsigned char* srcdata, int imageWidth, int imageHeight);
int rotateI420Degree90(unsigned char* dstyuv,unsigned char* srcdata, int imageWidth, int imageHeight);

#ifdef __cplusplus
}
}
#endif

#endif // __H_ORG_ROTATE_HEADER_H__