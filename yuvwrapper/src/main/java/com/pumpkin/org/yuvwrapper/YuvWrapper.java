package com.pumpkin.org.yuvwrapper;

import android.graphics.Bitmap;

/**
 * Created by rd0348 on 2017/10/13 0013.
 */

public class YuvWrapper {

    /*
    * 参数一 : 被处理的数据
    * 参数二 : 处理后的数据
    * 参数三 : 宽度
    * 参数四 : 高度
    * 参数五 : 是否需要旋转
    * 参数六 : 是否需要镜像
    * 参数七 : 镜像后的数据
    * 参数八 : 旋转后的数据
    * 参数九 : 旋转模式
    * */
    private native int Yuv(byte[] Src_data, byte[] Dst_data,
                           int src_width, int src_height,
                           boolean enableRotate, boolean enableMirror,
                           byte[] Dst_data_mirror, byte[] Dst_data_rotate,
                           int rotatemodel);

    /*
    * 用于摄像头数据转换
    * */
    private native int yuvN21ToN12(byte[] Src_data, byte[] Dst_data,
                                   int src_width, int src_height);


    /*
    * 下面处理一帧
    * */
    private native void yuvRGBToYuv(Bitmap bitmap, byte[] Dst_data);
    /*
    * libyuv 提供的API还有很多,可以熟读源代码就了解了
    * */


    /*
    * 下面是一般按照yuv格式规则裸写
    * 这个部分主要是为了后面使用opencl去加速这些代码,没有太大的直接使用价值
    * */

}
