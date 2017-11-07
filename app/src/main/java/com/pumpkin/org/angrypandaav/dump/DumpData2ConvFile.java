package com.pumpkin.org.angrypandaav.dump;

import android.os.Environment;

import com.apkfuns.logutils.LogUtils;
import com.panda.org.utilswrapper.TimeStamp;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * Created by rd0348 on 2017/11/6 0006.
 * pcm 转 wav
 * 从文件角度:wav实际上是在pcm数据前面加一个header封装数据
 */

public class DumpData2ConvFile implements IDumpData {

    private RandomAccessFile outputStream;

    public void createFile(String file_ext) throws IOException {

        String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + TimeStamp.getTimeStamp() + "." + file_ext;

        File f = new File(path);

        if (f.exists()) {
            f.delete();
        } else {
            File parentDir = f.getParentFile();
            if (!parentDir.exists()) {
                parentDir.mkdirs();
            }
        }

        RandomAccessFile outputStream = new RandomAccessFile(f, "rw");
        // 16K、16bit、单声道
        /* RIFF header */
        outputStream.writeBytes("RIFF"); // riff id
        outputStream.writeInt(0); // riff chunk size *PLACEHOLDER*
        outputStream.writeBytes("WAVE"); // wave type

        /* fmt chunk */
        outputStream.writeBytes("fmt "); // fmt id
        outputStream.writeInt(Integer.reverseBytes(16)); // fmt chunk size
        outputStream.writeShort(Short.reverseBytes((short) 1)); // format: 1(PCM)
        outputStream.writeShort(Short.reverseBytes((short) 1)); // channels: 1
        outputStream.writeInt(Integer.reverseBytes(16000)); // samples per second
        outputStream.writeInt(Integer.reverseBytes((int) (1 * 16000 * 16 / 8))); // BPSecond
        outputStream.writeShort(Short.reverseBytes((short) (1 * 16 / 8))); // BPSample
        outputStream.writeShort(Short.reverseBytes((short) (1 * 16))); // bPSample

        /* data chunk */
        outputStream.writeBytes("data"); // data id
        outputStream.writeInt(0); // data chunk size *PLACEHOLDER*

    }

    /*
    * 输入pcm data
    * 输出到wav文件
    * */
    @Override
    public void onFrame(byte[] data, int len) throws IOException {
        outputStream.write(data, 0/*offset*/, len);
        LogUtils.d("fwrite: " + len);
    }

    public void closeFile() throws IOException {

        try {
            outputStream.seek(4); // riff chunk size
            outputStream.writeInt(Integer.reverseBytes((int) (outputStream.length() - 8)));

            outputStream.seek(40); // data chunk size
            outputStream.writeInt(Integer.reverseBytes((int) (outputStream.length() - 44)));

            LogUtils.d("wav size: " + outputStream.length());

        } finally {
            outputStream.close();
        }

    }

}

/*
* WAV：wav是一种无损的音频文件格式，WAV符合 PIFF(Resource Interchange File Format)规范。所有的WAV都有一个文件头，这个文件头音频流的编码参数。WAV对音频流的编码没有硬性规定，除了PCM之外，还有几乎所有支持ACM规范的编码都可以为WAV的音频流进行编码。

* PCM:PCM（Pulse Code Modulation----脉码调制录音)。所谓PCM录音就是将声音等模拟信号变成符号化的脉冲列，再予以记录。PCM信号是由[1]、[0]等符号构成的数字信号，而未经过任何编码和压缩处理。与模拟信号比，它不易受传送系统的杂波及失真的影响。动态范围宽，可得到音质相当好的影响效果。

* 简单来说：wav是一种无损的音频文件格式，pcm是没有压缩的编码方式
* */

/*
* //音频头部格式
struct wave_pcm_hdr
{
    char            riff[4];                        // = "RIFF"
    SR_DWORD        size_8;                         // = FileSize - 8
    char            wave[4];                        // = "WAVE"
    char            fmt[4];                         // = "fmt "
    SR_DWORD        dwFmtSize;                      // = 下一个结构体的大小 : 16

    SR_WORD         format_tag;              // = PCM : 1
    SR_WORD         channels;                // = 通道数 : 1
    SR_DWORD        samples_per_sec;         // = 采样率 : 8000 | 6000 | 11025 | 16000
    SR_DWORD        avg_bytes_per_sec;       // = 每秒字节数 : dwSamplesPerSec * wBitsPerSample / 8
    SR_WORD         block_align;             // = 每采样点字节数 : wBitsPerSample / 8
    SR_WORD         bits_per_sample;         // = 量化比特数: 8 | 16

    char            data[4];                 // = "data";
    SR_DWORD        data_size;               // = 纯数据长度 : FileSize - 44
} ;

//默认音频头部数据
struct wave_pcm_hdr default_pcmwavhdr =
{
    { 'R', 'I', 'F', 'F' },
    0,
    {'W', 'A', 'V', 'E'},
    {'f', 'm', 't', ' '},
    16,
    1,
    1,
    16000,
    32000,
    2,
    16,
    {'d', 'a', 't', 'a'},
    0
};
* */