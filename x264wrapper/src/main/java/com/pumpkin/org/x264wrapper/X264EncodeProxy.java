package com.pumpkin.org.x264wrapper;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class X264EncodeProxy {

    private static X264EncodeProxy instance;

    private X264Wrapper mX264Wrapper;
    private Ix264Listener mIx264Listener;

    public static synchronized X264EncodeProxy getInstance() {
        if (instance == null) {
            instance = new X264EncodeProxy();
        }
        return instance;
    }

    public X264EncodeProxy() {
        mX264Wrapper = new X264Wrapper();
    }

    public void setmIx264Listener(Ix264Listener listener) {
        mIx264Listener = listener;
    }

    public void InitX264Encode(int width, int height, int fps, int bite) {

        if (mX264Wrapper != null) {
            mX264Wrapper.initX264Encode(width, height, fps, bite);
        }

    }

    public void onFrame(byte[] buffer, int length, int time) {

        mX264Wrapper.onFrame(buffer, length, time);

    }

    public int EncoderH264(int length, long time) {
        int result = -1;
        if (mX264Wrapper != null) {
            result = mX264Wrapper.encoderH264(length, time);
        }

        return result;
    }

    public void UninitX264Encode() {
        if (mX264Wrapper != null) {
            mX264Wrapper.uninitX264Encode();
        }
    }

}
