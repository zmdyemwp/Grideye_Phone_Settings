package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/2/23.
 */
/* package */  class MessagePacket_Grideye extends FusionNetPacket_Grideye {
    public MessagePacket_Grideye(byte[] data) {
        super(data);
        indexMap.put(FusionNetPacket.MessageContent, 0); // n bytes
        // indexMap.put(FusionNetPacket.AlarmCountWithInterval, 12 + n + 1);
    }
}
