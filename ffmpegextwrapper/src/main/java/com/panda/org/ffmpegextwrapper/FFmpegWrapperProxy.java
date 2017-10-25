package com.panda.org.ffmpegextwrapper;

/**
 * Created by rd0348 on 2017/10/19 0019.
 * 做一个代理类提供给外界APP使用
 */

public class FFmpegWrapperProxy {

    private static final FFmpegWrapperProxy ourInstance = new FFmpegWrapperProxy();

    private FFmpegWrapper fFmpegWrapper;

    public synchronized static FFmpegWrapperProxy getInstance() {
        return ourInstance;
    }

    private FFmpegWrapperProxy() {
        fFmpegWrapper = new FFmpegWrapper();
    }

    public String AvcodecInfo() {
        return fFmpegWrapper.avcodecinfo();
    }

    /*
    * 参数一 : mp4文件全路径
    * 参数二 : yuv文件全路径
    * */
    public int MP4ToYUVDecode(String srcpath, String destpath) {
        return fFmpegWrapper.decode(srcpath, destpath);
    }


    /*
    * filter
    * */
    public int filter(Object surface, String srcpath) {
        return fFmpegWrapper.filter(surface, srcpath);
    }

    /*
    * 实时处理编码
    * */
    public int initencode(int width, int height) {
        return fFmpegWrapper.initencode(width, height);
    }

    public int onframe(byte[] yuvimage) {

        return fFmpegWrapper.onframe(yuvimage);

    }

    public int flush() {
        return fFmpegWrapper.flush();
    }

    public int closeencode() {
        return fFmpegWrapper.closeencode();
    }

    /*
    * display in wallpaper
    * */
    public int wallpaper(String showres, Object surface) {
        return fFmpegWrapper.wallpaper(showres, surface);
    }

    public int demuxMp4(String srcpath, String vedpath, String audpath) {
        return fFmpegWrapper.demuxMp4(srcpath, vedpath, audpath);
    }

    /*
    * demux mp4 file to h264 and acc
    * add PTS to header so that playing
    * */
    public int demuxMp4ext(String srcpath, String vedpath, String audpath) {
        return fFmpegWrapper.demuxMp4ext(srcpath, vedpath, audpath);
    }

}
