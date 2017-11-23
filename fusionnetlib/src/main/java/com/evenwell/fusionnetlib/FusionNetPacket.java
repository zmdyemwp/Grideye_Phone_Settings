package com.evenwell.fusionnetlib;

import android.util.Log;

import java.util.HashMap;
import java.util.Map;

/**
 * Format
 * Packet Index - 1 byte
 * Total Packet Length - 4 byte
 * Command Task - 1 byte
 * Message Type - 1 byte
 * Command ID - 2 byte
 * Message ID - 2 byte
 * Req - 1 byte
 * Timeout - 1 byte
 * Message Length - 2 byte
 * Data Tag - 2 byte
 * Message Content - n byte
 * Message Alarm - 1 byte
 * Checksum - 4 byte
 * Created by KuroCHChung on 2016/1/13.
 */
/* package */ abstract class FusionNetPacket {

    private final static String TAG = FusionNetPacket.class.getSimpleName();
    private byte[] packet = null;
    private String belongsToDeviceMac;

    public static final String CommandTask = "CommandTask";
    public static final String TotalPacketLength = "TotalPacketLength";
    public static final String MessageType = "MessageType";
    public static final String CommandID = "CommandID";
    public static final String MessageID = "MessageID";
    public static final String Req = "Req";
    public static final String Timeout = "Timeout";
    public static final String MessageLength = "MessageLength";
    public static final String DataTag = "DataTag";
    public static final String MessageContent = "MessageContent";

    public static final String FeedbackType = "FeedbackType";
    public static final String AlarmCountWithInterval = "AlarmCountWithInterval";

    protected Map<String, Integer> indexMap = new HashMap<>();

    public static final int CommandTaskIndex = 4;
    public static final int TotalPacketLengthIndex = 0;

    public FusionNetPacket(byte[] data) {
        indexMap.put(FusionNetPacket.CommandTask, CommandTaskIndex);
        indexMap.put(FusionNetPacket.TotalPacketLength, TotalPacketLengthIndex);
        packet = data;
    }

    public int getCmdTask() {
        return packet[getFieldIndex(CommandTask)];
    }

    public long getPacketLen() {
        return ByteUtil.convertFourBytesToPositiveValue(
                packet[getFieldIndex(TotalPacketLength)],
                packet[getFieldIndex(TotalPacketLength)+1],
                packet[getFieldIndex(TotalPacketLength)+2],
                packet[getFieldIndex(TotalPacketLength)+3]
        );
    }

    public String getMessage() {
        if (!messageReady()) {
            Log.e(TAG, "getMessage: get message failed");
            return null;
        }

        String encodeString = null;
        try {
            byte[] message = new byte[getMessageLen() - 2]; // -2 data tag
            System.arraycopy(packet, getFieldIndex(MessageContent), message, 0, getMessageLen() - 2);
            encodeString = new String(message, "UTF-16LE");
        } catch (Exception e) {
            e.printStackTrace();
        }
        return encodeString;
    }

    public int getTimeoutValue() {
        return packet[getFieldIndex(Timeout)];
    }

    public int getMsgID() {
            return ByteUtil.convertTwoBytesToUnsignedInt(
                    packet[getFieldIndex(MessageID)],
                    packet[getFieldIndex(MessageID) + 1]);
    }

    public int getCmdID() {
        return ByteUtil.convertTwoBytesToPositiveValue(
                packet[getFieldIndex(CommandID)],
                packet[getFieldIndex(CommandID) + 1]);
    }

    public int getMessageLen() {
        if (!messageReady()) {
            return 0;
        }
        return ByteUtil.convertTwoBytesToPositiveValue(
                packet[getFieldIndex(MessageLength)],
                packet[getFieldIndex(MessageLength) + 1]);
    }

    public int getChecksum() {
        int size = packet.length;
        return (((int) packet[size-1] << 24) |
                ((int) packet[size-2] << 16) |
                ((int) packet[size-3] << 8) |
                packet[size-4]);
    }

    private boolean messageReady() {

        if (getCmdTask() != FusionNetConstants.CMD_TASK_SEND_MESSAGE) {
            Log.d(TAG, "messageReady: Cannot get message on the task(" + getCmdTask() + ")");
            return false;
        }

        return true;
    }

    public int getAlarmValue() {
        int size = packet.length;
        return ((int) packet[size-5]);
    }

    public void printPacket() {
        char[] message = null;
        if (getMessage() != null)
            message = getMessage().toCharArray();

        System.out.println("DUMP: Packet Length  = 0x" + Long.toHexString(getPacketLen()));
        System.out.println("DUMP: Command Task   = 0x" + Long.toHexString(getCmdTask()));
        System.out.println("DUMP: Command ID     = 0x" + Long.toHexString(getCmdID()));
        System.out.println("DUMP: Message ID     = 0x" + Long.toHexString(getMsgID()));
        System.out.println("DUMP: Timeout        = 0x" + Long.toHexString(getTimeoutValue()));
        System.out.println("DUMP: Message Length = 0x" + Long.toHexString(getMessageLen()));
        System.out.println("DUMP: Message        = " + getMessage());
        System.out.println("DUMP: Alarm          = " + getAlarmValue());
        System.out.println("DUMP: checksum       = " + getChecksum());

        if (message != null) {
            System.out.println("DUMP: Message info");
            for (int i = 0; i < message.length; i++) {
                System.out.println("DUMP: Message [0x" + Integer.toHexString(i).toUpperCase() + "] = 0x" +
                        Integer.toHexString(message[i]));
            }
        }
    }

    public String getBelongsToDeviceMac() {
        return belongsToDeviceMac;
    }

    public void setBelongsToDeviceMac(String belongsToDeviceMac) {
        this.belongsToDeviceMac = belongsToDeviceMac;
    }

    protected int getFieldIndex(String field) {
        return indexMap.get(field);
    }
}
