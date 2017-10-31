package com.pumpkin.org.angrypandaav.media;

import android.media.AudioRecord;

/**
 * Created by rd0348 on 2017/10/31 0031.
 */

public class AudioRecorderThread extends Thread {

    private int sampleRate = 16000;
    private AudioRecord audioRecord;
    private int minBufferSize = 0;

    public AudioRecorderThread() {

    }

    @Override
    public void run() {
        super.run();
    }
}
