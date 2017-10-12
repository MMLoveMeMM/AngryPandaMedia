//
// Created by rd0348 on 2017/10/12 0012.
//

#include <stdlib.h>
#ifndef ANGRYPANDAAV_X264ENCODE_H
#define ANGRYPANDAAV_X264ENCODE_H
#ifdef __cplusplus
extern "C"
{
#include "stdint.h"
#include "x264.h"
#include "x264_config.h"
#endif

enum bitrate_level {
    BIT_HIGH_LEVEL = 0,
    BIT_STANDARD_LEVEL = 1,
    BIT_MEDIUM_LEVEL = 2,
    BIT_LOW_LEVEL = 3
};

class x264encode {
public:
    x264_param_t *x264_param;
    x264_t *x264_encoder;

    x264_picture_t *in_pic;
    x264_picture_t *out_pic;

    uint8_t *picture_buf;

    unsigned int bitratelevel;

private:
    int xwidth;
    int xheight;
    int xfps;

public:
    void initX264Encode(int width, int height, int fps, int bite);

    void startEncoder(uint8_t *dataptr, char *&bufdata, int &buflen, int &isKeyFrame);

    void releaseEncoder();

private:
    void flush();
};

#ifdef __cplusplus
}
#endif

#endif //ANGRYPANDAAV_X264ENCODE_H
