package com.pumpkin.org.angrypandaav.basiccamera;

import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.os.Bundle;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.camera.CameraUtils;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/*
* 这个类优化了摄像机的数据帧内存使用
* */

public class BufferCameraActivity extends Activity implements SurfaceHolder.Callback, Camera.PreviewCallback{

    private final static String TAG="BufferCameraActivity";

    private int mCameraWidth, mCameraHeight;

    private SurfaceView surfaceView;
    private SurfaceHolder surfaceHolder;

    private boolean bOpening =false;

    private Button mStartBtn;

    private Camera.Parameters parameters;

    private Camera mCamera;

    private int height=1280;
    private int width=720;

    private int prevSize=height*width;

    private byte[] mPreBuffer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_buffer_camera);

        surfaceView=(SurfaceView)findViewById(R.id.surfaceview);
        surfaceHolder=surfaceView.getHolder();
        surfaceHolder.addCallback(this);

    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {

        Log.d(TAG, "onPreviewFrame data : "+data.length);

        if (mPreBuffer == null) {
            mPreBuffer = new byte[prevSize];
        }
        mCamera.addCallbackBuffer(mPreBuffer);

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {

        Log.d(TAG, "surfaceCreated in setupCamera");
        mCamera = getBackCamera();
        try {
            startcamera(mCamera);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (null != mCamera) {
            mCamera.setPreviewCallback(null);
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
    }

    private void startcamera(Camera mCamera) throws IOException {
        if(mCamera != null){
            mCamera.setPreviewCallback(this);
            mCamera.setDisplayOrientation(90);
            if(parameters == null){
                parameters = mCamera.getParameters();
            }

            List<Camera.Size> pszize = parameters.getSupportedPreviewSizes();
            if (null != pszize && 0 < pszize.size()) {
                int height[] = new int[pszize.size()];// 声明一个数组
                Map<Integer, Integer> map = new HashMap<Integer, Integer>();
                for (int i = 0; i < pszize.size(); i++) {
                    Camera.Size size = (Camera.Size) pszize.get(i);
                    int sizeheight = size.height;
                    int sizewidth = size.width;
                    height[i] = sizeheight;
                    map.put(sizeheight, sizewidth);
                    Log.d(TAG, "size.width:"+sizewidth+"\tsize.height:"+sizeheight);
                }
                Arrays.sort(height);
            }

            parameters.setPreviewFormat(ImageFormat.NV21);//如果是图片,则设置为PixelFormat.JPEG等
            /*
            * 这个预览尺寸是一个坑
            * 一定要参照上面系统支持的种类,而且要注意width和height
            * 所以最好自己获取系统,然后再做判断合适的预览大小,这样才能够兼容不同的手机
            * 不做兼容很容易崩溃
            * */
            List<Camera.Size> videoSiezes = parameters.getSupportedVideoSizes();
            int videoWidth = 0;
            int videoHeight = 0;
            if (videoSiezes != null && !videoSiezes.isEmpty()) {
                Camera.Size videoSize = CameraUtils.getMaxSize4Width(videoSiezes,width);
                Log.e("TAG", "获取到的：video_width===" + videoSize.width + " video_height=== " +  videoSize.height);
                videoWidth = videoSize.width;
                videoHeight = videoSize.height;
            }

            /*
                * 得到预览尺寸
                * */
            List<Camera.Size> previewSizes = parameters.getSupportedPreviewSizes();
            Camera.Size previewSize = CameraUtils.getProperSize4Ratio(previewSizes,(float) videoWidth / videoHeight);
            Log.i(TAG,"previewSize.width : "+previewSize.width+" previewSize.height : "+previewSize.height);
            parameters.setPreviewSize(previewSize.width,previewSize.height);

                /*
                * 保存用于处理编码
                * */
            width=previewSize.width;
            height=previewSize.height;

            prevSize = width*height;

            /*
            * 如果设置的格式是YV12,和格式有关系
            * parameters.setPreviewFormat(ImageFormat.YV12);
            * byte[] mPreBuffer = new byte[mCameraWidth * mCameraHeight * 3 / 2];
            * */

            byte[] mPreBuffer = new byte[mCameraWidth * mCameraHeight/* * 3 / 2*/];
            mCamera.addCallbackBuffer(mPreBuffer);
            mCamera.setPreviewCallbackWithBuffer(this);

            mCamera.setParameters(parameters);
            mCamera.setPreviewDisplay(surfaceHolder);
            mCamera.startPreview();

        }
    }

    @TargetApi(9)
    private Camera getBackCamera() {
        Camera c = null;
        try {
            c = Camera.open(0); // attempt to get a Camera instance
        } catch (Exception e) {
            e.printStackTrace();
        }
        return c; // returns null if camera is unavailable
    }

}
