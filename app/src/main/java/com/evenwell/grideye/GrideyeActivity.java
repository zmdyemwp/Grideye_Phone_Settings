package com.evenwell.grideye;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.os.Bundle;

import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatCallback;
import android.support.v7.app.AppCompatDelegate;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;
import android.view.LayoutInflater;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.evenwell.fusionnetlib.FusionNetMessage;
import com.evenwell.fusionnetlib.GrideyeConstants;
import com.evenwell.grideye.Peripheral.PermissionsCheckActivity;
import com.evenwell.grideye.Peripheral.service.FusionNetService;
import com.evenwell.grideye.Peripheral.util.FusionConst;
import com.evenwell.grideye.Peripheral.util.FusionUtils;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;


import 	android.view.MenuInflater;
/**
 * Created by HowardJHLi on 2017/5/16.
 */

public class GrideyeActivity extends Activity implements AppCompatCallback {

    private static final String TAG = GrideyeActivity.class.getSimpleName();

    public final static String DIALOG_TYPE = "dialog_type";
    public final static int TYPE_UI_IN_BACKGROUND = 1;
    public final static int TYPE_BT_STATE_INFO = 2;
    public final static int TYPE_UNDEFINE = -1;
    public final static int ERROR_WHEN_START_SERVICE = 901;
    public final static int ERROR_WHEN_STOP_SERVICE = 902;

    private SimpleDateFormat dFormat = new SimpleDateFormat(IOFileToInternalStorage.FILE_NAME_FORMAT);
    private SimpleDateFormat sdFormat = new SimpleDateFormat(IOFileToInternalStorage.TIME_FORMAT);

    private static final int ALL_STORED = 0;
    private static final int CHANGED_STORED = 1;

    private static final int CONNECT_STATUS = 1;
    private static final int DETECT_STATUS = 2;
    private static final int DETECT_IMG = 3;

    private final Object mLock = new Object();

    private Grideye_NDK mGrideye_NDK = new Grideye_NDK();
    private ReadGrideyeDataService mReadGrideyeDataService;

    private TextView mTexviewTab1;
    private TextView mTexviewTab2;
    private TextView mTexviewTab3;
    private RelativeLayout mView;

    private View currentView;
    private int mFocusTab;

    // declare write file class
    //private IOFileToInternalStorage IOFileToInternalStorage;
    private String mFileName = "";
    private DetectstatusClass mDSClass = new DetectstatusClass();;
    public static Handler sHandler;


    // set connect status view
    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";
    private ConnectStatusView mCSView;
    private View mConnectStatusView;
    public int mDataStateValue = 0;



    // set detect status view
    private DetectStatusView mDSView;
    private View mDetectStatusView;


    // set detect image view
    private DetectImageView mDIView;
    private View mDetectImgView;


    private int state = 0; // Alert state


    private AppCompatDelegate delegate;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.grideye_main);


        //let's create the delegate, passing the activity at both arguments (Activity, AppCompatCallback)
        delegate = AppCompatDelegate.create(this, this);

        //we need to call the onCreate() of the AppCompatDelegate
        delegate.onCreate(savedInstanceState);

        //we use the delegate to inflate the layout
        delegate.setContentView(R.layout.grideye_main);

        //Finally, let's add the Toolbar
        Toolbar toolbar= (Toolbar) findViewById(R.id.my_awesome_toolbar);
        delegate.setSupportActionBar(toolbar);

        // init tab ui view
        initTabView();

        // bind service
        Intent gattServiceIntent_dirver = new Intent(this, ReadGrideyeDataService.class);
        bindService(gattServiceIntent_dirver, mServiceConnection2, BIND_AUTO_CREATE);

        // bind service for ble peripheral
        setBlePeripheral();

        // clear uneccecery log data
        if (IOFileToInternalStorage.FileName.equals("")){
            IOFileToInternalStorage.removeAllDirectory(getApplicationContext(), null);
        }


        // start ip camera loop thread
        IPCameraService.startToGetIpCameraImage();

        // To receive call back msg from jni
        doHandler();

    }

    @Override
    protected void onResume() {
        super.onResume();
        startFusionNetService(true);
    }


    @Override
    protected void onDestroy() {
        Log.d(TAG, "ondestroy");
        super.onDestroy();
        startFusionNetService(false);
        stopReadGrideyeDataService();

        IOFileToInternalStorage.removeFile(IOFileToInternalStorage.FileName, getApplicationContext());
        IOFileToInternalStorage.FileName = "";

        mReadGrideyeDataService = null;
    }

    @Override
    protected void onPause() {
        super.onPause();
        startFusionNetService(false);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch(requestCode){
            case BT_ENABLER_RESULT_CODE:
                switch (resultCode){
                    case RESULT_CANCELED:
                        finish();
                        break;
                    case RESULT_OK:
                        if (android.os.Build.VERSION.SDK_INT >= 23) {
                            //Android M or above
                            permissionList = FusionUtils.needPermissionCheck(this);
                            if(permissionList != null && permissionList.size() > 0){
                                checkPermission();
                            }else{
                                setBlePeripheral();
                            }
                        }
                        break;
                }
                break;
            case PERMISSION_CHECK_RESULT_CODE:
                switch (resultCode){
                    case RESULT_CANCELED:
                        finish();
                        break;
                    case RESULT_OK:
                        setBlePeripheral();
                        break;
                }
                break;
        }
    }



    @Override
    public void onSupportActionModeStarted(android.support.v7.view.ActionMode actionMode) {

    }

    @Override
    public void onSupportActionModeFinished(android.support.v7.view.ActionMode actionMode) {

    }

    @Nullable
    @Override
    public android.support.v7.view.ActionMode onWindowStartingSupportActionMode(android.support.v7.view.ActionMode.Callback callback) {
        return null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        //return super.onCreateOptionsMenu(menu);
        MenuInflater inflater = delegate.getMenuInflater();
        inflater.inflate(R.menu.main, menu);
        return true;
    }

    
    void resetSettings() {
        byte[] bytes = {
                0x01,
                GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH1,
                GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH2,
                GrideyeConstants.settingPrefDefault.DEFAULT_LABELING_THRESH,
                GrideyeConstants.settingPrefDefault.DEFAULT_FNMV_THRESH,
                GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH1,
                GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH2,
                GrideyeConstants.settingPrefDefault.DEFAULT_HUMAN_FRAME_THRESH,
                GrideyeConstants.settingPrefDefault.DEFAULT_BED_LEFT,
                GrideyeConstants.settingPrefDefault.DEFAULT_BED_RIGHT,
                GrideyeConstants.settingPrefDefault.DEFAULT_LEFT_RANGE,
                GrideyeConstants.settingPrefDefault.DEFAULT_RIGHT_RANGE,
                GrideyeConstants.settingPrefDefault.DEFAULT_TOP_RANGE,
                GrideyeConstants.settingPrefDefault.DEFAULT_BOTTOM_RANGE,
        };
        WriteFile.writeSharedPref(this, bytes);

        byte[] bytes2 = {
                0x02,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_1,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_2,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_3,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_4,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_1,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_2,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_3,
                (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_4,
                GrideyeConstants.settingPrefDefault2.DEFAULT_RESET,
                GrideyeConstants.settingPrefDefault2.DEFAULT_RESET_SETTINGS,
                GrideyeConstants.settingPrefDefault2.DEFAULT_DELETE_LOGS,
        };
        WriteFile.writeSharedPref(this, bytes2);

        try {
            mGrideye_NDK.SetConfigValues(bytes);
        }catch (Exception ex){
            ex.getMessage();
            Log.d(TAG, ex.getMessage());
        }
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()){
            case R.id.menu_reset:
                initGrideyeDetection();
                break;
            case R.id.menu_set_reset:
                byte[] bytes = {
                        0x01,
                        GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH1,
                        GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH2,
                        GrideyeConstants.settingPrefDefault.DEFAULT_LABELING_THRESH,
                        GrideyeConstants.settingPrefDefault.DEFAULT_FNMV_THRESH,
                        GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH1,
                        GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH2,
                        GrideyeConstants.settingPrefDefault.DEFAULT_HUMAN_FRAME_THRESH,
                        GrideyeConstants.settingPrefDefault.DEFAULT_BED_LEFT,
                        GrideyeConstants.settingPrefDefault.DEFAULT_BED_RIGHT,
                        GrideyeConstants.settingPrefDefault.DEFAULT_LEFT_RANGE,
                        GrideyeConstants.settingPrefDefault.DEFAULT_RIGHT_RANGE,
                        GrideyeConstants.settingPrefDefault.DEFAULT_TOP_RANGE,
                        GrideyeConstants.settingPrefDefault.DEFAULT_BOTTOM_RANGE,
                };

                WriteFile.writeSharedPref(this, bytes);

                byte[] bytes2 = {
                        0x02,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_1,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_2,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_3,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_IP_CAM_ADDR_4,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_1,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_2,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_3,
                        (byte)GrideyeConstants.settingPrefDefault2.DEFAULT_FTP_ADDR_4,
                        GrideyeConstants.settingPrefDefault2.DEFAULT_RESET,
                        GrideyeConstants.settingPrefDefault2.DEFAULT_RESET_SETTINGS,
                        GrideyeConstants.settingPrefDefault2.DEFAULT_DELETE_LOGS,
                };
                WriteFile.writeSharedPref(this, bytes2);


                try {
                    mGrideye_NDK.SetConfigValues(bytes);
                }catch (Exception ex){
                    ex.getMessage();
                    Log.d(TAG, ex.getMessage());
                }

                break;
            case R.id.menu_log_del:
                IOFileToInternalStorage.removeAllFile(getApplicationContext(), null);
                IOFileToInternalStorage.FileName = "";
                break;
            default:
                break;
        }

        return super.onOptionsItemSelected(item);
    }

    /************************************************************************************/
    /******************************  private functions **********************************/
    /************************************************************************************/

    /* ------------------------------------------------------------------ */
    /* --------------------- Ble Peripheral service --------------------- */
    /* ------------------------------------------------------------------ */
    private FusionNetService mService;
    private boolean mIsServiceStart = false;
    private IntentFilter mFilter;
    private static final int PERMISSION_CHECK_RESULT_CODE
            = FusionConst.PermissionConst.RESULT_CODE;

    private List<String> permissionList;

    private static final int BT_ENABLER_RESULT_CODE = 1001;

    FusionNetMessage mMessage;

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mService = ((FusionNetService.LocalBinder) service).getService();
            if (mService != null) {
                mCSView.setStateText(ConnectStatusView.BLE_STATE, 1);
                mService.setUIHandler(h);
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mService = null;
            mCSView.setStateText(ConnectStatusView.BLE_STATE, 0);
        }
    };

    private void setBlePeripheral(){
        if (checkBleSupport()) {

            //check permissions flow
            if (android.os.Build.VERSION.SDK_INT >= 23) {
                //Android M or above
                permissionList = FusionUtils.needPermissionCheck(this);
                if (permissionList != null && permissionList.size() > 0) {
                    Log.e(TAG, "checkPermission");
                    checkPermission();
                    return;
                }
            }

            startFusionNetService(true);
        }
    }

    private void startFusionNetService(boolean isStart) {
        final Intent SERVICE_INTENT = new Intent(this, FusionNetService.class);
        if (isStart) {
            startService(SERVICE_INTENT);
            Intent intent = new Intent(this, FusionNetService.class);
            bindService(SERVICE_INTENT, mServiceConnection, BIND_AUTO_CREATE);
        } else {
            if (mService != null)
                mService.setUIHandler(null);
            try {
                unbindService(mServiceConnection);
                stopService(SERVICE_INTENT);
            } catch(IllegalArgumentException e){e.getMessage();}
        }
    }

    private void checkPermission(){
        
        Intent intent = new Intent(this, PermissionsCheckActivity.class);
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        String[] extra = new String[permissionList.size()];
        for(int i = 0; i < permissionList.size(); i++){
            extra[i] = permissionList.get(i);
        }
        intent.putExtra(FusionConst.PermissionConst.EXTRA_KEY_PERMISSIONS, extra);
        startActivityForResult(intent, PERMISSION_CHECK_RESULT_CODE);
        
    }

    private final BroadcastReceiver mExpiredReceiver = new BroadcastReceiver() {

        @Override
        public void onReceive(Context context, Intent intent) {
            //handleMsgExpired(true);
        }
    };

    private boolean checkBleSupport() {
        //check BT state
        BluetoothAdapter bluetooth = BluetoothAdapter.getDefaultAdapter();
        if (!bluetooth.isEnabled()) {
            //BT is not enabled
            Intent enabler = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enabler, BT_ENABLER_RESULT_CODE);
            return false;
        }
        //Check if device support le function
        if (!bluetooth.isMultipleAdvertisementSupported()) {
            Toast.makeText(this, "This Device not support le function!!!", Toast.LENGTH_LONG).show();
            finish();
            return false;
        }

        //check permissions flow
//        if (android.os.Build.VERSION.SDK_INT >= 23) {
//            //Android M or above
//            permissionList = FusionUtils.needPermissionCheck(this);
//            if (permissionList != null && permissionList.size() > 0) {
//                checkPermission();
//                return false;
//            }
//        }
        return true;
    }

    // handle charactistic write message value
    private Handler h = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case FusionConst.CommandType.TYPE_RECEIVE_MSG:
                    FusionNetMessage message = (FusionNetMessage) msg.obj;
                    Log.d("FusionNetService", "handle get Message: " + message.getMessage());
                    if (message != null) {
                        updateMsg(message);
                        //displayMessage(mMessage.getMessage(), mMessageReceivedTime);
                        if (FusionConst.DBG) {
//                            mFeedbackBtn.setVisibility(View.VISIBLE);
//                            mFeedbackBtn.setEnabled(true);
                        }
                    } else {
                        //mMessageTextView.setText(R.string.str_message_err);
                    }
                    break;
//                case FusionConst.CommandType.TYPE_RECEIVE_FEEDBACK_CONFIRM:
//                    mFeedbackBtn.setText(R.string.str_feedback_confirm);
//                    mFeedbackBtn.setEnabled(false);
//                    SharedPreferences.Editor PE = mPrefs.edit();
//                    PE.remove(Prefs.KEY_MSG_NOT_FEEDBACK).commit();
//                    Snackbar.make(mFeedbackBtn, "Feedback confirmed", Snackbar.LENGTH_SHORT)
//                                /*.setAction("Action", null)*/.show();
//                    break;
                default:
                    break;
            }
        }
    };
    private void updateMsg(FusionNetMessage message) {
        mMessage = message;
        //mMessageReceivedTime = System.currentTimeMillis();

        //byte[] bytes = mMessage.getMessage().getBytes();

        byte[] bytes = mMessage.getMessageByte();

        // Log.d("FusionNetService", "[ble setting] handle get byte Message: " + bytes.toString());
        for(int i = 0; i < bytes.length; i++) {
            Log.d("FusionNetService", String.format("updateMsg() [%d] 0x%02x", i, bytes[i]));
        }

        if(3 == bytes[0]) {             //  Reset
            Log.d("MIN", "COMMAND: RESET");
            initGrideyeDetection();
        } else if(4 == bytes[0]) {      //  Reset Settings
            Log.d("MIN", "COMMAND: RESET SETTINGS");
            resetSettings();
        } else if(5 == bytes[0]) {      //  Delete Logs
            Log.d("MIN", "COMMAND: DELETE LOGS");
            IOFileToInternalStorage.removeAllFile(getApplicationContext(), null);
            IOFileToInternalStorage.FileName = "";
        }

        WriteFile.writeSharedPref(this, bytes);
        try {
            mGrideye_NDK.SetConfigValues(bytes);
        }catch (Exception ex){
            ex.getMessage();
            Log.d(TAG, ex.getMessage());
        }

        Log.d("FusionNetService", "[ble setting] handle get byte Message: ");

        //if (mService != null && mMessage != null) {
            //mService.feedbackToMessage(mMessage);
        //}
    }

    /* -------------------- /
     *   switch Tab view
    / -------------------- */
    private void switchTabView(){

//        if (mView != null) {
//            mView.removeView(currentView);
//            mView = null;
//        }
        mView = (RelativeLayout) findViewById(R.id.view);
        mTexviewTab1.setBackgroundColor(Color.BLACK);
        mTexviewTab2.setBackgroundColor(Color.BLACK);
        mTexviewTab3.setBackgroundColor(Color.BLACK);


        if (mFocusTab == CONNECT_STATUS){
            mView.removeAllViews();
            //mView.removeAllViewsInLayout();
            mView.addView(mConnectStatusView,0, new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT));
            currentView = mConnectStatusView;
            mTexviewTab1.setBackgroundColor(Color.GRAY);

            FTPService.tryConnect(GrideyeActivity.this);
        }
        else if (mFocusTab == DETECT_STATUS){
            mView.removeAllViews();
            //mView.removeAllViewsInLayout();
            mView.addView(mDetectStatusView,0, new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT));
            currentView = mDetectStatusView;
            mTexviewTab2.setBackgroundColor(Color.GRAY);
        }
        else if (mFocusTab == DETECT_IMG){
            mView.removeAllViews();
            //mView.removeAllViewsInLayout();
            mView.addView(mDetectImgView,0, new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT));
            currentView = mDetectImgView;
            mTexviewTab3.setBackgroundColor(Color.GRAY);
        }

    }

    private void initTabView(){
        mTexviewTab1 = (TextView) findViewById(R.id.tabview1);
        mTexviewTab2 = (TextView) findViewById(R.id.tabview2);
        mTexviewTab3 = (TextView) findViewById(R.id.tabview3);

        mTexviewTab1.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (mFocusTab != CONNECT_STATUS) {
                    mFocusTab = CONNECT_STATUS;
                    switchTabView();
                    return true;
                }
                return false;
            }
        });

        mTexviewTab2.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (mFocusTab != DETECT_STATUS) {
                    mFocusTab = DETECT_STATUS;
                    switchTabView();
                    return true;
                }
                return false;
            }
        });

        mTexviewTab3.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (mFocusTab != DETECT_IMG) {
                    mFocusTab = DETECT_IMG;
                    switchTabView();
                    return true;
                }
                return false;
            }
        });

        setConnectStatusView();
        setDetectStatusView();
        setDetectImgView();

        mFocusTab = CONNECT_STATUS; // default tab is Connect Status view
        currentView = mConnectStatusView;  // default tab is Connect Status view

        switchTabView();
    }




    /* ------------------------------------------------------------------ */
    /* ----------------- Read grideye raw data service ------------------ */
    /* ------------------------------------------------------------------ */
    private void setConnectStatusView(){
        mCSView = new ConnectStatusView();

        LayoutInflater inflater = getLayoutInflater();
        mConnectStatusView = inflater.inflate(R.layout.connect_status_layout, null);

        mCSView.setLayout(mConnectStatusView);
    }

    private void setDetectStatusView(){
        mDSView = new DetectStatusView();

        LayoutInflater inflater = getLayoutInflater();
        mDetectStatusView = inflater.inflate(R.layout.detect_status_layout, null);
        mDSView.setLayout(mDetectStatusView);

    }

    private void setDetectImgView(){
        mDIView = new DetectImageView(this);

        LayoutInflater inflater = getLayoutInflater();
        mDetectImgView = inflater.inflate(R.layout.detect_img_layout, null);
        mDIView.setLayout(mDetectImgView);
    }




    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection2 = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            Log.i(TAG,"ServiceConnection onServiceConnected");
            mReadGrideyeDataService = ((ReadGrideyeDataService.LocalBinder) service).getService();

            ReadGrideyeData();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.i(TAG, "ServiceConnection onServiceDisconnected");
            mReadGrideyeDataService = null;
        }

    };
    private void stopReadGrideyeDataService() {
        Intent SERVICE_INTENT = new Intent(this, ReadGrideyeDataService.class);
        try {
            unbindService(mServiceConnection2);
            stopService(SERVICE_INTENT);
        } catch(IllegalArgumentException e){e.getMessage();}

    }

    private void ReadGrideyeData(){
        initGrideyeDetection();
        mReadGrideyeDataService.read();
    }


    private void initGrideyeDetection(){
        mGrideye_NDK.InitHumanDetect();
        mGrideye_NDK.setJNIEnv();

        mGrideye_NDK.SetConfigValues(WriteFile.readSharedPref(this));
    }


    private void cteateLogFolder(){
        // create new folder to write continuouse logs
        if (IOFileToInternalStorage.FileName.equals("") || IOFileToInternalStorage.FileName.equals(null)) {
            Date date = new Date();
            IOFileToInternalStorage.FileName = dFormat.format(date);
            IOFileToInternalStorage.createLogDirectory(getApplicationContext());
        }
    }

    // 20170620 -- add detect data
    private void storedLogFile(String dataString, String detectString, int imageStoredMode){
        Date date = new Date();
        SimpleDateFormat dFormat = new SimpleDateFormat(IOFileToInternalStorage.FILE_NAME_FORMAT);
        SimpleDateFormat sdFormat = new SimpleDateFormat(IOFileToInternalStorage.TIME_FORMAT);
        String subName = sdFormat.format(date);

        // write raw data log txt
        try {
             /* write raw data to local log file .txt */
//            if (IOFileToInternalStorage.FileName.equals("") || IOFileToInternalStorage.FileName.equals(null))
//                IOFileToInternalStorage.FileName = dFormat.format(date);
//
//            if (!IOFileToInternalStorage.findFile(IOFileToInternalStorage.FileName, getApplicationContext())){
//                IOFileToInternalStorage.createLogDirectory(getApplicationContext());
//            }

            if (IOFileToInternalStorage.FileName.equals("") || IOFileToInternalStorage.FileName.equals(null)) {
                IOFileToInternalStorage.FileName = dFormat.format(date);
                IOFileToInternalStorage.createLogDirectory(getApplicationContext());
            }

            // write new data in log file
            if (IOFileToInternalStorage.writeTxtFile(dataString, subName, getApplicationContext())) {
                // ConnectStatusView -- set log state
                mCSView.setStateText(ConnectStatusView.LOG_STATE, 1);
            } else {
                // ConnectStatusView -- set log state
                mCSView.setStateText(ConnectStatusView.LOG_STATE, 0);
            }

        } catch (Exception exl){
            exl.getStackTrace();
            // ConnectStatusView -- set log state
            mCSView.setStateText(ConnectStatusView.LOG_STATE, 0);
        }


//        // 20170620 - write detect status log txt
//        try{
//            if (IOFileToInternalStorage.FileName.equals("") || IOFileToInternalStorage.FileName.equals(null)) {
//                IOFileToInternalStorage.FileName = dFormat.format(date);
//                IOFileToInternalStorage.createLogDirectory(getApplicationContext());
//            }
//
//            // write new detect data in log file
//            if (IOFileToInternalStorage.writeTxtFile(detectString, subName, getApplicationContext())) {
//                // ConnectStatusView -- set log state
//                mCSView.setStateText(ConnectStatusView.LOG_STATE, 1);
//            } else {
//                // ConnectStatusView -- set log state
//                mCSView.setStateText(ConnectStatusView.LOG_STATE, 0);
//            }
//        } catch (Exception ex){
//            ex.getStackTrace();
//
//        }


        // write image
        try {
            if (imageStoredMode == ALL_STORED){
                IOFileToInternalStorage.writeImageFile(IPCameraService.sOutputImg, subName, getApplicationContext());
            }
            if (imageStoredMode == CHANGED_STORED) {
                // stored many image to tmp image list
                for (int i = DetectImageView.TEMP_IMAGE_FRAME_NUMBER - 1; i > 0; i--) {
                    mDIView.outputImgs[i] = mDIView.outputImgs[i - 1];
                }
                mDIView.outputImgs[0] = new StoredBitmap(IPCameraService.sOutputImg, subName);

                // if alert status had cahnged, write new image file!!!
                if (mDIView.isWriteImg)
                    IOFileToInternalStorage.writeImageFile(IPCameraService.sOutputImg, subName, getApplicationContext());
                if (mDIView.writeImgCount >= DetectImageView.NEW_IMAGE_FRAME_NUMBER) {
                    Log.e("IOFileToInternalStorage", "writing image is over than Thresh");
                    mDIView.isWriteImg = false;
                } else mDIView.writeImgCount++;
            }
        } catch (Exception exi){

        }


        try {
            // check the log file size is bigger then thresh
            if (IOFileToInternalStorage.checkFileSize(getApplicationContext())) {
                Log.e("IOFileToInternalStorage", "the log file size is bigger then thresh");

                // compress the current log folder to zip
                IOFileToInternalStorage.zipSubFolder(getApplicationContext());

                // delete current log folder
                IOFileToInternalStorage.removeFile(IOFileToInternalStorage.FileName, getApplicationContext());

                // transfer the zip log file to FTP Server
                IOFileToInternalStorage.uploadFliesToFTP(dataString, IPCameraService.sOutputImg, subName, getApplicationContext());

                IOFileToInternalStorage.FileName = "";
            }
        } catch (Exception exf){

        }
    }


    private void storedImgBuff(int imgStoredMode){
        if (imgStoredMode == CHANGED_STORED) {
            // if state change, stored image buff to internal storage
            Log.d("IOFileToInternalStorage", "state change, write image buff to internal");
            IOFileToInternalStorage.writePreImageFile(mDIView.outputImgs, getApplicationContext());
            mDIView.isWriteImg = true;
            mDIView.writeImgCount = 0;
        }
    }


    private void doHandler(){
        sHandler = new Handler(){
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                if(msg.what == Utils.callBackMsg.DATA_CHANGE) {
                    try {
                        // get the part of temperature data
                        String dataString = msg.getData().getString("raw_data");

                        // get new image file
                        mDIView.updateImageView();

                        // 20170620 -- get detect status data
                        String detectString = "";
                        detectString = mDSClass.getStringFormat();

                        if (dataString != null && !dataString.equals("")) {

                            mCSView.setStateText(ConnectStatusView.DATA_STATE, 1);

                            synchronized (mLock) {

                                // Manage stored log & image
                                storedLogFile(dataString, detectString, ALL_STORED);

                                try {
                                    // Start to grideye detect algorithm
                                    mGrideye_NDK.StartHumanDetection(DataParser.parseStringToShortArray(dataString));
                                    mCSView.setStateText(ConnectStatusView.DETECT_STATE, 1);
                                } catch (Exception exd){
                                    exd.getStackTrace();
                                    mCSView.setStateText(ConnectStatusView.DETECT_STATE, 0);
                                }
                            }

                            // Update temperature gridview
                            mDIView.updateGridViewData(dataString);

                        } else{
                            mCSView.setStateText(ConnectStatusView.DATA_STATE, 0);
                        }

                    } catch (Exception e){
                        e.getStackTrace();
                    }
                }

                if(msg.what == Utils.callBackMsg.HUMAN_DETECT){
                    Log.i("Howard","Handler HUMAN_DETECT : " + msg.getData().getString("human_detect"));


                    if (Integer.parseInt(mDSView.detectHumanNum.getText().toString()) < Integer.parseInt(msg.getData().getString("human_detect"))){
                        SimpleDateFormat sdFormat = new SimpleDateFormat("yyyy/MM/dd hh:mm:ss");
                        Date date = new Date();
                        String strDate = sdFormat.format(date);

                        mDSView.detectTime.setText(strDate);
                    }

                    // if number of human > 0
                    if (Integer.parseInt(msg.getData().getString("human_detect")) > 0) {
                        mDSView.isHuman.setText(getResources().getString(R.string.yes));
                        mDSView.isHuman.setTextColor(Color.GREEN);
                    } else {
                        mDSView.isHuman.setText(getResources().getString(R.string.no));
                        mDSView.isHuman.setTextColor(Color.RED);
                    }

                }
                if(msg.what == Utils.callBackMsg.HUMAN_AREA){
                    Log.i("Howard","Handler HUMAN_AREA : " +  msg.getData().getString("human_area"));
                    mDSView.humanArea.setText(msg.getData().getString("human_area"));
                }
                if(msg.what == Utils.callBackMsg.HUMAN_NUM){
                    Log.i("Howard","Handler HUMAN_NUM : " + msg.getData().getString("human_num"));
                    mDSView.humanNum.setText(msg.getData().getString("human_num"));
                }
                if(msg.what == Utils.callBackMsg.DETECT_NUM){
                    Log.i("Howard","Handler DETECT_NUM : " + msg.getData().getString("detect_num"));
                    mDSView.detectNum.setText(msg.getData().getString("detect_num"));
                }
                if(msg.what == Utils.callBackMsg.BACK_TEMP_STATE){
                    Log.i("Howard","Handler BACK_TEMP_STATE : " + msg.getData().getInt("back_temp_state"));
                    Log.i("Howard","Handler BACK_TEMP_STATE : " + String.valueOf(msg.getData().getInt("back_temp_state")));
                    int state = msg.getData().getInt("back_temp_state");
                    if (state == Utils.detect_status.BACK_TEMP_READY) {
                        Log.i("Howard","Handler BACK_TEMP_STATE : go to YES");
                        mDSView.backTempState.setText(getResources().getString(R.string.yes));
                    } else {
                        Log.i("Howard","Handler BACK_TEMP_STATE : go to NO");
                        mDSView.backTempState.setText(getResources().getString(R.string.no));
                    }
                }
                if(msg.what == Utils.callBackMsg.CENTER_X){
                    Log.i("Howard","Handler CENTER_X : " + msg.getData().getInt("center_x"));
                    String str = String.valueOf(msg.getData().getInt("center_x"));
                    if (str != null) {
                        mDSView.centerX.setText(str);
                    } else {
                        mDSView.centerX.setText("0");
                    }
                }
                if(msg.what == Utils.callBackMsg.CENTER_Y){
                    Log.i("Howard","Handler CENTER_Y : " + msg.getData().getInt("center_y"));
                    String str = String.valueOf(msg.getData().getInt("center_y"));
                    if (str != null) {
                        mDSView.centerY.setText(str);
                    } else {
                        mDSView.centerY.setText("0");
                    }
                }
                if(msg.what == Utils.callBackMsg.MAX_X){
                    Log.i("Howard","Handler MAX_X : " + msg.getData().getInt("max_x"));
                    String str = String.valueOf(msg.getData().getInt("max_x"));
                    if (str != null) {
                        mDSView.maxX.setText(str);
                    } else {
                        mDSView.maxX.setText("0");
                    }
                }
                if(msg.what == Utils.callBackMsg.MAX_Y){
                    Log.i("Howard","Handler MAX_Y : " + msg.getData().getInt("max_y"));
                    String str = String.valueOf(msg.getData().getInt("max_y"));
                    if (str != null) {
                        mDSView.maxY.setText(str);
                    } else {
                        mDSView.maxY.setText("0");
                    }
                }
                if(msg.what == Utils.callBackMsg.MIN_X){
                    Log.i("Howard","Handler MIN_X : " + msg.getData().getInt("min_x"));
                    String str = String.valueOf(msg.getData().getInt("min_x"));
                    if (str != null) {
                        mDSView.minX.setText(str);
                    } else {
                        mDSView.minX.setText("0");
                    }
                }
                if(msg.what == Utils.callBackMsg.MIN_Y){
                    Log.i("Howard","Handler MIN_Y : " + msg.getData().getInt("min_y"));
                    String str = String.valueOf(msg.getData().getInt("min_y"));
                    if (str != null) {
                        mDSView.minY.setText(str);
                    } else {
                        mDSView.minY.setText("0");
                    }
                }

                if(msg.what == Utils.callBackMsg.ALERT_STATUS){
                    Log.i("Howard","Handler ALERT_STATUS : " +  msg.getData().getInt("alert_status") + ", and state = " + state);

                    // if state change, stored image buff to internal storage
                    if (state != msg.getData().getInt("alert_status")){
                        storedImgBuff(ALL_STORED);
                    }

                    state = msg.getData().getInt("alert_status");

                    mDSView.alarm.setTextColor(Color.BLACK);
                    mDIView.alarm.setTextColor(Color.BLACK);

                    if (state == Utils.alert_status.STANDBY){
                        mDSView.alarm.setText(getResources().getString(R.string.standby));
                        mDSView.alarm.setBackgroundColor(Color.DKGRAY);
                        mDIView.alarm.setText(getResources().getString(R.string.standby));
                        mDIView.alarm.setBackgroundColor(Color.DKGRAY);
                    } else if (state == Utils.alert_status.MONITOR){
                        Log.i("Howard","Handler ALERT_STATUS : " +  msg.getData().getInt("alert_status") + ", alert to MONITOR");
                        mDSView.alarm.setText(getResources().getString(R.string.monitor));
                        mDSView.alarm.setBackgroundColor(Color.YELLOW);
                        mDIView.alarm.setText(getResources().getString(R.string.monitor));
                        mDIView.alarm.setBackgroundColor(Color.YELLOW);
                    } else if (state == Utils.alert_status.SAFE){
                        mDSView.alarm.setText(getResources().getString(R.string.safe));
                        mDSView.alarm.setBackgroundColor(Color.GREEN);
                        mDIView.alarm.setText(getResources().getString(R.string.safe));
                        mDIView.alarm.setBackgroundColor(Color.GREEN);
                    } else if (state == Utils.alert_status.ALERT){
                        mDSView.alarm.setText(getResources().getString(R.string.alert));
                        mDSView.alarm.setBackgroundColor(Color.RED);
                        mDIView.alarm.setText(getResources().getString(R.string.alert));
                        mDIView.alarm.setBackgroundColor(Color.RED);
                    }
                }

                if(msg.what == Utils.callBackMsg.HUMAN_OUTPUT_IMG_DATA){
                    short[] str = msg.getData().getShortArray("human_output");
                    Log.i("Howard","Handler HUMAN_OUTPUT_IMG_DATA : ");
                    for (int i = 0; i < str.length; i++){
                        //Log.i("Howard","HUMAN_OUTPUT_IMG_DATA [" + i + "]" + str[i]);
                    }
                }
            }
        };
    }

}
