package com.pumpkin.org.yuvwrapper;

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

}
