package com.panda.org.extractorwrapper.test;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.panda.org.extractorwrapper.ExtractorWrapper;
import com.panda.org.extractorwrapper.R;

public class ExtractorActivity extends Activity {

    private ExtractorWrapper mExtractorWrapper;
    private Button mExtractorBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_extractor);

        mExtractorWrapper = new ExtractorWrapper();
        mExtractorWrapper.setType(3);

        mExtractorBtn = (Button) findViewById(R.id.extractor);
        mExtractorBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mExtractorWrapper.start();
            }
        });

    }

}
