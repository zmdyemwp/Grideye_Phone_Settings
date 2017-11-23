package com.evenwell.fusionnetlib;

import java.io.Serializable;

/**
 * Created by KuroCHChung on 2016/1/14.
 */
public class FusionNetMessage implements Serializable {

    private int messageId;
    private int commandId;
    private String message;
    private byte[] messageByte;

    public String getMessage() {
        return message;
    }

    public byte[] getMessageByte() {
        return messageByte;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public void setMessage(byte[] message) {
        this.messageByte = message;
    }

    public int getMessageId() {
        return messageId;
    }

    public void setMessageId(int messageId) {
        this.messageId = messageId;
    }

    public int getCommandId() {
        return commandId;
    }

    public void setCommandId(int commandId) {
        this.commandId = commandId;
    }
}
