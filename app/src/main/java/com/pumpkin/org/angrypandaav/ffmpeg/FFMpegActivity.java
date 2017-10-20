package com.pumpkin.org.angrypandaav.ffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;
import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.manager.camera.CameraManager;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;

import java.util.concurrent.ArrayBlockingQueue;

public class FFMpegActivity extends Activity implements SurfaceHolder.Callback, IOnFrameListener {

    private CameraManager mCameraManager;

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    private FFMpegThread ffMpegThread;

    /*
    * 这个队列后面改到native中用C++内存池管理
    * */
    private final static int MAX_QUEUE = 10;
    private ArrayBlockingQueue<byte[]> queue = new ArrayBlockingQueue<byte[]>(MAX_QUEUE);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg);

        surfaceView = (SurfaceView) findViewById(R.id.surfaceview);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

        mCameraManager = CameraManager.getInstance();
        mCameraManager.setSurfaceView(surfaceHolder);// 这个设置是必须的,如果不设置同样有数据
        mCameraManager.setOnFrameListener(this);

        ffMpegThread = new FFMpegThread(queue);

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        mCameraManager.openCamera();
        mCameraManager.startCamera();

        ffMpegThread.start();
        ffMpegThread.initFFMpeg(mCameraManager.width, mCameraManager.height);

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

        ffMpegThread.closeFFMpeg();

    }

    @Override
    public void onFrame(byte[] data, int len) {

        /*
        * 如果容器未满,就添加进队列
        * 如果满了,可以进一步扩大容器总大小,或者丢失多余的帧
        * */
        if (queue.size() < MAX_QUEUE) {
            try {
                queue.put(data);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
