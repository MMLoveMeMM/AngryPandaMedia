#include "jni_callback_module.h"
#include "jni_core_module.h"

jfieldID GetYUVBuffer(){

    JNIEnv *env = NULL;
    CN_ATTACH_JVM(env);

    jfieldID drain_yuvbuffer;
    jclass cls_user = env->GetObjectClass(JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore));
    drain_yuvbuffer = env->GetFieldID(cls_user, YUVBUFFER_IN_JAVA_OBJ_NAME,
        "Ljava/nio/ByteBuffer;");

    CN_DETACH_JVM(env);

    return drain_yuvbuffer;
}

void EncodeCallback(void* pdata,int datalen)
{
	JNIEnv *env = NULL;
	CN_ATTACH_JVM(env);

    jclass cls_user = env->GetObjectClass(JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore));
    jobject user_obj = JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore);
    jbyteArray pcmdata = env->NewByteArray(datalen);
    env->SetByteArrayRegion(pcmdata, 0, datalen,(jbyte*)pdata);
    jmethodID callback_mtd = env->GetMethodID(cls_user, "H264DataCallBackFunc", "([BI)V");
    env->CallVoidMethod(user_obj,callback_mtd,pcmdata,datalen);

	JNI_DELETE_LOCAL_REF(cls_user);
	CN_DETACH_JVM(env);

}
