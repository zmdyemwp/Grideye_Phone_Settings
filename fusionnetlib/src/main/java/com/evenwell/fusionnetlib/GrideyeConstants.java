package com.evenwell.fusionnetlib;

/**
 * Created by HowardJHLi on 2017/5/26.
 */

public class GrideyeConstants {

    /* MinSMChien - Add command id */
    // public static final int SETTING_BYTE_SIZE = 13;
    public static final int SETTING_BYTE_SIZE = 14;
    public static final int SETTING_BYTE_SIZE_2 = 12;
    /* End Add command id */
    public final static String GRIDEYE_PREFERENCE = "Grideye.SharedPreferences";

    public class settingsIndex{
        public static final int DIFF_TEMP_THRESH = 1;
        public static final int LABELING_THRESH	= 2;
        public static final int FNMV_THRESH = 3;
        public static final int EDGE_THRESH = 4;
        public static final int HUMAN_FRAME_THRESH = 5;
        public static final int BED_LEFT = 6;
        public static final int BED_RIGHT = 7;
        public static final int LEFT_RANGE = 8;
        public static final int RIGHT_RANGE = 9;
        public static final int TOP_RANGE = 10;
        public static final int BOTTOM_RANGE = 11;
    }
    public class settingsPrefIndex{
        public static final int DIFF_TEMP_THRESH1 = 1;
        public static final int DIFF_TEMP_THRESH2 = 2;
        public static final int LABELING_THRESH = 3;
        public static final int FNMV_THRESH = 4;
        public static final int EDGE_THRESH1 = 5;
        public static final int EDGE_THRESH2 = 6;
        public static final int HUMAN_FRAME_THRESH = 7;
        public static final int BED_LEFT = 8;
        public static final int BED_RIGHT = 9;
        public static final int LEFT_RANGE = 10;
        public static final int RIGHT_RANGE = 11;
        public static final int TOP_RANGE = 12;
        public static final int BOTTOM_RANGE = 13;
    }
    public class settingsPrefName{
        public static final String DIFF_TEMP_THRESH1 = "diffTempThresh1";
        public static final String DIFF_TEMP_THRESH2 = "diffTempThresh2";
        public static final String LABELING_THRESH = "labelingThresh";
        public static final String FNMV_THRESH = "fnmvThresh";
        public static final String EDGE_THRESH1 = "edgeThresh1";
        public static final String EDGE_THRESH2 = "edgeThresh2";
        public static final String HUMAN_FRAME_THRESH = "humanFrameThresh";
        public static final String BED_LEFT = "bedLeft";
        public static final String BED_RIGHT = "bedRight";
        public static final String LEFT_RANGE = "leftRange";
        public static final String RIGHT_RANGE = "rightRange";
        public static final String TOP_RANGE = "topRange";
        public static final String BOTTOM_RANGE = "bottomRange";

        /* MinSMChien - New Settings */
        public static final String IP_CAM_ADDR_1 = "IP_CAM_ADDR_1";
        public static final String IP_CAM_ADDR_2 = "IP_CAM_ADDR_2";
        public static final String IP_CAM_ADDR_3 = "IP_CAM_ADDR_3";
        public static final String IP_CAM_ADDR_4 = "IP_CAM_ADDR_4";
        public static final String FTP_IP_ADDR_1 = "FTP_IP_ADDR_1";
        public static final String FTP_IP_ADDR_2 = "FTP_IP_ADDR_2";
        public static final String FTP_IP_ADDR_3 = "FTP_IP_ADDR_3";
        public static final String FTP_IP_ADDR_4 = "FTP_IP_ADDR_4";

        public static final String RESET = "RESET";
        public static final String RESET_SETTINGS = "RESET_SETTINGS";
        public static final String DELETE_LOGS = "DELETE_LOGS";
        /* End New Settings */
    }

    public class settingPrefDefault{
        public static final int DEFAULT_DIFF_TEMP_THRESH1 = 0;
        public static final int DEFAULT_DIFF_TEMP_THRESH2 = 50;
        public static final int DEFAULT_LABELING_THRESH	= 6;
        public static final int DEFAULT_FNMV_THRESH = -1;
        public static final int DEFAULT_EDGE_THRESH1 = 2;
        public static final int DEFAULT_EDGE_THRESH2 = 88;
        public static final int DEFAULT_HUMAN_FRAME_THRESH = 20;
        public static final int DEFAULT_BED_LEFT = 10;
        public static final int DEFAULT_BED_RIGHT = 4;
        public static final int DEFAULT_LEFT_RANGE = 11;
        public static final int DEFAULT_RIGHT_RANGE = 3;
        public static final int DEFAULT_TOP_RANGE = 12;
        public static final int DEFAULT_BOTTOM_RANGE = 2;
    }

    public class settingPrefDefault2{
        // IP Camera default: 10.57.45.3
        public static final int DEFAULT_IP_CAM_ADDR_1 = 10;
        public static final int DEFAULT_IP_CAM_ADDR_2 = 57;
        public static final int DEFAULT_IP_CAM_ADDR_3 = 45;
        public static final int DEFAULT_IP_CAM_ADDR_4 = 3;
        // FTP default: 10.57.54.254
        public static final int DEFAULT_FTP_ADDR_1 = 10;
        public static final int DEFAULT_FTP_ADDR_2 = 57;
        public static final int DEFAULT_FTP_ADDR_3 = 54;
        public static final int DEFAULT_FTP_ADDR_4 = 254;
        // NOT USED
        public static final int DEFAULT_RESET = 0;
        public static final int DEFAULT_RESET_SETTINGS = 0;
        public static final int DEFAULT_DELETE_LOGS = 0;
    }


}
