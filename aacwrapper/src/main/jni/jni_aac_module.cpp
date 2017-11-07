#include "jni_aac_module.h"
#include "faac.h"
using namespace panda;

AVFormatContext *outputCtx;
AVStream        *audioStream;
AVFrame         *audioFrame;
uint8_t         *frame_buf;
AVPacket        audioPkt;

int             audioSize;

void aac_log(void* p,int v,const char* str,va_list list){

    va_list v12;
    char line[1024];
    static int print_prefix=1;

    va_copy(v12,list);
    av_log_format_line(p,v,str,v12,line,sizeof(line),&print_prefix);
    va_end(v12);
    LOGI("%s",line);
}

/************************************************************************/
/* 创建音频流                                                    */
/************************************************************************/
AVStream *addAudioStream(AVFormatContext *oc, enum AVCodecID codec_id){
	AVCodecContext *c;
	AVStream *st;
	AVCodec *codec;
	codec = avcodec_find_encoder(codec_id);
	if (!codec) {
		LOGE("audio codec not found\n");
		return NULL;
	}
	st = avformat_new_stream(oc, codec);
	st->id =0;
	c = st->codec;
	c->sample_fmt =AV_SAMPLE_FMT_S16;
	c->bit_rate = 64000;
	c->sample_rate = 16000;
	c->channels = av_get_channel_layout_nb_channels(c->channel_layout);
	c->channel_layout = AV_CH_LAYOUT_MONO;
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	return st;
}
/************************************************************************/
/* 打开音频流                                                                     */
/************************************************************************/
int openAudioStream(AVFormatContext *oc, AVStream *st){
	AVCodecContext *c;
	AVCodec *codec;
	int nb_samples, ret;
	AVDictionary *opt = NULL;
	c = st->codec;
	codec = avcodec_find_encoder(c->codec_id);
	if (!codec) {
		LOGE("codec not found\n");
		return -1;
	}
	c = st->codec;
	ret = avcodec_open2(c, codec, &opt);
	av_dict_free(&opt);
	if (ret < 0){
		LOGE("avcodec_open2 error %d", ret);
		return -1;
	}
	return 1;
}

/************************************************************************/
/* 初始化ffmpeg                                                                     */
/************************************************************************/
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_initRecord(JNIEnv* env, jobject thiz, jstring outputFile){
	char* chOutputFilepath;
	audioSize=0;
	chOutputFilepath = jstringToChar(env, outputFile);

	//注册自己的日志回调接口
	av_log_set_callback(aac_log);
	av_register_all();
	avformat_alloc_output_context2(&outputCtx, NULL, NULL, chOutputFilepath);
	if (!outputCtx){
		LOGE("Could not init ffmpeg env");
		return -1;
	}

	audioStream = addAudioStream(outputCtx, AV_CODEC_ID_AAC);
	if (audioStream == NULL|| openAudioStream(outputCtx, audioStream) < 1){
		LOGE("open media failed");
		return -1;
	}
	if (avio_open(&outputCtx->pb, chOutputFilepath, AVIO_FLAG_WRITE) < 0){
		LOGE("open file failed\n");
		return -1;
	}

	audioFrame = av_frame_alloc();
	audioFrame->nb_samples = audioStream->codec->frame_size;
	audioFrame->format = audioStream->codec->sample_fmt;

	audioSize = av_samples_get_buffer_size(NULL, audioStream->codec->channels, audioStream->codec->frame_size, audioStream->codec->sample_fmt, 1);
	frame_buf = (uint8_t *)av_malloc(audioSize);
	avcodec_fill_audio_frame(audioFrame, audioStream->codec->channels, audioStream->codec->sample_fmt, (const uint8_t*)frame_buf, audioSize, 1);;
	avformat_write_header(outputCtx, NULL);
	av_new_packet(&audioPkt, audioSize);
	return 1;
}
/**
 * 写入音频数据
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_writeAudioData(JNIEnv* env, jobject thiz, jbyteArray pcmData){
	jbyte* srcBuf = (jbyte*) env->GetByteArrayElements(pcmData, 0);
	audioFrame->data[0] = (uint8_t*)srcBuf;
	int got_frame = 0;
	int ret = avcodec_encode_audio2(audioStream->codec, &audioPkt, audioFrame, &got_frame);
	if (ret < 0){
		LOGE("encoder failed");
	}
	if (got_frame==1){
		audioPkt.stream_index = 0;
		ret = av_interleaved_write_frame(outputCtx, &audioPkt);
	}
	av_free_packet(&audioPkt);
	env->ReleaseByteArrayElements(pcmData, srcBuf, 0);
	return 1;
}

/**
 * 用于输出编码器中剩余的AVPacket。
 */
int flush_all_encoder(AVFormatContext *fmt_ctx, unsigned int stream_index){
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
		CODEC_CAP_DELAY))
		return 0;
	while (1) {
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_audio2(fmt_ctx->streams[stream_index]->codec, &enc_pkt, NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame){
			ret = 0;
			break;
		}
		LOGE("Flush %d Encoder: Succeed to encode 1 frame!\tsize:%5d\n", stream_index,enc_pkt.size);
		/* mux encoded frame */

		enc_pkt.stream_index = stream_index;
		ret = av_interleaved_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
			break;
	}
	return ret;
}

/************************************************************************/
/* 关闭                                                                                                                                                                            */
/************************************************************************/
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_closeRecord(JNIEnv* env, jobject thiz){
	flush_all_encoder(outputCtx,0);
	av_write_trailer(outputCtx);
	avcodec_close(audioStream->codec);
	avio_close(outputCtx->pb);
	avformat_free_context(outputCtx);
	av_free(frame_buf);
	return 0;
}

/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    pcm2Aac
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_pcm2Aac(
        JNIEnv *env,jobject thiz,jstring pcmpath,jstring aacpath){

    return aac::pcm2aac(jstringToStdstring(env,pcmpath), jstringToStdstring(env,aacpath));

}
/*
 * Class:     com_panda_org_aacwrapper_aacwrapper
 * Method:    pcm2AacDetail
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I;
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_aacwrapper_aacwrapper_pcm2AacDetail(
        JNIEnv *env,jobject thiz,jstring pcmpath,jstring aacpath){

    return aac::pcm2aacdetail(jstringToStdstring(env,pcmpath), jstringToStdstring(env,aacpath));

}


static JNINativeMethod AACMethods[] =
{
    {"initRecord", "(Ljava/lang/String;)I",(void *)Java_com_panda_org_aacwrapper_aacwrapper_initRecord},
    {"writeAudioData", "([B)I",(void *)Java_com_panda_org_aacwrapper_aacwrapper_writeAudioData},
    {"closeRecord", "(V)I",(void *)Java_com_panda_org_aacwrapper_aacwrapper_closeRecord},

    {"pcm2Aac", "(Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_panda_org_aacwrapper_aacwrapper_pcm2Aac},
    {"pcm2AacDetail", "(Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_panda_org_aacwrapper_aacwrapper_pcm2AacDetail}

};

int register_android_jni_aac_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, AACMethods, sizeof(AACMethods) / sizeof(AACMethods[0]));
}