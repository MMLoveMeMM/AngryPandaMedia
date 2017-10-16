package com.pumpkin.org.ffmpegwrapper;

/**
 * Created by rd0348 on 2017/10/16 0016.
 */

public interface IYuvListener {
    void onFrameYuv(byte[] buffer, int length);
}
