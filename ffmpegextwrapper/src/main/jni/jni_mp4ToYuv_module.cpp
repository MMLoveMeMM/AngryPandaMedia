#include "jni_mp4ToYuv_module.h"
#include <string>
#include <stdio.h>

using namespace std;
/*
* 传入线程两个资源路径参数
*/
struct respath{
    char* msrcpath;
    char* mdestpath;
};

typedef struct respath RESPATH;
RESPATH *path;
RESPATH rpath;

/*
* 开辟一个native线程
*/
pthread_mutex_t mutexlock;
pthread_t pth;
/*
* 自定义ffmpeg 日志
*/
void ffmpeg_log(void *ptr, int level, const char* fmt, va_list vl){
    FILE *fp=fopen("/storage/emulated/0/ffmpeg_log.txt","a+");
    if(fp){
        vfprintf(fp,fmt,vl);
        fflush(fp);
        fclose(fp);
    }
}

int encode(char* srcpath,char* destpath){

    AVFormatContext *pFormatCtx;
    int             i, videoindex;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame *pFrame,*pFrameYUV; // 编码后存放的缓存
    uint8_t *out_buffer;
    AVPacket *packet; // 编码前
    int y_size;
    int ret, got_picture;
    struct SwsContext *img_convert_ctx;
    FILE *fp_yuv;
    int frame_cnt;
    clock_t time_start, time_finish;
    double  time_duration = 0.0;

    char input_str[500]={0};
    char output_str[500]={0};
    char info[1000]={0};
    sprintf(input_str,"%s",srcpath);
    sprintf(output_str,"%s",destpath);

    /*
    * 设置日志回写
    */
    av_log_set_callback(ffmpeg_log);

    /*
    * 注册编码,解码器等
    */
    av_register_all();
    /*
    * network不是必要的
    */
    avformat_network_init();
    /*
    * 上下格式报文
    */
    pFormatCtx = avformat_alloc_context();
    /*
    *
    */
    if(avformat_open_input(&pFormatCtx,input_str,NULL,NULL)!=0){
        LOGE("Couldn't open input stream.\n");
        return -1;
    }
    /*
    * 获取文件或者流的数据信息
    */
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        LOGE("Couldn't find stream information.\n");
        return -1;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    if(videoindex==-1){
        LOGE("Couldn't find a video stream.\n");
        return -1;
    }
    /*
    * 获得编码器上下报文
    */
    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    /*
    * 获取可用编码器
    */
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        LOGE("Couldn't find Codec.\n");
        return -1;
    }
    /*
    * 打开编码
    */
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        LOGE("Couldn't open codec.\n");
        return -1;
    }

    /*
    * 分配内存区
    */
    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
                         AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);// 同avpicture_fill


    packet=(AVPacket *)av_malloc(sizeof(AVPacket));

    /*
    * ffmpeg SWS 模块
    * 后面缩放用的sws上下报文
    */
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);


    sprintf(info,   "[Input     ]%s\n", input_str);
    sprintf(info, "%s[Output    ]%s\n",info,output_str);
    sprintf(info, "%s[Format    ]%s\n",info, pFormatCtx->iformat->name);
    sprintf(info, "%s[Codec     ]%s\n",info, pCodecCtx->codec->name);
    sprintf(info, "%s[Resolution]%dx%d\n",info, pCodecCtx->width,pCodecCtx->height);

    /*
    * 保存文件路径
    */
    fp_yuv=fopen(output_str,"wb+");
    if(fp_yuv==NULL){
        printf("Cannot open output file.\n");
        return -1;
    }

    frame_cnt=0;
    time_start = clock();

    /*
    * 读取编码/转码后的数据
    */
    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==videoindex){
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0){
                LOGE("Decode Error.\n");
                return -1;
            }
            if(got_picture){
                /*
                * 做个缩放
                */
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                          pFrameYUV->data, pFrameYUV->linesize);

                y_size=pCodecCtx->width*pCodecCtx->height;
                fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
                fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
                fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
                //Output info
                char pictype_str[10]={0};
                switch(pFrame->pict_type){
                    case AV_PICTURE_TYPE_I:sprintf(pictype_str,"I");break;
                    case AV_PICTURE_TYPE_P:sprintf(pictype_str,"P");break;
                    case AV_PICTURE_TYPE_B:sprintf(pictype_str,"B");break;
                    default:sprintf(pictype_str,"Other");break;
                }
                LOGI("Frame Index: %5d. Type:%s",frame_cnt,pictype_str);
                frame_cnt++;
            }
        }
        //av_free_packet(packet); // 这个函数已不建议使用,程序编译会有警告的
        av_packet_unref(packet);
    }

    /*
    * 由于编码后,可能还有剩余的几帧没有读取出来
    * 所以在编码完成后,去缓冲区看看是否还有剩余的帧没有取出,以防止尾帧丢失的现象发生
    * 当然,如果要求不高,能够接受丢尾帧,可以flush缓冲区
    */
    while (1) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0)
            break;
        if (!got_picture)
            break;
        sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                  pFrameYUV->data, pFrameYUV->linesize);
        int y_size=pCodecCtx->width*pCodecCtx->height;
        fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y
        fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
        fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
        //Output info
        char pictype_str[10]={0};
        switch(pFrame->pict_type){
            case AV_PICTURE_TYPE_I:sprintf(pictype_str,"I");break;
            case AV_PICTURE_TYPE_P:sprintf(pictype_str,"P");break;
            case AV_PICTURE_TYPE_B:sprintf(pictype_str,"B");break;
            default:sprintf(pictype_str,"Other");break;
        }
        LOGI("Frame Index: %5d. Type:%s",frame_cnt,pictype_str);
        frame_cnt++;
    }
    time_finish = clock();
    time_duration=(double)(time_finish - time_start);

    sprintf(info, "%s[Time      ]%fms\n",info,time_duration);
    sprintf(info, "%s[Count     ]%d\n",info,frame_cnt);

    sws_freeContext(img_convert_ctx);
    fclose(fp_yuv);
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
    return 0;
}

void *threadencode(void *st){

    path=(struct respath*)st;

    /*
    * 这个地方加锁不是必要的
    * 没有涉及到共享变量
    */
    pthread_mutex_lock (&mutexlock);
    encode(path->msrcpath,path->mdestpath);
    pthread_mutex_unlock (&mutexlock);

    pthread_mutex_destroy(&mutexlock);
    pthread_exit(NULL);
}

void encode_thread(char* srcp,char* destp){


    LOGI("encode_thread ... ");

    rpath.msrcpath=srcp;
    rpath.mdestpath=destp;

    pthread_mutex_init(&mutexlock, NULL);
    pthread_create(&pth, NULL, threadencode, (void *)&rpath);

}

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    decode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_decode(
        JNIEnv *env,jobject thiz, jstring jsrcpath, jstring jdestpath){

    LOGI("path destpath .........");
    char* src = jstringToChar(env,jsrcpath);
    char* dt = jstringToChar(env,jdestpath);

    encode_thread(src,dt); // native层要使用线程,java层不需要放到单独的线程中

    //encode(src,dt); //用这个要在java层放入线程中处理

    return 0;
}

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    avcodecinfo
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_avcodecinfo(
        JNIEnv *env,jobject thiz){

    char info[40000]={0};

    av_register_all();

    AVCodec *c_temp = av_codec_next(NULL);

    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%sdecode:", info);
        } else {
            sprintf(info, "%sencode:", info);
        }
        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, c_temp->name);
        c_temp = c_temp->next;
    }

    return env->NewStringUTF(info);/*CharToJstring(env, info);*///

}

static JNINativeMethod MP4TOYUVMethods[] =
{
    {"decode", "(Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_decode},
    {"avcodecinfo", "()Ljava/lang/String;",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_avcodecinfo}
};

int register_android_jni_mp4toyuv_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, MP4TOYUVMethods, sizeof(MP4TOYUVMethods) / sizeof(MP4TOYUVMethods[0]));
}
