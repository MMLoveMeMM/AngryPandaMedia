package com.pumpkin.org.angrypandaav.basiccamera;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.camera.CameraUtils;
import com.pumpkin.org.angrypandaav.mediac.vedio.p0.AvcEncoder;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ArrayBlockingQueue;


public class BasicCameraActivity extends Activity implements SurfaceHolder.Callback,Camera.PreviewCallback {

    private final static String TAG = "BasicCameraActivity";

    private final static int MAX_YUV_SIZE = 10;

    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;

    private SurfaceView mSurfaceView1;
    private SurfaceHolder mSurfaceHolder1;

    private Camera mCamera;

    private Camera.Parameters parameters;

    private int height=1280;
    private int width=720;

    private int framerate=30;
    private int biterate=8500*1000;

    private AvcEncoder avcCodec;

    private static ArrayBlockingQueue<byte[]> YUVQueue=new ArrayBlockingQueue<byte[]>(MAX_YUV_SIZE);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_basic_camera);

        mSurfaceView=(SurfaceView)findViewById(R.id.surfaceview);
        mSurfaceHolder=mSurfaceView.getHolder();
        mSurfaceHolder.addCallback(this);

        mSurfaceView1=(SurfaceView)findViewById(R.id.surfaceview1);
        mSurfaceHolder1=mSurfaceView1.getHolder();

        SupportAvcCodec();

    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {

        putYUVData(data,data.length);

    }

    public void putYUVData(byte[] buffer, int length) {
        if (YUVQueue.size() >= 10) {
            YUVQueue.poll();
        }
        YUVQueue.add(buffer);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mCamera = getBackCamera();
        startcamera(mCamera);
        try {
            avcCodec = new AvcEncoder(mSurfaceHolder1.getSurface(),YUVQueue,width,height,framerate,biterate);
        } catch (IOException e) {
            e.printStackTrace();
        }
        avcCodec.StartEncoderThread();
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
            avcCodec.StopThread();
        }
    }

    @SuppressLint("NewApi")
    private boolean SupportAvcCodec(){
        if(Build.VERSION.SDK_INT>=18){
            for(int j = MediaCodecList.getCodecCount() - 1; j >= 0; j--){
                MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(j);

                String[] types = codecInfo.getSupportedTypes();
                for (int i = 0; i < types.length; i++) {
                    if (types[i].equalsIgnoreCase("video/avc")) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    private void startcamera(Camera mCamera){
        if(mCamera != null){
            try {
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
                    // 设置
//                    parameters.setPictureSize(map.get(height[0]),height[0]);
                    //parameters.setPreviewSize(map.get(height[0]),height[0]);
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
                List<Size> previewSizes = parameters.getSupportedPreviewSizes();
                Size previewSize = CameraUtils.getProperSize4Ratio(previewSizes,(float) videoWidth / videoHeight);
                Log.i(TAG,"previewSize.width : "+previewSize.width+" previewSize.height : "+previewSize.height);
                parameters.setPreviewSize(previewSize.width,previewSize.height);

                /*
                * 保存用于处理编码
                * */
                width=previewSize.width;
                height=previewSize.height;

                //parameters.setPreviewSize(width, height);
                mCamera.setParameters(parameters);
                mCamera.setPreviewDisplay(mSurfaceHolder);
                mCamera.startPreview();

            } catch (IOException e) {
                e.printStackTrace();
            }
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
