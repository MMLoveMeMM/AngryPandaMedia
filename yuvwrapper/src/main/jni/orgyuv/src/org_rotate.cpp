#include "org_rotate.h"

int rotateI420Degree270(unsigned char* dstyuv,unsigned char* srcdata, int imageWidth, int imageHeight) {

    int i = 0, j = 0;
    int index = 0;
    int tempindex = 0;
    int div = 0;
    for (i = 0; i <imageHeight; i++) {
        div= i +1;
        tempindex= 0;
        for (j = 0; j <imageWidth; j++) {
            tempindex+= imageWidth;
            dstyuv[index++]= srcdata[tempindex-div];
        }
    }
    int start =imageWidth*imageHeight;
    int udiv = imageWidth *imageHeight / 4;
    int uWidth = imageWidth /2;
    int uHeight = imageHeight /2;
    index= start;
    for (i = 0; i < uHeight;i++) {
        div= i +1;
        tempindex= start;
        for (j = 0; j < uWidth;j++) {
            tempindex += uWidth;
            dstyuv[index]= srcdata[tempindex-div];
            dstyuv[index+udiv]= srcdata[tempindex-div+udiv];
            index++;
        }
    }
    return 0;

}

int rotateI420Degree180(unsigned char* dstyuv,unsigned char* srcdata, int imageWidth, int imageHeight)
{
    int i = 0, j = 0;
    int index = 0;
    int tempindex = 0;
    int ustart = imageWidth *imageHeight;
    tempindex= ustart;
    for (i = 0; i <imageHeight; i++) {
        tempindex-= imageWidth;
        for (j = 0; j <imageWidth; j++) {
            dstyuv[index++] = srcdata[tempindex + j];
        }
    }
    int udiv = imageWidth *imageHeight / 4;
    int uWidth = imageWidth /2;
    int uHeight = imageHeight /2;
    index= ustart;
    tempindex= ustart+udiv;
    for (i = 0; i < uHeight;i++) {
        tempindex-= uWidth;
        for (j = 0; j < uWidth;j++) {
            dstyuv[index]= srcdata[tempindex + j];
            dstyuv[index+ udiv] = srcdata[tempindex + j + udiv];
            index++;
        }
    }
    return 0;
}

int rotateI420Degree90(unsigned char* dstyuv,unsigned char* srcdata, int imageWidth, int imageHeight) {
    int i = 0, j = 0;
    int index = 0;
    int tempindex = 0;
    int div = 0;
    int ustart = imageWidth *imageHeight;
    for (i = 0; i <imageHeight; i++) {
        div= i;
        tempindex= ustart;
        for (j = 0; j <imageHeight; j++) {
            tempindex-= imageWidth;
            dstyuv[index++]= srcdata[tempindex + div];
        }
    }
    int udiv = imageWidth *imageHeight / 4;
    int uWidth = imageWidth /2;
    int uHeight = imageHeight /2;
    index= ustart;
    for (i = 0; i < uHeight;i++) {
        div= i ;
        tempindex= ustart+udiv;
        for (j = 0; j < uWidth;j++) {
            tempindex-= uWidth;
            dstyuv[index]= srcdata[tempindex + div];
            dstyuv[index+ udiv] = srcdata[tempindex + div + udiv];
            index++;
        }
    }
    return 0;
}
