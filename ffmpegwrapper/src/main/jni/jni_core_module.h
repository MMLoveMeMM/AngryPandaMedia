//
// Created by rd0348 on 2017/10/11 0011.
//

#ifndef ANGRYPANDAAV_JNI_CORE_MODULE_H
#define ANGRYPANDAAV_JNI_CORE_MODULE_H

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <string>
#include "jni_utils_module.h"
#include "jni_codec_module.h"

#ifndef _Included_jni_comm
#define _Included_jni_comm

//global java vm and env
extern JavaVM *g_JavaVM;
#ifdef __cplusplus
#define CN_ATTACH_JVM(jni_env) \
    JNIEnv *tmp_env;\
    int env_status = g_JavaVM->GetEnv((void **)&tmp_env, JNI_VERSION_1_6); \
    g_JavaVM->AttachCurrentThread(&jni_env,NULL);

#define CN_DETACH_JVM(jni_env) if( env_status == JNI_EDETACHED ){ g_JavaVM->DetachCurrentThread(); }
#else
#define CN_DETACH_JVM(jni_env) \
    JNIEnv *tmp_env;\
    int env_status = (*g_JavaVM)->GetEnv(android_jvm, (&tmp_env), JNI_VERSION_1_6); \
    (*g_JavaVM)->AttachCurrentThread(g_JavaVM, &jni_env,NULL);

#define DETACH_JVM(jni_env) if( env_status == JNI_EDETACHED ){ (*g_jvm)->DetachCurrentThread(android_jvm); }
#endif

typedef enum _JAVA_CLASS_TYPE_ {
	JAVA_CLASS_STRING = 0, JAVA_CLASS_INTEGER, JAVA_CLASS_IMCore, JAVA_CLASS_MAX

} JAVA_CLASS_TYPE;

extern const char *g_java_class_name[];
extern jobject g_java_class_obj[JAVA_CLASS_MAX];
extern jmethodID g_java_class_construct[JAVA_CLASS_MAX];

#define JNI_GET_CLASS_NAME(type)                    g_java_class_name[(type)]
#define JNI_GET_CLASS_OBJ(type)                        g_java_class_obj[(type)]
#define JNI_GET_CLASS_CONSTRUCT_METHODE(type)        g_java_class_construct[(type)]


//cnPalmchat static method
typedef enum _JAVA_STATIC_METHOD_ {
	JAVA_STATIC_METHOD_RESULT = 0,
	JAVA_STATIC_METHOD_SYS,
	JAVA_STATIC_METHOD_getNetworkState,
	JAVA_STATIC_METHOD_cnGetContactInfo,
	JAVA_STATIC_METHOD_MAX
} JAVA_STATIC_METHOD;

extern jmethodID g_static_method[JAVA_STATIC_METHOD_MAX];
#define JNI_GET_STATIC_METHOD(type)                    g_static_method[(type)]
#define JNI_SET_STATIC_METHOD(type, method)         g_static_method[(type)] = (method)


#define JNI_GET_FIELD_ARRAY_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "Ljava/util/ArrayList;")
#define JNI_GET_FIELD_ARRAY_ID_NAME(name)    env->GetFieldID(jclazz, (const char*)name,  "Ljava/util/ArrayList;")

#define JNI_GET_FIELD_OBJ_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "Ljava/lang/Object;")
#define JNI_GET_FIELD_OBJ_ID_NAME(name)        env->GetFieldID(jclazz, (const char*)name,  "Ljava/lang/Object;")

#define JNI_GET_FIELD_STR_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "Ljava/lang/String;")
#define JNI_GET_FIELD_STR_ID_NAME(name)        env->GetFieldID(jclazz, (const char*)name,  "Ljava/lang/String;")

#define JNI_GET_FIELD_BYTE_ARR_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "[B")
#define JNI_GET_FIELD_BYTE_ARR_ID_NAME(name)        env->GetFieldID(jclazz, (const char*)name,  "[B")

#define JNI_GET_FIELD_INT_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "I")
#define JNI_GET_FIELD_INT_ID_NAME(name)        env->GetFieldID(jclazz, (const char*)name,  "I")

#define JNI_GET_FIELD_BOOLEAN_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "Z")
#define JNI_GET_FIELD_BOOLEAN_ID_NAME(name)        env->GetFieldID(jclazz, (const char*)name,  "Z")

#define JNI_GET_FIELD_LONG_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "J")
#define JNI_GET_FIELD_LONG_ID_NAME(name)    env->GetFieldID(jclazz, (const char*)name,  "J")

#define JNI_GET_FIELD_DOUBLE_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "D")
#define JNI_GET_FIELD_DOUBLE_ID_NAME(name)    env->GetFieldID(jclazz, (const char*)name,  "D")

#define JNI_GET_FIELD_BYTE_ID(jclazz, name)    env->GetFieldID(jclazz, (const char*)name,  "B")
#define JNI_GET_FIELD_BYTE_ID_NAME(name)    env->GetFieldID(jclazz, (const char*)name,  "B")

//
#define JNI_GET_METHOD_ID(name, sig)        env->GetMethodID(jclazz, name, sig)


//get value
#define JNI_GET_OBJ_FIELD(obj, methodId)    env->GetObjectField(obj, methodId)
#define JNI_GET_OBJ_FIELD_ID(methodId)        env->GetObjectField(obj, methodId)

#define JNI_GET_INT_FIELD(obj, methodId)    env->GetIntField(obj, methodId)
#define JNI_GET_INT_FIELD_ID(methodId)        env->GetIntField(obj, methodId)

#define JNI_GET_LONG_FILED(obj, methodId)    env->GetLongField(obj, methodId)
#define JNI_GET_LONG_FILED_ID(methodId)        env->GetLongField(obj, methodId)

#define JNI_GET_DOUBLE_FILED(obj, methodId)    env->GetDoubleField(obj, methodId)
#define JNI_GET_DOUBLE_FILED_ID(methodId)    env->GetDoubleField(obj, methodId)

#define JNI_GET_BYTE_FILED(obj, methodId)    env->GetByteField(obj, methodId)
#define JNI_GET_BYTE_FILED_ID(methodId)        env->GetByteField(obj, methodId)

#define JNI_GET_BOOLEAN_FIELD(obj, methodId)    env->GetBooleanField(obj, methodId)
#define JNI_GET_BOOLEAN_FIELD_ID(methodId)        env->GetBooleanField(obj, methodId)

#define JNI_GET_UTF_STR(str, str_obj, type)        if(NULL != str_obj){\
                                                str = (type)env->GetStringUTFChars(str_obj, NULL);\
                                                }else{\
                                                str = (type)NULL;\
                                                }
#define JNI_GET_UTF_CHAR(str, str_obj)        JNI_GET_UTF_STR(str, str_obj, CHAR*);


#define JNI_RELEASE_STR_STR(str, str_obj)        if( NULL != str_obj && NULL != str){\
                                                    env->ReleaseStringUTFChars(str_obj, (const char*)str);\
                                                }

// for std::string begin

#define NULLSTR ""

#define JNI_GET_UTF_STR_EX(str, str_obj, type)    if(NULL != str_obj){\
                                                str = std::string((type)env->GetStringUTFChars(str_obj, NULL));\
                                                }else{\
                                                /*str = std::string((type)NULL);*/\
                                                }
#define JNI_GET_UTF_CHAR_EX(str, str_obj)        JNI_GET_UTF_STR_EX(str, str_obj, CHAR*);


#define JNI_RELEASE_STR_STR_EX(str, str_obj)    if( NULL != str_obj && !str.empty()){\
                                                    env->ReleaseStringUTFChars(str_obj, (const char*)str.c_str());\
                                                }

// for std::string end
#define JNI_GET_BYTE_ARRAY(b, array)            if( NULL != array){\
                                                    b = (INT8S*)env->GetByteArrayElements(array, (jboolean*)NULL);\
                                                    }else{\
                                                        b = NULL;\
                                                    }
#define JNI_RELEASE_BYTE_ARRAY(b, array)        if( NULL != array){\
                                                    env->ReleaseByteArrayElements(array, (jbyte*)b, JNI_ABORT);\
                                                }

#define JNI_DELETE_LOCAL_REF(obj)                   if( NULL != obj){\
                                                env->DeleteLocalRef(obj);\
                                            }
#define JNI_GET_NEW_UTF_STR(key, value)        if( NULL != key && support_comm_is_utf8((CHAR*)(key))){\
                                                value = env->NewStringUTF((const char*)key);\
                                            }else{\
                                                value = NULL;\
                                            }

void cnSetInt(JNIEnv *jni_env, jclass jclazz, jobject obj, const char *sig, jint value);
#define JNI_SET_INT(sig, value)  cnSetInt(env, jclazz, obj, sig, value)
#define JNI_SET_INT_ID(fieldId, value)  env->SetIntField(obj, fieldId, value)

void cnSetString(JNIEnv *jni_env, jclass jclazz, jobject obj, const char *sig, const char *value);
void cnSetStringId(JNIEnv *env, jclass jclazz, jobject obj, jfieldID fieldId, char *value);
#define JNI_SET_STRING(sig, value)                cnSetString(env, jclazz, obj, sig, value)
#define JNI_SET_STRING_ID(fieldId, value)        cnSetStringId(env, jclazz, obj, fieldId, value)

void cnSetLong(JNIEnv *env, jclass jclazz, jobject obj, const char *sig, jlong value);
void cnSetLongId(JNIEnv *env, jclass jclazz, jobject obj, jfieldID fieldId, jlong value);
#define JNI_SET_LONG(sig, value)                cnSetLong(env, jclazz, obj, sig, value)
#define JNI_SET_LONG_ID(fieldId, value)            cnSetLongId(env, jclazz, obj, fieldId, value)

void cnSetObject(JNIEnv *env, jclass jclazz, jobject obj, const char *sig, jobject value);
#define JNI_SET_OBJECT(sig, value)                cnSetObject(env, jclazz, obj, sig, value)
#define JNI_SET_OBJECT_ID(fieldId, value)        env->SetObjectField(obj, fieldId, value)

void cnSetByte(JNIEnv *env, jclass jclazz, jobject obj, const char *sig, jbyte value);
#define JNI_SET_BYTE(sig, value)                cnSetByte(env, jclazz, obj, sig, value)
#define JNI_SET_BYTE_ID(fieldId, value)            env->SetByteField(obj, fieldId, value)

void cnSetDouble(JNIEnv *env, jclass jclazz, jobject obj, const char *sig, jdouble value);
//pointer to object

jobject cnAllToObj(JNIEnv *env, JAVA_CLASS_TYPE type, void *ptr);
void *cnAllToPtr(JNIEnv *env, JAVA_CLASS_TYPE type, jobject obj);

jstring CharToJstring(JNIEnv *env, const char *c_char);
std::string jstringToStdstring(JNIEnv *env, jstring jstr);
char *jstringToChar(JNIEnv *env, jstring jstr);

int jniRegisterNativeMethods(JNIEnv *env, jclass clazz, const JNINativeMethod *gMethods,
							 int numMethods);

std::string GetString(char *c_char);


#endif

#endif //ANGRYPANDAAV_JNI_CORE_MODULE_H
