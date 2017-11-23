package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/1/18.
 */
/* package */ class FeedbackPacket extends FusionNetPacket {

    public FeedbackPacket(byte[] data) {
        super(data);
        indexMap.put(FusionNetPacket.CommandID, 5);
        indexMap.put(FusionNetPacket.MessageID, 7);
    }

}
