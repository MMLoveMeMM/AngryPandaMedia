#include "org_mirror.h"

void Mirror(unsigned char* yuv_temp, int nw, int nh, int w,
        int h) {
    int deleteW = (nw - h) / 2;
    int deleteH = (nh - w) / 2;
    int i, j;
    int a, b;
    unsigned char temp;
    //mirror y
    for (i = 0; i < h; i++){
        a= i * w;
        b= (i + 1) * w - 1;
        while (a < b) {
            temp= yuv_temp[a];
            yuv_temp[a]= yuv_temp[b];
            yuv_temp[b]= temp;
            a++;
            b--;
        }
    }
    //mirror u
    int uindex = w * h;
    for (i = 0; i < h / 2;i++) {
        a = i * w / 2;
        b= (i + 1) * w / 2 - 1;
        while (a < b) {
            temp= yuv_temp[a + uindex];
            yuv_temp[a+ uindex] = yuv_temp[b + uindex];
            yuv_temp[b+ uindex] = temp;
            a++;
            b--;
        }
    }
    //mirror v
    uindex= w * h / 4 * 5;
    for (i = 0; i < h / 2;i++) {
        a= i* w / 2;
        b= (i + 1) * w / 2 - 1;
        while (a < b) {
            temp= yuv_temp[a + uindex];
            yuv_temp[a+ uindex] = yuv_temp[b + uindex];
            yuv_temp[b+ uindex] = temp;
            a++;
            b--;
        }
    }
}
