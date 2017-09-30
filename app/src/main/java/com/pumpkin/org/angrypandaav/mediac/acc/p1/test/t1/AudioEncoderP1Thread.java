package com.pumpkin.org.angrypandaav.mediac.acc.p1.test.t1;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

import com.pumpkin.org.angrypandaav.mediac.acc.p1.AudioEncoderP1;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.test.i.IDumpAudio;

/**
 * Created by rd0348 on 2017/9/30 0030.
 */

public class AudioEncoderP1Thread extends Thread {

    private IDumpAudio dumpAudio;
    private AudioEncoderP1 audioEncoder;

    public AudioEncoderP1Thread(IDumpAudio dumpAudio) {
        this.dumpAudio = dumpAudio;
        audioEncoder = new AudioEncoderP1(dumpAudio);
    }

    @Override
    public void run() {

        int minBufferSize = AudioRecord.getMinBufferSize(44100,
                AudioFormat.CHANNEL_IN_MONO,
                AudioFormat.ENCODING_PCM_16BIT);
        AudioRecord audioRecord = new AudioRecord(
                MediaRecorder.AudioSource.MIC, 44100,
                AudioFormat.CHANNEL_IN_MONO,
                AudioFormat.ENCODING_PCM_16BIT, minBufferSize);
        audioRecord.startRecording();
        while (true) {
            byte[] buffer = new byte[2048];
            int len = audioRecord.read(buffer, 0, buffer.length);
            if (0 < len) {
                /*
                * 语音硬编码
                * */
                audioEncoder.onFrame(buffer, len);
            }
        }

    }
}
