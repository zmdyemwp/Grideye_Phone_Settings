package com.evenwell.fusionnetlib;

import java.nio.ByteBuffer;

/**
 * Created by KuroCHChung on 2016/1/21.
 */
/* package */ class ByteUtil {

    public static byte[] longToByteArray(long imeiL) {
        ByteBuffer buffer = ByteBuffer.allocate(8);
        buffer.putLong(imeiL);
        return buffer.array();
    }

    public static int convertTwoBytesToPositiveValue(byte b1, byte b2) {
        int val = convertTwoBytesToInt(b1, b2);
        return val >= 0 ? val : convertTwoBytesToUnsignedInt(b1, b2);
    }

    public static long convertFourBytesToPositiveValue(byte b1, byte b2, byte b3, byte b4) {
        long val = convertFourBytesToInt(b1, b2, b3, b4);
        return val >= 0 ? val : convertFourBytesToUnsignedInt(b1, b2, b3, b4);
    }

    public static int convertTwoBytesToInt (byte b1, byte b2)      // signed
    {
        return (b2 << 8) | (b1 & 0xFF);
    }

    public static int convertFourBytesToInt (byte b1, byte b2, byte b3, byte b4)
    {
        return (b4 << 24) | (b3 & 0xFF) << 16 | (b2 & 0xFF) << 8 | (b1 & 0xFF);
    }

    public static int convertTwoBytesToUnsignedInt (byte b1, byte b2)      // unsigned
    {
        return (b2 & 0xFF) << 8 | (b1 & 0xFF);
    }

    public static long convertFourBytesToUnsignedInt (byte b1, byte b2, byte b3, byte b4)
    {
        return (long) (b4 & 0xFF) << 24 | (b3 & 0xFF) << 16 | (b2 & 0xFF) << 8 | (b1 & 0xFF);
    }
}
