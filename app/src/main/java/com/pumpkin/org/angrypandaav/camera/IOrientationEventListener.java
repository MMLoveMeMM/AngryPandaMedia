package com.pumpkin.org.angrypandaav.camera;

import android.content.Context;
import android.hardware.Camera;
import android.util.Log;
import android.view.OrientationEventListener;

/**
 * Created by rd0348 on 2017/9/29 0029.
 */

public class IOrientationEventListener extends OrientationEventListener {

    public IOrientationEventListener(Context context) {
        super(context);
    }

    @Override
    public void onOrientationChanged(int orientation) {

        /*if (ORIENTATION_UNKNOWN == orientation) {
            return;
        }
        Camera.CameraInfo info = new Camera.CameraInfo();
        Camera.getCameraInfo(mCameraId, info);
        orientation = (orientation + 45) / 90 * 90;
        int rotation = 0;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            rotation = (info.orientation - orientation + 360) % 360;
        } else {
            rotation = (info.orientation + orientation) % 360;
        }
        Log.e("TAG","orientation: " + orientation);
        if (null != mCamera) {
            Camera.Parameters parameters = mCamera.getParameters();
            parameters.setRotation(rotation);
            mCamera.setParameters(parameters);
        }*/

    }

}
