package com.evenwell.grideye;

import android.util.Log;

import java.io.FileInputStream;

/**
 * Created by HowardJHLi on 2017/5/4.
 */

public class VirtualFile {
    private static final String TAG = "VirtualFile";
    private static final int BUFFER_SIZE = 256;

    public static String getStringValueByFile(String file){
        if(file == null) return null;
        try {
            String ret = readFile(file, '\0');
            if(ret != null){
                //if(PwlConst.DEBUG.DBG) Log.i(TAG, "[getStringValueByFile] [" +file+" = " +ret+"]" );
                return ret.trim();
            }
            //if(PwlConst.DEBUG.DBG) Log.i(TAG, "[getStringValueByFile] [" +file+" = null]" );
            Log.d(TAG,"[howardjhli] readFlie is null!!" );
            return null;
        } catch (Exception e) {
            e.printStackTrace();
            Log.e(TAG,"[GetStatus] parse fail, " + file + " = null" );
            return null;
        }
    }

    public static String readFile(String file, char endChar) {
        //if(!canReadFile(file)) return null;
        // Permit disk reads here, as /proc/meminfo isn't really "on
        // disk" and should be fast.  TODO: make BlockGuard ignore
        // /proc/ and /sys/ files perhaps?
        FileInputStream is = null;
        try {
            is = new FileInputStream(file);
            byte[] mBuffer = new byte[BUFFER_SIZE];
            int len = is.read(mBuffer);
            is.close();

            if (len > 0) {
                int i;
                for (i=0; i<len; i++) {
                    if (mBuffer[i] == endChar) {
                        break;
                    }
                }
                return new String(mBuffer, 0, i);
            }
        } catch (java.io.FileNotFoundException e) {
            //e.printStackTrace();
        } catch (java.io.IOException e) {
            //e.printStackTrace();
        } finally {
            if (is != null) {
                try {
                    is.close();
                } catch (java.io.IOException e) {
                    //e.printStackTrace();
                }
            }
        }
        return null;
    }

}




