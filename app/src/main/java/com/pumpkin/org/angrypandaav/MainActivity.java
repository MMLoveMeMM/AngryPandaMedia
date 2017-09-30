package com.pumpkin.org.angrypandaav;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.pumpkin.org.angrypandaav.mediac.vedio.p3.test.AvcDecoderP3Act;

public class MainActivity extends Activity implements View.OnClickListener{

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Button mCameraMCBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mCameraMCBtn=(Button)findViewById(R.id.camera_mc);
        mCameraMCBtn.setOnClickListener(this);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    @Override
    public void onClick(View v) {

        int id=v.getId();
        switch (id) {
            case R.id.camera_mc:
                startActivity(new Intent(this,AvcDecoderP3Act.class));
                break;
        }

    }

}
