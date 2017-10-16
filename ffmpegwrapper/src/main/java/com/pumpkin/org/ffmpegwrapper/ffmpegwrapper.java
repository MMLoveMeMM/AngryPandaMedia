package com.pumpkin.org.ffmpegwrapper;

/**
 * Created by rd0348 on 2017/10/16 0016.
 */

public class ffmpegwrapper {

    private IYuvListener listener;

    private native int FfmpegInit(int width, int height, String outpath);

    private native int FfmpegOnFrame(byte[] datas);

    private native int FfmpegClose();

    public void setListener(IYuvListener lis) {
        listener = lis;
    }

    private void YuvDataCallBackFunc(byte[] buffer, int length) {

        if (listener != null) {
            listener.onFrameYuv(buffer, length);
        } else {
            //LogUtils.i("call back listener is null !");
        }

    }

}
