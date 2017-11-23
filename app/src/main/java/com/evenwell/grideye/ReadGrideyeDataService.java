package com.evenwell.grideye;


import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;

/**
 * Created by HowardJHLi on 2017/5/9.
 */

public class ReadGrideyeDataService extends Service {
    private final static String TAG = ReadGrideyeDataService.class.getSimpleName();
    private static final String filePath = "/proc/AllHWList/grideye_temp";

    private Bundle bundle;

    public ReadGrideyeDataService() {
        //super("ReadGrideyeDataService");
    }

    public void read(){

        bundle = new Bundle();

        new Thread(new Runnable() {
            public void run() {
                //Thread for manage calculation
                while(true) {
                    try {

                        Thread.sleep(100);

                        bundle.clear();

                        String rawDataStr = DataParser.hexToDecString(VirtualFile.getStringValueByFile(filePath));

                        bundle.putString("raw_data", rawDataStr);

                        //if ( !rawDataStr.equals(null) && !rawDataStr.equals("")) {

                            //Log.d(TAG, "[howardjhli] get virtual data successed!");
                            //Log.d(TAG, "[howardjhli]" + VirtualFile.getStringValueByFile(filePath));

                            Message msg = new Message();
                            msg.what = Utils.callBackMsg.DATA_CHANGE;

                            msg.setData(bundle);GrideyeActivity.sHandler.sendMessage(msg);
                        //}



                    } catch (Exception ex){
                        Log.d(TAG, "[howardjhli] get virtual data failed!!");
                        ex.printStackTrace();

                        bundle.clear();

                        bundle.putString("raw_data", "");

                        Message msg = new Message();
                        msg.what = Utils.callBackMsg.DATA_CHANGE;

                        msg.setData(bundle);GrideyeActivity.sHandler.sendMessage(msg);
                    }
                }
            }
        }).start();

    }


    private final IBinder mBinder = new ReadGrideyeDataService.LocalBinder();

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }
    public class LocalBinder extends Binder {
        ReadGrideyeDataService getService() {
            return ReadGrideyeDataService.this;
        }
    }


}
