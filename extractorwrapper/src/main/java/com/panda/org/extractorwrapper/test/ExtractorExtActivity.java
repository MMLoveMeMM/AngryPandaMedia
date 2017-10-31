package com.panda.org.extractorwrapper.test;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;

import com.panda.org.extractorwrapper.ExtractorExtWrapper;
import com.panda.org.extractorwrapper.R;

public class ExtractorExtActivity extends Activity {

    private final static String SRC_PATH = Environment.getExternalStorageDirectory() + "/" + "input.mp4";
    private Button mExtractorBtn;

    private ExtractorExtWrapper extractorExtWrapper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_extractor_ext);

        extractorExtWrapper = new ExtractorExtWrapper(SRC_PATH, 1 * 1000 * 1000, 2 * 1000 * 1000);

        mExtractorBtn = (Button) findViewById(R.id.extractor);
        mExtractorBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                extractorExtWrapper.start();

            }
        });

    }
}
