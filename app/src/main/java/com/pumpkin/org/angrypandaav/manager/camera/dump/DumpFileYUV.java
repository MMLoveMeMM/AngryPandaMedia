package com.pumpkin.org.angrypandaav.manager.camera.dump;

import android.os.Environment;
import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class DumpFileYUV implements IOnFrameListener {

    private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/cameratest.yuv";
    private BufferedOutputStream outputStream;

    public void createFile() {
        File file = new File(path);
        if (file.exists()) {
            file.delete();
        }
        try {
            outputStream = new BufferedOutputStream(new FileOutputStream(file));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void closeFile() {

        if (outputStream != null) {
            try {
                outputStream.flush();
                outputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    @Override
    public void onFrame(byte[] data, int len) {

        if (outputStream != null) {
            try {
                outputStream.write(data, 0, len);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }
}
