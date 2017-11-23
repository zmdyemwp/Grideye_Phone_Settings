package com.evenwell.grideye;

import android.util.Log;

import com.google.common.primitives.Bytes;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
/**
 * Created by CrabCSChen on 2016/7/5.
 */
public class Utils {
    public static final String packetStartSymbol = "2A2A2A";
    public static final String packetEndSymbol = "0D0A";
    public static int START_SYMBOL_SIZE =  packetStartSymbol.length();
    public static int END_SYMBOL_SIZE =  packetEndSymbol.length();
    public static int DEBUG_SYMBOL_SIZE = 2;
    public static int THS_DATA_SIZE = 4;
    public static int RAW_DATA_DATA_SIZE = 64*2;
    public static int TRACKING_DATA_SIZE = 55*2;
    public static int ALARM_DATA_SIZE = 1*2;
    public static int CHECK_SETTING_SIZE = 3*2;
    public static int RAW_DATA_SIZE = START_SYMBOL_SIZE + THS_DATA_SIZE + RAW_DATA_DATA_SIZE + END_SYMBOL_SIZE;
    public static int DETECTION_DATA_SIZE = (11+225)*2;
    public static int FULL_DATA_SIZE = RAW_DATA_SIZE + DETECTION_DATA_SIZE + DEBUG_SYMBOL_SIZE;


    public static final int BED_MODE = 0;
    public static final int BATH_MODE = 1;





    public final static String IMG_DIRECTORY_NAME = "Viewer";


    public static class callBackMsg{
        public static final int DATA_CHANGE = 0;
        public static final int HUMAN_DETECT = 1;
        public static final int HUMAN_AREA = 2;
        public static final int HUMAN_NUM = 3;
        public static final int DETECT_NUM = 4;
        public static final int BACK_TEMP_STATE = 5;
        public static final int CENTER_X = 6;
        public static final int CENTER_Y = 7;
        public static final int ALERT_STATUS = 8;
        public static final int DETAIL_STATUS = 10;
        public static final int DETAIL_STATUS_CODE = 11;
        public static final int HUMAN_OUTPUT_IMG_DATA = 99;
        public static final int MAX_X = 12;
        public static final int MAX_Y = 13;
        public static final int MIN_X = 14;
        public static final int MIN_Y = 15;
    }


    public static class alert_status{
        public static final int EMPTY = -1;
        public static final int STANDBY = 0;
        public static final int MONITOR = 1;
        public static final int SAFE = 2;
        public static final int ALERT = 3;
    }


    public static class detect_status{
        public static final int BACK_TEMP_READY = 1;
        public static final int BACK_TEMP_NOT_READY = 0;
    }

    public static String processStartPackage(String packet) {
        String result = packet.substring(packet.indexOf(packetStartSymbol));
        return result;
    }

    public static String processEndPackage(String packet) {
        String result = packet.substring(0, packet.indexOf(packetEndSymbol)+packetEndSymbol.length());
        return result;
    }

    public static byte[] printResult(StringBuffer data){
//        Log.i("Crab","row data is: " + data.toString());
        //remove start symbol
        String dataString = data.toString().substring(packetStartSymbol.length() + 4);
        //remove end symbol
        dataString = dataString.substring(0, dataString.indexOf(packetEndSymbol));
        List<String> items = new ArrayList<>();
        for(int k=0; k<dataString.length(); k+=2){
            items.add(dataString.substring(k,k+2));
        }
        int i = 1;
        //transfer item list into bytearray
        /*byte[] resultbyte_1;
        Byte[] resultbyte_2 = items.toArray(new Byte[items.size()]);
        resultbyte_1 = toPrimitive(resultbyte_2);*/

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);
        for (String element : items) {
            try {
                out.writeUTF(element);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        byte[] bytes = baos.toByteArray();

        //
        String temp = "";
        for(int j=items.size()-1; j>0; j--){
            temp = temp + "-" + (Integer.parseInt(items.get(j).trim(), 16) * 0.25);
            if((i % 8) == 0) {
                Log.i("Crab", "result: " + temp);
                temp = "";
            }
            i++;
        }
        return bytes;
    }

    public static byte[] toPrimitive(Byte[] array)
    {
        if (array == null)
            return null;
        if (array.length == 0) {
            return new byte[0];
        }
        byte[] result = new byte[array.length];
        for (int i = 0; i < array.length; i++) {
            result[i] = array[i].byteValue();
        }
        return result;
    }

    public static boolean isNumeric(String str)
    {
        Pattern pattern = Pattern.compile("[0-9]*");
        Matcher isNum = pattern.matcher(str);

        if( !isNum.matches() )
        {
            return false;
        }
        return true;

    }

    public static int contains(byte[] data, byte[] target){
        return Bytes.indexOf(data, target);
    }
}

