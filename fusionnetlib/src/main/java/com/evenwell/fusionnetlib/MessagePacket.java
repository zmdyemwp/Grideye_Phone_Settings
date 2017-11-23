package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/1/18.
 */
/* package */ class MessagePacket extends FusionNetPacket {

    public MessagePacket(byte[] data) {
        super(data);
        indexMap.put(FusionNetPacket.MessageType, 5); // 1 byte
        indexMap.put(FusionNetPacket.CommandID, 6); // 2 bytes
        indexMap.put(FusionNetPacket.MessageID, 8); // 2 bytes
        indexMap.put(FusionNetPacket.Req, 10); // 1 byte
        indexMap.put(FusionNetPacket.Timeout, 11); // 1 byte
        indexMap.put(FusionNetPacket.MessageLength, 12);
        indexMap.put(FusionNetPacket.DataTag, 14);
        indexMap.put(FusionNetPacket.MessageContent, 16);
    }

}
