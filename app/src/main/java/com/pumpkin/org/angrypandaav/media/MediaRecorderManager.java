package com.pumpkin.org.angrypandaav.media;

import android.media.MediaRecorder;
import android.os.Environment;
import android.util.Log;

import com.panda.org.utilswrapper.time.TimeStamp;

import java.io.File;
import java.io.IOException;

/**
 * Created by rd0348 on 2017/10/21 0021.
 */

public class MediaRecorderManager {
    private static final MediaRecorderManager audInstance = new MediaRecorderManager();

    public static MediaRecorderManager getInstance() {
        return audInstance;
    }

    //文件路径
    private String filePath;
    //文件夹路径
    private String FolderPath;

    private MediaRecorder mMediaRecorder;
    private final String TAG = "fan";
    public static final int MAX_LENGTH = 1000 * 60 * 10;// 最大录音时长1000*60*10;

    private int mFormatType;
    private String mFileExt;

    /**
     * 文件存储默认sdcard/record
     */
    public MediaRecorderManager() {

        //默认保存路径为/sdcard/record/下
        this(Environment.getExternalStorageDirectory() + "/");
    }

    public MediaRecorderManager(String filePath) {

        File path = new File(filePath);
        if (!path.exists())
            path.mkdirs();

        this.FolderPath = filePath;
    }

    public void setFormatType(int type) {

        mFormatType = type;

        switch (mFormatType) {
            case MediaRecorder.OutputFormat.DEFAULT:
                mFileExt = "amr";
                break;
            case MediaRecorder.OutputFormat.THREE_GPP:
                mFileExt = "gpp";
                break;
            case MediaRecorder.OutputFormat.MPEG_4:
                mFileExt = "mpeg";
                break;
            case MediaRecorder.OutputFormat.RAW_AMR:
                mFileExt = "raw";
                break;
            case MediaRecorder.OutputFormat.AMR_WB:
                mFileExt = "amr";
                break;
            case MediaRecorder.OutputFormat.AAC_ADTS:
                mFileExt = "acc";
                break;
            case MediaRecorder.OutputFormat.WEBM:
                mFileExt = "webm";
                break;
            default:
                mFormatType = MediaRecorder.OutputFormat.DEFAULT;
                mFileExt = "amr";
                break;
        }

    }

    private long startTime;
    private long endTime;

    /**
     * 开始录音 使用amr格式
     * 录音文件
     *
     * @return
     */
    public void startRecord() {
        // 开始录音
        /* ①Initial：实例化MediaRecorder对象 */
        if (mMediaRecorder == null)
            mMediaRecorder = new MediaRecorder();
        try {
            /* ②setAudioSource/setVedioSource */
            mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);// 设置麦克风
            /* ②设置音频文件的编码：AAC/AMR_NB/AMR_MB/Default 声音的（波形）的采样 */
            mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.DEFAULT);
            /*
             * ②设置输出文件的格式：THREE_GPP/MPEG-4/RAW_AMR/Default THREE_GPP(3gp格式
             * ，H263视频/ARM音频编码)、MPEG-4、RAW_AMR(只支持音频且音频编码要求为AMR_NB)
             */
            mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);

            filePath = FolderPath + TimeStamp.getTimeStamp() + ".amr";
            /* ③准备 */
            mMediaRecorder.setOutputFile(filePath);
            mMediaRecorder.setMaxDuration(MAX_LENGTH);
            mMediaRecorder.prepare();
            /* ④开始 */
            mMediaRecorder.start();
            // AudioRecord audioRecord.
            /* 获取开始时间* */
            startTime = System.currentTimeMillis();
            Log.e("fan", "startTime" + startTime);
        } catch (IllegalStateException e) {
            Log.i(TAG, "call startAmr(File mRecAudioFile) failed!" + e.getMessage());
        } catch (IOException e) {
            Log.i(TAG, "call startAmr(File mRecAudioFile) failed!" + e.getMessage());
        }
    }

    /**
     * 停止录音
     */
    public long stopRecord() {
        if (mMediaRecorder == null)
            return 0L;
        endTime = System.currentTimeMillis();

        try {
            mMediaRecorder.stop();
            mMediaRecorder.reset();
            mMediaRecorder.release();
            mMediaRecorder = null;

            filePath = "";

        } catch (RuntimeException e) {
            mMediaRecorder.reset();
            mMediaRecorder.release();
            mMediaRecorder = null;

            File file = new File(filePath);
            if (file.exists())
                file.delete();

            filePath = "";

        }
        return endTime - startTime;
    }

    /**
     * 取消录音
     */
    public void cancelRecord() {

        try {

            mMediaRecorder.stop();
            mMediaRecorder.reset();
            mMediaRecorder.release();
            mMediaRecorder = null;

        } catch (RuntimeException e) {
            mMediaRecorder.reset();
            mMediaRecorder.release();
            mMediaRecorder = null;
        }
        File file = new File(filePath);
        if (file.exists())
            file.delete();

        filePath = "";

    }

}
