package com.evenwell.grideye.Peripheral.util;

/**
 * Created by KashKHYang on 2016/4/27.
 */
public class FusionConst {
    public static boolean DBG = false;

    //shared preferences related
    public static class Prefs {
        public static final String NAME = "prefs_fusionband_simulator";
        public static final String KEY_SERVICE_STATE = "key_service_state";
        public static final String KEY_MSG_TIME = "key_msg_time";
        public static final String KEY_MSG_CONTENT = "key_msg_content";
        public static final String KEY_MSG_ID = "key_msg_id";
        public static final String KEY_MSG_COMMAND_ID = "key_msg_command_id";
        public static final String KEY_MSG_NOT_FEEDBACK = "key_msg_not_feedback";
        public static final String KEY_ENG_ENABLE_FEEDBACK = "key_eng_enable_feedback";
    }

    public static class Notification {
        public static final int ID_MSG = 0x01;
        public static final int ID_ONGOING = 0x02;
    }

    public static class PermissionConst {
        public static final int RESULT_CODE = 1013;
        public static final String EXTRA_KEY_PERMISSIONS = "extra_key_permissions";
    }

    public static class CommandType {
        public static final int TYPE_RECEIVE_MSG = 1;
        public static final int TYPE_RECEIVE_FEEDBACK_CONFIRM = 2;
    }
}
