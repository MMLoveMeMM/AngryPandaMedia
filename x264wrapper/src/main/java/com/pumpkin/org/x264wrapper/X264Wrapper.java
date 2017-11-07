package com.pumpkin.org.x264wrapper;

import com.apkfuns.logutils.LogUtils;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Created by rd0348 on 2017/10/11 0011.
 */

public class X264Wrapper {

    static {
        System.loadLibrary("x264encoder");
    }

    private Ix264Listener listener;

    private ByteBuffer mH264buffer;

    public native void initX264Encode(int width, int height, int fps, int bite);

    public native int encoderH264(int length, long time);

    public native void uninitX264Encode();

    public void setListener(Ix264Listener listener) {
        this.listener = listener;
        LogUtils.i("call back listener init set !");
    }

    public void onFrame(byte[] buffer, int length, long time) {
        if (mH264buffer == null || mH264buffer.capacity() < length) {
            mH264buffer = ByteBuffer.allocateDirect(((length / 1024) + 1) * 1024);
        }
        mH264buffer.rewind();
        mH264buffer.put(buffer, 0, length);
        encoderH264(length, time);
    }

    private void H264DataCallBackFunc(byte[] buffer, int length) throws IOException {
        LogUtils.i("call back buffer len : " + length);
        LogUtils.i("call back buffer : " + buffer);
        if (listener != null) {
            listener.onFrameH264(buffer, length);
        } else {
            LogUtils.i("call back listener is null !");
        }

    }

}
