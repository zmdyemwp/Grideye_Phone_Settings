package com.evenwell.fusionnetlib;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattServer;
import android.bluetooth.BluetoothGattServerCallback;
import android.util.Log;

import com.evenwell.fusionnetlib.spec.FusionNetCallback;

/**
 * Created by KuroCHChung on 2016/1/13.
 */
/* package */ class FusionNetGattServerCallback extends BluetoothGattServerCallback {

    private static final String TAG = FusionNetGattServerCallback.class.getSimpleName();

    private FusionNetCallback fusionNetCallbackSpec;

    private FusionNetInternal fusionNet;
    private FusionNetPacketReceiver fusionNetPacketReceiver = new FusionNetPacketReceiver();
    private GrideyePacketReceiver grideyePacketReceiver = new GrideyePacketReceiver();
    private BluetoothGattServer mGattServer;

    public FusionNetGattServerCallback(FusionNetInternal fusionNet) {
        this.fusionNet = fusionNet;
    }

    @Override
    public void onConnectionStateChange(BluetoothDevice device, final int status, int newState) {
        super.onConnectionStateChange(device, status, newState);
        if (status == BluetoothGatt.GATT_SUCCESS) {
            if (newState == BluetoothGatt.STATE_CONNECTED) {
                Log.v(TAG, "Connected to device: " + device.getAddress());
                if(fusionNetCallbackSpec != null) {
                    fusionNetCallbackSpec.onConnectionStateChange(FusionNet.CONNECTED);
                }

            } else {
                Log.v(TAG, "Disconnected from device: " + device.getAddress());
                fusionNetPacketReceiver.clearChannel(device.getAddress());
                grideyePacketReceiver.clearChannel(device.getAddress());
                if(fusionNetCallbackSpec != null) {
                    fusionNetCallbackSpec.onConnectionStateChange(FusionNet.DISCONNECT);
                }
            }
        }  else {
            Log.e(TAG, "Error when connecting: " + status);
        }
    }

    public void setGattServer(BluetoothGattServer gattServer) {
        this.mGattServer = gattServer;
    }

    @Override
    public void onMtuChanged(BluetoothDevice device, int mtu) {
        super.onMtuChanged(device, mtu);
        Log.d(TAG, "onMtuChanged");
    }

    @Override
    public void onCharacteristicReadRequest(BluetoothDevice device, int requestId, int offset, BluetoothGattCharacteristic characteristic) {
        super.onCharacteristicReadRequest(device, requestId, offset, characteristic);
        Log.d(TAG, "onCharacteristicReadRequest");

        if (offset != 0) {
            Log.d(TAG, "onCharacteristicReadRequest - offset != 0");
            mGattServer.sendResponse(device, requestId, BluetoothGatt.GATT_INVALID_OFFSET, offset,
            /* value (optional) */ null);
            return;
        }
        Log.d(TAG, "onCharacteristicReadRequest - offset == 0");

        if(1 == sRequiredConfig) {
            Log.e(TAG, "Send Config_1 +++");
            mGattServer.sendResponse(device, requestId, BluetoothGatt.GATT_SUCCESS,
                    offset, fusionNet.getResponseData_1());
            Log.e(TAG, "Send Config_1 ---");
        } else if(2 == sRequiredConfig) {
            Log.e(TAG, "Send Config_2 +++");
            byte[] data = fusionNet.getResponseData_2();
            for(int i = 0; i < data.length; i++) {
                Log.d(TAG, String.format("[%d] 0x%02x", i, data[i]));
            }
            
            mGattServer.sendResponse(device, requestId, BluetoothGatt.GATT_SUCCESS,
                    offset, data);
            Log.e(TAG, "Send Config_2 ---");
        } else {
            Log.e(TAG, "Unknown Config ID: " + sRequiredConfig);
        }
    }

    @Override
    public void onExecuteWrite(BluetoothDevice device, int requestId, boolean execute) {
        super.onExecuteWrite(device, requestId, execute);
        Log.d(TAG, "onExecuteWrite: "
                + "requestId: " + requestId
                + "execute: " + execute);
    }

    @Override
    public void onNotificationSent(BluetoothDevice device, int status) {
        super.onNotificationSent(device, status);
        Log.d(TAG, "onNotificationSent: " + status);
    }

    @Override
    public void onDescriptorReadRequest(
            BluetoothDevice device, int requestId, int offset, BluetoothGattDescriptor descriptor) {
        super.onDescriptorReadRequest(device, requestId, offset, descriptor);
        Log.d(TAG, "onDescriptorReadRequest - "
        + "requestId: " + requestId
        + ", "
        + "offset: " + offset);

        if(descriptor.getValue() != null) {
            FusionNetUtil.logPrintByteArray(TAG, descriptor.getValue());
        } else {
            Log.d(TAG, "descriptor value null");
        }

        mGattServer.sendResponse(device, requestId,
                BluetoothGatt.GATT_SUCCESS, offset, BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE);
    }

    @Override
    public void onDescriptorWriteRequest(
            BluetoothDevice device, int requestId, BluetoothGattDescriptor descriptor,
            boolean preparedWrite, boolean responseNeeded, int offset, byte[] value) {
        super.onDescriptorWriteRequest(device, requestId, descriptor, preparedWrite, responseNeeded, offset, value);

        Log.d(TAG, "onDescriptorWriteRequest"
                + "requestId: " + requestId
                + " offset: " + offset
        + " preparedWrite: " + preparedWrite
        + " responseNeeded: " + responseNeeded
        + " offset: " + offset);

        FusionNetUtil.logPrintByteArray(TAG, value);

        mGattServer.sendResponse(device, requestId,
                BluetoothGatt.GATT_SUCCESS, offset, value);

    }

    private static int sRequiredConfig = 0;

    @Override
    public void onCharacteristicWriteRequest(BluetoothDevice device, int requestId,
                                             BluetoothGattCharacteristic characteristic,
                                             boolean preparedWrite, boolean responseNeeded,
                                             int offset, byte[] value) {

        Log.d(TAG, "onCharacteristicWriteRequest:" + value.length);
        FusionNetUtil.logPrintByteArray(TAG, value);

        if(1 == value.length) {
            if(1 == value[0] || 2 == value[0]) {
                sRequiredConfig = 0xff & value[0];
                Log.d("MIN", "CMD = " + sRequiredConfig);
                if (responseNeeded) {
                    mGattServer.sendResponse(device, requestId,
                    BluetoothGatt.GATT_SUCCESS, offset, value);
                }
                return;
            } else if(3 == value[0]) {      //  Reset
            } else if(4 == value[0]) {      //  Reset Settings
            } else if(5 == value[0]) {      //  Delete Logs
                Log.d("MIN", "UNKNOWN COMMAND: " + value[0]);
            }
        }

        /* ----------------   For GrideyeDataChannel ----------------- */

        // 1. Get communication channel for this device.
        GrideyeDataChannel channel = grideyePacketReceiver.getDataChannel(device.getAddress());
        Log.d(TAG, "FusionNetDataChannel channel: " + channel);

        // 20170525 - howard -
        // 2. attach data to this channel.
        if(!channel.attach(value)) {
            Log.d(TAG, "channel attach failed!");
            // attach channel failed! clear channel and return.
            grideyePacketReceiver.clearChannel(device.getAddress());
            if (responseNeeded) {
                mGattServer.sendResponse(device, requestId,
                        BluetoothGatt.GATT_FAILURE, offset, value);
            }
            return;
        } else {
            Log.d(TAG, "channel attach successed!");
            if(channel.isReady()) {
                Log.d(TAG, "channel is Ready!");
                onMessageReceived(channel);
                grideyePacketReceiver.clearChannel(device.getAddress()); // clear packet
            }
            // 20170525 - howard
            if (responseNeeded) {
                mGattServer.sendResponse(device, requestId,
                        BluetoothGatt.GATT_SUCCESS, offset, value);
            }
        }


        /* ----------------   For FusionNetDataChannel ----------------- */

//        // 1. Get communication channel for this device.
//        FusionNetDataChannel channel = fusionNetPacketReceiver.getDataChannel(device.getAddress());
//        Log.d(TAG, "FusionNetDataChannel channel: " + channel);
//
//        // 2. attach data to this channel.
//        if(!channel.attach(value)) {
//            Log.d(TAG, "channel attach failed!");
//            // attach channel failed! clear channel and return.
//            fusionNetPacketReceiver.clearChannel(device.getAddress());
//            if (responseNeeded) {
//                mGattServer.sendResponse(device, requestId,
//                        BluetoothGatt.GATT_FAILURE, offset, value);
//            }
//            return;
//        }

//        Log.d(TAG, "Command Task: " + channel.getCmdTask());
//
//
//        switch (channel.getCmdTask()) {
//            // Send Message Command
//            case FusionNetConstants.CMD_TASK_SEND_MESSAGE:
//                if(channel.isReady()) {
//
//                    onMessageReceived(channel);
//                    fusionNetPacketReceiver.clearChannel(device.getAddress()); // clear packet
//                }
//                if (responseNeeded) {
//                    mGattServer.sendResponse(device, requestId,
//                            BluetoothGatt.GATT_SUCCESS, offset, value);
//                }
//                break;
//            // Feedback Confirm Command
//            case FusionNetConstants.CMD_TASK_FEEDBACK_CONFIRM:
//                if(channel.isReady()) {
//                    Log.d(TAG, "fusionNetCallbackSpec CMD_TASK_FEEDBACK_CONFIRM!");
//                    if (fusionNetCallbackSpec != null) {
//                        fusionNetCallbackSpec.onFeedbackConfirm(
//                                packetToMessage(channel.getFusionNetPacket()));
//                    }
//                    fusionNet.processNextFeedbackMessage();
//                    fusionNetPacketReceiver.clearChannel(device.getAddress()); // clear packet
//                }
//                break;
//            // Help Confirm Command
//            case FusionNetConstants.CMD_TASK_HELP_CONFIRM:
//                Log.d(TAG, "fusionNetCallbackSpec CMD_TASK_HELP_CONFIRM!");
//                if (fusionNetCallbackSpec != null) {
//                    fusionNetCallbackSpec.onHelpConfirm();
//                }
//                fusionNet.onHelpConfirm();
//                fusionNetPacketReceiver.clearChannel(device.getAddress()); // clear packet
//                break;
//            // Fusion Help, Feedback Clear
//            case FusionNetConstants.FUSION_CMD_CLEAR:
//                byte[] data = channel.getReceivedData();
//                switch(data[5]) {
//                    case 0x01: // feedback
//                        Log.d(TAG, "fusionNetCallbackSpec FUSION_CMD_CLEAR! (Feedback)");
//                        if (fusionNetCallbackSpec != null) {
//                            fusionNetCallbackSpec.onFeedbackConfirm(
//                                    packetToMessage(channel.getFusionNetPacket()));
//                        }
//                        fusionNet.processNextFeedbackMessage();
//                        fusionNetPacketReceiver.clearChannel(device.getAddress()); // clear packet
//                        break;
//                    case 0x02: // help
//                        Log.d(TAG, "fusionNetCallbackSpec FUSION_CMD_CLEAR! (HELP)");
//                        if (fusionNetCallbackSpec != null) {
//                            fusionNetCallbackSpec.onHelpConfirm();
//                        }
//                        fusionNet.onHelpConfirm();
//                        fusionNetPacketReceiver.clearChannel(device.getAddress()); // clear packet
//                        break;
//                }
//                break;
//        }

    }

    private void onMessageReceived(FusionNetDataChannel channel) {
        if(fusionNetCallbackSpec != null) {
            Log.d(TAG, "fusionNetCallbackSpec CMD_TASK_SEND_MESSAGE!");
            FusionNetMessage message = new FusionNetMessage();
            FusionNetPacketV2 packet = channel.getFusionNetPacket();
            message.setMessage(packet.getMessage());
            message.setMessageId(packet.getMsgID());
            message.setCommandId(packet.getCmdID());
            fusionNetCallbackSpec.onMessageReceived(message);
        }
    }

    private void onMessageReceived(GrideyeDataChannel channel) {
        if(fusionNetCallbackSpec != null) {
            Log.d(TAG, "fusionNetCallbackSpec CMD_TASK_SEND_MESSAGE!");
            FusionNetMessage message = new FusionNetMessage();
            FusionNetPacket_Grideye packet = channel.getGrideyePacket();
            message.setMessage(packet.getMessage());
            fusionNetCallbackSpec.onMessageReceived(message);
        }
    }

    //CrabCSChen@FIHTDC add for SMFT-1925, 2016/05/23 {
    private void onErrorOccur(int error_type){
        Log.d(TAG,"Error Occur!!! error type is: " + error_type );
        fusionNetCallbackSpec.onErrorOccur(error_type);
    }
    //CrabCSChen@FIHTDC add for SMFT-1925, 2016/05/23 }
    private FusionNetMessage packetToMessage(FusionNetPacketV2 fusionNetPacket) {
        FusionNetMessage message = new FusionNetMessage();
        message.setMessageId(fusionNetPacket.getMsgID());

        switch(fusionNetPacket.getCmdTask()) {
            case FusionNetConstants.CMD_TASK_SEND_MESSAGE:
                message.setMessage(fusionNetPacket.getMessage());
                break;
        }

        //message.setCommandId(fusionNetPacket.getCmdID());
        return message;
    }

    public void setFusionNetCallbackSpec(FusionNetCallback fusionNetCallbackSpec) {
        this.fusionNetCallbackSpec = fusionNetCallbackSpec;
    }
}
