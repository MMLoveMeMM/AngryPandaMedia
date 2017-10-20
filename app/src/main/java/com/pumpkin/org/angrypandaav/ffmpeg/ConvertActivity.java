package com.pumpkin.org.angrypandaav.ffmpeg;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;

import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;
import com.pumpkin.org.angrypandaav.R;

public class ConvertActivity extends Activity {

    private static String srcpath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/tiger.mp4";
    private static String destpath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/tiger001.yuv";

    private Button mButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_convert);

        mButton = (Button) findViewById(R.id.start);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /*
                * 这里是否需要新建一个线程
                * 根据native层决定
                * 如果native层会创建线程,那么java层就不需要再创建线程
                * 默认native层创建了线程
                * */
                FFmpegWrapperProxy.getInstance().MP4ToYUVDecode(srcpath, destpath);
                /*new Thread(new Runnable() {
                    @Override
                    public void run() {
                        FFmpegWrapperProxy.getInstance().MP4ToYUVDecode(srcpath,destpath);
                    }
                }).start();*/
            }
        });

    }


}
