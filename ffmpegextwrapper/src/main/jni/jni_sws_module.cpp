#include "jni_sws_module.h"

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    swscale
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_swscale(
        JNIEnv *env,jobject thiz, jstring spath, jstring dpath){

        char* srcpath = jstringToChar(env,spath);
        char* destpath = jstringToChar(env,dpath);

        //Parameters
        FILE *src_file =fopen(srcpath/*"ws.yuv"*/, "rb");
        const int src_w=1920,src_h=1080;
        AVPixelFormat src_pixfmt=AV_PIX_FMT_YUV420P;

        //该函数可以获得指定像素格式每个像素占用的比特数（Bit Per Pixel）,av_pix_fmt_desc_get()可以获得指定像素格式的AVPixFmtDescriptor结构体。
        int src_bpp=av_get_bits_per_pixel(av_pix_fmt_desc_get(src_pixfmt));

        FILE *dst_file = fopen(destpath/*"ws.rgb"*/, "wb");
        const int dst_w=1280,dst_h=720;
        AVPixelFormat dst_pixfmt=AV_PIX_FMT_RGB24;
        int dst_bpp=av_get_bits_per_pixel(av_pix_fmt_desc_get(dst_pixfmt));//av_pix_fmt_desc_get()可以获得指定像素格式的AVPixFmtDescriptor结构体。

        //Structures
        uint8_t *src_data[4];
        int src_linesize[4];

        uint8_t *dst_data[4];
        int dst_linesize[4];

        int rescale_method=SWS_BICUBIC;
        struct SwsContext *img_convert_ctx;
        uint8_t *temp_buffer=(uint8_t *)malloc(src_w*src_h*src_bpp/8);

        int frame_idx=0;
        int ret=0;
        ret= av_image_alloc(src_data, src_linesize,src_w, src_h, src_pixfmt, 1);//分配资源控件
        if (ret< 0) {
            LOGE( "Could not allocate source image\n");
            return -1;
        }
        ret = av_image_alloc(dst_data, dst_linesize,dst_w, dst_h, dst_pixfmt, 1);
        if (ret< 0) {
            LOGE( "Could not allocate destination image\n");
            return -1;
        }
        //-----------------------------

        img_convert_ctx =sws_alloc_context();//为SwsContext结构体分配内存。
        //Show AVOption
        av_opt_show2(img_convert_ctx,stdout,AV_OPT_FLAG_VIDEO_PARAM,0);
        //Set Value,设置内部结构体的参数
        av_opt_set_int(img_convert_ctx,"sws_flags",SWS_BICUBIC|SWS_PRINT_INFO,0);
        av_opt_set_int(img_convert_ctx,"srcw",src_w,0);
        av_opt_set_int(img_convert_ctx,"srch",src_h,0);
        av_opt_set_int(img_convert_ctx,"src_format",src_pixfmt,0);
        //'0' for MPEG (Y:0-235);'1' for JPEG (Y:0-255)
        av_opt_set_int(img_convert_ctx,"src_range",1,0);
        av_opt_set_int(img_convert_ctx,"dstw",dst_w,0);
        av_opt_set_int(img_convert_ctx,"dsth",dst_h,0);
        av_opt_set_int(img_convert_ctx,"dst_format",dst_pixfmt,0);
        av_opt_set_int(img_convert_ctx,"dst_range",1,0);
        sws_init_context(img_convert_ctx,NULL,NULL);//对SwsContext中的各种变量进行赋值

        while(1)
        {
            if (fread(temp_buffer, 1, src_w*src_h*src_bpp/8, src_file) != src_w*src_h*src_bpp/8){
                break;
            }

            switch(src_pixfmt){
            case AV_PIX_FMT_GRAY8:{
                memcpy(src_data[0],temp_buffer,src_w*src_h);
                break;
                                  }
            case AV_PIX_FMT_YUV420P:{
                memcpy(src_data[0],temp_buffer,src_w*src_h);                    //Y
                memcpy(src_data[1],temp_buffer+src_w*src_h,src_w*src_h/4);      //U
                memcpy(src_data[2],temp_buffer+src_w*src_h*5/4,src_w*src_h/4);  //V
                break;
                                    }
            case AV_PIX_FMT_YUV422P:{
                memcpy(src_data[0],temp_buffer,src_w*src_h);                    //Y
                memcpy(src_data[1],temp_buffer+src_w*src_h,src_w*src_h/2);      //U
                memcpy(src_data[2],temp_buffer+src_w*src_h*3/2,src_w*src_h/2);  //V
                break;
                                    }
            case AV_PIX_FMT_YUV444P:{
                memcpy(src_data[0],temp_buffer,src_w*src_h);                    //Y
                memcpy(src_data[1],temp_buffer+src_w*src_h,src_w*src_h);        //U
                memcpy(src_data[2],temp_buffer+src_w*src_h*2,src_w*src_h);      //V
                break;
                                    }
            case AV_PIX_FMT_YUYV422:{
                memcpy(src_data[0],temp_buffer,src_w*src_h*2);                  //Packed
                break;
                                    }
            case AV_PIX_FMT_RGB24:{
                memcpy(src_data[0],temp_buffer,src_w*src_h*3);                  //Packed
                break;
                                    }
            default:{
                LOGE("Not Support Input Pixel Format.\n");
                break;
                                  }
            }
            // 转换一帧图像
            sws_scale(img_convert_ctx, src_data, src_linesize, 0, src_h, dst_data, dst_linesize);//转换像素
            LOGE("Finish process frame %5d\n",frame_idx);
            frame_idx++;

            switch(dst_pixfmt){
            case AV_PIX_FMT_GRAY8:{
                fwrite(dst_data[0],1,dst_w*dst_h,dst_file);
                break;
                                  }
            case AV_PIX_FMT_YUV420P:{
                fwrite(dst_data[0],1,dst_w*dst_h,dst_file);                 //Y
                fwrite(dst_data[1],1,dst_w*dst_h/4,dst_file);               //U
                fwrite(dst_data[2],1,dst_w*dst_h/4,dst_file);               //V
                break;
                                    }
            case AV_PIX_FMT_YUV422P:{
                fwrite(dst_data[0],1,dst_w*dst_h,dst_file);                 //Y
                fwrite(dst_data[1],1,dst_w*dst_h/2,dst_file);               //U
                fwrite(dst_data[2],1,dst_w*dst_h/2,dst_file);               //V
                break;
                                    }
            case AV_PIX_FMT_YUV444P:{
                fwrite(dst_data[0],1,dst_w*dst_h,dst_file);                 //Y
                fwrite(dst_data[1],1,dst_w*dst_h,dst_file);                 //U
                fwrite(dst_data[2],1,dst_w*dst_h,dst_file);                 //V
                break;
                                    }
            case AV_PIX_FMT_YUYV422:{
                fwrite(dst_data[0],1,dst_w*dst_h*2,dst_file);               //Packed
                break;
                                    }
            case AV_PIX_FMT_RGB24:{
                fwrite(dst_data[0],1,dst_w*dst_h*3,dst_file);               //Packed
                break;
                                  }
            default:{
                LOGE("Not Support Output Pixel Format.\n");
                break;
                                }
            }
        }

        // 释放SwsContext结构体
        sws_freeContext(img_convert_ctx);

        free(temp_buffer);
        fclose(dst_file);
        av_freep(&src_data[0]);
        av_freep(&dst_data[0]);

        return 0;

}

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    wallpaper
 * Signature: (Ljava/lang/String;Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_wallpaper(
        JNIEnv *env,jobject thiz,jstring spath, jobject surface){

    // sd卡中的视频文件地址,可自行修改或者通过jni传入
    char* srcpath = jstringToChar(env,spath);
    //char *file_name = "/storage/emulated/0/ws.mp4";
    char *file_name = srcpath; // "/storage/emulated/0/video.avi";

    av_register_all();

    AVFormatContext *pFormatCtx = avformat_alloc_context();

    // Open video file
    if (avformat_open_input(&pFormatCtx, file_name, NULL, NULL) != 0) {

        LOGE("Couldn't open file:%s\n", file_name);
        return -1; // Couldn't open file
    }

    // Retrieve stream information
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        LOGE("Couldn't find stream information.");
        return -1;
    }

    // Find the first video stream
    int videoStream = -1, i;
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO
            && videoStream < 0) {
            videoStream = i;
        }
    }
    if (videoStream == -1) {
        LOGE("Didn't find a video stream.");
        return -1; // Didn't find a video stream
    }

    // Get a pointer to the codec context for the video stream
    AVCodecContext *pCodecCtx = pFormatCtx->streams[videoStream]->codec;

    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL) {
        LOGE("Codec not found.");
        return -1; // Codec not found
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGE("Could not open codec.");
        return -1; // Could not open codec
    }

    // 获取native window
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);

    // 获取视频宽高
    int videoWidth = pCodecCtx->width;
    int videoHeight = pCodecCtx->height;

    // 设置native window的buffer大小,可自动拉伸
    ANativeWindow_setBuffersGeometry(nativeWindow, videoWidth, videoHeight,
                                     WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGE("Could not open codec.");
        return -1; // Could not open codec
    }

    // Allocate video frame
    AVFrame *pFrame = av_frame_alloc();

    // 用于渲染
    AVFrame *pFrameRGBA = av_frame_alloc();
    if (pFrameRGBA == NULL || pFrame == NULL) {
        LOGE("Could not allocate video frame.");
        return -1;
    }

    // Determine required buffer size and allocate buffer
    // buffer中数据就是用于渲染的,且格式为RGBA
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pCodecCtx->width, pCodecCtx->height,
                                            1);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrameRGBA->data, pFrameRGBA->linesize, buffer, AV_PIX_FMT_RGBA,
                         pCodecCtx->width, pCodecCtx->height, 1);

    // 由于解码出来的帧格式不是RGBA的,在渲染之前需要进行格式转换
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width,
                                                pCodecCtx->height,
                                                pCodecCtx->pix_fmt,
                                                pCodecCtx->width,
                                                pCodecCtx->height,
                                                AV_PIX_FMT_RGBA,
                                                SWS_BILINEAR,
                                                NULL,
                                                NULL,
                                                NULL);

    int frameFinished;
    AVPacket packet;
    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream) {

            // Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            // 并不是decode一次就可解码出一帧
            if (frameFinished) {

                // lock native window buffer
                ANativeWindow_lock(nativeWindow, &windowBuffer, 0);

                // 格式转换
                sws_scale(sws_ctx, (uint8_t const *const *) pFrame->data,
                          pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGBA->data, pFrameRGBA->linesize);

                // 获取stride,数据通过交换地址
                uint8_t *dst = (uint8_t *) windowBuffer.bits;
                int dstStride = windowBuffer.stride * 4;
                uint8_t *src = (pFrameRGBA->data[0]);
                int srcStride = pFrameRGBA->linesize[0];

                // 由于window的stride和帧的stride不同,因此需要逐行复制
                int h;
                for (h = 0; h < videoHeight; h++) {
                    memcpy(dst + h * dstStride, src + h * srcStride, srcStride);
                }

                ANativeWindow_unlockAndPost(nativeWindow);
            }

        }
        av_packet_unref(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGBA);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);
    return 0;

}

static JNINativeMethod SWSMethods[] =
{
    {"swscale", "(Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_swscale},
    {"wallpaper", "(Ljava/lang/String;Ljava/lang/Object;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_wallpaper}
};

int register_android_jni_sws_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, SWSMethods, sizeof(SWSMethods) / sizeof(SWSMethods[0]));
}