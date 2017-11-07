#include "jni_aac_module.h"


AVFormatContext *fmt_ctx;
AVStream *audioStream;
AVFrame *audioFrame;
uint8_t *frame_buf;
AVPacket audioPkt;

int audioSize;

void aac_log(void* p,int v,const char* str,va_list list){

    va_list v12;
    char line[1024];
    static int print_prefix=1;

    va_copy(v12,list);
    av_log_format_line(p,v,str,v12,line,sizeof(line),&print_prefix);
    va_end(v12);
    LOGI("%s",line);
}

AVStream *addAudioStream(AVFormatContext *oc,enum AVCodecID codec_id){

    AVCodecContext *c;
    AVStream *st;
    AVCodec *codec;
    codec=avcodec_find_encoder(codec_id);
    if(!codec){
        LOGE("audio codec not found !");
        return NULL;
    }

    st=avformat_new_stream(oc,codec);

    st->id=0;
    c=st->codec;
    c->sample_fmt=AV_SAMPLE_FMT_S16;
    c->bit_rate=64000;
    c->sample_rate=16000;
    c->channels=av_get_channel_layout_nb_channels(c->channel_layout);
    c->channel_layout=AV_CH_LAYOUT_MONO;
    if(oc->oformat->flags & AVFMT_GLOBALHEADER){
        c->flags |=CODEC_FLAG_GLOBAL_HEADER;
    }
    return st;
}

int openAudioStream(AVFormatContext *oc,AVStream *st){

    AVCodecContext *c;
    AVCodec *codec;
    int nb_samples,ret;
    AVDictionary *opt=NULL;
    c=st->codec;
    codec=avcodec_find_encoder(c->codec_id);
    if(!codec){
        LOGE("avcodec open2 error %d",ret);
        return -1;
    }
    return 0;
}

int flush_all_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index){

    int ret;
    int got_frame;
    AVPacket enc_pkt;

    if(!(fmt_ctx->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY)){
        return 0;
    }

    while(1){
        enc_pkt.data=NULL;
        enc_pkt.size=0;
        av_init_packet(&enc_pkt);
        ret=avcodec_encode_audio2(fmt_ctx->streams[stream_index]->codec,&enc_pkt,NULL,&got_frame);
        av_frame_free(NULL);
        if(ret<0){
            break;
        }
        if(!got_frame){
            ret=0;
            break;
        }
        LOGE("flush %d encoder : sucess to encode 1 frame !",stream_index);

        enc_pkt.stream_index=stream_index;
        ret= av_interleaved_write_frame(fmt_ctx, &enc_pkt);
        if(ret<0){
            break;
        }

    }

    return ret;

}
/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    initRecord
 * Signature: (Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_initRecord(
        JNIEnv *env,jobject thiz,jstring outpath){
    int ret=0;
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_initRecord entry !");

    av_log_set_callback(aac_log);

    av_register_all();
    ret = avformat_alloc_output_context2(&fmt_ctx,NULL,NULL,jstringToChar(env,outpath));
    if(ret<0){
        LOGE("could not init ffmpeg !");
        return -1;
    }

    audioStream = addAudioStream(fmt_ctx,AV_CODEC_ID_AAC);
    if(audioStream == NULL || openAudioStream(fmt_ctx,audioStream)<0){
        LOGE("open media failed");
        return -1;
    }
    if(avio_open(&fmt_ctx->pb,jstringToChar(env,outpath),AVIO_FLAG_WRITE)<0){
        LOGE("open file failed");
        return -1;
    }

    audioFrame=av_frame_alloc();
    audioFrame->nb_samples=audioStream->codec->frame_size;
    audioFrame->format=audioStream->codec->sample_fmt;

    audioSize=av_samples_get_buffer_size(NULL,audioStream->codec->channels,audioStream->codec->frame_size, audioStream->codec->sample_fmt, 1);
    frame_buf=(uint8_t *)av_malloc(audioSize);
    avcodec_fill_audio_frame(audioFrame,audioStream->codec->channels, audioStream->codec->sample_fmt, (const uint8_t*)frame_buf, audioSize, 1);

    avformat_write_header(fmt_ctx,NULL);
    av_new_packet(&audioPkt,audioSize);

    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_initRecord exit !");

    return 0;
}

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    writeAudioData
 * Signature: ([B)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData(
        JNIEnv *env,jobject thiz,jbyteArray pcmdata){

    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData entry !");
    jbyte* srcBuf=(jbyte*)env->GetByteArrayElements(pcmdata, 0);
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData 777 !");
    jsize oldsize = env->GetArrayLength(pcmdata);
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData 888 !");
    //memcpy(audioFrame->data[0],(unsigned char*)srcBuf,(int)oldsize);
    audioFrame->data[0]=(unsigned char*)srcBuf;
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData 999 !");
    int got_frame=0;
    int ret=avcodec_encode_audio2(audioStream->codec,&audioPkt,audioFrame,&got_frame);
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData 1000 !");
    if(ret<0){
        LOGE("encoder failed !");
        return ret;
    }
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData 1001 !");
    if(got_frame==1){
        audioPkt.stream_index=0;
        ret=av_interleaved_write_frame(fmt_ctx,&audioPkt);
    }
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData 1002 !");
    av_free_packet(&audioPkt);
    env->ReleaseByteArrayElements(pcmdata, (jbyte*)srcBuf, 0);
    LOGI("Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData exit !");
    return 0;
}

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    recordClose
 * Signature: (V)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_recordClose(
        JNIEnv *env,jobject thiz){

    flush_all_encoder(fmt_ctx,0);
    av_write_trailer(fmt_ctx);
    avcodec_close(audioStream->codec);

    avio_close(fmt_ctx->pb);
    avformat_free_context(fmt_ctx);
    av_free(frame_buf);

}

static JNINativeMethod AACMethods[] =
{
    {"initRecord", "(Ljava/lang/String;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_initRecord},
    {"writeAudioData", "([B)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_writeAudioData},
    {"recordClose", "(V)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_recordClose},
};

int register_android_jni_aac_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, AACMethods, sizeof(AACMethods) / sizeof(AACMethods[0]));
}