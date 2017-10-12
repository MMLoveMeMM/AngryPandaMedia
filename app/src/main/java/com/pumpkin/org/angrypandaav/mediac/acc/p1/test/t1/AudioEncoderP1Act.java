package com.pumpkin.org.angrypandaav.mediac.acc.p1.test.t1;

import android.app.Activity;
import android.os.Bundle;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.AudioEncoderP1;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.test.i.DumpFileAudio;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.test.i.DumpPlayerAudio;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

public class AudioEncoderP1Act extends Activity {

    private DumpFileAudio dumpAudio;
    private AudioEncoderP1Thread audioEncoderP1Thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_encoder_p1);

        dumpAudio = new DumpFileAudio();
        dumpAudio.createFile();

        audioEncoderP1Thread = new AudioEncoderP1Thread(dumpAudio);
        audioEncoderP1Thread.start();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (dumpAudio != null) {
            dumpAudio.closeFile();
        }
    }
}
