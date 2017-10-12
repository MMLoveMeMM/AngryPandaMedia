package com.pumpkin.org.angrypandaav.dump;

import android.os.Environment;
import android.util.Log;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by rd0348 on 2017/9/30 0030.
 */

public class DumpData2File implements IDumpData {

    //private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test1.acc";
    private BufferedOutputStream outputStream;

    public void createFile(String file_ext) {

        String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + TimeStamp.getTimeStamp() + "." + file_ext;

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
                Log.i("STREAM", "data : " + data.toString());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

}
