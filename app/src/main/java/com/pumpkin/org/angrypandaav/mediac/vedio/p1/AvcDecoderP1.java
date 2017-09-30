package com.pumpkin.org.angrypandaav.mediac.vedio.p1;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Created by rd0348 on 2017/9/29 0029.
 * 这个硬解码器有个设置不好是:这里将数据输出到surface上面
 * 很多情况可能不需要将数据输出到surface上面
 * 这个解码将在P2级将数据输出给其他形式
 */

public class AvcDecoderP1 {

    private final static String TAG ="AvcDecoderP1";

    private final static String MINE_TYPE ="video/avc";
    private MediaCodec mediaCodec;
    private MediaFormat mediaFormat;

    private int width;
    private int height;
    private int framerate = 30;

    private int framecnt = 0;

    private SurfaceHolder holder;

    private boolean iscodec=false;

    public AvcDecoderP1(SurfaceHolder holder, int width, int height, int framerate) {

        this.width = width;
        this.height = height;
        this.framerate = framerate;

        this.holder=holder;

    }

    public AvcDecoderP1(SurfaceHolder holder) {

        this.width = holder.getSurfaceFrame().width();
        this.height = holder.getSurfaceFrame().height();

        this.holder=holder;

    }

    /*
    * 预备编码
    * */
    public void startMediacodec() throws IOException {

        if(!iscodec) {
            initMediacodec();
        }

    }
    /*
    * 初始化解码器
    * */
    private void initMediacodec() throws IOException {

        /*
        * 创建硬解码器
        * */
        mediaCodec = MediaCodec.createDecoderByType(MINE_TYPE);
        MediaFormat mediaFormat = MediaFormat.createVideoFormat(MINE_TYPE, width, height);
        mediaCodec.configure(mediaFormat, holder.getSurface(), null, 0);
        mediaCodec.start();

    }

    private byte[] configbyte;
    public void onFrame(byte[] buf, int offset, int length, int flag) {

        ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
        ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();

        //-1表示一直等待；0表示不等待；其他大于0的参数表示等待毫秒数
        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(buf, offset, length);
            mediaCodec.queueInputBuffer(inputBufferIndex, 0, length, framecnt * 1000000 / framerate, flag);
            framecnt++;
        }

        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        //循环解码，直到数据全部解码完成
        while (outputBufferIndex >= 0) {

            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
            byte[] outData = new byte[bufferInfo.size];
            outputBuffer.get(outData);
            if(bufferInfo.flags == 2/*BUFFER_FLAG_CODEC_CONFIG*/){
                configbyte = new byte[bufferInfo.size];
                configbyte = outData;
            }else if(bufferInfo.flags == 1/*MediaCodec.BufferInfo.BUFFER_FLAG_KEY_FRAME*/){
                byte[] keyframe = new byte[bufferInfo.size + configbyte.length];
                System.arraycopy(configbyte, 0, keyframe, 0, configbyte.length);
                System.arraycopy(outData, 0, keyframe, configbyte.length, outData.length);

                // 这个keyframe 即是需要保存的YUV数据
                // 数据可以保存到文件,或者通过网络传输
                Log.i(TAG,"keyframe len : "+keyframe.length);
            }else{
                // outData 即是需要保存的YUV数据
                // 数据可以保存到文件,或者通过网络传输
                Log.i(TAG,"outData len : "+outData.length);
            }

            //true : 将解码的数据显示到surface上
            mediaCodec.releaseOutputBuffer(outputBufferIndex, true);
            outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        }

    }

    /**
     *停止解码，释放解码器
     */
    public void stopCodec() {

        try {
            mediaCodec.stop();
            mediaCodec.release();
            mediaCodec = null;
        } catch (Exception e) {
            e.printStackTrace();
            mediaCodec = null;
        }
    }

}
