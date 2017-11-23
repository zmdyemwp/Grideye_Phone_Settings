package com.evenwell.fusionnetlib;

import android.util.Log;

/**
 * Created by KuroCHChung on 2016/1/13.
 */
/* package */ class FusionNetUtil {

    private static final String TAG = FusionNetUtil.class.getSimpleName();

    private final static String[] hashMap = new String[] {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
        "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
        "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d",
        "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
        "y", "z"
    };

    public static String encodeIMEI(String imei) {
        long l = Long.parseLong(imei);
        StringBuilder sb = new StringBuilder(7);
        for (int i = 6; i >= 0; i--) {
            byte b = (byte)(l & 0xFF);
            int m = ((int) b & 0xFF) % 62;
            sb.append(hashMap[m]);
            l >>= 8;
        }
        return sb.toString();
    }

    public static byte[] concatByteArray(byte[] a, byte[] b) {
        byte[] c = new byte[a.length + b.length];
        System.arraycopy(a, 0, c, 0, a.length);
        System.arraycopy(b, 0, c, a.length, b.length);
        return c;
    }

    public static void logPrintByteArray(String TAG, byte[] data) {
        final StringBuilder stringBuilder = new StringBuilder(data.length);
        for(byte byteChar : data) {
            stringBuilder.append(String.format("%02X ", byteChar));
        }
        Log.d(TAG, stringBuilder.toString());
    }

    public static void printByteArray(byte[] data) {
        final StringBuilder stringBuilder = new StringBuilder(data.length);
        for(byte byteChar : data) {
            stringBuilder.append(String.format("%02X ", byteChar));
        }
        System.out.println(stringBuilder.toString());
    }

    public static byte[] hexStringToBytes(String hexString) {

        char[] hex = hexString.toCharArray();
        // 2 Characters is 1 byte
        int length = hex.length / 2;
        byte[] rawData = new byte[length];
        for (int i = 0; i < length; i++) {
            // hex to dec
            int high = Character.digit(hex[i * 2], 16);
            int low = Character.digit(hex[i * 2 + 1], 16);
            // for example: String 4C
            // High 4 => (0000 0100)
            // Low C => 12 => (0000 1100)
            // shift the first bit << 4, ex: 00000100 => 01000000 (4=>64)
            // then OR with second bits, ex: 10000000 | 00001100 => 01001100 (76)
            int value = (high << 4) | low;
            // complement 0xFFFFFFFF
            if (value > 127)
                value -= 256;

            // force to bytes
            rawData [i] = (byte) value;
        }
        return rawData ;
    }

}
