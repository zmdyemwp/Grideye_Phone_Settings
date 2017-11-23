package com.evenwell.fusionnetlib;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by KuroCHChung on 2016/1/18.
 */
/* package */ class GrideyePacketReceiver {

    private final static String TAG = GrideyePacketReceiver.class.getSimpleName();

    private Map<String, GrideyeDataChannel> deviceChannels = new HashMap<>();

    public void clearChannel(String deviceAddress) {
        deviceChannels.remove(deviceAddress);
    }

    public GrideyeDataChannel getDataChannel(String deviceAddress) {
        GrideyeDataChannel channel = deviceChannels.get(deviceAddress);
        if(channel == null) {
            channel = new GrideyeDataChannel();
            deviceChannels.put(deviceAddress, channel);
        }
        return channel;
    }

}
