package com.pumpkin.org.angrypandaav.camera;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.Surface;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Created by rd0348 on 2017/9/29 0029.
 */

public class CameraUtils {

    /**
     * 保证预览方向正确
     *
     * @param activity
     * @param cameraId
     * @param camera
     */
    public static void setCameraDisplayOrientation(Activity activity, int cameraId, Camera camera) {
        android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(cameraId, info);
        int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();

        int degrees = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degrees = 0;
                break;
            case Surface.ROTATION_90:
                degrees = 90;
                break;
            case Surface.ROTATION_180:
                degrees = 180;
                break;
            case Surface.ROTATION_270:
                degrees = 270;
                break;
        }
        int result;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degrees) % 360;
            result = (360 - result) % 360;  // compensate the mirror
        } else {  // back-facing
            result = (info.orientation - degrees + 360) % 360;
        }
        camera.setDisplayOrientation(result);
    }

    /**
     * 设置Camera参数
     */
    private void setCameraParameters(Camera mCamera,Camera.Parameters mParameters,int height,int width) {
        if (mCamera != null) {
            mParameters = mCamera.getParameters();
            List<Camera.Size> pictureSizeList = mParameters.getSupportedPictureSizes();
            /* 从列表中选取合适的分辨率 */
            Camera.Size picSize = getProperSize4Ratio(pictureSizeList, (float) height / width);
            mParameters.setPictureSize(picSize.width, picSize.height);
            Log.e("TAG","最终设置的picsize: picSize.width: " + picSize.width + " picSize.height: " + picSize.height);

            List<Camera.Size> videoSiezes = mParameters.getSupportedVideoSizes();
            int videoWidth = 0;
            int videoHeight = 0;
            if (videoSiezes != null && !videoSiezes.isEmpty()) {
//                Camera.Size videoSize = VideoUtil.getInstance().getPropVideoSize(videoSiezes,surfaceView.getWidth());
                Camera.Size videoSize = getMaxSize4Width(videoSiezes,width);
                Log.e("TAG", "获取到的：video_width===" + videoSize.width + " video_height=== " +  videoSize.height);
                videoWidth = videoSize.width;
                videoHeight = videoSize.height;
            }
            List<Camera.Size> previewSizes = mParameters.getSupportedPreviewSizes();
//            Camera.Size previewSize = VideoUtil.getInstance().getPropPreviewSize(mParameters.getSupportedPreviewSizes(), videoWidth);
            Camera.Size previewSize = getProperSize4Ratio(previewSizes,(float) videoWidth / videoHeight);
            mParameters.setPreviewSize(previewSize.width, previewSize.height);
            Log.e("TAG", "最终设置的预览尺寸,previewSize.width: " + previewSize.width + " previewSize.height: " + previewSize.height);

            List<String> focusModes = mParameters.getSupportedFocusModes();
            if (focusModes != null && focusModes.size() > 0) {
                if (focusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE)) {
                    mParameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);  //设置自动对焦
                }
            }
            mCamera.setParameters(mParameters);
        }
    }


    /**
     * 根据比例得到合适的尺寸的最大尺寸
     */
    public static Size getProperSize4Ratio(List<Size> sizeList, float displayRatio) {
        Collections.sort(sizeList, new SizeL2hComparator());
        Size result = null;
        for (Size size : sizeList) {
            float curRatio = ((float) size.width) / size.height;
            if (curRatio == displayRatio) {
                result = size;
            }
        }

        if (null == result) {
            for (Size size : sizeList) {
                float curRatio = ((float) size.width) / size.height;
                if (curRatio == 3f / 4) {
                    result = size;
                }
            }
        }
        return result;
    }

    /**
     * 根据宽度得到最大合适的尺寸
     * @param sizeList
     * @param Width
     * @return
     */
    public static Size getMaxSize4Width(List<Size> sizeList, int Width) {
        // 先对传进来的size列表进行排序
        Collections.sort(sizeList, new SizeL2hComparator());
        Size result = null;
        for (Size size : sizeList) {
            if (size.height == Width) {
                result = size;
            }
        }
        return result;
    }

    /**
     * 获取支持的最大尺寸
     */
    public static Camera.Size getMaxSize(List<Camera.Size> sizeList) {
        // 先对传进来的size列表进行排序
        Collections.sort(sizeList, new SizeL2hComparator());
        Camera.Size result = null;
        if(sizeList != null && !sizeList.isEmpty()){
            result = sizeList.get(sizeList.size() - 1);
        }
        return result;
    }

    /**
     * 从小到大排序
     */
    private static class SizeL2hComparator implements Comparator<Camera.Size> {
        @Override
        public int compare(Camera.Size size1, Camera.Size size2) {
            if (size1.width < size2.width) {
                return -1;
            }else if (size1.width > size2.width) {
                return 1;
            }
            return 0;
        }
    }

    public static int getRecorderRotation(int cameraId){
        android.hardware.Camera.CameraInfo info = new                 android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(cameraId, info);
        return info.orientation;
    }

}
