package com.evenwell.fusionnetlib;

import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.content.Context;
import android.telephony.TelephonyManager;

/**
 * Advertisement Format
 * BLE Header - 3 bytes
 * Name Length - 1 byte
 * Name identify - 1 byte
 * Phone Prefix (FAA_) - 4 byte
 * IMEI - 7 byte
 * FF Length - 1 byte
 * Manufacture Format - 1 byte
 * Manufacture ID - 2 bytes // ignore nordic app version 2 bytes, because manufacture data occupied it.
 * Cmd ID - 2 bytes
 * Msg ID - 2 bytes
 * Has Feedback - 1 byte
 * Function - 1 byte
 * Nordic BLE Version - 2 byte
 * BT Version - 2 byte
 */
/* package */ class FusionNetAdvertiserV1 implements FusionNetAdvertiser {

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
                .setIncludeDeviceName(true)
                .build();
    }

    @Override
    public String getDeviceName(Context context) {
        return "FAA_" + FusionNetUtil.encodeIMEI(
                ((TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE))
                        .getDeviceId());
    }

    public byte[] help() {
        return new byte[]{
                0x00,
                0x00, // command id
                0x00,
                0x00, // message id
                0x00, // has feedback
                0x01, // function, 0x01 help
                0x00,
                0x00, // nordic ble version
                0x00,
                0x00 // bt version
        };
    }

    public byte[] feedback(FusionNetMessage message) {
        return new byte[] {
                (byte) ((message.getCommandId() >> 8) & 0xFF),
                (byte) (message.getCommandId() & 0xFF), // command id
                (byte) ((message.getMessageId() >> 8) & 0xFF),
                (byte) (message.getMessageId() & 0xFF),  // message id
                0x01, // has feedback
                0x00, // function
                0x00,
                0x00,// nordic ble version
                0x00,
                0x00 // bt version
        };
    }

    @Override
    public byte[] outOfRange() {
        return new byte[]{
                0x00,
                0x00, // command id
                0x00,
                0x00, // message id
                0x00, // has feedback
                0x04, // function, 0x04 station sync
                0x00,
                0x00, // nordic ble version
                0x00,
                0x00 // bt version
        };
    }

    @Override
    public byte[] getDefaultManufactureData() {
        return new byte[]{};
    }


}
