//
// Created by rd0348 on 2017/10/12 0012.
//

#ifndef ANGRYPANDAAV_X264ENCODEPROXY_H
#define ANGRYPANDAAV_X264ENCODEPROXY_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*H264DataCallBack)(void *pdata, int datalen);

void initX264Encode(int width, int height, int fps, int bite, H264DataCallBack h264DataCallBack);
void encoderH264(void *pdata, unsigned int datalen, long long time);
void uninitX264Encode();

#ifdef __cplusplus
}
#endif

#endif //ANGRYPANDAAV_X264ENCODEPROXY_H
