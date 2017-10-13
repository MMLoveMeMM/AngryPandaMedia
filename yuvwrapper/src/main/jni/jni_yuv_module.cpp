#include<stdio.h>
#include "jni_yuv_module.h"
#include "yuvconvert.h"

/*
 * Class:     com_protruly_im_core_IMCore
 * Method:    Yuv
 * Signature: ([B[BIIZZ[B[BI)I
 */
JNIEXPORT jint JNICALL Java_com_protruly_im_core_IMCore_Yuv(JNIEnv *env, jobject instance, jbyteArray Src_data_,
                                               jbyteArray Dst_data_, jint src_width,
                                               jint src_height, jboolean enableRotate,
                                               jboolean enableMirror, jbyteArray Dst_data_mirror_,
                                               jbyteArray Dst_data_rotate_, jint rotatemodel) {
    jbyte *Src_data = env->GetByteArrayElements(Src_data_, NULL);
    jbyte *Dst_data = env->GetByteArrayElements(Dst_data_, NULL);
    jbyte *Dst_data_mirror = env->GetByteArrayElements(Dst_data_mirror_, NULL);
    jbyte *Dst_data_rotate = env->GetByteArrayElements(Dst_data_rotate_, NULL);
    // TODO

    unsigned char* src_data=(unsigned char*)Src_data;
    unsigned char* dst_data=(unsigned char*)Dst_data;
    unsigned char* dst_data_mirror=(unsigned char*)Dst_data_mirror;
    unsigned char* dst_data_rotate=(unsigned char*)Dst_data_rotate;

    yuvconvert mconvert;
    mconvert.yuv(src_data,dst_data,src_width,src_height,enableRotate,enableMirror,dst_data_mirror,dst_data_rotate,rotatemodel);

    env->ReleaseByteArrayElements(Src_data_, Src_data, 0);
    env->ReleaseByteArrayElements(Dst_data_, Dst_data, 0);
    env->ReleaseByteArrayElements(Dst_data_mirror_, Dst_data_mirror, 0);
    env->ReleaseByteArrayElements(Dst_data_rotate_, Dst_data_rotate, 0);
}

static JNINativeMethod YUVMethods[] =
{
    {"Yuv", "([B[BIIZZ[B[BI)I",(void *)Java_com_protruly_im_core_IMCore_Yuv},
};

int register_android_jni_yuv_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, YUVMethods, sizeof(YUVMethods) / sizeof(YUVMethods[0]));
}