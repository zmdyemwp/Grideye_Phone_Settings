package com.evenwell.fusionnetlib;

import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

/**
 * Created by KuroCHChung on 2016/1/20.
 */
/* package */ class GrideyeDataChannel {

    private final static String TAG = FusionNetDataChannel.class.getSimpleName();
    private FusionNetPacket_Grideye grideyePacket;
    private int packetIndex = -1;
    private boolean packetReady = false;
    private ByteArrayOutputStream totalObj = new ByteArrayOutputStream();
    private byte[] packet = null;

    private final static int PACKAGE_LEN = 1;

    public boolean attach(byte[] data) {
//        if (data[0] != this.packetIndex + 1) {
//            Log.e(TAG, "Order is incorrect.");
//            return false;
//        }
        //this.packetIndex = data[0];
        try {
            totalObj.write(data, 0, data.length); // Remove index field.
        } catch (Exception e) {
            Log.e(TAG, "Write data to Output stream failed " + e);
        }
        packetReady = isReady();
        return true;
    }

    public byte[] getReceivedData() {
        return totalObj.toByteArray();
    }

    public boolean isReady() {
        if (!packetReady) {
            byte[] data = totalObj.toByteArray();
            Log.d(TAG, "data.length = " + data.length);

            //if ((data.length) == PACKAGE_LEN) { // -5 for total length and command id
                if(grideyePacket == null) {

                    Log.d(TAG, "Ready! Initial Packet MessagePacket");
                    grideyePacket = new MessagePacket_Grideye(data);

//                    switch (getCmdTask()) {
//                        case FusionNetConstants.CMD_TASK_SEND_MESSAGE:
//                            Log.d(TAG, "Ready! Initial Packet MessagePacket");
//                            fusionNetPacket = new MessagePacket_Grideye(data);
//                            break;
//                        case FusionNetConstants.FUSION_CMD_CLEAR:
//                            Log.d(TAG, "Ready! Clear Command");
//                            fusionNetPacket = new FeedbackPacketV2(data);
//                            break;
////                        case FusionNetConstants.CMD_TASK_FEEDBACK_CONFIRM:
////                            Log.d(TAG, "Ready! Initial Packet FeedbackPacket");
////                            fusionNetPacket = new FeedbackPacket(data);
////                            break;
//                    }
                    return true;
                }

            //}

//            if ((data.length - 4) == getPacketLen()) {
//                packet = totalObj.toByteArray();
//                if(fusionNetPacket == null) {
//                    switch (getCmdTask()) {
//                        case FusionNetConstants.CMD_TASK_SEND_MESSAGE:
//                            Log.d(TAG, "Ready! Initial Packet MessagePacket");
//                            fusionNetPacket = new MessagePacket(data);
//                            break;
//                        case FusionNetConstants.CMD_TASK_FEEDBACK_CONFIRM:
//                            Log.d(TAG, "Ready! Initial Packet FeedbackPacket");
//                            fusionNetPacket = new FeedbackPacket(data);
//                            break;
//                    }
//                }
//                return true;
//            }
        }
        return packetReady;
    }

    public int getPacketLen() {
        if (totalObj.size() > 0) {
            byte[] data = totalObj.toByteArray();
//            ByteBuffer wrapped = ByteBuffer.wrap(new byte[]{
//                    data[FusionNetPacket.TotalPacketLengthIndex+3],
//                    data[FusionNetPacket.TotalPacketLengthIndex+2],
//                    data[FusionNetPacket.TotalPacketLengthIndex+1],
//                    data[FusionNetPacket.TotalPacketLengthIndex]
//            });
            ByteBuffer wrapped = ByteBuffer.wrap(new byte[]{
                    data[FusionNetPacketV2.TotalPacketLengthIndex],
                    data[FusionNetPacketV2.TotalPacketLengthIndex+1],
                    data[FusionNetPacketV2.TotalPacketLengthIndex+2],
                    data[FusionNetPacketV2.TotalPacketLengthIndex+3],
            });
            // return wrapped.getInt();
            return wrapped.getInt();
        } else {
            return -1;
        }
    }

    public int getCmdTask() {
        if (packet == null)
            return totalObj.toByteArray()
                    [FusionNetPacketV2.CommandTaskIndex];

        return packet[FusionNetPacketV2.CommandTaskIndex];
    }

    public FusionNetPacket_Grideye getGrideyePacket() {
        return grideyePacket;
    }

}
