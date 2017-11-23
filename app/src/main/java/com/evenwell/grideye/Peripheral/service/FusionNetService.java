package com.evenwell.grideye.Peripheral.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.PowerManager;
import android.util.Log;

import com.evenwell.grideye.GrideyeActivity;
import com.evenwell.grideye.R;
import com.evenwell.grideye.Peripheral.util.FusionConst;
import com.evenwell.grideye.Peripheral.util.FusionConst.Prefs;
import com.evenwell.grideye.Peripheral.util.FusionUtils;
import com.evenwell.fusionnetlib.FusionNet;
import com.evenwell.fusionnetlib.FusionNetMessage;
import com.evenwell.fusionnetlib.spec.FusionNetCallback;

public class FusionNetService extends Service {
    public static final String ACTION_MSG_EXPIRED =
            "com.fihtdc.fusionnet.band.simulator.msg_expired";

    private final static String TAG = FusionNetService.class.getSimpleName();
    public static long MSG_EXPIRED_TIME = 15 * 60 * 1000;//15 min
    private static PowerManager.WakeLock mWakeLock;
    private final IBinder mBinder = new LocalBinder();
    private final Runnable mReleaser = new Runnable() {
        public void run() {
            release();
        }
    };
    private FusionNet fusionNet;
    private SharedPreferences mPrefs;
    private long mMessageReceivedTime;
    private boolean initialized = false;
    private Handler mHandler = null;
    private Handler mWakeLockHandler = new Handler();
    private FusionNetCallback callback = new FusionNetCallback() {
        @Override
        public void onMessageReceived(FusionNetMessage message) {
            Log.i(TAG, "Received Message ID: " + message.getMessageId()
                    + ", Command ID: " + message.getCommandId()
                    + ", Content:" + message.getMessage());
            mMessageReceivedTime = System.currentTimeMillis();
            if (mHandler != null) {
                Log.i(TAG, "UI in the front");
                Message msg = new Message();
                msg.what = FusionConst.CommandType.TYPE_RECEIVE_MSG;
                msg.obj = message;
                mHandler.sendMessage(msg);
            } else {
                Log.i(TAG, "UI not in the front");
                //TODO: show the dialog to notify user??
                //CrabCSChen@FIHTDC add for show dialog, 2016/05/05 {
                Intent startDialogIntent = new Intent(FusionNetService.this, GrideyeActivity.class);
                // CrabCSChen@FIHTDC add for SMFT-1945, 2016/06/08 {
                startDialogIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
                // CrabCSChen@FIHTDC add for SMFT-1945, 2016/06/08 }
                startDialogIntent.putExtra(GrideyeActivity.DIALOG_TYPE, GrideyeActivity.TYPE_UI_IN_BACKGROUND);
                startDialogIntent.putExtra("TITLE", getResources().getString(R.string.app_name));
                startDialogIntent.putExtra("CONTENT", message.getMessage());
                //CrabCSChen@FIHTDC modify for SMFT-1942, 2016/06/06 {
                startDialogIntent.putExtra("RECEIVE_TIME",mMessageReceivedTime);
                //CrabCSChen@FIHTDC modify for SMFT-1942, 2016/06/06 }
                FusionNetService.this.startActivity((startDialogIntent)
                        .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK));
                //CrabCSChen@FIHTDC add for show dialog, 2016/05/05 }
            }

            //send the message to the actionbar
            FusionUtils.buildNotification(FusionNetService.this,
                    FusionConst.Notification.ID_MSG, message.getMessage());
            acquireWakeLock(2000);//2 sec

            //vibrate 2 sec
            /*if(mVibrator == null)
                mVibrator = (Vibrator) FusionNetService.this.
                                getSystemService(Service.VIBRATOR_SERVICE);
            mVibrator.vibrate(500);*/

            //TODO: ringtone -> display ringtone in notificationmanager

            FusionUtils.setupExpiredAlarm(
                    FusionNetService.this, mMessageReceivedTime, MSG_EXPIRED_TIME);
            SharedPreferences.Editor PE = mPrefs.edit();
            PE.putLong(Prefs.KEY_MSG_TIME, mMessageReceivedTime).commit();
            PE.putString(Prefs.KEY_MSG_CONTENT, message.getMessage()).commit();
            PE.putInt(Prefs.KEY_MSG_COMMAND_ID, message.getCommandId()).commit();
            PE.putInt(Prefs.KEY_MSG_ID, message.getMessageId()).commit();
            PE.remove(Prefs.KEY_MSG_NOT_FEEDBACK).commit();
        }

        @Override
        public void onFeedbackConfirm(FusionNetMessage message) {
            Log.d(TAG, "onFeedbackConfirm Message ID: " + message.getMessageId()
                    + ", Command ID: " + message.getCommandId());
            if (mHandler != null) {
                Log.i(TAG, "UI in the front");
                Message msg = new Message();
                msg.what = FusionConst.CommandType.TYPE_RECEIVE_FEEDBACK_CONFIRM;
                msg.obj = message;
                mHandler.sendMessage(msg);
            } else {
                Log.i(TAG, "UI not in the front");
                //TODO: show message dialog or toast??
            }
        }

        @Override
        public void onHelpConfirm() {
            //no need if do not support trigger help from phone
            Log.d(TAG, "onHelpConfirm()");
            /*if(mHandler != null){
                Log.i(TAG,"UI in the front");
                mHandler.onHelpConfirm();
            }else{
                Log.i(TAG,"UI not in the front");
                //show message dialog or toast
            }*/
        }

        @Override
        public void onConnectionStateChange(int state) {
            Log.d(TAG, "State Change: " + state);
        }

        @Override
        public void onServiceStart() {

        }

        //CrabCSChen@FIHTDC add for SMFT-1926, 2016/05/23 {
        @Override
        public void onErrorOccur(int error_type) {
            Log.i(TAG,"Error occur: " + error_type);
            Intent startDialogIntent = new Intent(FusionNetService.this, GrideyeActivity.class);
            startDialogIntent.putExtra(GrideyeActivity.DIALOG_TYPE, error_type);
            startDialogIntent.putExtra("TITLE", getResources().getString(R.string.app_name));
            startDialogIntent.putExtra("CONTENT", getResources().getString(R.string.str_bt_error_description));
            FusionNetService.this.startActivity((startDialogIntent)
                    .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK));
        }
        //CrabCSChen@FIHTDC add for SMFT-1926, 2016/05/23 }
    };

    public FusionNetService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (fusionNet == null) {
            initialize();
            startFusionNetService();
        }
        if (mPrefs == null)
            mPrefs = getSharedPreferences(Prefs.NAME, 0);
        return START_STICKY;
    }

    public void stopFusionNetService() {
        fusionNet.stopService();
        FusionUtils.cancelNotification(FusionNetService.this,
                FusionConst.Notification.ID_ONGOING);

    }

    public void help() {
        fusionNet.help();
    }

    public void startFusionNetService() {
        fusionNet.startService();
        FusionUtils.buildNotification(FusionNetService.this,
                FusionConst.Notification.ID_ONGOING, null);
    }

    public void feedbackToMessage(FusionNetMessage message) {
        Log.d(TAG, "feedbackToMessage");
        fusionNet.feedbackTo(message);
    }

    public void initialize() {
        if (!initialized) {
            fusionNet = FusionNet.getInstance(this);
            fusionNet.registerFusionNetCallback(callback);
            initialized = true;
        }
    }

    public void setUIHandler(Handler h) {
        mHandler = h;
    }

    public void acquireWakeLock(long timeout) {
        Log.i(TAG, "acquireWakeLock");
        if (mWakeLock == null) {
            PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
            mWakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK |
                    PowerManager.ACQUIRE_CAUSES_WAKEUP |
                    PowerManager.ON_AFTER_RELEASE, "FusionNet WakeLock");
        }
        mWakeLock.acquire();
        mWakeLockHandler.postDelayed(mReleaser, timeout);
    }

    public void release() {
        Log.i(TAG, "release WakeLock");
        if (mWakeLock != null) {
            if (mWakeLock.isHeld()) {
                Log.i(TAG, "do release WakeLock");
                mWakeLock.release();
            }
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.d(TAG, "onBind");
        return mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.d(TAG, "onUnbind");
        return super.onUnbind(intent);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        try {
            stopFusionNetService();
            mWakeLock = null;
        } catch (Exception e) {
        }
    }

    public class LocalBinder extends Binder {
        public FusionNetService getService() {
            return FusionNetService.this;
        }
    }
}
