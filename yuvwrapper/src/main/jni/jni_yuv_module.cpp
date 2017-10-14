#include<stdio.h>
#include <android/bitmap.h>
#include "jni_yuv_module.h"
#include "yuvconvert.h"

/*
 * Class:     com_pumpkin_org_yuvwrapper
 * Method:    Yuv
 * Signature: ([B[BIIZZ[B[BI)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_yuvwrapper_Yuv(JNIEnv *env, jobject instance, jbyteArray Src_data_,
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

    yuvconvert::yuv(src_data,dst_data,src_width,src_height,enableRotate,enableMirror,dst_data_mirror,dst_data_rotate,rotatemodel);

    env->ReleaseByteArrayElements(Src_data_, Src_data, 0);
    env->ReleaseByteArrayElements(Dst_data_, Dst_data, 0);
    env->ReleaseByteArrayElements(Dst_data_mirror_, Dst_data_mirror, 0);
    env->ReleaseByteArrayElements(Dst_data_rotate_, Dst_data_rotate, 0);

    return 0;
}

/*
 * Class:     com_pumpkin_org_yuvwrapper
 * Method:    yuvN21ToN12
 * Signature: ([B[BII)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_yuvwrapper_yuvN21ToN12
  (JNIEnv *env, jobject thiz, jbyteArray Src_data_, jbyteArray Dest_data_, jint src_width, jint src_height){

    jbyte *Src_data = env->GetByteArrayElements(Src_data_, NULL);
    jbyte *Dest_data = env->GetByteArrayElements(Dest_data_, NULL);

    unsigned char* src_data = (unsigned char*)Src_data;
    unsigned char* dest_data = (unsigned char*)Dest_data;

    jsize src_len = env->GetArrayLength(Src_data_);
    int len_src = (int)src_len; // NV21和NV12 数据长度没有变化
    yuvconvert::N21ToN12(src_data,dest_data,src_width,src_height);

    //int len_sec =width*height*3>>1; // NV21和NV12 数据长度没有变化
    env->SetByteArrayRegion(Dest_data_, 0, len_src, (jbyte*)(dest_data));

    env->ReleaseByteArrayElements(Src_data_, (jbyte*)src_data, 0);
    env->ReleaseByteArrayElements(Dest_data_, (jbyte*)dest_data, 0);
    return 0;
 }

/*
 * Class:     com_pumpkin_org_yuvwrapper
 * Method:    yuvRGBToYuv
 * Signature: (Landroid/graphics/Bitmap[BIIII)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_yuvwrapper_rgbToYuvBylibyuv
  (JNIEnv *env, jobject thiz, jobject bitmapcolor, jbyteArray destYuv, jint src_width,jint src_height,jint dest_width,jint dest_height){

    AndroidBitmapInfo infocolor;
    int ret;

    if ((ret = AndroidBitmap_getInfo(env, bitmapcolor, &infocolor)) < 0) {
        //LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return ret;
    }

    //LOGI("color image :: width is %d; height is %d; stride is %d; format is %d;flags is %d",
    //infocolor.width, infocolor.height, infocolor.stride, infocolor.format, infocolor.flags);

    if (infocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        //LOGE("Bitmap format is not RGBA_8888 !");
        return ret;
    }

    // 移位的运算要快
    int src_len = infocolor.width*infocolor.height<<2;//infocolor.width*infocolor.height*4;//In format argb8888, per pixel cost 4 bytes
    void* argb_array = NULL;

    if ((ret = AndroidBitmap_lockPixels(env, bitmapcolor, &argb_array)) < 0) {
        //LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return ret;
    }

    int filtering = 3; // 1. linear; 2. bilinear; 3. box
    int dest_len = dest_width*dest_height*3>>2; // yuv
    unsigned char* dest_yuv_data;
    unsigned char* dest_y_data;
    unsigned char* dest_u_data;
    unsigned char* dest_v_data;
    unsigned char* dest_yuv_data_final;
    /*
    * 这样在当个函数中分配新建新的内存块很不好
    * 如果反复的处理,就会导致反复的分配新的内存,这样很可能导致大量的碎片内存,以及分配内存失败
    * 这样分配然后释放内存,感觉很傻逼,这里只是做测试
    * 正常的要实现分配好全局内存块,然后后面复用这个全局内存块,内存池更好啦
    */
    dest_yuv_data = (unsigned char*)malloc(sizeof(unsigned char)*dest_len);
    dest_y_data = (unsigned char*)malloc(sizeof(unsigned char)*dest_len*2/3); // NV12或者NV21,Y占2/3,U占1/6,V占1/6,而且排列顺序是按照YUV固定排列的
    dest_u_data = (unsigned char*)malloc(sizeof(unsigned char)*dest_len*1/6);
    dest_v_data = (unsigned char*)malloc(sizeof(unsigned char)*dest_len*1/6);
    dest_yuv_data_final = (unsigned char*)malloc(sizeof(unsigned char)*dest_len);

    yuvconvert::ARGBScale((unsigned char*)&argb_array, infocolor.stride,
                     infocolor.width,  infocolor.height,
                     dest_yuv_data, dest_width * 4,
                     dest_width,  dest_height,
                     filtering);

    yuvconvert::ARGBToI420(dest_yuv_data, dest_width * 4 ,
               dest_y_data,  dest_width,
               dest_u_data,  dest_width / 2,
               dest_v_data,  dest_width / 2,
               dest_width,   dest_height);

    // merge y plane to output_data
    memcpy(dest_yuv_data_final, dest_y_data, (dest_len * 2 / 3 ) );
    // merge v plane to output_data
    memcpy(dest_yuv_data_final + (dest_len * 2 / 3), dest_u_data, (dest_len / 6 ) );
    // merge u plane to output_data
    memcpy(dest_yuv_data_final + (dest_len * 5 / 6 ), dest_v_data, (dest_len/ 6) );

    LOGI("unlocking pixels");
    AndroidBitmap_unlockPixels(env, bitmapcolor);
    env->SetByteArrayRegion(destYuv, 0, dest_len, (jbyte*)(dest_yuv_data_final));

    /*
    * 释放上面分配的内存
    */
    free(dest_yuv_data);
    free(dest_y_data);
    free(dest_u_data);
    free(dest_v_data);
    return ret;
}

static JNINativeMethod YUVMethods[] =
{
    {"Yuv", "([B[BIIZZ[B[BI)I",(void *)Java_com_pumpkin_org_yuvwrapper_Yuv},
    {"yuvN21ToN12", "([B[BII)I",(void *)Java_com_pumpkin_org_yuvwrapper_yuvN21ToN12},
    {"yuvRGBToYuv", "(Landroid/graphics/Bitmap[B)I",(void *)Java_com_pumpkin_org_yuvwrapper_rgbToYuvBylibyuv},
};

int register_android_jni_yuv_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, YUVMethods, sizeof(YUVMethods) / sizeof(YUVMethods[0]));
}