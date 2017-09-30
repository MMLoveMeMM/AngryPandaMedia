package com.pumpkin.org.angrypandaav.mediac.acc.p1.test.t2;

import android.app.Activity;
import android.os.Bundle;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.test.i.DumpPlayerAudio;

public class AACDecoderP1Act extends Activity {

    private DumpPlayerAudio dumpPlayerAudio;

    private AACDecoderThread aacDecoderThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_aacdecoder_p1);

        dumpPlayerAudio = new DumpPlayerAudio();
        dumpPlayerAudio.startPlayer();

        aacDecoderThread = new AACDecoderThread(dumpPlayerAudio);
        aacDecoderThread.start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        if (dumpPlayerAudio != null) {
            dumpPlayerAudio.stopPlayer();
        }
    }
}
