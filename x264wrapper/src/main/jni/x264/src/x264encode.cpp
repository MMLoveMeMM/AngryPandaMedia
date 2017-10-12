//
// Created by rd0348 on 2017/10/12 0012.
//

#include "x264encode.h"


void x264encode::initX264Encode(int width, int height, int fps, int bite) {

    x264_encoder = NULL;
    x264_param = new x264_param_t;
    x264_param_default_preset(x264_param, "fast", "zerolatency");

    if (bite == 0) {
        bite = 1;
    }
    if (bite > 0 && bite <= 64) {
        bitratelevel = BIT_LOW_LEVEL;
    } else if (bite > 64 && bite <= 128) {
        bitratelevel = BIT_MEDIUM_LEVEL;
    } else if (bite > 128 && bite < 256) {
        bitratelevel = BIT_STANDARD_LEVEL;
    } else if (bite > 256 && bite <= 384) {
        bitratelevel = BIT_HIGH_LEVEL;
    } else {
        bitratelevel = BIT_STANDARD_LEVEL;
    }

    /*
     * 1pass VBR, nominal QP. 实际质量，值越大图像越花,越小越清晰
     * */
    if (bitratelevel == BIT_LOW_LEVEL) {
        x264_param->rc.f_rf_constant = 32;
    } else if (bitratelevel == BIT_MEDIUM_LEVEL) {
        x264_param->rc.f_rf_constant = 29;
    } else if (bitratelevel == BIT_STANDARD_LEVEL) {
        x264_param->rc.f_rf_constant = 26;
    } else if (bitratelevel == BIT_HIGH_LEVEL) {
        x264_param->rc.f_rf_constant = 24;
    } else {
        x264_param->rc.f_rf_constant = 24;
    }

    x264_param->b_repeat_headers = 1; //是否复制sps和pps放在每个关键帧的前面
    x264_param->rc.i_rc_method = X264_RC_CRF; //码率控制方式 ： X264_RC_CQP恒定质量,X264_RC_CRF恒定码率,  X264_RC_ABR平均码率

    x264_param->i_width = width; //视频图像的宽
    x264_param->i_height = height; //视频图像的高
    x264_param->i_frame_total = 0; //编码帧的总数, 如果不知道具体总数,采用默认 0
    x264_param->i_keyint_max = 0; //设定IDR帧(I帧)之间的最间隔，在此间隔设置IDR关键帧

    x264_param->i_fps_den = 1; //帧率的分母
    x264_param->i_fps_num = fps; //帧率的分子
    x264_param->i_timebase_den = x264_param->i_fps_num; // 时间基的分母
    x264_param->i_timebase_num = x264_param->i_fps_den; // 时间基的分子

    x264_param->i_cqm_preset = X264_CQM_FLAT; // 自定义量化矩阵(CQM), 初始化量化模式为flat

    xfps = fps;
    xheight = height;
    xwidth = width;

    x264_param->analyse.i_me_method = X264_ME_HEX; // 运动估计算法 (X264_ME_*)
    x264_param->analyse.i_subpel_refine = 2; // 亚像素运动估计质量
    x264_param->i_frame_reference = 1; //最大参考帧数目
    x264_param->analyse.b_mixed_references = 0; // 允许每个宏块的分区有它自己的参考号
    x264_param->analyse.i_trellis = 0; // Trellis量化提高效率，对每个8x8的块寻找合适的量化值，需要CABAC，0 ：即关闭  1：只在最后编码时使用  2：在所有模式决策上启用
    x264_param->b_sliced_threads = 0; // 如果为false，则一个slice只编码成一个NALU; 否则有几个线程，在编码成几个NALU。缺省为true。
    x264_param->i_threads = 4; // 并行编码多帧; 线程数，为0则自动多线程编码

    x264_param->analyse.b_transform_8x8 = 0;
    x264_param->b_cabac = 0; // 自适应算术编码cabac开关
    x264_param->b_deblocking_filter = 1; // 去块滤波开关
    x264_param->psz_cqm_file = NULL; // 读取JM格式的外部量化矩阵文件，忽略其他cqm选项
    x264_param->analyse.i_weighted_pred = X264_WEIGHTP_NONE; // P帧权重
    x264_param->rc.i_lookahead = 10; // 决定mbtree向前预测的帧数
    x264_param->i_bframe = 0; // 两个参考帧之间的B帧数目

#ifndef __ANDROID__
    x264_encoder = x264_encoder_open_133(x264_param);
#endif
    x264_encoder = x264_encoder_open_148(x264_param);
    if (x264_encoder == NULL) {

    }
    int iMaxFrames = x264_encoder_maximum_delayed_frames(x264_encoder);

    in_pic = new x264_picture_t;
    out_pic = new x264_picture_t;
    x264_picture_init(out_pic);
    x264_picture_alloc(in_pic, X264_CSP_I420, x264_param->i_width, x264_param->i_height);
    in_pic->img.i_csp = X264_CSP_I420;
    in_pic->img.i_plane = 3;

}

void x264encode::startEncoder(uint8_t *dataptr, char *&bufdata, int &buflen, int &isKeyFrame) {

    int width = xwidth;
    int height = xheight;

    picture_buf = dataptr;

    memcpy(in_pic->img.plane[0], picture_buf, width * height);
    memcpy(in_pic->img.plane[1], picture_buf + width * height, width * height / 4);
    memcpy(in_pic->img.plane[2], picture_buf + width * height * 5 / 4, width * height / 4);

    in_pic->i_type = X264_TYPE_AUTO;

    in_pic->i_qpplus1 = 0;
    in_pic->param = x264_param;
    in_pic->i_pts = 0;
    x264_nal_t *nal;
    int i_nal = 0;
    int result;

    if (x264_encoder != NULL) {
        result = x264_encoder_encode(x264_encoder, &nal, &i_nal, in_pic, out_pic);
        isKeyFrame = out_pic->b_keyframe;
        in_pic->i_pts++;
    }

    if (result < 0) {
        //LOGI("encode failure ...");
    } else if (result == 0) {
        //LOGI("save to buffer waiting for flush ...");
    } else {
        int bufsize = 0;
        for (int i = 0; i < i_nal; i++) {
            bufsize += nal[i].i_payload;
        }

        char *tempdata = new char[bufsize];
        memset(tempdata, 0, bufsize);
        bufdata = tempdata;
        for (int i = 0; i < i_nal; i++) {
            if (nal[i].p_payload != NULL) {
                memcpy(tempdata, nal[i].p_payload, nal[i].i_payload);
                tempdata += nal[i].i_payload;
            }
        }
        buflen = bufsize;

    }


}

void x264encode::releaseEncoder() {

    this->flush();

    x264_picture_clean(in_pic);
    x264_encoder_close(x264_encoder);
    x264_encoder=NULL;

    if(in_pic!=NULL){
        delete in_pic;
        in_pic=NULL;
    }
    if(out_pic!=NULL){
        delete out_pic;
        out_pic=NULL;
    }
    if(x264_param!=NULL){
        delete x264_param;
        x264_param=NULL;
    }

}

void x264encode::flush() {

    x264_picture_t pic_out;
    x264_nal_t *nal;
    int i_nal;
    if(x264_encoder_encode(x264_encoder,&nal,&i_nal,NULL,&pic_out)<0){

    }

}