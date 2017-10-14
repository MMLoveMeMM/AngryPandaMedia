#include "org_scale.h"

int crop_yuv (unsigned char* data, unsigned char*dst, int width, int height,
        int goalwidth, int goalheight) {
    int i, j;
    int h_div = 0, w_div = 0;
    w_div= (width - goalwidth) / 2;
    if (w_div % 2)
        w_div--;
    h_div= (height - goalheight) / 2;
    if (h_div % 2)
        h_div--;
    //u_div = (height-goalheight)/4;
    int src_y_length = width *height;
    int dst_y_length =goalwidth * goalheight;
    for (i = 0; i <goalheight; i++)
        for (j = 0; j <goalwidth; j++) {
            dst[i* goalwidth + j] = data[(i + h_div) * width + j + w_div];
        }
    int index = dst_y_length;
    int src_begin =src_y_length + h_div * width / 4;
    int src_u_length =src_y_length / 4;
    int dst_u_length =dst_y_length / 4;
    for (i = 0; i <goalheight / 2; i++)
        for (j = 0; j <goalwidth / 2; j++) {
            int p = src_begin + i *(width >> 1) + (w_div >> 1) + j;
            dst[index]= data[p];
            dst[dst_u_length+ index++] = data[p + src_u_length];
        }
    return 0;
}
