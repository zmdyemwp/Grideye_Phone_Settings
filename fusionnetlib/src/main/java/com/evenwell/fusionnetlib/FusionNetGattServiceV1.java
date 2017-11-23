package com.evenwell.fusionnetlib;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;

import java.util.UUID;

/**
 * Created by KuroCHChung on 2016/2/18.
 * for SMFT
 */
/* package */ class FusionNetGattServiceV1 extends BluetoothGattService {

    public FusionNetGattServiceV1() {

        super(UUID.fromString(FusionNetConstants.SERVICE_CONTROL_UUID),
                BluetoothGattService.SERVICE_TYPE_PRIMARY);

        BluetoothGattCharacteristic cs_control = new BluetoothGattCharacteristic(
                UUID.fromString(FusionNetConstants.CHAR_GENERAL_CONTROL),
                BluetoothGattCharacteristic.PROPERTY_READ |
                        BluetoothGattCharacteristic.PROPERTY_WRITE,
                BluetoothGattCharacteristic.PERMISSION_READ |
                        BluetoothGattCharacteristic.PERMISSION_WRITE
        );
        addCharacteristic(cs_control);

        /* Cancel Control */
        BluetoothGattCharacteristic cs_cancel = new BluetoothGattCharacteristic(
                UUID.fromString(FusionNetConstants.CHAR_CANCEL_CONTROL),
                BluetoothGattCharacteristic.PROPERTY_READ |
                        BluetoothGattCharacteristic.PROPERTY_WRITE,
                BluetoothGattCharacteristic.PERMISSION_READ |
                        BluetoothGattCharacteristic.PERMISSION_WRITE
        );
        addCharacteristic(cs_cancel);

        /* ACK */
        BluetoothGattCharacteristic cs_ack = new BluetoothGattCharacteristic(
                UUID.fromString(FusionNetConstants.CHAR_ACK),
                BluetoothGattCharacteristic.PROPERTY_READ |
                        BluetoothGattCharacteristic.PROPERTY_WRITE,
                BluetoothGattCharacteristic.PERMISSION_READ |
                        BluetoothGattCharacteristic.PERMISSION_WRITE
        );
        addCharacteristic(cs_ack);

        /* ACK */
        BluetoothGattCharacteristic cs_read = new BluetoothGattCharacteristic(
                UUID.fromString(FusionNetConstants.CHAR_READ),
                BluetoothGattCharacteristic.PROPERTY_READ |
                        BluetoothGattCharacteristic.PROPERTY_WRITE,
                BluetoothGattCharacteristic.PERMISSION_READ |
                        BluetoothGattCharacteristic.PERMISSION_WRITE
        );
        addCharacteristic(cs_read);
    }
}
