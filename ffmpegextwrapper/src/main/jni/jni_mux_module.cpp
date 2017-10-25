#include "jni_mux_module.h"

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    demuxMp4
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_demuxMp4(
        JNIEnv *env,jobject thiz, jstring spath, jstring vedpath, jstring audpath){

    char* srcpath = jstringToChar(env,spath);
    char* h264path = jstringToChar(env,vedpath);//输出视频文件路径
    char* accpath = jstringToChar(env,audpath);//输出音频文件路径

    int i, vid_idx, aud_idx;
    FILE *fp_vides = NULL, *fp_audes = NULL;
    AVFormatContext *pFormatCtx = NULL;
    AVPacket pkt;
    av_register_all();
    avformat_open_input(&pFormatCtx, srcpath, NULL, NULL);
    avformat_find_stream_info(pFormatCtx, NULL);
    fp_vides = fopen(h264path, "wb");
    fp_audes = fopen(accpath, "wb");
    // 1, handle stream info
    for (i=0; i<pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type ==AVMEDIA_TYPE_VIDEO)
            vid_idx = i;
        else if (pFormatCtx->streams[i]->codec->codec_type ==AVMEDIA_TYPE_AUDIO)
            aud_idx = i;
        else
            ;//such as subtitile
    }
    while (av_read_frame(pFormatCtx, &pkt) >= 0)
    {
        // 2, handle pkt data
        if (pkt.stream_index == vid_idx)
            fwrite(pkt.data, pkt.size, 1, fp_vides);
        else if (pkt.stream_index == aud_idx)
            fwrite(pkt.data, pkt.size, 1, fp_audes);
        else
            ;// such as subtitile
        av_packet_unref(&pkt);
    }
    fclose(fp_vides);
    fclose(fp_audes);
    avformat_close_input(&pFormatCtx);
    return 0;

}

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    demuxMp4ext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_demuxMp4ext(
        JNIEnv *env,jobject thiz, jstring spath, jstring vpath, jstring apath){

    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx_audio = NULL, *ofmt_ctx_video = NULL;
    AVPacket pkt;
    AVOutputFormat *ofmt_audio = NULL, *ofmt_video = NULL;

    int videoindex = -1, audioindex = -1;
    int frame_index = 0;

    const char *in_filename = jstringToChar(env,spath);//"荷塘月色.mkv";
    const char *out_filename_video = jstringToChar(env,vpath);//"荷塘月色.h264";
    const char *out_filename_audio = jstringToChar(env,apath);//"荷塘月色.aac";

    LOGI("in_filename : %s",in_filename);
    LOGI("out_filename_video : %s",out_filename_video);
    LOGI("out_filename_audio : %s",out_filename_audio);

    av_register_all();

    if (avformat_open_input(&ifmt_ctx, in_filename, NULL, NULL) != 0)
    {
        LOGI("Couldn't open input file.\n");
        return -1;
    }
    if (avformat_find_stream_info(ifmt_ctx, NULL) < 0)
    {
        LOGI("Couldn't find stream information.\n");
        return -1;
    }

    // Allocate an AVFormatContext for an output format
    avformat_alloc_output_context2(&ofmt_ctx_video, NULL, "h264", out_filename_video);
    if (!ofmt_ctx_video)
    {
        LOGI("Couldn't create output context ** .\n");
        return -1;
    }
    ofmt_video = ofmt_ctx_video->oformat;

    avformat_alloc_output_context2(&ofmt_ctx_audio, NULL, NULL, out_filename_audio);
    if (!ofmt_ctx_audio)
    {
        LOGI("Couldn't create output context && .\n");
        return -1;
    }
    ofmt_audio = ofmt_ctx_audio->oformat;

    // 一般情况下nb_streams只有两个流，就是streams[0],streams[1]，分别是音频和视频流，不过顺序不定
    for (int i = 0; i < ifmt_ctx->nb_streams; i++)
    {
        AVFormatContext *ofmt_ctx;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVStream *out_stream = NULL;

        // 根据音视频类型，根据输入流创建输出流
        if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoindex = i;
            out_stream = avformat_new_stream(ofmt_ctx_video, in_stream->codec->codec);
            ofmt_ctx = ofmt_ctx_video;
        }
        else if (ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioindex = i;
            out_stream = avformat_new_stream(ofmt_ctx_audio, in_stream->codec->codec);
            ofmt_ctx = ofmt_ctx_audio;
        }
        else
            break;

        if (!out_stream)
        {
            LOGI("Failed allocating output stream.\n");
            return -1;
        }
        // 复制到输出流
        if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0)
        {
            LOGI("Failed to copy context from input to output stream codec context.\n");
            return -1;
        }
        out_stream->codec->codec_tag = 0;

        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    // Print detailed information about the input or output format
    LOGI("\n==============Input Video=============\n");
    av_dump_format(ifmt_ctx, 0, in_filename, 0);
    LOGI("\n==============Output Video============\n");
    av_dump_format(ofmt_ctx_video, 0, out_filename_video, 1);
    LOGI("\n==============Output Audio============\n");
    av_dump_format(ofmt_ctx_audio, 0, out_filename_audio, 1);
    LOGI("\n======================================\n");

    // 打开输出文件
    if (!(ofmt_video->flags & AVFMT_NOFILE))
    {
        if (avio_open(&ofmt_ctx_video->pb, out_filename_video, AVIO_FLAG_WRITE) < 0)
        {
            LOGI("Couldn't open output vedio file '%s'", out_filename_video);
            return -1;
        }
    }
    if (!(ofmt_audio->flags & AVFMT_NOFILE))
    {
        if (avio_open(&ofmt_ctx_audio->pb, out_filename_audio, AVIO_FLAG_WRITE) < 0)
        {
            LOGI("Couldn't open output audio file '%s'", out_filename_audio);
            return -1;
        }
    }

    // 写文件头部
    if (avformat_write_header(ofmt_ctx_video, NULL) < 0)
    {
        LOGI("Error occurred when opening video output file\n");
        return -1;
    }
    if (avformat_write_header(ofmt_ctx_audio, NULL) < 0)
    {
        LOGI("Error occurred when opening video output file\n");
        return -1;
    }

    // : 下面的需要注意
    // 分离某些封装格式（例如MP4/FLV/MKV等）中的H.264的时候，需要首先写入SPS和PPS，否则会导致分离出来的数据
    // 没有SPS、PPS而无法播放。使用ffmpeg中名称为“h264_mp4toannexb”的bitstream filter处理
    AVBitStreamFilterContext *h264bsfc = av_bitstream_filter_init("h264_mp4toannexb");

    while (1)
    {
        AVFormatContext *ofmt_ctx;
        AVStream *in_stream, *out_stream;

        //Get an AVPacket
        if (av_read_frame(ifmt_ctx, &pkt) < 0)
            break;
        in_stream = ifmt_ctx->streams[pkt.stream_index];
        // stream_index标识该AVPacket所属的视频/音频流
        if (pkt.stream_index == videoindex)
        {
            // 前面已经通过avcodec_copy_context()函数把输入视频/音频的参数拷贝至输出视频/音频的AVCodecContext结构体
            // 所以使用的就是ofmt_ctx_video的第一个流streams[0]
            out_stream = ofmt_ctx_video->streams[0];
            ofmt_ctx = ofmt_ctx_video;
            LOGI("Write Video Packet. size:%d\tpts:%lld\n", pkt.size, pkt.pts);
            av_bitstream_filter_filter(h264bsfc, in_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
        }
        else if (pkt.stream_index == audioindex)
        {
            out_stream = ofmt_ctx_audio->streams[0];
            ofmt_ctx = ofmt_ctx_audio;
            LOGI("Write Audio Packet. size:%d\tpts:%lld\n", pkt.size, pkt.pts);
        }
        else
            continue;

        // DTS（Decoding Time Stamp）解码时间戳
        // PTS（Presentation Time Stamp）显示时间戳
        // 转换PTS/DTS
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;
        pkt.stream_index = 0;

        // 写
        if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0)
        {
            LOGI("Error muxing packet\n");
            break;
        }

        LOGI("Write %8d frames to output file\n",frame_index);
        av_free_packet(&pkt);
        frame_index++;
    }

    av_bitstream_filter_close(h264bsfc);

    // 写文件尾部
    av_write_trailer(ofmt_ctx_video);
    av_write_trailer(ofmt_ctx_audio);


    avformat_close_input(&ifmt_ctx);
    if (ofmt_ctx_video && !(ofmt_video->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx_video->pb);

    if (ofmt_ctx_audio && !(ofmt_audio->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx_audio->pb);

    avformat_free_context(ofmt_ctx_video);
    avformat_free_context(ofmt_ctx_audio);

    return 0;

}

static JNINativeMethod MUXMethods[] =
{
    {"demuxMp4", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_demuxMp4},
    {"demuxMp4ext", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_demuxMp4ext}
};

int register_android_jni_mux_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, MUXMethods, sizeof(MUXMethods) / sizeof(MUXMethods[0]));
}
