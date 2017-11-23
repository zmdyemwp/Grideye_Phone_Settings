package com.evenwell.grideye;

import java.util.Date;

/**
 * Created by HowardJHLi on 2017/6/20.
 */

public class DetectstatusClass {
    public boolean isHuman;
    public int humanArea;
    public int detectNum;
    public int detectHumanNum;
    public int minX;
    public int minY;
    public int maxX;
    public int maxY;
    public int backTempState;
    public int centerX;
    public int centerY;
    public Date detectTime;
    public int detectStatus;
    public int detailStatus;
    public int detailStatusCode;
    public int humanNum;
    public int alarm;

    public DetectstatusClass(){}
    public String getStringFormat(){
        String result = "";
        return result;
    }
}
