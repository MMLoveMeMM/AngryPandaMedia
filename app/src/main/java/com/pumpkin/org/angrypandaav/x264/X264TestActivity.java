package com.pumpkin.org.angrypandaav.x264;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.dump.DumpData2File;
import com.pumpkin.org.angrypandaav.manager.camera.CameraManager;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;

import java.util.concurrent.ArrayBlockingQueue;

public class X264TestActivity extends Activity implements SurfaceHolder.Callback, IOnFrameListener {

    private final static int MAX_YUV_SIZE = 10;
    private static ArrayBlockingQueue<byte[]> queue = new ArrayBlockingQueue<byte[]>(MAX_YUV_SIZE);

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    private CameraManager mCameraManager;

    private DumpData2File dumpData2File;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_x264_test);

        surfaceView = (SurfaceView) findViewById(R.id.surfaceview);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

        mCameraManager = CameraManager.getInstance();
        dumpData2File.createFile("yuv");
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        mCameraManager.setSurfaceView(surfaceHolder);
        mCameraManager.setOnFrameListener(this);
        mCameraManager.openCamera();
        mCameraManager.startCamera();

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    @Override
    public void onFrame(byte[] data, int len) {

        queue.add(data); // 加入x264编码队列

    }
}
