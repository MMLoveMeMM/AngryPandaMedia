package com.pumpkin.org.angrypandaav.mediac.vedio.p1.test;

import android.os.Environment;
import android.util.Log;
import android.view.SurfaceHolder;

import com.pumpkin.org.angrypandaav.mediac.vedio.p1.AvcDecoderP1;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Created by rd0348 on 2017/9/30 0030.
 */

public class AVCDecoderP1Thread extends Thread {

    private final static String TAG ="AVCDecoderP1Thread";

    private final static String FILE_PATH= Environment.getExternalStorageDirectory().getAbsolutePath() + "/test1.h264";

    private AvcDecoderP1 avcDecoderP1;

    private SurfaceHolder surfaceHolder;

    public AVCDecoderP1Thread(SurfaceHolder holder) throws IOException {
        surfaceHolder=holder;

        avcDecoderP1=new AvcDecoderP1(surfaceHolder);

        /*
        * 预备解码
        * */
        avcDecoderP1.startMediacodec();

    }
    @Override
    public void run() {
        super.run();

        /*
        * 读取文件数据
        * */
        File file = new File(FILE_PATH);
        if(!file.exists() || !file.canRead()){
            Log.e(TAG,"failed to open h264 file.");
            return;
        }

        try {
            int len = 0;
            FileInputStream fis = new FileInputStream(file);
            /*
            * 1024*200 : VR手机勉强可以显示
            * 1024*25 : 一加手机
            * 这个地方的大小不可控,比较坑爹的
            * 这种直接从H264文件读取数据就塞到解码器中,怎么调节都是坑爹的
            * */
            byte[] buf = new byte[1024*25];
            while ((len = fis.read(buf)) > 0){
                avcDecoderP1.onFrame(buf,0,len,0);
                Log.i(TAG,"buf len : "+len);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch(IOException e){
            e.printStackTrace();
        }

    }
}
