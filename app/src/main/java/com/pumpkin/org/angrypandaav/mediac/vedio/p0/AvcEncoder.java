package com.pumpkin.org.angrypandaav.mediac.vedio.p0;

/**
 * Created by rd0348 on 2017/9/28 0028.
 */

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.concurrent.ArrayBlockingQueue;

import android.annotation.SuppressLint;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.os.Environment;
import android.view.Surface;


public class AvcEncoder
{
    private final static String TAG = "MeidaCodec";

    private int TIMEOUT_USEC = 12000;

    private MediaCodec mediaCodec;
    int m_width;
    int m_height;
    int m_framerate;
    byte[] m_info = null;

    public byte[] configbyte;

    /*
    * 这个队列用于YUV420SP转H264
    * */
    private ArrayBlockingQueue<byte[]> YUVQueue=new ArrayBlockingQueue<byte[]>(10);

    /*
    * 这个队列用于H264转YUV,YUV可以显示
    * */
    private ArrayBlockingQueue<byte[]> H264Queue=new ArrayBlockingQueue<byte[]>(10);


    private AvcDecoder mAvcDecoder;

    @SuppressLint("NewApi")
    public AvcEncoder(Surface mSurfaceView1, ArrayBlockingQueue<byte[]> queue, int width, int height, int framerate, int bitrate) throws IOException {

        YUVQueue=queue;

        m_width  = width;
        m_height = height;
        m_framerate = framerate;

        /*
        * 先设置编码格式
        * height,和width高度一致,表示视频不做大小缩放处理
        * */
        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
        mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar);
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, width*height*5);
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 30);
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);
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

        /*
        * 将转换的数据保存到sdcard本地
        * */
        createfile();
        /*
        * 如果要通过RSTP或者其他网络协议传输,就要再将转换的数据加入到网络数据传输队列中
        * */

        /*
        * 解码
        * */
        mAvcDecoder=new AvcDecoder(mSurfaceView1,H264Queue,width,height);

    }

    private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test1.h264";
    private BufferedOutputStream outputStream;
    FileOutputStream outStream;
    private void createfile(){
        File file = new File(path);
        if(file.exists()){
            file.delete();
        }
        try {
            outputStream = new BufferedOutputStream(new FileOutputStream(file));
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    @SuppressLint("NewApi")
    private void StopEncoder() {
        try {
            mediaCodec.stop();
            mediaCodec.release();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    /*
    * 同步转码方式
    * 输入/输出向转码模块申请buffer空间
    * input buffer ===> 转码 <==== output buffer
    * 转码正式开始流向
    * input buffer ===> 转码 ====> output buffer
    *
    * */
    ByteBuffer[] inputBuffers;
    ByteBuffer[] outputBuffers;

    public boolean isRuning = false;

    public void StopThread(){
        isRuning = false;
        try {
            StopEncoder();
            outputStream.flush();
            outputStream.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    int count = 0;

    public void StartEncoderThread(){
        Thread EncoderThread = new Thread(new Runnable() {

            @SuppressLint("NewApi")
            @Override
            public void run() {
                isRuning = true;
                byte[] input = null;
                long pts =  0;
                long generateIndex = 0;

                while (isRuning) {
                    if (YUVQueue.size() >0){

                        /*
                        * 从共有队列里面获取数据
                        * */
                        input = YUVQueue.poll();

                        /*
                        * android系统的摄像头拍摄的数据是yuv420sp的,这种数据需要再转成yuv420p
                        * 即NV21转成NV12格式,准确的说是数据排列方式
                        * */
                        byte[] yuv420sp = new byte[m_width*m_height*3/2];
                        NV21ToNV12(input,yuv420sp,m_width,m_height);
                        input = yuv420sp;//转换后的数据
                    }
                    if (input != null) {
                        try {
                            long startMs = System.currentTimeMillis();
                            ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
                            ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();

                            /*
                            * 申请传入编码数据的空间
                            * */
                            int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
                            if (inputBufferIndex >= 0) {
                                pts = computePresentationTime(generateIndex);
                                ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
                                inputBuffer.clear();
                                inputBuffer.put(input);
                                /*
                                * 申请空间成功以后,就把数据写入编码数据空间
                                * pts 一般是时间戳
                                * */
                                mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, pts, 0);
                                generateIndex += 1;
                            }

                            MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                            int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, TIMEOUT_USEC);
                            while (outputBufferIndex >= 0) {
                                //Log.i("AvcEncoder", "Get H264 Buffer Success! flag = "+bufferInfo.flags+",pts = "+bufferInfo.presentationTimeUs+"");
                                ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                                byte[] outData = new byte[bufferInfo.size];
                                outputBuffer.get(outData);
                                if(bufferInfo.flags == 2){
                                    configbyte = new byte[bufferInfo.size];
                                    configbyte = outData;
                                }else if(bufferInfo.flags == 1){
                                    byte[] keyframe = new byte[bufferInfo.size + configbyte.length];
                                    System.arraycopy(configbyte, 0, keyframe, 0, configbyte.length);
                                    System.arraycopy(outData, 0, keyframe, configbyte.length, outData.length);

                                    outputStream.write(keyframe, 0, keyframe.length);

                                    H264Queue.add(keyframe);
                                }else{
                                    outputStream.write(outData, 0, outData.length);

                                    H264Queue.add(outData);
                                }

                                mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, TIMEOUT_USEC);
                            }

                        } catch (Throwable t) {
                            t.printStackTrace();
                        }
                    } else {
                        try {
                            Thread.sleep(500);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        });
        EncoderThread.start();

        mAvcDecoder.ThreadDecode();

    }

    private void NV21ToNV12(byte[] nv21,byte[] nv12,int width,int height){
        if(nv21 == null || nv12 == null)return;
        int framesize = width*height;
        int i = 0,j = 0;
        System.arraycopy(nv21, 0, nv12, 0, framesize);
        for(i = 0; i < framesize; i++){
            nv12[i] = nv21[i];
        }
        for (j = 0; j < framesize/2; j+=2)
        {
            nv12[framesize + j-1] = nv21[j+framesize];
        }
        for (j = 0; j < framesize/2; j+=2)
        {
            nv12[framesize + j] = nv21[j+framesize-1];
        }
    }

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    private long computePresentationTime(long frameIndex) {
        return 132 + frameIndex * 1000000 / m_framerate;
    }
}