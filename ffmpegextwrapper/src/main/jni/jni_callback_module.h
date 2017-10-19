#ifndef __H_CALLBACK_HEADER_H__
#define __H_CALLBACK_HEADER_H__
#include<jni.h>
/*
* 这个module 专门处理jni回调java层接口或者变量的
*/
/*
* 用于喂数据传递的buffer
*/
#define YUVBUFFER_IN_JAVA_OBJ_NAME   "mH264buffer"

jfieldID GetYUVBuffer();
/*
* 编码完成后,回调到java
*/
extern void EncodeCallback(void* pdata,int datalen);

#endif