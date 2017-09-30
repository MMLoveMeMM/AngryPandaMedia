package com.pumpkin.org.angrypandaav.mediac.acc.p1;

/**
 * Created by rd0348 on 2017/9/30 0030.
 */

public class AudioTag {

    private byte[] data;

    private int currentBit = 0;

    public AudioTag() {
        data = new byte[2];
    }

    public int getCurrentBit() {
        return currentBit;
    }

    public byte[] getData() {
        return data;
    }

    public void addCurrentBit(int currentBit) {
        this.currentBit += currentBit;
    }

}
