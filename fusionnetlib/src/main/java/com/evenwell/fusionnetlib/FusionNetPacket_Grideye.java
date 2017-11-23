package com.evenwell.fusionnetlib;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by KuroCHChung on 2016/2/23.
 */
/* package */ abstract class FusionNetPacket_Grideye {

    private byte[] packet = null;

    protected Map<String, Integer> indexMap = new HashMap<>();

    public static final int CommandTaskIndex = 0;
    public static final int TotalPacketLengthIndex = 1;

    public FusionNetPacket_Grideye(byte[] data) {
        indexMap.put(FusionNetPacket.CommandTask, CommandTaskIndex);
        indexMap.put(FusionNetPacket.TotalPacketLength, TotalPacketLengthIndex);
        packet = data;
    }

    public int getCmdTask() {
        return packet[getFieldIndex(FusionNetPacket.CommandTask)];
    }

    public long getPacketLen() {
        return ByteUtil.convertFourBytesToPositiveValue(
                packet[getFieldIndex(FusionNetPacket.TotalPacketLength)+3],
                packet[getFieldIndex(FusionNetPacket.TotalPacketLength)+2],
                packet[getFieldIndex(FusionNetPacket.TotalPacketLength)+1],
                packet[getFieldIndex(FusionNetPacket.TotalPacketLength)]
        );
    }

    public int getMsgID() {
        return ByteUtil.convertTwoBytesToUnsignedInt(
                packet[getFieldIndex(FusionNetPacket.MessageID)],
                packet[getFieldIndex(FusionNetPacket.MessageID) + 1]);
    }

    public int getCmdID() {
        return ByteUtil.convertTwoBytesToPositiveValue(
                packet[getFieldIndex(FusionNetPacket.CommandID)],
                packet[getFieldIndex(FusionNetPacket.CommandID) + 1]);
    }

    public int getMessageLen() {
        // 4 byte total length
        // 1 byte command task ID
        // 1 byte message type
        // 2 byte SN
        // 2 byte MsgID
        // 1 byte FeedbackType
        // 1 byte Timeout
        // 1 byte AlarmCountWithInterval
        // 13
        return packet.length;
    }
    /*
    public String getMessage() {

        String encodeString = null;
        try {
            byte[] message = new byte[getMessageLen()]; // -1 byte for AlarmCountWithInterval
            System.arraycopy(packet, getFieldIndex(FusionNetPacket.MessageContent), message, 0, getMessageLen());
            encodeString = new String(message, "UTF-16LE");


        } catch (Exception e) {
            e.printStackTrace();
        }
        return encodeString;
    }
*/
    public byte[] getMessage() {
        byte[] message = null;
        try {
            message = new byte[getMessageLen()]; // -1 byte for AlarmCountWithInterval
            System.arraycopy(packet, getFieldIndex(FusionNetPacket.MessageContent), message, 0, getMessageLen());

        } catch (Exception e) {
            e.printStackTrace();
        }
        return message;
    }

    public int getTimeoutValue() {
        return packet[getFieldIndex(FusionNetPacket.Timeout)];
    }

    protected int getFieldIndex(String field) {
        return indexMap.get(field);
    }

}
