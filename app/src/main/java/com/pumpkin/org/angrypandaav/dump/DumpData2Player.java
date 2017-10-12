package com.pumpkin.org.angrypandaav.dump;

import android.media.AudioFormat;

import com.pumpkin.org.angrypandaav.mediac.acc.AudioTrackPlayer;

/**
 * Created by rd0348 on 2017/9/30 0030.
 */

public class DumpData2Player implements IDumpData {

    //采样率
    private static final int KEY_SAMPLE_RATE = 48000;

    //用于播放解码后的pcm
    private AudioTrackPlayer mPlayer;

    public void startPlayer() {
        mPlayer = new AudioTrackPlayer(KEY_SAMPLE_RATE, AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT);
        mPlayer.init();
    }

    public void stopPlayer() {
        if (mPlayer != null) {
            mPlayer.release();
            mPlayer = null;
        }
    }

    @Override
    public void onFrame(byte[] data, int len) {

        if (mPlayer != null) {
            mPlayer.playAudioTrack(data, 0, len);
        }


    }


}
