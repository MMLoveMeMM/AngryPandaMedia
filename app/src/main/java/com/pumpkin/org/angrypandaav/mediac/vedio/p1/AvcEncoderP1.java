package com.pumpkin.org.angrypandaav.mediac.vedio.p1;

import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Created by rd0348 on 2017/9/29 0029.
 */

public class AvcEncoderP1 {

    private MediaCodec mediaCodec;
    private MediaFormat mediaFormat;

    private int width;
    private int height;
    private int framerate;
    private int bitrate;

    private int framecnt=0;

    public AvcEncoderP1(int width, int height, int framerate, int bitrate) {

        this.width = width;
        this.height = height;
        this.framerate = framerate;
        this.bitrate = bitrate;

    }

    /*
    * 初始化硬件编码器
    * */
    private void initMediaCodec() {

        /*
        * 创建硬编码器
        * 编码格式一定要设置好
        * 解码的时候可以不用设置,或者参照编码的设置进行解码
        * */
        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
        mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar);
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, width * height * 5);
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, framerate);
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);// 这个间隔越大,B,P帧一般来说就会越多
        try {
            /*
            * 做视频编码
            * */
            mediaCodec = MediaCodec.createEncoderByType("video/avc");
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        mediaCodec.start();

    }

    /*
    * 开始喂数据转码
    * */
    private byte[] configbyte;
    private void onFrame(byte[] buf, int offset, int length, int flag) {

        ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers(); // 申请空间
        ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();

        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(buf, offset, length); // 将YUV420P数据加入缓存
            // 下面即进行编码
            mediaCodec.queueInputBuffer(inputBufferIndex, 0, length, framecnt * 1000000 / framerate, 0);
            framecnt++;
        }

        //尝试获取输出数据的信息，关于bytebuffer的信息将封装在bufferinfo里面，返回该bytebuffer在队列中的位置
        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo,0);
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
            }else{
                // outData 即是需要保存的H264数据
                // 数据可以保存到文件,或者通过网络传输
            }

            mediaCodec.releaseOutputBuffer(outputBufferIndex, true);
            outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        }

    }



}
