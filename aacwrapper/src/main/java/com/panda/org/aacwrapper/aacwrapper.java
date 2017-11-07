package com.panda.org.aacwrapper;

/**
 * Created by rd0348 on 2017/11/1 0001.
 */

public class aacwrapper {

    static {
        System.loadLibrary("aacwrapper");
    }

    public native int initRecord(String outputPath);

    public native int writeAudioData(byte[] pcmData);

    public native int closeRecord();

    public native int pcm2Aac(String pcmfile, String aacfile);

    public native int pcm2AacDetail(String pcmfile, String aacfile);

}
