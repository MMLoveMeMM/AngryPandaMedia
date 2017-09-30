package com.pumpkin.org.angrypandaav.mediac.vedio.p3.test;

import android.app.Activity;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.pumpkin.org.angrypandaav.R;

public class AvcDecoderP3Act extends Activity implements SurfaceHolder.Callback,Camera.PreviewCallback{

    private final static String TAG="AvcDecoderP3Act";

    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    private AvcDecoderP3Thread avcDecoderP3Thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_avc_decoder_p3);

        mSurfaceView=(SurfaceView)findViewById(R.id.surfaceview);
        mSurfaceHolder=mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);


    }


    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        avcDecoderP3Thread=new AvcDecoderP3Thread(mSurfaceHolder);
        avcDecoderP3Thread.start();

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if(avcDecoderP3Thread!=null) {
            avcDecoderP3Thread.stopThread();
        }
    }
}
