package com.evenwell.grideye.Peripheral.util;

import android.Manifest;
import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.RingtoneManager;
import android.support.v4.app.NotificationCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;

import com.evenwell.grideye.R;
import com.evenwell.grideye.Peripheral.service.FusionNetService;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by KashKHYang on 2016/4/28.
 */
public class FusionUtils {
    private static final String TAG = FusionUtils.class.getSimpleName();

    public static void cancelNotification(final Context ctx, int notification_id) {
        NotificationManager nm = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
        nm.cancel(notification_id);
    }

    public static void buildNotification(Context ctx, int notification_id, String message) {
        NotificationCompat.Builder builder = new NotificationCompat.Builder(ctx);
        int sdk = android.os.Build.VERSION.SDK_INT;
        if (sdk >= 21) {
            //Android 5.0
            builder.setColor(ctx.getResources().getColor(R.color.colorPrimary));
        }
        //TODO: update the icon and other param
        switch (notification_id) {
            case FusionConst.Notification.ID_MSG:
                builder.setSmallIcon(R.drawable.fusion_app_message);
                builder.setContentTitle(ctx.getResources().getString(R.string.app_name));
                builder.setContentText(message + " " + ctx.getResources().getString(R.string.str_message_received));
                builder.setTicker(message + " " + ctx.getResources().getString(R.string.str_message_received));
                builder.setOngoing(false);
                builder.setAutoCancel(true);
                builder.setWhen(System.currentTimeMillis());
                break;
            case FusionConst.Notification.ID_ONGOING:
                builder.setSmallIcon(R.drawable.fusion_app_active);
                builder.setContentTitle(ctx.getResources().getString(R.string.app_name));
                builder.setContentText(ctx.getResources().getString(R.string.str_service_running));
                builder.setTicker(ctx.getResources().getString(R.string.str_service_running));
                builder.setOngoing(true);
                builder.setAutoCancel(false);
                builder.setWhen(System.currentTimeMillis());
                break;
            default:
                Log.e(TAG, "undefined notification id");
                return;
        }

        Intent i = new Intent();
        i.setClassName("com.fihtdc.fusionnet.band.simulator",
                "com.fihtdc.fusionnet.band.simulator.ui.SimulatorMainActivity");
        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
        PendingIntent mPendingIntent = PendingIntent.getActivity(ctx, 0, i, PendingIntent.FLAG_UPDATE_CURRENT);
        builder.setContentIntent(mPendingIntent);
        NotificationManager nm = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
        //CrabCSChen@FIHTDC add for ringtone and vibrate, 2016/05/05 {
        if(notification_id == FusionConst.Notification.ID_MSG) {
            builder.setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION));
            builder.setVibrate(new long[]{100, 500, 100, 500});
        }
        //CrabCSChen@FIHTDC add for ringtone and vibrate, 2016/05/05 }
        nm.notify(notification_id, builder.build());
    }

    public static void setupExpiredAlarm(Context ctx, long start_time, long period) {
        final Intent MSG_EXPIRED_INTENT = new Intent(FusionNetService.ACTION_MSG_EXPIRED);
        PendingIntent mPendingIntent = PendingIntent.getBroadcast(ctx, 1,
                MSG_EXPIRED_INTENT, PendingIntent.FLAG_UPDATE_CURRENT);
        AlarmManager am = (AlarmManager) ctx.getSystemService(Context.ALARM_SERVICE);
        am.setExact(AlarmManager.RTC, start_time + period, mPendingIntent);
    }

    public static List<String> needPermissionCheck(Context ctx) {
        ArrayList<String> ret = new ArrayList<>();
        /*if (ContextCompat.checkSelfPermission(ctx,
                Manifest.permission.BLUETOOTH)
                != PackageManager.PERMISSION_GRANTED) {
            ret.add(Manifest.permission.BLUETOOTH);
        }
        if (ContextCompat.checkSelfPermission(ctx,
                Manifest.permission.BLUETOOTH_ADMIN)
                != PackageManager.PERMISSION_GRANTED) {
            ret.add(Manifest.permission.BLUETOOTH_ADMIN);
        }*/
        if (ContextCompat.checkSelfPermission(ctx,
                Manifest.permission.READ_PHONE_STATE)
                != PackageManager.PERMISSION_GRANTED) {
            ret.add(Manifest.permission.READ_PHONE_STATE);
        }
        return ret;
    }
}
