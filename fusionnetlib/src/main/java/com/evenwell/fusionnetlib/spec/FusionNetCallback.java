package com.evenwell.fusionnetlib.spec;

import com.evenwell.fusionnetlib.FusionNetMessage;

/**
 * Created by KuroCHChung on 2016/1/11.
 */
public interface FusionNetCallback {
    void onMessageReceived(FusionNetMessage message);
    void onFeedbackConfirm(FusionNetMessage message);
    void onHelpConfirm();
    void onConnectionStateChange(int state);
    void onServiceStart();
    //CrabCSChen@FIHTDC add for SMFT-1926, 2016/05/23 {
    void onErrorOccur(int error_type);
    //CrabCSChen@FIHTDC add for SMFT-1926, 2016/05/23 }
}
