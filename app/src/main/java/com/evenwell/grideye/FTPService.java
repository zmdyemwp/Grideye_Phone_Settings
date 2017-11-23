package com.evenwell.grideye;

/**
 * Created by HowardJHLi on 2017/5/4.
 */

import android.os.AsyncTask;
import android.util.Log;

import org.apache.commons.net.ftp.*;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.SocketException;

import android.content.Context;

import android.content.SharedPreferences;
import com.evenwell.fusionnetlib.GrideyeConstants;


public class FTPService extends AsyncTask<Void, Void, FTPClient>{

    private static final String TAG = "FTPService";
    private static String ServerIP = "10.57.54.254";
    private static String FTPAccount = "fih";
    private static String FTPPassword = "Foxconn123";

    public static int sFtpConnectState = 0;
    public String localTextFilePath;
    public String localImgFilePath;
    public String localZipFilePath;
    public String workDirectory;
    public String resFileName;

    private FTPClient ftpClient;

    private Context mContext;


    private boolean connect(){
        if (ftpClient == null) ftpClient = new FTPClient();
        //ftpClient.connect(InetAddress.getByName(server));
        try {
            try {
                if (!ftpClient.isConnected()) {
                String ip_addr = getFTPIPAddr();
                ftpClient.connect(InetAddress.getByName(ip_addr));
                // ftpClient.connect(InetAddress.getByName(ServerIP));
            }
            }catch (SocketException se){
                Log.d(TAG, se.getStackTrace().toString());
            }
            // Get FTP server reply to check connect successful
            int reply = ftpClient.getReplyCode();
            boolean isFTPConnect = FTPReply.isPositiveCompletion(reply);
            if (!isFTPConnect){
                Log.d(TAG, "FTP Server connect failed");
                return false;
            }

            // Login!
            if (ftpClient.login(FTPAccount, FTPPassword)){

                // Set file type
                ftpClient.setFileType(FTP.BINARY_FILE_TYPE);

                // Transfer file can choose passitive or active
                ftpClient.enterLocalPassiveMode();

                // Make working directory
                ftpClient.makeDirectory(workDirectory);

                // change directory to working directory path
                ftpClient.changeWorkingDirectory(workDirectory);

                // add directory for saving image
                ftpClient.makeDirectory(Utils.IMG_DIRECTORY_NAME);


                // change directory to up layer directory
                //ftpClient.changeToParentDirectory();


                return true;
            } else {
                Log.d(TAG, "FTP Server login failed");
            }


        } catch (IOException e) {
            e.printStackTrace();
        }

        return false;
    }


    private void close(){
        try{
            if (ftpClient.isConnected()) ftpClient.logout();
            ftpClient.disconnect();
            ftpClient = null;
        } catch (IOException ex){
            ex.printStackTrace();
        }
    }

    private static final Object mLock = new Object();

    String getFTPIPAddr() {
        // "10.57.54.254"
        String result;
        SharedPreferences sharedPref = mContext.getSharedPreferences(
                GrideyeConstants.GRIDEYE_PREFERENCE, Context.MODE_PRIVATE);
        result = String.format("%s.%s.%s.%s", 
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_1, "10"),
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_2, "57"),
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_3, "54"),
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_4, "254"));
        return result;
    }
    static String getFTPIPAddr(Context context) {
        String result;
        SharedPreferences sharedPref = context.getSharedPreferences(
                GrideyeConstants.GRIDEYE_PREFERENCE, Context.MODE_PRIVATE);
        result = String.format("%s.%s.%s.%s", 
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_1, "10"),
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_2, "57"),
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_3, "54"),
        sharedPref.getString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_4, "254"));
        return result;
    }

    public static boolean tryConnect(final Context context){
        Thread connThread = new Thread(new Runnable() {
            public void run() {

                FTPClient ftpClient = new FTPClient();
                synchronized (mLock) {
                    try {

                        sFtpConnectState = 0;

                        try {
                            if (!ftpClient.isConnected()) {
                                String ip_address = getFTPIPAddr(context);
                                Log.d(TAG, "FTP address: " + ip_address);
                                // ftpClient.connect(InetAddress.getByName(ip_address));
                                ftpClient.connect(InetAddress.getByName(ServerIP));
                            }
                        } catch (SocketException se) {
                            Log.d(TAG, se.getStackTrace().toString());
                        }
                        // Get FTP server reply to check connect successful
                        int reply = ftpClient.getReplyCode();
                        boolean isFTPConnect = FTPReply.isPositiveCompletion(reply);
                        if (!isFTPConnect) {
                            Log.d(TAG, "FTP Server connect failed");
                        } else {
                            sFtpConnectState = 1;
                        }

                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                    // close the ftp connect
                    try {
                        if (ftpClient.isConnected()) ftpClient.logout();
                        ftpClient.disconnect();
                        ftpClient = null;
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            }
        });

        connThread.start();

        return true;
    }


    public void setFTPClient(String localTextFilePath, String localImgFilePath, String workDirectory, String resFileName){
        this.localTextFilePath = localTextFilePath;
        this.localImgFilePath = localImgFilePath;
        this.workDirectory = workDirectory;
        this.resFileName = resFileName;
    }
    public void setFTPClient(String localZipFilePath, String sFileName, Context context){
        this.localZipFilePath = localZipFilePath;
        this.resFileName = sFileName;
        this.mContext = context;
    }


    public void uploadFile() throws IOException {
        boolean isUpload = false;
        BufferedInputStream buffInput = null;

        if (ftpClient != null) {
            if (ftpClient.isConnected()){
                try {
        //          /* Upload text file to FTP Server */
        //          buffInput = new BufferedInputStream(new FileInputStream(localTextFilePath));
        //          isUpload = ftpClient.storeFile(resFileName, buffInput);
        //
        //          /* Upload img file to FTP Server */
        //          buffInput = new BufferedInputStream(new FileInputStream(localImgFilePath));
        //          isUpload = ftpClient.storeFile(resFileName, buffInput);

                    /* Upload zip file to FTP Server */
                    buffInput = new BufferedInputStream(new FileInputStream( localZipFilePath));
                    isUpload = ftpClient.storeFile(resFileName + ".zip", buffInput);

                    buffInput.close();
                } catch (SocketException se){
                    se.printStackTrace();
                }

            } else {
                Log.d(TAG, "FTP Server not connected");
            }

        } else {
            Log.d(TAG, "FTP Client not exist");
            return;
        }

        /* check current zip file is upload successed */
        if (isUpload){
            /* re upload failed upload files */
            if (IOFileToInternalStorage.missFileNames.size() > 0){
                uploadMissFile();
            }
        } else{
            // add to upload failed file list
            IOFileToInternalStorage.missFileNames.add(resFileName);

            if (IOFileToInternalStorage.missFileNames.size() > IOFileToInternalStorage.getFileNumber(mContext)){
                for (int i = 0; i < IOFileToInternalStorage.getFileNumber(mContext); i++) {
                    IOFileToInternalStorage.missFileNames.set(i, IOFileToInternalStorage.missFileNames.get(i+1));
                }
                IOFileToInternalStorage.missFileNames.remove(IOFileToInternalStorage.missFileNames.size()-1);
            }

            Log.d(TAG, "upload file to FTP failed, miss file size: " + IOFileToInternalStorage.missFileNames.size());

        }

    }

    private boolean uploadMissFile(){
        BufferedInputStream buffInput = null;
        for (int i = 0; i < IOFileToInternalStorage.missFileNames.size(); i++){
            try {
                buffInput = new BufferedInputStream(new FileInputStream( localZipFilePath));
                if (!ftpClient.storeFile(IOFileToInternalStorage.missFileNames.get(i) + ".zip", buffInput)) {
                    return false;
                }
            }catch (Exception e){
                e.printStackTrace();
                return false;
            }
        }
        IOFileToInternalStorage.missFileNames.clear();
        return true;
    }

    /* keep upload file to FTP server */
//    protected FTPClient doInBackground(Void... args) {
//        connect();
//        FTPClient ftp = this.ftpClient;
//
//        while(true) {
//            try {
//                uploadFile();
//            } catch (IOException e) {
//                e.printStackTrace();
//                close();
//            }
//
//            try {
//
//                Thread.sleep(1000);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//                close();
//            }
//        }
//    }

    /* One time upload file to FTP server */
    protected FTPClient doInBackground(Void... args) {
        connect();
        FTPClient ftp = this.ftpClient;

        try {
            uploadFile();
            //close();
        } catch (IOException e) {
            e.printStackTrace();
            close();
        }

        try {

            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
            close();
        }

        return ftp;
    }


    protected void onPostExecute(FTPClient result) {
        Log.v("FTPTask","FTP connection complete");
        //ftpClient = result;
        //Where ftpClient is a instance variable in the main activity

    }



}
