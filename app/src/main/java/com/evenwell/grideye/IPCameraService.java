package com.evenwell.grideye;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.IOException;
import java.io.InputStream;
import java.net.Authenticator;
import java.net.PasswordAuthentication;
import java.net.URL;
import java.net.URLConnection;

/**
 * Created by HowardJHLi on 2017/5/16.
 */

public class IPCameraService {
    //private static final String IPCamAddr = "http://10.57.45.172/Image.jpg";
    private static final String IPCamAddr = "http://10.57.45.3/Image.jpg";
    public static Bitmap sOutputImg;
    public static int sIpcamState = 0;

    public static void startToGetIpCameraImage(){
        try {

            Thread IPCamThread = new Thread(new Runnable() {
                public void run() {
                    while(true) {
                        try {
                            sOutputImg = GetURLBitmap(new URL(IPCamAddr));
                        } catch (Exception e) {
                            sIpcamState = 0;
                            return;
                        }


//                        if (outputImg != null) {
//                            runOnUiThread(new Runnable() {
//                                public void run() {
//                                    synchronized (mLock) {
//                                        myImageView.setImageBitmap(outputImg);
//                                    }
//                                }
//                            });
//                        }
                    }
                }
            });

            IPCamThread.start();
            /*
            try {
                t.join();
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            */
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    //static URLConnection conn;
    private static Bitmap GetURLBitmap(URL url)
    {
        try
        {
            //if (conn == null) {
            URLConnection conn = url.openConnection();

            conn.connect();

            //String userpass = "admin" + ":" + "000000";
            //String basicAuth = "Basic " + new String(new Base64().encode(userpass.getBytes()));
            //conn.setRequestProperty ("Authorization", basicAuth);

            Authenticator.setDefault(new Authenticator() {
                protected PasswordAuthentication getPasswordAuthentication() {
                    return new PasswordAuthentication("admin", "000000".toCharArray());
                }
            });
            //}

            InputStream isCover = conn.getInputStream();
            Bitmap bmpCover = BitmapFactory.decodeStream(isCover);
            isCover.close();
            sIpcamState = 1;
            return bmpCover;
        }
        catch (IOException e)
        {
            sIpcamState = 0;
            e.printStackTrace();
            return null;
        }
    }

}
