package com.pumpkin.org.x264wrapper;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class X264EncodeProxy {

    private static X264EncodeProxy instance;

    private X264Wrapper mX264Wrapper;

    public static synchronized X264EncodeProxy getInstance() {
        if (instance == null) {
            instance = new X264EncodeProxy();
        }
        return instance;
    }

    public X264EncodeProxy() {
        mX264Wrapper = new X264Wrapper();
    }

    public void setIx264Listener(Ix264Listener listener) {
        mX264Wrapper.setListener(listener);
    }

    public void InitX264Encode(int width, int height, int fps, int bite) {

        if (mX264Wrapper != null) {
            mX264Wrapper.initX264Encode(width, height, fps, bite);
        }

    }

    public void onFrame(byte[] buffer, int length, int time) {

        mX264Wrapper.onFrame(buffer, length, time);

    }

    public void UninitX264Encode() {
        if (mX264Wrapper != null) {
            mX264Wrapper.uninitX264Encode();
        }
    }

}
