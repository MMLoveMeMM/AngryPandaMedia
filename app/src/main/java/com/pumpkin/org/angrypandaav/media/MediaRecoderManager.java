package com.pumpkin.org.angrypandaav.media;

/**
 * Created by rd0348 on 2017/10/21 0021.
 */

public class MediaRecoderManager {
    private static final MediaRecoderManager ourInstance = new MediaRecoderManager();

    public static MediaRecoderManager getInstance() {
        return ourInstance;
    }

    private MediaRecoderManager() {
    }
}
