#include "jni_yuvTorgb_module.h"

/*
 * Class:     com_panda_org_ffmpegextwrapper_FFmpegWrapper
 * Method:    yuvTorgb
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_yuvTorgb(
        JNIEnv *env,jobject thiz, jstring spath, jstring dpath){

    char* srcpath = jstringToChar(env,spath);
    char* destpath = jstringToChar(env,dpath);
    //Parameters
    FILE *src_file =fopen(srcpath/*"ws.yuv"*/, "rb");
    const int src_w=1920,src_h=1080;
    AVPixelFormat src_pixfmt=AV_PIX_FMT_YUV420P;  // 输入的格式

    //该函数可以获得指定像素格式每个像素占用的比特数（Bit Per Pixel）,av_pix_fmt_desc_get()可以获得指定像素格式的AVPixFmtDescriptor结构体。
    int src_bpp=av_get_bits_per_pixel(av_pix_fmt_desc_get(src_pixfmt));

    FILE *dst_file = fopen(destpath/*"ws.rgb"*/, "wb");
    const int dst_w=1280,dst_h=720;
    AVPixelFormat dst_pixfmt=AV_PIX_FMT_RGB24; // 输出格式
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
        printf( "Could not allocate source image\n");
        return -1;
    }
    ret = av_image_alloc(dst_data, dst_linesize,dst_w, dst_h, dst_pixfmt, 1);
    if (ret< 0) {
        printf( "Could not allocate destination image\n");
        return -1;
    }
    //-----------------------------

    img_convert_ctx =sws_alloc_context();//为SwsContext结构体分配内存。
    //Show AVOption
    av_opt_show2(img_convert_ctx,stdout,AV_OPT_FLAG_VIDEO_PARAM,0);
    //Set Value
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
            printf("Not Support Input Pixel Format.\n");
            break;
                              }
        }

        sws_scale(img_convert_ctx, src_data, src_linesize, 0, src_h, dst_data, dst_linesize);//转换像素
        printf("Finish process frame %5d\n",frame_idx);
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
            printf("Not Support Output Pixel Format.\n");
            break;
                            }
        }
    }

    sws_freeContext(img_convert_ctx);

    free(temp_buffer);
    fclose(dst_file);
    av_freep(&src_data[0]);
    av_freep(&dst_data[0]);

    return 0;
}

static JNINativeMethod YuvToRGBMethods[] =
{
    {"yuvTorgb", "(Ljava/lang/Object;Ljava/lang/String;)I",(void *)Java_com_panda_org_ffmpegextwrapper_FFmpegWrapper_yuvTorgb}
};

int register_android_jni_yuvtorgb_module(JNIEnv* env, jclass clazz){
        return jniRegisterNativeMethods(env, clazz, YuvToRGBMethods, sizeof(YuvToRGBMethods) / sizeof(YuvToRGBMethods[0]));
}