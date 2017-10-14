#include "org_performance.h"

/*
* 处理不用旋转的图像，格式转换加裁剪
*/
void detailPic0(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h) {
    int deleteW = (nw - w) / 2;
    int deleteH = (nh - h) / 2;
    //处理y 旋转加裁剪
    int i, j;
    int index = 0;
    for (j = deleteH; j < nh- deleteH; j++) {
        for (i = deleteW; i < nw- deleteW; i++){
            yuv_temp[index++]= d[j * nw + i];
        }
    }
    //处理u
    index= w * h;
    for (i = nh + deleteH / 2;i < nh / 2 * 3 - deleteH / 2; i++){
        for (j = deleteW + 1; j< nw - deleteW; j += 2){
            yuv_temp[index++]= d[i * nw + j];
        }
    }

    //处理v 旋转裁剪加格式转换
    for (i = nh + deleteH / 2;i < nh / 2 * 3 - deleteH / 2; i++){
        for (j = deleteW; j < nw- deleteW; j += 2){
            yuv_temp[index++]= d[i * nw + j];
        }
    }

}
/*
* 格式转换、裁剪加旋转90度
*/
void detailPic90(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h) {
    int deleteW = (nw - h) / 2;
    int deleteH = (nh - w) / 2;
    int i, j;
    for (i = 0; i < h; i++){
        for (j = 0; j < w; j++){
            yuv_temp[(h- i) * w - 1 - j] = d[nw * (deleteH + j) + nw - deleteW-i];
        }
    }
    int index = w * h;
    for (i = deleteW + 1; i< nw - deleteW; i += 2){
        for (j = nh / 2 * 3 -deleteH / 2; j > nh + deleteH / 2; j--){
            yuv_temp[index++]= d[(j - 1) * nw + i];
        }
    }

    for (i = deleteW; i < nw- deleteW; i += 2){
        for (j = nh / 2 * 3 -deleteH / 2; j > nh + deleteH / 2; j--){
            yuv_temp[index++]= d[(j - 1) * nw + i];
        }
    }

}

/*
* 格式转换、裁剪加旋转180度
*/
void detailPic180(uint8_t* d, uint8_t* yuv_temp, int nw, int nh, int w, int h) {
    int deleteW = (nw - w) / 2;
    int deleteH = (nh - h) / 2;
    //处理y 旋转加裁剪
    int i, j;
    int index = w * h;
    for (j = deleteH; j < nh- deleteH; j++) {
        for (i = deleteW; i < nw- deleteW; i++){
            yuv_temp[--index]= d[j * nw + i];
        }
    }
    //处理u
    index= w * h * 5 / 4;
    for (i = nh + deleteH / 2;i < nh / 2 * 3 - deleteH / 2; i++){
        for (j = deleteW + 1; j< nw - deleteW; j += 2){
            yuv_temp[--index]= d[i * nw + j];
        }
    }

    //处理v
    index= w * h * 3 / 2;
    for (i = nh + deleteH / 2;i < nh / 2 * 3 - deleteH / 2; i++){
        for (j = deleteW; j < nw- deleteW; j += 2){
            yuv_temp[--index]= d[i * nw + j];
        }
    }

}

/*
* 格式转换、裁剪加旋转270度
*/
void detailPic270(unsigned char* d, unsigned char* yuv_temp, int nw, int nh, int w, int h) {
    int deleteW = (nw - h) / 2;
    int deleteH = (nh - w) / 2;
    int i, j;
    //处理y 旋转加裁剪
    for (i = 0; i < h; i++){
        for (j = 0; j < w; j++){
            yuv_temp[i* w + j] = d[nw * (deleteH + j) + nw - deleteW - i];
        }
    }
    //处理u 旋转裁剪加格式转换
    int index = w * h;
    for (i = nw - deleteW - 1;i > deleteW; i -= 2){
        for (j = nh + deleteH / 2;j < nh / 2 * 3 - deleteH / 2; j++){
            yuv_temp[index++]= d[(j) * nw + i];
        }
    }

    //处理v 旋转裁剪加格式转换
    for (i = nw - deleteW - 2;i >= deleteW; i -= 2) {
        for (j = nh + deleteH / 2;j < nh / 2 * 3 - deleteH / 2; j++){
            yuv_temp[index++]= d[(j) * nw + i];
        }
    }

}
