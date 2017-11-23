package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/2/26.
 */
/* package */ class FeedbackPacketV2 extends FusionNetPacketV2 {
    public FeedbackPacketV2(byte[] data) {
        super(data);
        //indexMap.put(FusionNetPacket.CommandID, 5);
        indexMap.put(FusionNetPacket.MessageID, 6);
    }
}