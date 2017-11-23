package com.evenwell.fusionnetlib;

import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.content.Context;

/**
 * Created by KuroCHChung on 2016/1/23.
 */
/* package */ interface FusionNetAdvertiser {
    byte[] help();
    byte[] feedback(FusionNetMessage message);
    byte[] outOfRange();
    byte[] getDefaultManufactureData();
    AdvertiseSettings getAdvertiseSettings();
    AdvertiseData getAdvertiseData(byte[] data);
    String getDeviceName(Context context);
}
