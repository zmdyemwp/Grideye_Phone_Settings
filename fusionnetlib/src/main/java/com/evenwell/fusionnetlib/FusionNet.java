package com.evenwell.fusionnetlib;

import android.content.Context;
import android.util.Log;

import com.evenwell.fusionnetlib.spec.FusionNetCallback;
import com.evenwell.fusionnetlib.spec.FusionNetSpec;

/**
 * Created by KuroCHChung on 2016/1/15.
 */
public class FusionNet implements FusionNetSpec {

    public final static String TAG = FusionNet.class.getSimpleName();

    public final static int CONNECTED = 1;
    public final static int DISCONNECT = 2;

    private static FusionNet instance;
    private FusionNetInternal fusionNetInternal;

    private static final String NOT_START_EXCEPTION = "Fusion net service not Start, do you forget FusionNet.startService()?";
    /**
     * private constructor to initialize internal fusion net service.
     * @param context
     */
    private FusionNet(Context context) {
        Log.d(TAG, "FusionNet Init");
        fusionNetInternal = new FusionNetInternal(context);
    }

    /**
     * Create a FusionNet service instance (singleton).
     * @param context any android context is acceptable
     * @return a FusionNet service instance (singleton)
     */
    public static FusionNet getInstance(Context context) {
        if(instance == null) {
            synchronized(FusionNet.class) {
                if(instance == null) {
                    instance = new FusionNet(context);
                }
            }
        }
        return instance;
    }

    /**
     * Register a FusionNet event callback.
     * @param callback
     */
    @Override
    public void registerFusionNetCallback(FusionNetCallback callback) {
        Log.d(TAG, "registerFusionNetCallback");
        fusionNetInternal.registerFusionNetCallback(callback);
    }

    /**
     * Unregister FusionNet event callback.
     */
    @Override
    public void unregisterFusionNetCallback() {
        Log.d(TAG, "unregisterFusionNetCallback");
        fusionNetInternal.unregisterFusionNetCallback();
    }

    /**
     * send help
     */
    @Override
    public void help() {
        if(isServiceStarted()) {
            if(fusionNetInternal.isAdvertiseSuccessed()) {
                fusionNetInternal.help();
            } else {
                throw new FusionNetNotSupportedDeviceException();
            }
        } else {
            throw new RuntimeException(NOT_START_EXCEPTION);
        }
    }

    /**
     * Feedback to specific message.
     * @param message
     */
    @Override
    public void feedbackTo(FusionNetMessage message) {
        if(isServiceStarted()) {
            if(fusionNetInternal.isAdvertiseSuccessed()) {
                fusionNetInternal.addFeedbackTask(message);
                fusionNetInternal.processNextFeedbackMessage();
            } else {
                throw new FusionNetNotSupportedDeviceException();
            }
        } else {
            throw new RuntimeException(NOT_START_EXCEPTION);
        }
    }

    /**
     * Start FusionNet service.
     */
    @Override
    public void startService() {
        if(!isServiceStarted()) {
            if(fusionNetInternal.startService()) {
                Log.d(TAG, "FusionNet service starting...");
            }
        }
    }

    /**
     * Stop FusionNet service
     */
    public void stopService() {
        if(isServiceStarted()) {
            if (fusionNetInternal.stopService()) {
                Log.d(TAG, "FusionNet stop Service Success");
            }
        }
    }

    @Override
    public boolean isSupportedDevice() {
        if(isServiceStarted()) {
            return fusionNetInternal.isAdvertiseSuccessed();
        } else {
            throw new RuntimeException(NOT_START_EXCEPTION);
        }
    }

    @Override
    public boolean isServiceStarted() {
        return fusionNetInternal.isServiceStarted();
    }
}
