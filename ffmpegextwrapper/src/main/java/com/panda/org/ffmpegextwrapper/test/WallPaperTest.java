package com.panda.org.ffmpegextwrapper.test;

import android.os.Environment;
import android.service.wallpaper.WallpaperService;
import android.view.SurfaceHolder;

import com.panda.org.ffmpegextwrapper.FFmpegWrapperProxy;

/**
 * Created by rd0348 on 2017/10/21 0021.
 * only for system
 */

public class WallPaperTest extends WallpaperService {

    private static String srcpath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/tiger.mp4";

    // 实现WallpaperService必须实现的抽象方法
    public Engine onCreateEngine() {
        return new VideoEngine();
    }

    class VideoEngine extends Engine {


        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);

            FFmpegWrapperProxy.getInstance().wallpaper(srcpath, getSurfaceHolder().getSurface());
            // 设置处理触摸事件
            setTouchEventsEnabled(true);

        }

    }
}
