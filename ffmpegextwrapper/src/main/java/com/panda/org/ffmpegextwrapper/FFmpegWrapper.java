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
    * 实时处理编码(需要x264支持)
    * */
    public native int initencode(int width, int height);

    public native int onframe(byte[] yuvimage);

    public native int flush();

    public native int closeencode();

    /*
    * swscale
    * */
    public native int swscale(String srcpath, String destpath);

    /*
    * mp4 to KTV
    * */
    public native int mp4Toavi(String srcpath, String destpath);

    /*
    * yuv to rgb
    * */
    public native int yuvTorgb(String srcpath, String destpath);

    /*
    * display in wallpaper
    * */
    public native int wallpaper(String showres, Object surface);

    /*
    * pcm to acc
    * */
    public native int pcmToacc(String srcpath, String destpath);

    /*
    * YUV数据编码为h264/h265
    * */
    public native int yuvToHxxx(String srcpath, String destpath);

    /*
    * demux mp4 file to h264 and acc
    * only get datas
    * */
    public native int demuxMp4(String srcpath, String vedpath, String audpath);

    /*
    * demux mp4 file to h264 and acc
    * add PTS to header so that playing
    * */
    public native int demuxMp4ext(String srcpath, String vedpath, String audpath);

    /*
    * aac
    */
    public native int initRecord(String outpath);

    public native int writeAudioData(byte[] datas);

    public native int recordClose();

}
