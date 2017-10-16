#include "jni_codec_module.h"
#include "encodeproxy.h"
using namespace libffmpeg;
void EncodeCallback(void* pdata,int datalen)
{
	JNIEnv *env = NULL;
	CN_ATTACH_JVM(env);

    jclass cls_user = env->GetObjectClass(JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore));
    jobject user_obj = JNI_GET_CLASS_OBJ(JAVA_CLASS_IMCore);
    jbyteArray pcmdata = env->NewByteArray(datalen);
    env->SetByteArrayRegion(pcmdata, 0, datalen,(jbyte*)pdata);
    jmethodID callback_mtd = env->GetMethodID(cls_user, "YuvDataCallBackFunc", "([BI)V");
    env->CallVoidMethod(user_obj,callback_mtd,pcmdata,datalen);

	JNI_DELETE_LOCAL_REF(cls_user);
	CN_DETACH_JVM(env);

}

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    FfmpegInit
 * Signature: (IILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_FfmpegInit
  (JNIEnv *env, jobject thiz, jint width, jint height, jstring outpath){

    ffmpeg_encode_init(width,height,jstringToStdstring(env, outpath));
    ffmpeg_encode_cb_init(width,height,EncodeCallback);

    return 0;

  }

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    FfmpegOnFrame
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_FfmpegOnFrame
  (JNIEnv *env, jobject thiz, jbyteArray yuv_datas){

    unsigned char* src_data = (unsigned char*)env->GetByteArrayElements(yuv_datas, NULL);

    ffmpeg_encode_process(src_data);
    return 0;
  }

/*
 * Class:     com_pumpkin_org_ffmpegwrapper
 * Method:    FfmpegClose
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_pumpkin_org_ffmpegwrapper_FfmpegClose
  (JNIEnv *env, jobject thiz){
    ffmpeg_encode_close();
    return 0;
  }

