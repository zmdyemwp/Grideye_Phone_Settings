package com.evenwell.grideye.Peripheral.receiver;

import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;

import com.evenwell.grideye.GrideyeActivity;
import com.evenwell.grideye.Peripheral.service.FusionNetService;
import com.evenwell.grideye.Peripheral.util.FusionConst;
import com.evenwell.grideye.Peripheral.util.FusionUtils;

import java.util.List;

/**
 * Created by KashKHYang on 2016/4/27.
 */
public class BluetoothStateReceiver extends BroadcastReceiver {
    private static final String TAG = BluetoothStateReceiver.class.getSimpleName();

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        SharedPreferences mPrefs = context.getSharedPreferences(FusionConst.Prefs.NAME, 0);
        boolean mIsServiceStart = mPrefs.getBoolean(FusionConst.Prefs.KEY_SERVICE_STATE, false);
        if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)) {
            if (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1)
                    == BluetoothAdapter.STATE_OFF) {
                if (mIsServiceStart) {
                    context.stopService(new Intent(context, FusionNetService.class)
                            .putExtra(GrideyeActivity.DIALOG_TYPE,GrideyeActivity.TYPE_BT_STATE_INFO));
                    context.startActivity((new Intent(context, GrideyeActivity.class))
                                                .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
                                                .putExtra(GrideyeActivity.DIALOG_TYPE,
                                                        GrideyeActivity.TYPE_BT_STATE_INFO));
                }
            } else {
                if (mIsServiceStart) {
                    List<String> permissionList = FusionUtils.needPermissionCheck(context);
                    if(permissionList != null && permissionList.size() > 0){
                        Log.i(TAG,"permission check fail, fail to restart fusion net");
                        context.startService(new Intent(context, FusionNetService.class));
                    }
                }
            }
        }
    }
}
