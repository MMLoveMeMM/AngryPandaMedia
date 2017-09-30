package com.pumpkin.org.angrypandaav.mediac.acc.p1.test;

import android.app.Activity;
import android.os.Bundle;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.AudioEncoderP1;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

public class AudioEncoderP1Act extends Activity {

    private AudioEncoderP1 audioEncoder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_encoder_p1);

        audioEncoder = new AudioEncoderP1();
        new Thread() {
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
//						audioEncoder.fireAudio(buffer, len);
                    }
                }
            };
        }.start();

    }
}
