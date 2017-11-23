package com.evenwell.fusionnetlib;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by KuroCHChung on 2016/1/18.
 */
/* package */ class FusionNetPacketReceiver {

    private final static String TAG = FusionNetPacketReceiver.class.getSimpleName();

    private Map<String, FusionNetDataChannel> deviceChannels = new HashMap<>();

    public void clearChannel(String deviceAddress) {
        deviceChannels.remove(deviceAddress);
    }

    public FusionNetDataChannel getDataChannel(String deviceAddress) {
        FusionNetDataChannel channel = deviceChannels.get(deviceAddress);
        if(channel == null) {
            channel = new FusionNetDataChannel();
            deviceChannels.put(deviceAddress, channel);
        }
        return channel;
    }

}
