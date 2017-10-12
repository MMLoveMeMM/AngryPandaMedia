package com.pumpkin.org.x264wrapper;

/**
 * Created by rd0348 on 2017/10/11 0011.
 */

public interface Ix264Listener {

    void onFrameH264(byte[] buffer, int length);

}
