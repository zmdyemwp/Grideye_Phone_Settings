package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/2/23.
 */
/* package */  class MessagePacketV2 extends FusionNetPacketV2 {
    public MessagePacketV2(byte[] data) {
        super(data);
        indexMap.put(FusionNetPacket.MessageType, 5); // 1 byte
        indexMap.put(FusionNetPacket.CommandID, 6); // 2 bytes
        indexMap.put(FusionNetPacket.MessageID, 8); // 2 bytes
        indexMap.put(FusionNetPacket.FeedbackType, 10); // 1 byte
        indexMap.put(FusionNetPacket.Timeout, 11); // 1 byte
        indexMap.put(FusionNetPacket.MessageContent, 12); // n bytes
        // indexMap.put(FusionNetPacket.AlarmCountWithInterval, 12 + n + 1);
    }
}
