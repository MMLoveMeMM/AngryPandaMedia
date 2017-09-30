package com.pumpkin.org.angrypandaav.mediac.vedio.p1.test;

import android.app.Activity;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.pumpkin.org.angrypandaav.R;

import java.io.IOException;

public class AVCDecoderP1Act extends Activity implements SurfaceHolder.Callback,Camera.PreviewCallback{

    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    private AVCDecoderP1Thread avcDecoderP1Thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_avcdecoder_p1);

        mSurfaceView=(SurfaceView)findViewById(R.id.surfaceview);
        mSurfaceHolder=mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);

    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        try {
            avcDecoderP1Thread=new AVCDecoderP1Thread(mSurfaceHolder);
            avcDecoderP1Thread.start();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }
}
