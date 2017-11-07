#ifndef __H_HELP_MODULE_H__
#define __H_HELP_MODULE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "jni_utils_module.h"
#include "jni_aac_module.h"

extern int register_android_jni_aac_module(JNIEnv* env, jclass clazz);

#ifdef __cplusplus
}
#endif

#endif //__H_HELP_MODULE_H__