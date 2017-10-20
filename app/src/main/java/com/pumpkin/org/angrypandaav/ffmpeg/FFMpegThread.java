package com.pumpkin.org.angrypandaav.ffmpeg;

import com.apkfuns.logutils.LogUtils;
import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;

import java.util.concurrent.ArrayBlockingQueue;

import static com.pumpkin.org.angrypandaav.ffmpeg.FFMpegThread.FFMpegMachine.IDLE_STATE;

/**
 * Created by rd0348 on 2017/10/20 0020.
 */

public class FFMpegThread extends Thread {

    private ArrayBlockingQueue<byte[]> queue;

    private int width;
    private int height;

    enum FFMpegMachine {
        INIT_STATE,
        DOING_STATE,
        CLOSE_STATE,
        IDLE_STATE
    }

    ;

    enum EncodeMachine {
        IDLE_CODEC_STATE,
        INIT_CODEC_STATE,
        DOING_CODEC_STATE,
        CLOSE_CODEC_STATE
    }

    private FFMpegMachine state;

    private EncodeMachine processstate;

    public FFMpegThread(ArrayBlockingQueue<byte[]> queue) {

        state = FFMpegMachine.IDLE_STATE;
        processstate = EncodeMachine.IDLE_CODEC_STATE;

        this.queue = queue;
    }

    @Override
    public void run() {
        super.run();

        while (true) {

            switch (state) {
                case IDLE_STATE:
                    //do nothing
                    processstate = EncodeMachine.IDLE_CODEC_STATE;
                    break;
                case INIT_STATE:
                    LogUtils.i("initencode **************");
                    if (processstate == EncodeMachine.IDLE_CODEC_STATE) {
                        LogUtils.i("initencode +++++++++++++++++++++++");
                        state = FFMpegMachine.IDLE_STATE;//初始化以后等待
                        int ret = FFmpegWrapperProxy.getInstance().initencode(width, height);
                        if (ret == -1) {
                            LogUtils.e("encode fail ...");
                        } else {
                            processstate = EncodeMachine.INIT_CODEC_STATE;
                            state = FFMpegMachine.DOING_STATE;
                            LogUtils.e("encode device OK ...");
                        }
                    }
                    break;
                case DOING_STATE:
                    //LogUtils.i("doing encoding entry !");
                    if (processstate == EncodeMachine.INIT_CODEC_STATE || processstate == EncodeMachine.DOING_CODEC_STATE) {
                        if (!queue.isEmpty()) {
                        /*
                        * 不为空就取出数据进行编码处理
                        * */
                            byte[] datas;
                            try {
                                LogUtils.i("doing encoding ...");
                                datas = queue.take(); // 如果没有数据,这个地方会blocking
                                if (datas != null) {
                                    FFmpegWrapperProxy.getInstance().onframe(datas);
                                }
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                        processstate = EncodeMachine.DOING_CODEC_STATE;
                    }

                    break;
                case CLOSE_STATE:
                    if (processstate == EncodeMachine.DOING_CODEC_STATE) {
                        FFmpegWrapperProxy.getInstance().closeencode();
                        processstate = EncodeMachine.IDLE_CODEC_STATE;
                    }
                    state = FFMpegMachine.IDLE_STATE;//关闭以后等待

                    break;
                default:
                    break;
            }

        }


    }

    public void initFFMpeg(int width, int height) {

        this.width = width;
        this.height = height;

        state = FFMpegMachine.INIT_STATE;
        processstate = EncodeMachine.IDLE_CODEC_STATE;
    }

    public void startFFMpeg() {
        state = FFMpegMachine.DOING_STATE;
    }

    public void closeFFMpeg() {
        state = FFMpegMachine.CLOSE_STATE;
        processstate = EncodeMachine.IDLE_CODEC_STATE;
    }

}
