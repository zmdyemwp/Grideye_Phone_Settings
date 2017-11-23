package com.evenwell.fusionnetlib;

import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.content.Context;
import android.content.SharedPreferences;
import android.telephony.TelephonyManager;
import android.util.Log;

/**
 * Created by KuroCHChung on 2016/1/23.
 */
/* package */ class FusionNetAdvertiser_Grideye implements FusionNetAdvertiser {

    private final static String TAG = FusionNetAdvertiser_Grideye.class.getSimpleName();

    private byte formatVersion = 0x02;
    private byte nameLength = 0x08;
    private byte nameIdentify = 0x09;
    private byte[] prefixName = "FAA_".getBytes();
    private byte[] imei;

    // function index
    private byte feedback = 0x01;
    private byte help = 0x02;
    private byte timeSync = 0x03;
    private byte stationSync = 0x04;
    private byte batteryReport = 0x05;


    private Context mContext;

    public FusionNetAdvertiser_Grideye(Context context) {
        //imei = ByteUtil.longToByteArray(Long.parseLong(getDeviceName(context)));
        this.mContext = context;

    }



    public byte[] help_1() {
        return responseData_1();
    }

    public byte[] help_2() {
        return responseData_2();
    }

    @Override
    public byte[] help() {

//        SharedPreferences sharedPref = mContext.getSharedPreferences(
//                FusionNetConstants.FUSION_PREFERENCE, Context.MODE_PRIVATE);
//
//        int helpCount = sharedPref.getInt(FusionNetConstants.HELP_COUNTER_KEY, -1);
//        SharedPreferences.Editor editor = sharedPref.edit();
//        editor.putInt(FusionNetConstants.HELP_COUNTER_KEY, ++helpCount);
//        editor.commit(); // immediately
//
//        Log.d(TAG, "Help with count: " + helpCount);
//
//        return new byte[]{
//                0x00,
//                0x00, // command id
//                0x00,
//                0x00, // message id
//                0x00,
//                help, // function index
//                prefixName[0], // F
//                prefixName[1], // A
//                prefixName[2], // A
//                prefixName[3], // _
//                (byte) helpCount, // parameter
//                formatVersion, // format version
//                nameLength, // name length
//                nameIdentify,
//                imei[1],
//                imei[2],
//                imei[3],
//                imei[4],
//                imei[5],
//                imei[6],
//                imei[7]
//        };

        return responseData_1();
    }

    @Override
    public byte[] feedback(FusionNetMessage message) {
        return new byte[] {
                (byte) ((message.getCommandId() >> 8) & 0xFF),
                (byte) (message.getCommandId() & 0xFF), // command id
                (byte) ((message.getMessageId() >> 8) & 0xFF),
                (byte) (message.getMessageId() & 0xFF),  // message id
                0x01, //has feedback
                feedback, // function index
                prefixName[0], // F
                prefixName[1], // A
                prefixName[2], // A
                prefixName[3], // _
                0X00, // parameter
                formatVersion, // format version
                nameLength, // name length
                nameIdentify,
                imei[1],
                imei[2],
                imei[3],
                imei[4],
                imei[5],
                imei[6],
                imei[7]
        };
    }


    public byte[] responseData_2() {
        byte[] result = new byte[GrideyeConstants.SETTING_BYTE_SIZE_2];
        SharedPreferences sharedPref = mContext.getSharedPreferences(
                GrideyeConstants.GRIDEYE_PREFERENCE, Context.MODE_PRIVATE);
        result[0] = 0x02;
        // IP Camera default: 10.57.45.3
        result[1] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_1, "10"));
        result[2] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_2, "57"));
        result[3] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_3, "45"));
        result[4] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_4, "3"));
        // FTP default: 10.57.54.254
        result[5] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_1, "10"));
        result[6] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_2, "57"));
        result[7] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_3, "54"));
        result[8] = (byte)Integer.parseInt(sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_4, "0"));
        //  Reset/ Reset Settings/ Delete Logs
        result[9] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.RESET, "0"));
        result[10] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.RESET_SETTINGS, "0"));
        result[11] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.DELETE_LOGS, "0"));
        return result;
    }

    public byte[] responseData_1() {

        byte[] settingDatas = new byte[GrideyeConstants.SETTING_BYTE_SIZE];

        SharedPreferences sharedPref = mContext.getSharedPreferences(
                GrideyeConstants.GRIDEYE_PREFERENCE, Context.MODE_PRIVATE);

        settingDatas[0] = 0x01;

        settingDatas[GrideyeConstants.settingsPrefIndex.DIFF_TEMP_THRESH1] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH1,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH1)));
        Log.d(TAG, "[shared pref] DIFF_TEMP_THRESH1 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH1, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.DIFF_TEMP_THRESH2] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH2, 
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH2)));
        Log.d(TAG, "[shared pref] DIFF_TEMP_THRESH2 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH2, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.LABELING_THRESH] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.LABELING_THRESH,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_LABELING_THRESH)));
        Log.d(TAG, "[shared pref] LABELING_THRESH : " + sharedPref.getString(GrideyeConstants.settingsPrefName.LABELING_THRESH, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.FNMV_THRESH] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.FNMV_THRESH,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_FNMV_THRESH)));
        Log.d(TAG, "[shared pref] FNMV_THRESH : " + sharedPref.getString(GrideyeConstants.settingsPrefName.FNMV_THRESH, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.EDGE_THRESH1] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH1,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH1)));
        Log.d(TAG, "[shared pref] EDGE_THRESH1 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH1, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.EDGE_THRESH2] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH2,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH2)));
        Log.d(TAG, "[shared pref] EDGE_THRESH2 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH2, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.HUMAN_FRAME_THRESH] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.HUMAN_FRAME_THRESH,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_HUMAN_FRAME_THRESH)));
        Log.d(TAG, "[shared pref] HUMAN_FRAME_THRESH : " + sharedPref.getString(GrideyeConstants.settingsPrefName.HUMAN_FRAME_THRESH, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.BED_LEFT] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.BED_LEFT,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_BED_LEFT)));
        Log.d(TAG, "[shared pref] BED_LEFT : " + sharedPref.getString(GrideyeConstants.settingsPrefName.BED_LEFT, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.BED_RIGHT] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.BED_RIGHT,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_BED_RIGHT)));
        Log.d(TAG, "[shared pref] BED_RIGHT : " + sharedPref.getString(GrideyeConstants.settingsPrefName.BED_RIGHT, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.LEFT_RANGE] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.LEFT_RANGE,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_LEFT_RANGE)));
        Log.d(TAG, "[shared pref] LEFT_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.LEFT_RANGE, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.RIGHT_RANGE] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.RIGHT_RANGE,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_RIGHT_RANGE)));
        Log.d(TAG, "[shared pref] RIGHT_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.RIGHT_RANGE, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.TOP_RANGE] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.TOP_RANGE,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_TOP_RANGE)));
        Log.d(TAG, "[shared pref] TOP_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.TOP_RANGE, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.BOTTOM_RANGE] = Byte.parseByte(
        sharedPref.getString(GrideyeConstants.settingsPrefName.BOTTOM_RANGE,
        String.format("%d", GrideyeConstants.settingPrefDefault.DEFAULT_BOTTOM_RANGE)));
        Log.d(TAG, "[shared pref] BOTTOM_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.BOTTOM_RANGE, "0"));

        Log.d(TAG, "[ble setting] response data:");
        for (int i=0;i<GrideyeConstants.SETTING_BYTE_SIZE;i++){
            Log.d(TAG, "[ble setting] response data[" + i + "]:" + "= " + settingDatas[i] );
        }


        return settingDatas;
    }


    @Override
    public byte[] outOfRange() {
        return new byte[0];
    }

    @Override
    public byte[] getDefaultManufactureData() {
//        return new byte[] {
//                0x00,
//                0x00, // command id
//                0x00,
//                0x00,  // message id
//                0x00, // has feedback
//                0x00, // has help
//                prefixName[0], // F
//                prefixName[1], // A
//                prefixName[2], // A
//                prefixName[3], // _
//                0X00, // parameter
//                formatVersion, // format version
//                nameLength, // name length
//                nameIdentify,
//                imei[1],
//                imei[2],
//                imei[3],
//                imei[4],
//                imei[5],
//                imei[6],
//                imei[7]
//        };

        return new byte[]{};
    }

    @Override
    public AdvertiseSettings getAdvertiseSettings() {
        return new AdvertiseSettings.Builder()
                .setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_LOW_LATENCY)
                .setTxPowerLevel(AdvertiseSettings.ADVERTISE_TX_POWER_HIGH)
                .setConnectable(true)
                .build();
    }

    @Override
    public AdvertiseData getAdvertiseData(byte[] data) {
        return new AdvertiseData.Builder()
                .setIncludeDeviceName( true )
                .addManufacturerData(0, data)
                .build();

    }

    @Override
    public String getDeviceName(Context context) {
//        return ((TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE))
//                .getDeviceId();
        return "";
    }
}
