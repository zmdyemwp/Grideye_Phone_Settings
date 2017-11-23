package com.evenwell.grideye;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * Created by CrabChen on 7/11/16.
 */
public class PacketManagerTwo {
    public ByteArrayOutputStream byteArrayOutputStream = null;
    public byte[] ret = null;
    public PacketManagerTwo(){
        //byteArrayOutputStream = new ByteArrayOutputStream();
    }
    public void addByteArrayToStream(byte[] data) throws IOException{
        //byteArrayOutputStream.write(data);
        if(ret == null){
            ret = new byte[data.length];
            copy(data, ret);
        }else {
            byte[] temp = new byte[ret.length];
            copy(ret, temp);
            ret = new byte[temp.length + data.length];
            int pos = 0;
            for(byte b: temp){
                ret[pos] = b;
                pos++;
            }
            for(byte b: data){
                ret[pos] = b;
                pos++;
            }
        }
    }
    public byte[] getByteArrayFromStream(){
        //return byteArrayOutputStream.toByteArray();
        return ret;
    }
    public void resetOutputStream(){
        ret = null;
        byteArrayOutputStream = null;
        byteArrayOutputStream = new ByteArrayOutputStream();
    }

    private void copy(byte[] from, byte[] to){
        if(from.length == to.length){
            for(int i = 0; i < to.length; i++){
                //do the copy
                to[i] = from[i];
            }
        }
    }

}
