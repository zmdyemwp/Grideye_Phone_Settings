package com.evenwell.grideye;

import android.os.Bundle;
import android.os.Message;
import android.util.Log;

/**
 * Created by HowardJHLi on 2017/5/3.
 */

public class Grideye_NDK {
    static {
        System.loadLibrary("grideye_jni");
    }

    public Grideye_NDK(){}

    private native void initializeHumanDetect();
    private native void startHumanDetection(short[] data);
    private native void setDetectionConfig(byte[] data);
    public  native void setJNIEnv();
    private final Object mLock = new Object();

    public void StartHumanDetection(short[] data){
        synchronized(mLock) {
            startHumanDetection(data);
        }
    }

    public void InitHumanDetect(){
        synchronized(mLock) {
            Log.i("Grideye_NDK","initializeHumanDetect");
            initializeHumanDetect();
        }
    }

    public void SetConfigValues(byte[] data){
        synchronized(mLock) {
            setDetectionConfig(data);
        }
    }

/* ---------------------------------------------------------------------------
 *                   Call back from JNI
 * ---------------------------------------------------------------------------*/

    public void callbackFromLib_isHuman(int code){
        Log.i("Kash","callbackFromLib_isHuman code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.HUMAN_DETECT;
        Bundle b = new Bundle();
        b.putString("human_detect",Integer.toString(code));
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_humanArea(int code){
        Log.i("Kash","callbackFromLib_humanArea code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.HUMAN_AREA;
        Bundle b = new Bundle();
        b.putString("human_area",Integer.toString(code));
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_humanNum(int code){
        Log.i("Kash","callbackFromLib_humanNum code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.HUMAN_NUM;
        Bundle b = new Bundle(); b.putString("human_num",Integer.toString(code));
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_detectNum(int code){
        Log.i("Kash","callbackFromLib_detectNum code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.DETECT_NUM;
        Bundle b = new Bundle(); b.putString("detect_num",Integer.toString(code));
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_backTempState(int code){
        Log.i("Kash","callbackFromLib_backTempState code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.BACK_TEMP_STATE;
        Bundle b = new Bundle(); b.putInt("back_temp_state", code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_centerX(int code){
        Log.i("Kash","callbackFromLib_centerX code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.CENTER_X;
        Bundle b = new Bundle(); b.putInt("center_x",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_centerY(int code){
        Log.i("Kash","callbackFromLib_centerY code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.CENTER_Y;
        Bundle b = new Bundle(); b.putInt("center_y",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_maxX(int code){
        Log.i("Kash","callbackFromLib_maxX code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.MAX_X;
        Bundle b = new Bundle(); b.putInt("max_x",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }
    public void callbackFromLib_maxY(int code){
        Log.i("Kash","callbackFromLib_maxY code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.MAX_Y;
        Bundle b = new Bundle(); b.putInt("max_y",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }
    public void callbackFromLib_minX(int code){
        Log.i("Kash","callbackFromLib_minX code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.MIN_X;
        Bundle b = new Bundle(); b.putInt("min_x",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }
    public void callbackFromLib_minY(int code){
        Log.i("Kash","callbackFromLib_minY code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.MIN_Y;
        Bundle b = new Bundle(); b.putInt("min_y",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_alertStatus(int code){
        Log.i("Kash","callbackFromLib_alertStatus code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.ALERT_STATUS;
        Bundle b = new Bundle(); b.putInt("alert_status",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_detailStatus(int code){
        Log.i("Kash","callbackFromLib_detailStatus code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.DETAIL_STATUS;
        Bundle b = new Bundle(); b.putInt("detail_status",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_humanOutputImg(short[] code){
        Log.i("Howard","callbackFromLib_humanOutputImg code: " + code);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.HUMAN_OUTPUT_IMG_DATA;
        Bundle b = new Bundle(); b.putShortArray("human_output",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }

    public void callbackFromLib_detailStatusCode(int[] code){
        Log.i("Kash","callbackFromLib_detailStatusCode code: " + code[2]);

        Message msg = new Message();
        msg.what = Utils.callBackMsg.DETAIL_STATUS_CODE;

        Bundle b = new Bundle(); b.putIntArray("detail_status_code",code);
        msg.setData(b);
        GrideyeActivity.sHandler.sendMessage(msg);
    }
}
