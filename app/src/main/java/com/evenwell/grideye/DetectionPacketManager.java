package com.evenwell.grideye;

import android.util.Log;


import java.util.ArrayList;
import java.util.List;

/**
 * Created by HowardJHLi on 2016/8/18.
 */

public class DetectionPacketManager {

    static final int BEGIN = 3;

    static final int DETECT_HUMAN_NUM = 3;
    static final int ALARM_STATUS = 5;
    static final int STATUS_VALUE = 7;
    static final int CENTER_X = 9;
    static final int CENTER_Y = 11;
    static final int DETAIL_STATUS = 13;

    public String getHumanArea() {
        return humanArea;
    }

    public void setHumanArea(String humanArea) {
        this.humanArea = humanArea;
    }

    public String getDetectHumanNum() {
        return detectHumanNum;
    }

    public void setDetectHumanNum(String detectHumanNum) {
        this.detectHumanNum = detectHumanNum;
    }

    public String getAlarmStatus() {
        return alarmStatus;
    }

    public void setAlarmStatus(String alarmStatus) {
        this.alarmStatus = alarmStatus;
    }

    public String getStatusValue() {
        return statusValue;
    }

    public void setStatusValue(String statusValue) {
        this.statusValue = statusValue;
    }

    public String getCenterX() {
        return centerX;
    }

    public void setCenterX(String centerX) {
        this.centerX = centerX;
    }

    public String getCenterY() {
        return centerY;
    }

    public void setCenterY(String centerY) {
        this.centerY = centerY;
    }

    public String getDetailStatus() {
        return detailStatus;
    }

    public void setDetailStatus(String detailStatus) {
        this.detailStatus = detailStatus;
    }


    private String humanArea;
    private String detectHumanNum;
    private String alarmStatus;
    private String statusValue;
    private String centerX;
    private String centerY;
    private String detailStatus;
    private String minX;
    private String maxX;
    private String minY;
    private String maxY;

    private String humanNum;
    private String quadrant1;
    private String quadrant2;
    private String quadrant3;
    private String quadrant4;
    private String alarm;
    private String fnmvFrame;
    private String objFnmvFrame;

    private int checkKey;
    private int checkValue;



    public int getCheckKey() { return checkKey; }

    public void setCheckKey(int checkKey) { this.checkKey = checkKey; }

    public int getCheckValue() { return checkValue; }

    public void setCheckValue(int checkValue) { this.checkValue = checkValue; }


    public String getObjFnmvFrame() { return objFnmvFrame; }

    public void setObjFnmvFrame(String objFnmvFrame) { this.objFnmvFrame = objFnmvFrame; }

    public String getFnmvFrame() { return fnmvFrame; }

    public void setFnmvFrame(String fnmvFrame) { this.fnmvFrame = fnmvFrame; }

    public String getAlarm() { return alarm; }

    public void setAlarm(String alarm) { this.alarm = alarm; }

    public String getMinX() {
        return minX;
    }

    public void setMinX(String minX) {
        this.minX = minX;
    }

    public String getMaxX() {
        return maxX;
    }

    public void setMaxX(String maxX) {
        this.maxX = maxX;
    }

    public String getMinY() {
        return minY;
    }

    public void setMinY(String minY) {
        this.minY = minY;
    }

    public String getMaxY() {
        return maxY;
    }

    public void setMaxY(String maxY) { this.maxY = maxY; }


    public String getQuadrant1() {
        return quadrant1;
    }

    public void setQuadrant1(String quadrant1) {
        this.quadrant1 = quadrant1;
    }

    public String getHumanNum() {
        return humanNum;
    }

    public void setHumanNum(String humanNum) {
        this.humanNum = humanNum;
    }

    public String getQuadrant2() {
        return quadrant2;
    }

    public void setQuadrant2(String quadrant2) {
        this.quadrant2 = quadrant2;
    }

    public String getQuadrant3() {
        return quadrant3;
    }

    public void setQuadrant3(String quadrant3) {
        this.quadrant3 = quadrant3;
    }

    public String getQuadrant4() {
        return quadrant4;
    }

    public void setQuadrant4(String quadrant4) {
        this.quadrant4 = quadrant4;
    }

    public void getDetectionPacket(String sb_rawdata){
        if(sb_rawdata == null) return;
//        String packetStartSymbol = "2A2A2A";
//        int DEBUGMODE_SIZE = 2;
//        String packetEndSymbol = "0D0A";
//        // remove head symbol
//        String dataString = sb_rawdata.toString().substring(packetStartSymbol.length() + DEBUGMODE_SIZE);
//        //remove end symbol
//        dataString = dataString.substring(0, dataString.indexOf(packetEndSymbol));
        String dataString = sb_rawdata.toString();
        List<String> items = new ArrayList<>();
        for(int k=0; k<dataString.length(); k+=2){
            items.add(dataString.substring(k,k+2));
        }
//        if (items.size() < 11){
//            Log.d("Grideye", "data not correctly");
//            return;
//        }

        initial();

        transferASCIIToInt(items);
    }


    private void initial(){
        setHumanArea("0");
        setDetectHumanNum("0");
        setAlarmStatus("0");
        setStatusValue("0");
        setCenterX("0");
        setCenterY("0");
        setMinX("0");
        setMaxX("0");
        setMinY("0");
        setMaxY("0");
        setHumanNum("0");
        setQuadrant1("0");
        setQuadrant2("0");
        setQuadrant3("0");
        setQuadrant4("0");
        setFnmvFrame("0");
        setObjFnmvFrame("0");

        setCheckKey(0);
        setCheckValue(0);
    }
    public void transferASCIIToInt(List<String> items){

        /* human area */
        // items[0] = digits
        // items[1] = tens
        // items[2] = hundreds

//        String humanArea = Integer.parseInt(items.get(1), 16) == 32 ?
//                String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(0), 16))) :
//                String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(0), 16)))+
//                String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(1), 16)));


        setHumanNum(String.valueOf(Integer.parseInt(items.get(0), 16)));
        setQuadrant1(String.valueOf(Integer.parseInt(items.get(1), 16)));
        setQuadrant2(String.valueOf(Integer.parseInt(items.get(2), 16)));
        setQuadrant3(String.valueOf(Integer.parseInt(items.get(3), 16)));
        setQuadrant4(String.valueOf(Integer.parseInt(items.get(4), 16)));

        for (int i=25; i<55; i+=6) {
            if (Integer.parseInt(items.get(i), 16) != 0) {
                setCenterX(String.valueOf(Integer.parseInt(items.get(i), 16)) + "." + String.valueOf(Integer.parseInt(items.get(i + 1), 16)) + String.valueOf(Integer.parseInt(items.get(i + 2), 16)));
                setCenterY(String.valueOf(Integer.parseInt(items.get(i + 3), 16)) + "." + String.valueOf(Integer.parseInt(items.get(i + 4), 16)) + String.valueOf(Integer.parseInt(items.get(i + 5), 16)));
            }
        }
        for (int i=0; i<5; i++){
            if ( (Integer.parseInt(items.get(10+i), 16)) > 0 )
                setHumanArea(String.valueOf(Integer.parseInt(items.get(10+i), 16)));
        }
        for (int i=0; i<5; i++){
            if ( (Integer.parseInt(items.get(15+i), 16)) > 0 )
                setFnmvFrame(String.valueOf(Integer.parseInt(items.get(15+i), 16)));
        }
        for (int i=0; i<5; i++){
            if ( (Integer.parseInt(items.get(20+i), 16)) > 0 )
                setObjFnmvFrame(String.valueOf(Integer.parseInt(items.get(20+i), 16)));
        }


        setAlarm(String.valueOf(Integer.parseInt(items.get(55), 16)));


        setCheckKey(Integer.parseInt(items.get(56), 16));

        int val = Integer.parseInt(items.get(57),16)*256 + Integer.parseInt(items.get(58),16);

        setCheckValue(val);

//        String humanArea =  (Integer.parseInt(items.get(0), 16) == Ascii.SPACE ? " " : String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(0), 16)))) +
//                            (Integer.parseInt(items.get(1), 16) == Ascii.SPACE ? " " : String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(1), 16)))) +
//                            (Integer.parseInt(items.get(2), 16) == Ascii.SPACE ? " " : String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(2), 16))));
//
//        setHumanArea(humanArea);
//
//        setDetectHumanNum(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(3), 16))));
//        setAlarmStatus(String.valueOf(Character.getNumericValue｀｀(Integer.parseInt(items.get(5), 16))));
//        setStatusValue(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(7), 16))));
        //setCenterX(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(9), 16))));
        //setCenterY(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(11), 16))));
//        setDetailStatus(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(13), 16))));
//        setMinX(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(15), 16))));
//        setMaxX(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(17), 16))));
//        setMinY(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(19), 16))));
//        setMaxY(String.valueOf(Character.getNumericValue(Integer.parseInt(items.get(21), 16))));

        Log.d("Howard", "DetectionPacketManager checkKeyitems = " + items.get(56) + ", checkValueitems = " + items.get(57) + items.get(58));
        Log.d("Howard", "DetectionPacketManager checkKey = " + checkKey + ", checkValue = " + checkValue);
    }

}
