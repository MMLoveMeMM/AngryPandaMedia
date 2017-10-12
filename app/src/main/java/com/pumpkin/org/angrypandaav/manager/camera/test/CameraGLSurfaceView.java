package com.pumpkin.org.angrypandaav.manager.camera.test;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.pumpkin.org.angrypandaav.manager.camera.CameraManager;
import com.pumpkin.org.angrypandaav.manager.camera.dump.IOnFrameListener;
import com.pumpkin.org.angrypandaav.manager.camera.test.shape.PreviousView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class CameraGLSurfaceView extends GLSurfaceView implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener, IOnFrameListener {

    private final static String TAG = "CameraGLSurfaceView";

    private Context mContext;
    private SurfaceTexture mSurface;
    private int mTextureID = -1;
    private PreviousView mPreviousView;

    private CameraManager mCameraManager;

    public CameraGLSurfaceView(Context context) {
        super(context);
        mContext = context;
        this.setEGLContextClientVersion(2);
        this.setRenderer(this);
        this.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        mCameraManager = CameraManager.getInstance();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        mTextureID = createTextureID();
        mSurface = new SurfaceTexture(mTextureID);// 根据纹理绑定
        mSurface.setOnFrameAvailableListener(this);// 每显示一帧将会回调,在回调的时候刷新Renderer
        mPreviousView = new PreviousView(mTextureID); // GL界面渲染

        mCameraManager.setOnFrameListener(this); // 图像数据回调
        mCameraManager.openCamera();
        mCameraManager.setSurfaceTexture(mSurface);
        mCameraManager.startCamera();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {

        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        mSurface.updateTexImage();
        float[] mtx = new float[16];
        mSurface.getTransformMatrix(mtx);
        mPreviousView.draw(mtx);

    }

    private int createTextureID() {
        int[] texture = new int[1];

        GLES20.glGenTextures(1, texture, 0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, texture[0]);
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
                GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_LINEAR);
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
                GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
                GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
                GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);

        return texture[0];
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        this.requestRender();// 刷新GLSurfaceView界面
    }

    @Override
    public void onFrame(byte[] data, int len) {

        Log.i(TAG, "data len : " + len);

    }
}
