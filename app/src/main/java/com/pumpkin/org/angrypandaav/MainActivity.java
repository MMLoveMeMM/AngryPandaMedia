package com.pumpkin.org.angrypandaav;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.pumpkin.org.angrypandaav.manager.camera.test.CameraGLTestActivity;
import com.pumpkin.org.angrypandaav.manager.camera.test.CameraTestActivity;
import com.pumpkin.org.angrypandaav.mediac.acc.p1.test.t1.AudioEncoderP1Act;


public class MainActivity extends Activity implements View.OnClickListener{

    private Button mCameraMCBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mCameraMCBtn=(Button)findViewById(R.id.camera_mc);
        mCameraMCBtn.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {

        int id=v.getId();
        switch (id) {
            case R.id.camera_mc:
                startActivity(new Intent(this, CameraGLTestActivity.class));
                break;
        }

    }

}
