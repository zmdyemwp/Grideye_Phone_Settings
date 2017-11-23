package com.evenwell.grideye.Peripheral.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

/**
 * Created by KashKHYang on 2016/4/27.
 */
public class SecretCodeReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent pIntent) {
        Intent intent = new Intent();
        intent.setClassName("com.fihtdc.fusionnet.band.simulator",
                    "com.fihtdc.fusionnet.band.simulator.ui.EngActivity");
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(intent);
    }
}
