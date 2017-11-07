package com.pumpkin.org.angrypandaav.media.test;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.pumpkin.org.angrypandaav.R;
import com.pumpkin.org.angrypandaav.dump.DumpData2ConvFile;
import com.pumpkin.org.angrypandaav.dump.DumpData2File;
import com.pumpkin.org.angrypandaav.dump.IDumpData;
import com.pumpkin.org.angrypandaav.media.AudioRecorderThread;

import java.io.IOException;

public class MediaActivity extends Activity {

    private Button mMediaBtn;

    private AudioRecorderThread audioRecorderThread;
    private DumpData2ConvFile dumpData2File;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_media);

        dumpData2File = new DumpData2ConvFile();
        audioRecorderThread = new AudioRecorderThread(dumpData2File, "acctest.aac");
        mMediaBtn = (Button) findViewById(R.id.media);
        mMediaBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                audioRecorderThread.start();
            }
        });

        try {
            dumpData2File.createFile("wav");
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        try {
            dumpData2File.closeFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
