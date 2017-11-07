package com.pumpkin.org.angrypandaav.media;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Environment;

import com.apkfuns.logutils.LogUtils;
import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;
import com.pumpkin.org.angrypandaav.dump.IDumpData;

import java.io.IOException;

/**
 * Created by rd0348 on 2017/10/31 0031.
 */

public class AudioRecorderThread extends Thread {

    //private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/accdd.acc";

    private int sampleRate = 16000;
    private AudioRecord audioRecord;
    private int minBufferSize = 0;
    private boolean isRecording = false;
    private String path;

    private IDumpData dumpData;

    public AudioRecorderThread(IDumpData dumpData, String filename) {

        minBufferSize = AudioRecord.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
        minBufferSize = 2048;
        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, minBufferSize);
        isRecording = false;

        path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + filename;

        this.dumpData = dumpData;

    }

    @Override
    public synchronized void start() {
        audioRecord.startRecording();
        isRecording = true;

        FFmpegWrapperProxy.getInstance().initRecord(path);

        super.start();//这个非常重要,不调用,下面的run是不会启动,即线程不会启动,只会单做普通的方法使用
    }

    @Override
    public void run() {
        super.run();

        while (isRecording) {

            byte[] bytes = new byte[minBufferSize];
            if (audioRecord == null) {
                return;
            }
            int res = audioRecord.read(bytes, 0, minBufferSize);
            if (res > 0 && isRecording == true) {
                //FFmpegWrapperProxy.getInstance().writeAudioData(bytes);
                LogUtils.i(bytes);
                try {
                    dumpData.onFrame(bytes, bytes.length);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

        }

    }

    public void stopRecord() {

        FFmpegWrapperProxy.getInstance().recordClose();
        isRecording = false;
        audioRecord.stop();
        audioRecord.release();
        audioRecord = null;

    }

}
