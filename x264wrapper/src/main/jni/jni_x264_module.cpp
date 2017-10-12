//
// Created by rd0348 on 2017/10/11 0011.
//

#include "jni_x264_module.h"

JNIEXPORT void JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_initX264Encode(JNIEnv *env, jobject instance,
        jint width, jint height, jint fps,
jint bite) {

// TODO

}

JNIEXPORT jint JNICALL
        Java_com_pumpkin_org_x264wrapper_X264Wrapper_encoderH264(JNIEnv *env, jobject instance, jint length,
        jlong time) {

// TODO
        return 0;
}

JNIEXPORT void JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_uninitX264Encode(JNIEnv *env, jobject instance) {

// TODO

}

static JNINativeMethod X264Methods[] = {{"initX264Encode", "(IIII)V",(void *)Java_com_pumpkin_org_x264wrapper_X264Wrapper_initX264Encode},
                                     {"encoderH264", "(IJ)I",(void *)Java_com_pumpkin_org_x264wrapper_X264Wrapper_encoderH264},
                                     {"uninitX264Encode","()V",(void *)Java_com_pumpkin_org_x264wrapper_X264Wrapper_uninitX264Encode}};

int register_android_jni_x264_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, X264Methods, sizeof(X264Methods) / sizeof(X264Methods[0]));
}