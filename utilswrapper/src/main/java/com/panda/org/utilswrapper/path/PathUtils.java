package com.panda.org.utilswrapper.path;

import android.os.Environment;

/**
 * Created by rd0348 on 2017/11/7 0007.
 */

public class PathUtils {

    public static String getSDPath() {
        return Environment.getExternalStorageDirectory() + "/";
    }

    public static String getSDPath(String filename) {
        return Environment.getExternalStorageDirectory() + "/" + filename;
    }

}
