package com.pumpkin.org.angrypandaav.dump;

import java.io.IOException;

/**
 * Created by rd0348 on 2017/9/30 0030.
 */

public interface IDumpData {

    public void onFrame(byte[] data, int len) throws IOException;

}
