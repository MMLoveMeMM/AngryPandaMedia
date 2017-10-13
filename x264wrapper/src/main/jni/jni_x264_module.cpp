//
// Created by rd0348 on 2017/10/11 0011.
//
#include "jni_callback_module.h"
#include "jni_x264_module.h"

jobject g_user_obj;
jfieldID drain_yuvbuffer;

void H264EncodeCallback(void* pdata,int datalen)
{
	JNIEnv *env = NULL;
	CN_ATTACH_JVM(env);

    /*
    * 不能够这样获取回调
    * 如果这样获取的的对象和实际对象是两个单独的对象
    * 只能够使用全局的g_user_obj,保证下面调用的都是同一个java对象
    * 也可以将回调改为静态的,那就没问题了
    */
    jclass cls_user = env->GetObjectClass(JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore));
    // jobject user_obj = JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore); // 不能够新建一个类,不然就成了两个对象了
    jobject user_obj = g_user_obj;
    jbyteArray pcmdata = env->NewByteArray(datalen);
    env->SetByteArrayRegion(pcmdata, 0, datalen,(jbyte*)pdata);
    jmethodID callback_mtd = env->GetMethodID(cls_user, "H264DataCallBackFunc", "([BI)V");
    env->CallVoidMethod(user_obj,callback_mtd,pcmdata,datalen);

	//JNI_DELETE_LOCAL_REF(cls_user);
	CN_DETACH_JVM(env);

}

JNIEXPORT void JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_initX264Encode(JNIEnv *env, jobject user_obj,
        jint width, jint height, jint fps,
jint bite) {

    // TODO
    LOGI("Java_com_pumpkin_org_x264wrapper_X264Wrapper_initX264Encode");
    g_user_obj = env->NewGlobalRef(user_obj); // 获取全局对象
    //drain_yuvbuffer = GetYUVBuffer();
    jclass cls_user = env->GetObjectClass(user_obj);
    drain_yuvbuffer = env->GetFieldID(cls_user, YUVBUFFER_IN_JAVA_OBJ_NAME,
        "Ljava/nio/ByteBuffer;");

    initX264Encode(width,height,fps,bite,H264EncodeCallback);
}

JNIEXPORT jint JNICALL
        Java_com_pumpkin_org_x264wrapper_X264Wrapper_encoderH264(JNIEnv *env, jobject user_obj, jint length,
        jlong time) {

        // TODO
        LOGI("Java_com_pumpkin_org_x264wrapper_X264Wrapper_encoderH264");
        /*
        * 每次都这样去拿那个buffer,是不是有点坑爹
        */
        jobject input = (jobject) env->GetObjectField(user_obj, drain_yuvbuffer);
        unsigned char * jb = (unsigned char *) env->GetDirectBufferAddress(input);
        encoderH264(jb, length, time);

        return 0;
}

JNIEXPORT void JNICALL
Java_com_pumpkin_org_x264wrapper_X264Wrapper_uninitX264Encode(JNIEnv *env, jobject instance) {

    // TODO
    LOGI("Java_com_pumpkin_org_x264wrapper_X264Wrapper_uninitX264Encode");
    uninitX264Encode();
}

static JNINativeMethod X264Methods[] = {{"initX264Encode", "(IIII)V",(void *)Java_com_pumpkin_org_x264wrapper_X264Wrapper_initX264Encode},
                                     {"encoderH264", "(IJ)I",(void *)Java_com_pumpkin_org_x264wrapper_X264Wrapper_encoderH264},
                                     {"uninitX264Encode","()V",(void *)Java_com_pumpkin_org_x264wrapper_X264Wrapper_uninitX264Encode}};

int register_android_jni_x264_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, X264Methods, sizeof(X264Methods) / sizeof(X264Methods[0]));
}