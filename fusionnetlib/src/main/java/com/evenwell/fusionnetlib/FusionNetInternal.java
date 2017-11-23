package com.evenwell.fusionnetlib;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothGattServer;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.AdvertiseCallback;
import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.bluetooth.le.BluetoothLeAdvertiser;
import android.content.Context;
import android.util.Log;

import com.evenwell.fusionnetlib.spec.FusionNetCallback;

import java.util.LinkedList;
import java.util.Queue;


/**
 * Real fusion net service implementation.
 * Created by KuroCHChung on 2016/1/18.
 */
/* package */ class FusionNetInternal {

    private final static String TAG = FusionNetInternal.class.getSimpleName();

    private BluetoothGattService mBluetoothGattService;
    private BluetoothManager mBluetoothManager;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothLeAdvertiser mAdvertiser;

    private Queue<FusionNetMessage> feedbackQueue = new LinkedList<>();

    private FusionNetMessage currentFeedbackMessage;

    private FusionNetAdvertiser fusionNetAdvertiser;

    private boolean helping = false;
    private boolean serviceStarted = false;
    private boolean advertiseSuccess = false;

    private FusionNet mFusionNet;

    private FusionNetCallback callback;

    private final AdvertiseCallback mAdvCallback = new AdvertiseCallback() {
        @Override
        public void onStartFailure(int errorCode) {
            super.onStartFailure(errorCode);
            Log.e(TAG, "Not broadcasting: " + errorCode);
            advertiseSuccess = false;
            callback.onServiceStart();
        }

        @Override
        public void onStartSuccess(AdvertiseSettings settingsInEffect) {
            super.onStartSuccess(settingsInEffect);
            Log.v(TAG, "Broadcasting");
            advertiseSuccess = true;
            callback.onServiceStart();
        }
    };
    private BluetoothGattServer mGattServer;
    private final FusionNetGattServerCallback mGattServerCallback = new FusionNetGattServerCallback(this);

    private Context mContext;

    public FusionNetInternal(Context context) {
        mContext = context;
        mBluetoothManager = (BluetoothManager) mContext.getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = mBluetoothManager.getAdapter();

        // Prepare BLE Gatt Server Service (Service & Characteristic)
        mBluetoothGattService = new FusionNetGattServiceV2();

        // Advertisement
        fusionNetAdvertiser = new FusionNetAdvertiser_Grideye(mContext);
    }

    /* package */ void clearAdvertise() {
        Log.d(TAG, "clearAdvertise");
        advertising(fusionNetAdvertiser.getDefaultManufactureData());
    }

    byte[] getResponseData_1() {
        return ((FusionNetAdvertiser_Grideye)fusionNetAdvertiser).help_1();
    }

    byte[] getResponseData_2() {
        return ((FusionNetAdvertiser_Grideye)fusionNetAdvertiser).help_2();
    }


    void onHelpConfirm() {
        Log.d(TAG, "onHelpConfirm");
        if(currentFeedbackMessage != null) {
            advertising(fusionNetAdvertiser.feedback(currentFeedbackMessage));
        } else {
            clearAdvertise();
            helping = false; // change state back
        }
    }

    void help() {
        // advertise help data
        if(isHelping()) {
            Log.d(TAG, "already helping!");
        } else {
            setHelping(true);
            advertising(fusionNetAdvertiser.help());
        }
    }

    void processNextFeedbackMessage() {
        Log.d(TAG, "nextFeedbackMessage");

        if(helping) {
            Log.d(TAG, "helping, waiting for help confirm...");
            return;
        }

        if(feedbackQueue.size() > 0) {
            // poll message from queue
            currentFeedbackMessage = feedbackQueue.poll();
            // advertise feed back data
            advertising(fusionNetAdvertiser.feedback(currentFeedbackMessage));
        } else {
            currentFeedbackMessage = null;
            clearAdvertise();
        }
    }

    void advertising(byte[] data) {

        mAdvertiser = mBluetoothAdapter.getBluetoothLeAdvertiser();

        mAdvertiser.stopAdvertising(mAdvCallback);
        //Log.d(TAG, "IMEI or Name: " + fusionNetAdvertiser.getDeviceName(mContext));

        // Here we reset adapter name to FAA_IMEI every time, prevent user change by their self.
//        Log.d(TAG, "Origin Adapter Name: " + mBluetoothAdapter.getName());
//        mBluetoothAdapter.setName(fusionNetAdvertiser.getDeviceName(mContext));
        //mBluetoothAdapter.setName("Grideye Server");
//        Log.d(TAG, "New Adapter Name: " + mBluetoothAdapter.getName());

        AdvertiseSettings mAdvSettings = fusionNetAdvertiser.getAdvertiseSettings();
        AdvertiseData mAdvData = fusionNetAdvertiser.getAdvertiseData(data);
        mAdvertiser.startAdvertising(mAdvSettings, mAdvData, mAdvCallback);
    }

    boolean startService() {
        try {
            if (mContext != null) {
                mGattServer = mBluetoothManager.openGattServer(mContext, mGattServerCallback);
                mGattServer.addService(mBluetoothGattService);
                mGattServerCallback.setGattServer(mGattServer);
                advertising(fusionNetAdvertiser.getDefaultManufactureData());
                setServiceStarted(true);
                return true;
            } else {
                Log.d(TAG, "No Android Context Bind to FusionNet");
                return false;
            }
        }catch(NullPointerException e){
            e.printStackTrace();
            //CrabCSChen@FIHTDC add for SMFT-1926, 2016/05/23 {
            callback.onErrorOccur(FusionNetConstants.ERROR_WHEN_START_SERVICE);
            //CrabCSChen@FIHTDC add for SMFT-1926, 2016/05/23 }
            return false;
        }
    }

    boolean stopService() {
        try {
            if (mGattServer != null) {
                mGattServer.close();
            }
            if (mBluetoothAdapter.isEnabled() && mAdvertiser != null) {
                // If stopAdvertising() gets called before close() a null
                // pointer exception is raised.
                mAdvertiser.stopAdvertising(mAdvCallback);
            }
            serviceStarted = false;
            helping = false;

        } catch(Exception e) {
            //CrabCSChen@FIHTDC add for SMFT-1925, 2016/05/23 {
            callback.onErrorOccur(FusionNetConstants.ERROR_WHEN_STOP_SERVICE);
            //CrabCSChen@FIHTDC add for SMFT-1925, 2016/05/23 }
           return false;
        }
        return true;
    }

    void registerFusionNetCallback(FusionNetCallback callback) {
        this.callback = callback;
        mGattServerCallback.setFusionNetCallbackSpec(callback);
    }

    void unregisterFusionNetCallback() {
        this.callback = null;
        mGattServerCallback.setFusionNetCallbackSpec(null);
    }

    public boolean isHelping() {
        return helping;
    }

    public void setHelping(boolean helping) {
        this.helping = helping;
    }

    public boolean isServiceStarted() {
        return serviceStarted;
    }

    public boolean isAdvertiseSuccessed() {
        return advertiseSuccess;
    }

    public void setServiceStarted(boolean serviceStarted) {
        this.serviceStarted = serviceStarted;
    }

    public void addFeedbackTask(FusionNetMessage message) {
        feedbackQueue.add(message);
    }
}
