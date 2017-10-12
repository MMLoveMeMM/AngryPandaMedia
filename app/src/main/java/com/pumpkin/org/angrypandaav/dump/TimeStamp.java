package com.pumpkin.org.angrypandaav.dump;

/**
 * Created by rd0348 on 2017/10/12 0012.
 */

public class TimeStamp {

    /*
    * getTime方法返回的就是10位的时间戳
    * */
    public static String getTimeStamp() {
        float time = System.currentTimeMillis() / 1000;//获取系统时间的10位的时间戳
        String str = String.valueOf(time);
        return str;
    }

}
