package com.panda.org.ffmpegextwrapper;

/**
 * Created by rd0348 on 2017/10/19 0019.
 * 上次这个没做成功
 */

public class FFmpegWrapper {

    static {
        System.loadLibrary("ffmpegextwrapper");
    }

    /*
    * MP4 转换 YUV
    * */
    public native String avcodecinfo();

    public native int decode(String srcpath, String destpath);

    /*
    * 转码显示
    * */
    public native int nativewindow(Object surface, String srcpath);

    /*
    * filter
    * */
    public native int filter(Object surface, String srcpath);

    /*
    * 实时处理编码
    * */
    public native int initencode(int width, int height);

    public native int onframe(byte[] yuvimage);

    public native int flush();

    public native int closeencode();

}
