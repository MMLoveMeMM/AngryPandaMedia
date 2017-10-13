package com.pumpkin.org.angrypandaav.x264;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.dump.DumpData2File;
import com.pumpkin.org.angrypandaav.manager.camera.CameraManager;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;
import com.pumpkin.org.angrypandaav.x264.p1.X264EncodeThread;

import java.util.concurrent.ArrayBlockingQueue;

public class X264TestActivity extends Activity implements SurfaceHolder.Callback, IOnFrameListener {

    private final static int MAX_YUV_SIZE = 10;
    private static ArrayBlockingQueue<byte[]> queue = new ArrayBlockingQueue<byte[]>(MAX_YUV_SIZE);

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    private CameraManager mCameraManager;

    private DumpData2File dumpData2File;

    private X264EncodeThread x264EncodeThread;

    private Button mStopBtn;
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        /*set it to be no title*/
        requestWindowFeature(Window.FEATURE_NO_TITLE);
       /*set it to be full screen*/
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_x264_test);

        surfaceView = (SurfaceView) findViewById(R.id.surfaceview);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);

        mCameraManager = CameraManager.getInstance();
        dumpData2File = new DumpData2File();
        dumpData2File.createFile("yuv");

        x264EncodeThread = new X264EncodeThread(mCameraManager.width, mCameraManager.height, queue);
        x264EncodeThread.setDump(dumpData2File);

        mStopBtn = (Button) findViewById(R.id.stop);
        mStopBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (x264EncodeThread != null) {
                    x264EncodeThread.stopEncode(true);
                }
            }
        });

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        mCameraManager.setSurfaceView(surfaceHolder);
        mCameraManager.setOnFrameListener(this);
        mCameraManager.openCamera();
        mCameraManager.startCamera();

        x264EncodeThread.start(); // 启动解码

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

        if (dumpData2File != null) {
            dumpData2File.closeFile();
        }

    }

    @Override
    public void onFrame(byte[] data, int len) {

        if (queue.size() < MAX_YUV_SIZE) {
            queue.add(data); // 加入x264编码队列
        } else {
            /*
            * 队列满了占时不处理
            * 这个地方是编码速度低于预览速度就会导致
            * */
        }

    }
}
