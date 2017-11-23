package com.evenwell.fusionnetlib.spec;

import com.evenwell.fusionnetlib.FusionNetMessage;

/**
 * Created by KuroCHChung on 2016/1/11.
 */
public interface FusionNetSpec {
    void registerFusionNetCallback(FusionNetCallback callback);
    void unregisterFusionNetCallback();
    void help();
    void feedbackTo(FusionNetMessage message);
    void startService();
    void stopService();
    boolean isServiceStarted();
    boolean isSupportedDevice();
}
