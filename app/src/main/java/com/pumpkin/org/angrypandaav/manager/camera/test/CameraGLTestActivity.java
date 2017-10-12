package com.pumpkin.org.angrypandaav.manager.camera.test;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class CameraGLTestActivity extends Activity {

    private CameraGLSurfaceView cameraGLSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        /*set it to be no title*/
        requestWindowFeature(Window.FEATURE_NO_TITLE);
       /*set it to be full screen*/
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        super.onCreate(savedInstanceState);

        cameraGLSurfaceView = new CameraGLSurfaceView(CameraGLTestActivity.this);
        setContentView(cameraGLSurfaceView/*R.layout.activity_camera_gltest*/);

    }
}
