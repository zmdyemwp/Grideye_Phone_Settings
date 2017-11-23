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
/* package */ class FusionNetAdvertiserV2 implements FusionNetAdvertiser {

    private final static String TAG = FusionNetAdvertiserV2.class.getSimpleName();

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

    public FusionNetAdvertiserV2(Context context) {
        imei = ByteUtil.longToByteArray(Long.parseLong(getDeviceName(context)));
        this.mContext = context;
    }

    @Override
    public byte[] help() {

        SharedPreferences sharedPref = mContext.getSharedPreferences(
                FusionNetConstants.FUSION_PREFERENCE, Context.MODE_PRIVATE);

        int helpCount = sharedPref.getInt(FusionNetConstants.HELP_COUNTER_KEY, -1);
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putInt(FusionNetConstants.HELP_COUNTER_KEY, ++helpCount);
        editor.commit(); // immediately

        Log.d(TAG, "Help with count: " + helpCount);

        return new byte[]{
                0x00,
                0x00, // command id
                0x00,
                0x00, // message id
                0x00,
                help, // function index
                prefixName[0], // F
                prefixName[1], // A
                prefixName[2], // A
                prefixName[3], // _
                (byte) helpCount, // parameter
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

    @Override
    public byte[] outOfRange() {
        return new byte[0];
    }

    @Override
    public byte[] getDefaultManufactureData() {
        return new byte[] {
                0x00,
                0x00, // command id
                0x00,
                0x00,  // message id
                0x00, // has feedback
                0x00, // has help
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
        return new AdvertiseData.Builder().addManufacturerData(0, data)
                .setIncludeDeviceName(false)
                .build();
    }

    @Override
    public String getDeviceName(Context context) {
        return ((TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE))
                .getDeviceId();
    }
}
