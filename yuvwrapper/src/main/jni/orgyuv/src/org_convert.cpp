#include "org_convert.h"

// 参考 : http://www.cnblogs.com/dwdxdy/p/3713968.html

void NV21ToI420(unsigned char* dstyuv,unsigned char* data, int imageWidth, int imageHeight){

    int Ustart =imageWidth*imageHeight;
    int i,j;
    int uWidth = imageWidth/2;
    int uHeight = imageWidth/2;
    //y
    memcpy(dstyuv,data,imageWidth*imageHeight);
    int tempindex = 0 ;
    int srcindex= 0;
    //u
    for(i= 0 ;i <uHeight;i++)
    {
      for(j = 0;j <uWidth ;j++ )
      {
      dstyuv[Ustart+tempindex+j]= data[Ustart+(srcindex<<1)+1];
      srcindex++;
     }
    tempindex+= uWidth;
    }
    //v
    for (i = 0; i < uHeight;i++)
    {
     for (j = 0; j < uWidth;j++)
     {
      dstyuv[Ustart+tempindex + j] = data[Ustart + (srcindex << 1 )];
      srcindex++;
     }
     tempindex+= uWidth;
    }

}



