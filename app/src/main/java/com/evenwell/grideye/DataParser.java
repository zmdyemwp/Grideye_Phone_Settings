package com.evenwell.grideye;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Created by HowardJHLi on 2017/5/2.
 */

public class DataParser {

    public static short[] parseByteArrayToShortArray(byte[] bytes){
        short[] shorts = new short[bytes.length/2];
        // to turn bytes to shorts as either big endian or little endian.
        ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).asShortBuffer().get(shorts);
        return shorts;
    }

    public static short[] parseStringToShortArray(String str){
        short[] shorts = new short[str.length()/2];

        String[] strs = new String[str.length()/2];
        int j = 0;
        for (int i = 0; i < str.length(); i =i+2) {
            try {
                shorts[j] = Short.valueOf(str.substring(i, i + 2), 16);
            } catch (Exception ex){shorts[j] = 0;}
            j++ ;
        }

        return shorts;
    }

    public static String filterSpliterZero(String str){
        String[] filterStrs = new String[str.length()/4];
        int j = 0;
        for (int i = 0; i < str.length(); i =i+2) {
            if (i%4 == 0){
                filterStrs[j] = str.substring(i, i+2);
                j++ ;
            }
        }

        StringBuilder builder = new StringBuilder();
        for(String s : filterStrs) {
            builder.append(s);
        }
        return builder.toString();
    }


    public static String hexToDecString(String hexStr){

        String result = "";

        if (hexStr != null) {

            if (hexStr.length() > 4) {
                String[] decStrs = new String[(hexStr.length() + 1) / 4];

                int j = 0;
                for (int i = 0; i < hexStr.length(); i = i + 4) {

                    String shexValue = hexStr.substring(i + 1, i + 3);

                    //int hexValue = Integer.parseInt(hexStr.substring(i, i+2));

                    //int idecValue = Integer.parseInt(shexValue, 16);
                    //int idecValue = Integer.parseInt(shexValue);

                    //decStrs[j] = Integer.toString(idecValue);
                    decStrs[j] = shexValue;

                    j++;

                }

                StringBuilder builder = new StringBuilder();
                for (String s : decStrs) {
                    builder.append(s);
                }

                result = builder.toString();
                //return builder.toString();
            }
        }
        return result;
    }

}
