package com.evenwell.grideye.Peripheral.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import com.evenwell.grideye.Peripheral.service.FusionNetService;
import com.evenwell.grideye.Peripheral.util.FusionConst;

/**
 * Created by KashKHYang on 2016/4/27.
 */
public class BootReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if (null != intent.getAction() &&
                intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
            SharedPreferences mPrefs = context.getSharedPreferences(FusionConst.Prefs.NAME, 0);
            boolean mIsServiceStart = mPrefs.getBoolean(FusionConst.Prefs.KEY_SERVICE_STATE, false);
            if (mIsServiceStart) {
                context.startService(new Intent(context, FusionNetService.class));
            }
        }
    }
}
