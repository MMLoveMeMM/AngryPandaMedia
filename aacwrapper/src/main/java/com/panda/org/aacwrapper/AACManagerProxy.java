package com.panda.org.aacwrapper;

/**
 * Created by rd0348 on 2017/11/7 0007.
 */

public class AACManagerProxy {
    private static final AACManagerProxy ourInstance = new AACManagerProxy();
    private aacwrapper wrapper;

    public static AACManagerProxy getInstance() {
        return ourInstance;
    }

    private AACManagerProxy() {
        wrapper = new aacwrapper();
    }

    public int InitRecord(String outputPath) {
        return wrapper.initRecord(outputPath);
    }

    public int WriteAudioData(byte[] pcmData) {
        return wrapper.writeAudioData(pcmData);
    }

    public int CloseRecord() {
        return wrapper.closeRecord();
    }

    public int Pcm2Aac(String pcmfile, String aacfile) {
        return wrapper.pcm2Aac(pcmfile, aacfile);
    }

    public int Pcm2AacDetail(String pcmfile, String aacfile) {
        return wrapper.pcm2AacDetail(pcmfile, aacfile);
    }

}
