package com.evenwell.grideye.Peripheral.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import com.evenwell.grideye.Peripheral.service.FusionNetService;
import com.evenwell.grideye.Peripheral.util.FusionConst;
import com.evenwell.grideye.Peripheral.util.FusionUtils;

/**
 * Created by KashKHYang on 2016/4/27.
 */
public class MessageExpiredReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if (null != intent.getAction() &&
                intent.getAction().equals(FusionNetService.ACTION_MSG_EXPIRED)) {
            //remove the message in the actionbar
            FusionUtils.cancelNotification(context, FusionConst.Notification.ID_MSG);

//            //CrabCSChen@FIHTDC add for cancel dialog, 2016/05/16{
//            if(GrideyeActivity.getInstance()!=null)
//                GrideyeActivity.getInstance().finish();
//            //CrabCSChen@FIHTDC add for cancel dialog, 2016/05/16}

            //remove from preference
            SharedPreferences mPrefs =
                    context.getSharedPreferences(FusionConst.Prefs.NAME, 0);
            SharedPreferences.Editor PE = mPrefs.edit();
            PE.remove(FusionConst.Prefs.KEY_MSG_TIME).commit();
            PE.remove(FusionConst.Prefs.KEY_MSG_CONTENT).commit();
            PE.remove(FusionConst.Prefs.KEY_MSG_ID).commit();
            PE.remove(FusionConst.Prefs.KEY_MSG_COMMAND_ID).commit();
            PE.remove(FusionConst.Prefs.KEY_MSG_NOT_FEEDBACK).commit();
        }
    }
}
