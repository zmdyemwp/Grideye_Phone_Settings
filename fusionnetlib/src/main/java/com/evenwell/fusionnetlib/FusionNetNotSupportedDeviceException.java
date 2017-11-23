package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/2/27.
 */
public class FusionNetNotSupportedDeviceException extends RuntimeException {
    public FusionNetNotSupportedDeviceException() {
        super("Your device is not support by FusionNet Service!");
    }
}
