/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.evenwell.grideye;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Switch;
import android.widget.TextView;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DetectStatusView extends Activity {
    private final static String TAG = DetectStatusView.class.getSimpleName();


    public static int DATA_CHANGE = 0;
    public static int HUMAN_DETECT = 1;
    public static int HUMAN_AREA = 2;
    public static int HUMAN_NUM = 3;
    public static int DETECT_NUM = 4;
    public static int BACK_TEMP_STATE = 5;
    public static int CENTER_X = 6;
    public static int CENTER_Y = 7;
    public static int ALERT_STATUS = 8;
    public static int DETAIL_STATUS = 10;
    public static int FILTER_DATA = 9;
    public static int DETAIL_STATUS_CODE = 11;
    public static int HUMAN_OUTPUT_IMG_DATA = 12;


    public static int no_Human = 0;
    public static int human_in = 1;
    public static int human_in_bed_board = 2;
    public static int human_in_bed = 3;
    public static int human_out_bed_board = 4;
    public static int human_out_bed = 5;
    public static int human_in_care = 6;
    public static int human_out_care = 7;
    public static int human_head = 8;
    public static int human_no_head = 9;
    public static int human_out = 10;


    // howardjhli

    /*  GET DETECTION RESULT DATA FROM GRIDEYE BOARD - defined packet manage for detection result items */
    DetectionPacketManager mDPacketManager = new DetectionPacketManager();
    private final static String DEBUG_MODE = "01";
    private final static String NORMAL_MODE = "00";

    public static String[] human_detct_result;
    public TextView isHuman;
    public TextView humanArea;
    public TextView detectNum;
    public TextView detectHumanNum;
    public TextView minX;
    public TextView minY;
    public TextView maxX;
    public TextView maxY;
    public TextView backTempState;
    public TextView centerX;
    public TextView centerY;
    public TextView detectTime;
    public TextView detectStatus;
    public TextView detailStatus;
    public TextView detailStatusCode;
    public TextView humanExist;
    public TextView humanNum;
    public TextView alarm;
    public TextView fnmvFrame;


    private final int EMPTY = -1;
    private final int NORMAL = 0;
    private final int PREALARM = 2;
    private final int ALARM = 1;




    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);



//        resetButton = (Button)findViewById(R.id.reset);
//        resetButton.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//
//                try {
//                    // write to grideye board by BLE
//                    //writeValueToCharactistic(RESET, 1);
//                    gdk.InitHumanDetect();
//
//                    writeFile.removeFile(fileName);
//
//                    Date date = new Date();
//                    SimpleDateFormat dFormat = new SimpleDateFormat("yyyyMMdd-HHmmss");
//                    fileName = dFormat.format(date);
//                    writeFile.createTextFile(fileName);
//
//                } catch(Exception e){}
//
//            }
//        });

    }


    @Override
    protected void onResume() {
        super.onResume();

    }

    @Override
    protected void onPause() {
        super.onPause();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        return super.onOptionsItemSelected(item);
    }



    public void setLayout(View view){
        //detectStatus = (TextView)view.findViewById(R.id.detect_status);
        detailStatus = (TextView)view.findViewById(R.id.detail_status);
        detailStatusCode = (TextView)view.findViewById(R.id.detail_status_code);
        isHuman = (TextView)view.findViewById(R.id.is_human_bed);
        humanArea = (TextView)view.findViewById(R.id.human_area);
        detectHumanNum = (TextView)view.findViewById(R.id.detect_human_num);
        detectNum = (TextView)view.findViewById(R.id.detect_num);
        backTempState = (TextView)view.findViewById(R.id.back_temp_state);
        centerX= (TextView)view.findViewById(R.id.center_x);
        centerY= (TextView)view.findViewById(R.id.center_y);
        detectTime = (TextView)view.findViewById(R.id.detect_time);
        minX = (TextView)view.findViewById(R.id.min_x);
        maxX = (TextView)view.findViewById(R.id.max_x);
        minY = (TextView)view.findViewById(R.id.min_y);
        maxY = (TextView)view.findViewById(R.id.max_y);
        humanExist = (TextView)view.findViewById(R.id.human_exist);
        humanNum = (TextView)view.findViewById(R.id.human_num);
        alarm = (TextView)view.findViewById(R.id.alarm);
        fnmvFrame = (TextView)view.findViewById(R.id.fnmv_frame);
    }




    private void detectHumanNumText(String value){
        if (Integer.parseInt(detectHumanNum.getText().toString()) < Integer.parseInt(value)) {
            SimpleDateFormat sdFormat = new SimpleDateFormat("yyyy/MM/dd hh:mm:ss");
            Date date = new Date();
            String strDate = sdFormat.format(date);

            detectTime.setText(strDate);
        }

        // if number of human > 0
        if (Integer.parseInt(value) > 0) {
            isHuman.setText(getResources().getString(R.string.yes));
            isHuman.setTextColor(Color.GREEN);
        } else {
            isHuman.setText(getResources().getString(R.string.no));
            isHuman.setTextColor(Color.RED);
        }

        detectHumanNum.setText(value);
    }

    private void alarmStatusText(String value){
        int state = Integer.parseInt(value);
        if (state == EMPTY) {
            Log.i("Crab", "Handler DETECT_STATUS : " + state);
            detectStatus.setText(getResources().getString(R.string.empty));
            detectStatus.setTextColor(Color.GRAY);
        } else if (state == NORMAL) {
            Log.i("Crab", "Handler DETECT_STATUS : " + state);
            detectStatus.setText(getResources().getString(R.string.normal));
            detectStatus.setTextColor(Color.GREEN);
        } else if (state == PREALARM) {
            detectStatus.setText(getResources().getString(R.string.prealarm));
            Log.i("Crab", "Handler DETECT_STATUS : " + state);
            detectStatus.setTextColor(Color.YELLOW);
        } else if (state == ALARM) {
            detectStatus.setText(getResources().getString(R.string.alarm));
            Log.i("Crab", "Handler DETECT_STATUS : " + state);
            detectStatus.setTextColor(Color.RED);
        }
    }

    private void detaiStatusCodeText(String value){

        //value = (Integer.toBinaryString(Integer.parseInt(value)));

        value = (Integer.toBinaryString(Integer.parseInt(value)));

        String codeStr = "000";
        String formatStr = "%3s";
        codeStr = String.format(formatStr, value).replace(' ', '0');;


        String tempStr = codeStr;

        detailStatusCode.setText(codeStr);

        String currentStr = detailStatusCode.getText().toString();

    }
    private void detaiStatusText(String value){

        int state = Integer.parseInt(value);
        Log.i("Crab","Handler DETAIL_STATUS : " + state);
        if (state == no_Human){
            detailStatus.setText(getResources().getString(R.string.no_human));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_in){
            detailStatus.setText(getResources().getString(R.string.human_in));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_in_bed_board){
            detailStatus.setText(getResources().getString(R.string.human_in_bed_board));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_in_bed){
            detailStatus.setText(getResources().getString(R.string.human_in_bed));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_head){
            detailStatus.setText(getResources().getString(R.string.human_head));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_no_head){
            detailStatus.setText(getResources().getString(R.string.human_no_head));
            detailStatus.setTextColor(Color.YELLOW);
        } else if (state == human_in_care){
            detailStatus.setText(getResources().getString(R.string.human_in_care));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_out_care){
            detailStatus.setText(getResources().getString(R.string.human_out_care));
            detailStatus.setTextColor(Color.GREEN);
        } else if (state == human_out_bed_board){
            detailStatus.setText(getResources().getString(R.string.human_out_bed_board));
            detailStatus.setTextColor(Color.YELLOW);
        } else if (state == human_out_bed){
            detailStatus.setText(getResources().getString(R.string.human_out_bed));
            detailStatus.setTextColor(Color.RED);
        } else if (state == human_out){
            detailStatus.setText(getResources().getString(R.string.human_out));
        }
    }
    private void humanExistText(String value){
        int hn = Integer.parseInt(value);

        if (hn > 0) {
            humanExist.setText(R.string.yes);
        }
        else {
            humanExist.setText(R.string.no);
        }
        humanNum.setText(value);
    }

    private void alarmText(String value){
        int state = Integer.parseInt(value);
        if (state == NORMAL) {
            Log.i("Howard", "Handler alarmText : " + state);
            alarm.setText(getResources().getString(R.string.normal));
            alarm.setTextColor(Color.WHITE);
            alarm.setBackgroundColor(Color.GREEN);
        } else if (state == ALARM) {
            Log.i("Howard", "Handler alarmText : " + state);
            alarm.setText(getResources().getString(R.string.alarm));
            //alarm.setTextColor(Color.RED);
            alarm.setBackgroundColor(Color.RED);
        } else if (state == PREALARM) {
            Log.i("Howard", "Handler alarmText : " + state);
            alarm.setText(getResources().getString(R.string.prealarm));
            alarm.setTextColor(Color.WHITE);
            alarm.setBackgroundColor(Color.YELLOW);

        } else {
            Log.i("Howard", "Handler alarmText : " + state);
            alarm.setText("NONE");
            alarm.setTextColor(Color.WHITE);
            alarm.setBackgroundColor(Color.GRAY);
        }
    }

    private void fnmvFrameText(String value, String valueObj){
        if (mDPacketManager.getAlarm() == null) return;
        int alarmState = Integer.parseInt(mDPacketManager.getAlarm());

        if (alarmState == PREALARM){
            fnmvFrame.setText(valueObj);
        } else if (alarmState == ALARM){
            fnmvFrame.setText(valueObj);
        } else {
            fnmvFrame.setText(value);
        }
    }

    private boolean change = false;
    private TimerTask task = null;
    private Timer timer = null;
    private void setFlashBackground(final int status) {

        if (task != null) task.cancel();
        if (timer != null) timer.cancel();

        task = new TimerTask() {
            public void run() {
                runOnUiThread(new Runnable() {
                    public void run() {
                        if (change) {
                            change = false;
                            if (status != NORMAL) {
                                //alarm.setTextColor(Color.TRANSPARENT);
                                alarm.setBackgroundColor(Color.TRANSPARENT);
                            }
                        } else {
                            change = true;
                            alarm.setTextColor(Color.WHITE);
                            if (status == NORMAL) {
                                alarm.setBackgroundColor(Color.GREEN);
                            } else if (status == ALARM) {
                                alarm.setBackgroundColor(Color.RED);
                            } else if (status == PREALARM) {
                                alarm.setBackgroundColor(Color.YELLOW);
                            }
                        }
                    }
                });
            }
        };

        timer = new Timer();
        if (status == PREALARM) timer.schedule(task, 0, 600);
        else if (status == ALARM)  timer.schedule(task, 0, 100);
    }



    private static String printBinaryform(int number) {
        int remainder;

        String formatStr = "%03d";
        String formatAns = String.format(formatStr, (Integer.toBinaryString(number)));

        String value = (Integer.toBinaryString(number));




        String tempAppend = "";
        while(true) {
            if (number <= 1) {
                System.out.print(number);
                tempAppend += String.valueOf(number >> 1);
                return tempAppend;   // KICK OUT OF THE WHILE
            }

            remainder = number % 2;
            tempAppend += String.valueOf(number >> 1);
        }
    }

}
