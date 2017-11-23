package com.evenwell.fusionnetlib;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;

import java.util.UUID;

/**
 * Created by KuroCHChung on 2016/2/18.
 */
public class FusionNetGattServiceV2 extends BluetoothGattService {

    public FusionNetGattServiceV2() {
        super(UUID.fromString(FusionNetConstants.FusionService.UUID),
                BluetoothGattService.SERVICE_TYPE_PRIMARY);

        BluetoothGattCharacteristic characteristic1 = new BluetoothGattCharacteristic(
                UUID.fromString(FusionNetConstants.FusionService.CHARACTERISTIC_1),
                BluetoothGattCharacteristic.PROPERTY_NOTIFY |
                        BluetoothGattCharacteristic.PROPERTY_READ|
                        BluetoothGattCharacteristic.PROPERTY_WRITE,
                BluetoothGattCharacteristic.PERMISSION_READ |
                        BluetoothGattCharacteristic.PERMISSION_WRITE
        );
        BluetoothGattDescriptor descriptor1 = new BluetoothGattDescriptor(
                UUID.fromString(FusionNetConstants.CLIENT_CHARACTERISTIC_CONFIG),
                BluetoothGattDescriptor.PERMISSION_READ | BluetoothGattDescriptor.PERMISSION_WRITE);
        characteristic1.addDescriptor(descriptor1);
        addCharacteristic(characteristic1);


        BluetoothGattCharacteristic characteristic2 = new BluetoothGattCharacteristic(
                UUID.fromString(FusionNetConstants.FusionService.CHARACTERISTIC_2),
                        BluetoothGattCharacteristic.PROPERTY_NOTIFY,
                BluetoothGattCharacteristic.PERMISSION_READ
        );
        BluetoothGattDescriptor descriptor2 = new BluetoothGattDescriptor(
                UUID.fromString(FusionNetConstants.CLIENT_CHARACTERISTIC_CONFIG),
                BluetoothGattDescriptor.PERMISSION_READ | BluetoothGattDescriptor.PERMISSION_WRITE);
        characteristic2.addDescriptor(descriptor2);
        addCharacteristic(characteristic2);
    }
}
