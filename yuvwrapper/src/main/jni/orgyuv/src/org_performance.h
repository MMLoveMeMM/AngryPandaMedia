#ifndef __H_ORG_PERFORMANCE_HEADER_H__
#define __H_ORG_PERFORMANCE_HEADER_H__
#include<stdlib.h>
#ifdef __cplusplus
namespace liborg{
extern "C"{
#endif

void detailPic0(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h);
void detailPic90(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h);
void detailPic180(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h);
void detailPic270(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h);

#ifdef __cplusplus
}
}
#endif

#endif //__H_ORG_PERFORMANCE_HEADER_H__