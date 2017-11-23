package com.evenwell.grideye;

import android.util.Log;

/**
 * Created by CrabCSChen on 2016/7/5.
 */
public class PacketManager {
    StringBuffer mStringbuffer;
    boolean bStart;
    public PacketManager(){
        mStringbuffer = new StringBuffer();
        bStart = false;
    }
    public void savePacket(String packet){
        mStringbuffer.append(packet);
    }
    public StringBuffer getPacket(){
        return mStringbuffer;
    }
    public int getStringBufferLength(){
        return mStringbuffer.length();
    }
    public boolean getStart(){
        return bStart;
    }
    public void setStart(boolean value){
        bStart = value;
    }
    public void cleanBuferr(){
        mStringbuffer.setLength(0);
    }

    public void printResult(){
        Log.i("Crab","PacketManager: " + mStringbuffer.toString());
    }
}
