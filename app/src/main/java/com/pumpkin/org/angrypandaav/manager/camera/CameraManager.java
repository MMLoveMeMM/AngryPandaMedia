package com.pumpkin.org.angrypandaav.manager.camera;

import android.annotation.TargetApi;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.util.Log;
import android.view.SurfaceHolder;

import com.pumpkin.org.angrypandaav.camera.CameraUtils;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;

import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class CameraManager implements Camera.PreviewCallback {

    private final static String TAG = "CameraManager";

    private static final CameraManager ourInstance = new CameraManager();

    public int height = 1280;
    public int width = 720;

    private SurfaceHolder surfaceHolder;
    private Camera mCamera;
    private Camera.Parameters parameters;

    private IOnFrameListener mIOnFrameListener;

    private SurfaceTexture surfaceTexture;

    public static CameraManager getInstance() {
        return ourInstance;
    }

    private CameraManager() {

    }

    public void setSurfaceTexture(SurfaceTexture texture) {
        surfaceTexture = texture;
        surfaceHolder = null;//非必须的
    }

    public void setSurfaceView(SurfaceHolder surface) {
        surfaceHolder = surface;
    }

    public void setOnFrameListener(IOnFrameListener listener) {
        mIOnFrameListener = listener;
    }

    @TargetApi(9)
    public void openCamera() {

        if (mCamera != null) {
            mCamera.release();
            mCamera = null;
        }
        try {
            mCamera = Camera.open(0); // attempt to get a Camera instance
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public void startCamera() {

        if (mCamera != null) {
            try {
                mCamera.setPreviewCallback(this);
                mCamera.setDisplayOrientation(90);
                if (parameters == null) {
                    parameters = mCamera.getParameters();
                }

                /*
                * 获取相机支持的预览尺寸
                * */
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
                        Log.d(TAG, "size.width:" + sizewidth + "\tsize.height:" + sizeheight);
                    }
                    Arrays.sort(height);
                    // 如果拍照需要设置
                    //parameters.setPictureSize(map.get(height[0]),height[0]);
                    //parameters.setPreviewSize(map.get(height[0]),height[0]);
                }

                /*
                * 获取相机支持的预览格式
                * */
                List<Integer> previewFormats = mCamera.getParameters().getSupportedPreviewFormats();
                parameters.setPreviewFormat(ImageFormat.NV21);//如果是图片,则设置为PixelFormat.JPEG等
                /*
                * 获取相机支持的预览帧率
                * */
                List<Integer> previewFrameRates = mCamera.getParameters().getSupportedPreviewFrameRates();
                int fpsrang[] = new int[2]; // [0] : min ,[1] : max
                mCamera.getParameters().getPreviewFpsRange(fpsrang);
                for (int i = 0; i < fpsrang.length; i++) {
                    Log.i(TAG, "fps support arange : " + fpsrang[i]);
                }
                mCamera.getParameters().setPreviewFpsRange(fpsrang[0], fpsrang[1]);
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
                    Camera.Size videoSize = CameraUtils.getMaxSize4Width(videoSiezes, width);
                    Log.e("TAG", "获取到的：video_width===" + videoSize.width + " video_height=== " + videoSize.height);
                    videoWidth = videoSize.width;
                    videoHeight = videoSize.height;
                }
                /*
                * 得到预览尺寸
                * */
                List<Camera.Size> previewSizes = parameters.getSupportedPreviewSizes();
                Camera.Size previewSize = CameraUtils.getProperSize4Ratio(previewSizes, (float) videoWidth / videoHeight);
                Log.i(TAG, "previewSize.width : " + previewSize.width + " previewSize.height : " + previewSize.height);

                parameters.setPreviewSize(previewSize.width, previewSize.height);

                /*
                * 保存用于处理编码
                * */
                width = previewSize.width;
                height = previewSize.height;

                mCamera.setParameters(parameters);
                if (surfaceHolder != null) {
                    mCamera.setPreviewDisplay(surfaceHolder);
                } else if (surfaceTexture != null) {
                    mCamera.setPreviewTexture(surfaceTexture);
                }
                mCamera.startPreview();

            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    public void stopCamera() {

        if (null != mCamera) {
            mCamera.setPreviewCallback(null);
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }

    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        if (mIOnFrameListener != null) {
            mIOnFrameListener.onFrame(data, data.length);
        }
    }

    /*
    * 一般安卓设备预览数据是YV12,需要转换成420P
    * 转换算法效率不是很高,后面放到so里面去处理
    * */
    public void swapYV12toI420(byte[] yv12bytes, byte[] i420bytes, int width, int height) {
        System.arraycopy(yv12bytes, 0, i420bytes, 0, width * height);
        System.arraycopy(yv12bytes, width * height + width * height / 4, i420bytes, width * height, width * height / 4);
        System.arraycopy(yv12bytes, width * height, i420bytes, width * height + width * height / 4, width * height / 4);
    }

    public void NV21ToNV12(byte[] nv21, byte[] nv12, int width, int height) {
        if (nv21 == null || nv12 == null) return;
        int framesize = width * height;
        int i = 0, j = 0;
        System.arraycopy(nv21, 0, nv12, 0, framesize);
        for (i = 0; i < framesize; i++) {
            nv12[i] = nv21[i];
        }
        for (j = 0; j < framesize / 2; j += 2) {
            nv12[framesize + j - 1] = nv21[j + framesize];
        }
        for (j = 0; j < framesize / 2; j += 2) {
            nv12[framesize + j] = nv21[j + framesize - 1];
        }
    }

}
