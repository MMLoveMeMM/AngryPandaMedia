package com.panda.org.aacwrapper.test;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.panda.org.aacwrapper.AACManagerProxy;
import com.panda.org.aacwrapper.R;
import com.panda.org.utilswrapper.path.PathUtils;

public class AccActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_acc);


    }

    @Override
    protected void onResume() {
        super.onResume();

        new Thread(new Runnable() {
            @Override
            public void run() {
                AACManagerProxy.getInstance().Pcm2Aac(PathUtils.getSDPath(""), PathUtils.getSDPath(""));
            }
        }).start();

    }
}
