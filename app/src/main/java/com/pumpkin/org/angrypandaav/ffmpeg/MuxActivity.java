package com.pumpkin.org.angrypandaav.ffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;

import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;
import com.pumpkin.org.angrypandaav.R;

public class MuxActivity extends Activity {

    private String srcpath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/input.mp4";
    private String vediopath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" /*+ TimeStamp.getTimeStamp() */ + "445566.h264";
    private String audiopath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" /*+ TimeStamp.getTimeStamp()*/ + "445566.aac";

    private Button mMuxBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mux);

        mMuxBtn = (Button) findViewById(R.id.mux);
        mMuxBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                //FFmpegWrapperProxy.getInstance().demuxMp4(srcpath,vediopath,audiopath);
                FFmpegWrapperProxy.getInstance().demuxMp4ext(srcpath, vediopath, audiopath);
            }
        });

    }
}
