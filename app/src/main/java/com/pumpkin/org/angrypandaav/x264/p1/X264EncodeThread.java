package com.pumpkin.org.angrypandaav.x264.p1;

import com.pumpkin.org.angrypandaav.dump.IDumpData;
import com.pumpkin.org.x264wrapper.Ix264Listener;
import com.pumpkin.org.x264wrapper.X264EncodeProxy;

import java.util.concurrent.ArrayBlockingQueue;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class X264EncodeThread extends Thread implements Ix264Listener {

    private X264EncodeProxy mX264EncodeProxy;

    private ArrayBlockingQueue<byte[]> queue;//共享数据
    private int m_width;
    private int m_height;

    private IDumpData dump;

    public X264EncodeThread(int width, int height, ArrayBlockingQueue<byte[]> queue) {
        this.m_width = width;
        this.m_height = height;
        this.queue = queue;

        mX264EncodeProxy.setmIx264Listener(this); // 回调解码后的数据

    }

    /*
    * 文件处理方式
    * */
    public void setDump(IDumpData dump) {
        this.dump = dump;
    }

    @Override
    public void run() {
        super.run();

        mX264EncodeProxy.InitX264Encode(m_width, m_height, 25, 256);

        byte[] input = null;
        /*
        * 将queue中的数据进行编码
        * */
        while (true) {

            if (queue.size() > 0) {
                input = queue.poll();
                /*
                * YUV420SP 转YUV420P
                * */
                byte[] yuv420sp = new byte[m_width * m_height * 3 / 2];
                //NV21ToNV12(input,yuv420sp,m_width,m_height);

            }

            if (input != null) {
                /*
                * 有数据开始转码
                * */
                mX264EncodeProxy.onFrame(input, input.length, 0);
            }

        }

    }

    /*
    * 将YUV420SP转换成YUV420P
    * */
    public void NV21ToNV12(byte[] nv21, byte[] nv12, int width, int height) {
        if (nv21 == null || nv12 == null) return;
        int framesize = width * height;
        int i = 0, j = 0;
        System.arraycopy(nv21, 0, nv12, 0, framesize);
        for (i = 0; i < framesize; i++) {
            nv12[i] = nv21[i];
        }
        for (j = 0; j < framesize / 2; j += 2) {
            nv12[framesize + j - 1] = nv21[j + framesize];
        }
        for (j = 0; j < framesize / 2; j += 2) {
            nv12[framesize + j] = nv21[j + framesize - 1];
        }
    }


    @Override
    public void onFrameH264(byte[] buffer, int length) {

        /*
        * 解码以后的数据回调处
        * */
        dump.onFrame(buffer, length); // 这里保存到文件中

    }
}
