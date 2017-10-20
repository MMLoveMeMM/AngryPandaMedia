package com.pumpkin.org.angrypandaav.ffmpeg;

import android.app.Activity;
import android.os.Environment;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;
import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.manager.camera.CameraManager;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;

public class FilterActivity extends Activity implements SurfaceHolder.Callback, IOnFrameListener {

    private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/tiger.mp4";

    private CameraManager mCameraManager;
    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    private String filter_res;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_filter);

        surfaceView = (SurfaceView) findViewById(R.id.surfaceview);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

    }

    @Override
    protected void onResume() {
        super.onResume();


    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        new Thread(new Runnable() {
            @Override
            public void run() {
                FFmpegWrapperProxy.getInstance().filter(surfaceHolder.getSurface(), path);
            }
        }).start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    public void onFrame(byte[] data, int len) {

    }

}
