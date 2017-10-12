package com.pumpkin.org.angrypandaav.manager.camera.test;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.manager.camera.CameraManager;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;

/*
* 测试CameraManager中的Camera的工作
* */
public class CameraTestActivity extends Activity implements SurfaceHolder.Callback, IOnFrameListener {

    private final static String TAG = "CameraTestActivity";

    private CameraManager mCameraManager;

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.camera_test);

        surfaceView = (SurfaceView) findViewById(R.id.surfaceview);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

        mCameraManager = CameraManager.getInstance();
        mCameraManager.setSurfaceView(null/*surfaceHolder*/);// 这个设置是必须的,如果不设置同样有数据
        mCameraManager.setOnFrameListener(this);

    }

    @Override
    public void onFrame(byte[] data, int len) {

        Log.i(TAG, "data len : " + len);

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mCameraManager.openCamera();
        mCameraManager.startCamera();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        mCameraManager.stopCamera();
    }

}
