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
import android.bluetooth.BluetoothGattCharacteristic;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ExpandableListView;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class ConnectStatusView extends Activity {
    private final static String TAG = ConnectStatusView.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    public static final String CHARACTISTIC_NOTIFY= "0783b03e-8535-b5a0-7140-a304d2495cb8";
    public static final String CHARACTISTIC_WRITE= "0783b03e-8535-b5a0-7140-a304d2495cba";

    private ReadGrideyeDataService mReadGrideyeDataService;


    private TextView mDataState;
    private TextView mDetectState;
    private TextView mLogState;
    private TextView mIpCamStae;
    private TextView mFtpState;
    private TextView mBleState;
    public int ipcamState = 0;

    private TextView mConnectionState;
    private TextView mDataField;
    private String mDeviceName;
    private String mDeviceAddress;
    private ExpandableListView mGattServicesList;

    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;
    private BluetoothGattCharacteristic mWriteCharacteristic;
    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

    private final static int RESET = 1;
    private final static int MODE = 2;
    private final static int BLEFT_KEY = 3;
    private final static int BRIGHT_KEY = 4;
    private final static int EDGE_FRAME = 5;
    private final static int FNMV_FRAME = 6;
    private final static int ALARM_FNMV_FRAME = 7;
    private final static int ALARM_HUMAN_THRESH = 8;

    private Grideye_NDK gdk = new Grideye_NDK();

    //Crab
    GridView temp_gridview;
    GridView humanQuadrant_gridview;
    TempAdapter mTempAdatper;
    quadrantAdapter mQuadrantAdapter;
    static Handler handler;
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

    private TextView tv_max, tv_min;
    private Button btn_apply;
    //Crab

    // howardjhli

    /*  GET DETECTION RESULT DATA FROM GRIDEYE BOARD - defined packet manage for detection result items */
    DetectionPacketManager mDPacketManager = new DetectionPacketManager();
    private final static String DEBUG_MODE = "01";
    private final static String NORMAL_MODE = "00";

    public static String[] human_detct_result;
    private TextView isHuman;
    private TextView humanArea;
    private TextView detectNum;
    private TextView detectHumanNum;
    private TextView minX;
    private TextView minY;
    private TextView maxX;
    private TextView maxY;
    private TextView backTempState;
    private TextView centerX;
    private TextView centerY;
    private TextView detectTime;
    private TextView detectStatus;
    private TextView detailStatus;
    private TextView detailStatusCode;
    private TextView humanExist;
    private TextView humanNum;
    private TextView alarm;
    private TextView fnmvFrame;

    private EditText diffTempThresh;
    private EditText labelingThresh;
    private EditText humanThresh;
    private EditText boundaryThresh;
    private EditText filterFrame;
    private EditText setValue;

    /* editText for config value */
    private EditText bedLeftBoard;
    private EditText bedRightBoard;
    private Switch modeSwitch;


    private Button humanDetectButton;
    private Button filterButton;
    private Button sendValueButton;
    private Button removeFileButton;
    private Button insertStepButton;
    private Button hideGrideViewButton;
    private Button configButton;
    private Button resetButton;


    private int bedLeftBoardValue = 0;
    private int bedRightBoardValue = 0;

    private final int EMPTY = -1;
    private final int NORMAL = 0;
    private final int PREALARM = 2;
    private final int ALARM = 1;

    private final int BACK_TEMP_READY = 1;
    private final int BACK_TEMP_NOT_READY = 0;

    private final int groupPosition =2;
    private final int childPosition =0;

    int move_num = 0;

    private  final Object mLock = new Object();

    private String fileName = "";

    private WriteFile writeFile;


    // for show ip cam image view
    private ImageView myImageView;
    private Bitmap outputImg;


    private View mView;

    private Button mResetButton;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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

    public static final int DATA_STATE = 0;
    public static final int DETECT_STATE = 1;
    public static final int LOG_STATE = 2;
    public static final int IPCAM_STATE = 3;
    public static final int FTP_STATE = 4;
    public static final int BLE_STATE = 5;

    public void setLayout(View view){
        mView = view;

        //final Intent intent = getIntent();
        //mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        //mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);
        //getActionBar().setTitle(mDeviceName);
        //getActionBar().setDisplayHomeAsUpEnabled(true);

        // Sets up UI references.
        //((TextView) view.findViewById(R.id.device_address)).setText(mDeviceAddress);

        mResetButton = (Button) view.findViewById(R.id.reset_button);


        mDataState = (TextView) view.findViewById(R.id.data_value);
        mDetectState = (TextView) view.findViewById(R.id.detect_state);
        mLogState = (TextView) view.findViewById(R.id.log_state);
        mIpCamStae = (TextView) view.findViewById(R.id.ipcam_value);
        mFtpState = (TextView) view.findViewById(R.id.ftp_value);
        mBleState = (TextView) view.findViewById(R.id.ble_state);

        updateStateThread();

    }

    public void setStateText(int state, int stateValue){
        if (state == DATA_STATE){
            if (stateValue == 1)
                mDataState.setText(R.string.connected);
            else
                mDataState.setText(R.string.disconnected);
        }
        else if (state == DETECT_STATE){
            if (stateValue == 1)
                mDetectState.setText(R.string.connected);
            else
                mDetectState.setText(R.string.disconnected);
        }
        else if (state == LOG_STATE){
            if (stateValue == 1)
                mLogState.setText(R.string.connected);
            else
                mLogState.setText(R.string.disconnected);
        }
        else if (state == IPCAM_STATE){
            if (stateValue == 1)
                mIpCamStae.setText(R.string.connected);
            else
                mIpCamStae.setText(R.string.disconnected);
        }
        else if (state == FTP_STATE){
            if (stateValue == 1)
                mFtpState.setText(R.string.connected);
            else
                mFtpState.setText(R.string.disconnected);
        }
        else if (state == BLE_STATE){
            if (stateValue == 1)
                mBleState.setText(R.string.connected);
            else
                mBleState.setText(R.string.disconnected);
        }

    }

    private void updateStateThread(){
        try {

            Thread usThread = new Thread(new Runnable() {
                public void run() {
                    while(true) {
                        runOnUiThread(new Runnable() {
                            public void run() {
                            synchronized (mLock) {
                                setStateText(IPCAM_STATE, IPCameraService.sIpcamState);
                                setStateText(FTP_STATE, FTPService.sFtpConnectState );
                            }
                            }
                        });
                        try {
                            Thread.sleep(5000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            });

            usThread.start();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void updateConnectStatus(){

    }
}
