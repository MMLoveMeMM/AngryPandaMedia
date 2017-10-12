package com.pumpkin.org.angrypandaav.mediac.vedio.p0;

import android.annotation.SuppressLint;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Build;
import android.os.Environment;
import android.view.Surface;
import android.view.SurfaceView;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.io.IOException;
import java.util.concurrent.ArrayBlockingQueue;

/**
 * Created by rd0348 on 2017/9/29 0029.
 * 这个虽然可以大致解码,但是显然数据在显示存在问题
 * 因为读取文件和读取帧是两码事,读取文件的一个缓存不等于一帧数据
 * 读取文件仍然需要识别sps和pps,两个sps或者pps之间的数据才是一帧有效数据
 * 所以需要判断(H264帧大小不超过200k)
 * 参考p3代码,p1,p2代码是从解码功能角度是可行的,但是显示是不正确的---没有对帧的头部进行处理,导致解码出来的帧数据混乱.
 * 如果是rtsp传输,一般是一帧一帧的数据传输,只需要判断每帧的头部类型就可以了.
 */

public class AvcDecoder {

    private MediaCodec mediaCodec;

    /*
    * 这个队列用于H264转YUV,YUV可以显示
    * */
    private ArrayBlockingQueue<byte[]> H264Queue=new ArrayBlockingQueue<byte[]>(10);

    byte[] sps = { 0, 0, 0, 1, 103, 100, 0, 40, -84, 52, -59, 1, -32, 17, 31, 120, 11, 80, 16, 16, 31, 0, 0, 3, 3, -23, 0, 0, -22, 96, -108 };
    byte[] pps = { 0, 0, 0, 1, 104, -18, 60, -128 };

    public AvcDecoder(Surface surface , ArrayBlockingQueue<byte[]> que, int width, int height) throws IOException {

        H264Queue=que;

        mediaCodec = MediaCodec.createDecoderByType("video/avc");
        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
        mediaFormat.setByteBuffer("csd-0"  , ByteBuffer.wrap(sps));
        mediaFormat.setByteBuffer("csd-1", ByteBuffer.wrap(pps));
        //mediaCodec.configure(mediaFormat, surface, null, 0); // 这个不用显示
        mediaCodec.start();

        createfile();
    }

    public AvcDecoder(int width, int height, SurfaceView surface) throws IOException {

        mediaCodec = MediaCodec.createDecoderByType("video/avc");
        MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
        //mediaCodec.configure(mediaFormat, surface, null, 0); // 这个不用显示
        mediaCodec.start();

    }

    private static String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/test1.yuv";
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
    * 下面和编码一样,同样开一个线程处理
    * */
    public void ThreadDecode(){


        boolean isEOS = false;
        long startMs = System.currentTimeMillis();

        Thread ThreadMediacDecode=new Thread(new Runnable() {
            @Override
            public void run() {

                byte[] h264Data=H264Queue.poll();/*new byte[1000]*/;

                for(;;) {
                    ByteBuffer inputBuffer;// = mediaCodec.getInputBuffers();
                    ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
                    MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                    // 这里解释一下  传0是不等待 传-1是一直等待 但是传-1会在很多机器上挂掉，所以还是用0吧 丢帧总比挂掉强
                    int inputBufferIndex = mediaCodec.dequeueInputBuffer(0);
                    if (inputBufferIndex >= 0) {
                        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
                            // 从输入队列里去空闲buffer
                            inputBuffer = mediaCodec.getInputBuffers()[inputBufferIndex];
                            inputBuffer.clear();
                        } else {
                            // SDK_INT > LOLLIPOP
                            inputBuffer = mediaCodec.getInputBuffer(inputBufferIndex);
                        }
                        if (null != inputBuffer) {
                            // h264 data, offset, length
                            inputBuffer.put(h264Data, 0, h264Data.length);
                            // data from object send to mediacodec's input queue
                            /*
                            * 参考 : http://blog.csdn.net/lqhed/article/details/51812283
                            * */
                            switch (h264Data[4] & 0x1f) {
                                case 0x65/*KEY_FRAME*/:
                                    mediaCodec.queueInputBuffer(inputBufferIndex, 0, h264Data.length, 0L, MediaCodec.BUFFER_FLAG_KEY_FRAME);
                                    break;
                                case 7/*SPS_FRAME*/:
                                case 8/*PPS_FRAME*/:
                                    mediaCodec.queueInputBuffer(inputBufferIndex, 0, h264Data.length, 0L, MediaCodec.BUFFER_FLAG_CODEC_CONFIG);
                                    break;
                                default:
                                    mediaCodec.queueInputBuffer(inputBufferIndex, 0, h264Data.length, 0L, 0);
                                    break;
                            }
                        }
                        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
                        while (outputBufferIndex > 0){
                            /*
                            * 这个地方暂时不显示
                            * */
                            /*if (surface.isValid()) {
                                // 将解码后数据渲染到surface上
                                mediaCodec.releaseOutputBuffer(outputBufferIndex, true);
                            }*/
                            /*
                            * 写到本地文件中
                            * */
                            outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
                            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                            byte[] outData = new byte[bufferInfo.size];
                            outputBuffer.get(outData);
                            try {
                                outputStream.write(outData, 0, outData.length);
                            } catch (IOException e) {
                                e.printStackTrace();
                            }

                        }
                    }
                }


            }
        });

        ThreadMediacDecode.start();

    }

}
