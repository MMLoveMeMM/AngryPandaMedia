package com.pumpkin.org.x264wrapper;

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

    public void onFrame(byte[] buffer, int length, long time) {
        if (mH264buffer == null || mH264buffer.capacity() < length) {
            mH264buffer = ByteBuffer.allocateDirect(((length / 1024) + 1) * 1024);
        }
        mH264buffer.rewind();
        mH264buffer.put(buffer, 0, length);
        encoderH264(length, time);
    }

    private void H264DataCallBackFunc(byte[] buffer, int length) {
        listener.onFrameH264(buffer, length);
    }

}
