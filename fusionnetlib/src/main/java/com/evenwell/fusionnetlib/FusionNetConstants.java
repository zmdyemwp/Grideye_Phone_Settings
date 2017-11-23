package com.evenwell.fusionnetlib;

/**
 * Created by KuroCHChung on 2016/1/13.
 */
/* package */ class FusionNetConstants {

    public static String SERVICE_CONTROL_UUID = "00002B00-0000-1000-8000-00805f9b34fb";
    public static String CHAR_GENERAL_CONTROL = "00002B01-0000-1000-8000-00805f9b34fb";
    public static String CHAR_CANCEL_CONTROL = "00002B02-0000-1000-8000-00805f9b34fb";
    public static String CHAR_ACK = "00002B03-0000-1000-8000-00805f9b34fb";
    public static String CHAR_READ = "00002B04-0000-1000-8000-00805f9b34fb";
    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";

    public class FusionService {
        public static final String UUID = "46494854-4443-5365-7276-696365020000";
        public static final String CHARACTERISTIC_1 = "46494854-4443-5365-7276-696365020001";
        public static final String CHARACTERISTIC_2 = "46494854-4443-5365-7276-696365020002";
    }

    public static final int CMD_TASK_SET_NAME = 0x41;
    public static final int CMD_TASK_SEND_MESSAGE = 0x61;
    public static final int CMD_TASK_FEEDBACK_CONFIRM = 0x65;
    public static final int CMD_TASK_HELP_CONFIRM = 0x66;
    public static final int CMD_TASK_DELETE_MESSAGE = 0x09;

    public static final int FUSION_CMD_CLEAR = 0x72;


    public final static String HELP_COUNTER_KEY = "HelpCounter";
    public final static String FUSION_PREFERENCE = "FusionNet.SharedPreferences";
    //CrabCSChen@FIHTDC add for SMFT-1925, 2016/05/23 {
    public final static int ERROR_WHEN_START_SERVICE = 901;
    public final static int ERROR_WHEN_STOP_SERVICE = 902;
    //CrabCSChen@FIHTDC add for SMFT-1925, 2016/05/23 }
}