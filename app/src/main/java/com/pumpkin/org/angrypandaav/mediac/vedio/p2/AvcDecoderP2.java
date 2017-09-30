package com.pumpkin.org.angrypandaav.mediac.vedio.p2;

import android.media.MediaCodec;
import android.media.MediaFormat;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Created by rd0348 on 2017/9/29 0029.
 */

public class AvcDecoderP2 {

    private MediaCodec mediaCodec;

    private String minetype="video/avc";

    private int framecnt = 0;

    private boolean iscodec=false;

    private int width;
    private int height;

    private int framerate;

    public AvcDecoderP2(int width,int height,String mine) {

        this.width=width;
        this.height=height;
        if(mine!=null){
            minetype=mine;
        }

    }

    /*
    * 初始化解码器
    * */
    public void initMediacodec() throws IOException {
        /*
        * 创建硬解码器
        * */
        mediaCodec = MediaCodec.createDecoderByType(minetype);
        MediaFormat mediaFormat = MediaFormat.createVideoFormat(minetype, width, height);
        /*
        * 参数设置
        * 下面的参数设置最终是由编码决定的
        * 所以不需要也可以
        * */
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE,
                25);
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE,
                1200000);
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL,
                1);
        /*
        * 下面不配置surface输出显示
        * */
        mediaCodec.configure(mediaFormat, null, null, 0); // 这个地方不需要设置surface
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
            mediaCodec.queueInputBuffer(inputBufferIndex, 0, length, framecnt * 1000000 / framerate, 0);
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

                // 这个keyframe 即是需要保存的H264数据
                // 数据可以保存到文件,或者通过网络传输
                // 一般数据使用 生成者-消费者 的方式进行数据交换

            }else{
                // outData 即是需要保存的H264数据
                // 数据可以保存到文件,或者通过网络传输
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
