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

}
