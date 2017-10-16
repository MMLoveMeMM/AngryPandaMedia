#include "encode.h"
#include <stdio.h>
#include <time.h>

using namespace libffmpeg;
using namespace std;
encode::encode(){
}

encode::~encode(){
}

void encode::custom_log(void *ptr, int level, const char* fmt, va_list vl){

    /*
    * 这个地方后面换成xlog的
    */
    FILE *fp=fopen("/storage/emulated/0/av_log.txt","a+");
    if(fp){
        vfprintf(fp,fmt,vl);
        fflush(fp);
        fclose(fp);
    }

}

void encode::init(int width,int height){



}

void encode::init_o(int width,int height,const std::string& out_path){

    yuv_width=width;
    yuv_height=height;
    /*
    * yuv 分量的体量,要根据实际编码的做不通处理
    */
    y_length=width*height;
    uv_length=width*height/4;

    av_log_set_callback(custom_log);

    av_register_all();

    avformat_alloc_output_context2(&ofmt_ctx,NULL,"flv",out_path.c_str());

    pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if(!pCodec){
        //LOGI("can not find encoder !");
        return;
    }

    pCodecCtx=avcodec_alloc_context3(pCodec);
    pCodecCtx->pix_fmt=PIX_FMT_YUV420P;
    pCodecCtx->width=width;
    pCodecCtx->height=height;
    pCodecCtx->time_base.num=1;
    pCodecCtx->time_base.den=30;
    pCodecCtx->bit_rate=800000;
    pCodecCtx->gop_size=300;

    if(ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER){
        pCodecCtx->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    //H264 codec param
    pCodecCtx->me_range=16;
    pCodecCtx->max_qdiff=4;
    pCodecCtx->qcompress=0.6;
    pCodecCtx->qmin=10;
    pCodecCtx->qmax=51;

    AVDictionary *param;
    av_dict_set(&param,"preset","ultrafast",0);
    av_dict_set(&param,"tune","zerolatency",0);

    if(avcodec_open2(pCodecCtx,pCodec,&param)<0){
        //LOGI("fail to open encoder !");
        return;
    }

    video_st = avformat_new_stream(ofmt_ctx,pCodec);
    if(video_st==NULL){
        return;
    }

    video_st->time_base.num=1;
    video_st->time_base.den=30;
    video_st->codec=pCodecCtx;

    if(avio_open(&ofmt_ctx->pb,out_path.c_str(),AVIO_FLAG_READ_WRITE)<0){
        return ;
    }

    avformat_write_header(ofmt_ctx,NULL);
    start_time=av_gettime();

}

void encode::onframecb(unsigned char* data,OnCallBackFrame onframe){

}

void encode::onframe(unsigned char* in){

    int ret;
    int enc_got_frame=0;
    int i=0;

    pFrameYUV = avcodec_alloc_frame();

    uint8_t *out_buffer=(uint8_t*)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
    avpicture_fill((AVPicture*)pFrameYUV,out_buffer,PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height);

    //安卓摄像头数据为NV21格式，此处将其转换为YUV420P格式
    //jbyte* in=(jbyte*)env->GetByteArrayElements(yuv,0);
    //memcpy(pFrameYUV->data[0],in,y_length);
    memcpy(pFrameYUV->data[0],in,y_length);
    for(i=0;i<uv_length;i++){
        *(pFrameYUV->data[2]+i)=*(in+y_length+i*2);
        *(pFrameYUV->data[1]+i)=*(in+y_length+i*2+1);
    }

    pFrameYUV->format=AV_PIX_FMT_YUV420P;
    pFrameYUV->width=yuv_width;
    pFrameYUV->height=yuv_height;

    enc_pkt.data=NULL;
    enc_pkt.size=0;
    av_init_packet(&enc_pkt);
    ret=avcodec_encode_video2(pCodecCtx,&enc_pkt,pFrameYUV,&enc_got_frame);
    av_frame_free(&pFrameYUV);

    if(enc_got_frame==1){

        framecnt++;
        enc_pkt.stream_index=video_st->index;

        AVRational time_base=ofmt_ctx->streams[0]->time_base;
        AVRational r_framerate1={60,2};
        AVRational time_base_q = {1,AV_TIME_BASE};

        int64_t calc_duration = (double)(AV_TIME_BASE)*(1/av_q2d(r_framerate1));

        enc_pkt.pts = av_rescale_q(framecnt*calc_duration,time_base_q,time_base);
        enc_pkt.dts =enc_pkt.pts;
        enc_pkt.duration=av_rescale_q(calc_duration,time_base_q,time_base);
        enc_pkt.pos=-1;

        //Delay
        int64_t pts_time = av_rescale_q(enc_pkt.dts,time_base,time_base_q);
        int64_t now_time = av_gettime()-start_time;
        if(pts_time>now_time){
            av_usleep(pts_time-now_time);
        }
        ret=av_interleaved_write_frame(ofmt_ctx,&enc_pkt);
        av_free_packet(&enc_pkt);

    }

}

void encode::flushcb(OnCallBackFrame onframe){

}

void encode::flush(){

    int ret;
    int got_frame;
    AVPacket enc_pkt;
    if(!(ofmt_ctx->streams[0]->codec->codec->capabilities & CODEC_CAP_DELAY)){
        return;
    }

    while(1){

        enc_pkt.data=NULL;
        enc_pkt.size=0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2(ofmt_ctx->streams[0]->codec,&enc_pkt,NULL,&got_frame);
        if(ret<0){
            break;
        }
        if(!got_frame){
            ret=0;
            break;
        }

        AVRational time_base=ofmt_ctx->streams[0]->time_base;
        AVRational r_framerate1={60,2};
        AVRational time_base_q ={1,AV_TIME_BASE};

        int64_t calc_duration=(double)(AV_TIME_BASE)*(1/av_q2d(r_framerate1));
        enc_pkt.pts=av_rescale_q(framecnt*calc_duration,time_base_q,time_base);
        enc_pkt.dts=enc_pkt.pts;
        enc_pkt.duration=av_rescale_q(calc_duration,time_base_q,time_base);

        enc_pkt.pos=-1;
        framecnt++;
        ofmt_ctx->duration=enc_pkt.duration*framecnt;

        ret=av_interleaved_write_frame(ofmt_ctx,&enc_pkt);
        if(ret<0){
            break;
        }

    }

    av_write_trailer(ofmt_ctx);

}

void encode::close(){

    if(video_st){
        avcodec_close(video_st->codec);
    }
    avio_close(ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

}