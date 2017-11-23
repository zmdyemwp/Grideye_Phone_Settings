package com.evenwell.grideye;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Environment;
import android.renderscript.Sampler;
import android.util.Log;

import com.evenwell.fusionnetlib.GrideyeConstants;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Date;

import net.lingala.zip4j.core.ZipFile;
import net.lingala.zip4j.exception.ZipException;
import net.lingala.zip4j.model.ZipParameters;
import net.lingala.zip4j.util.Zip4jConstants;

/**
 * Created by HowardJHLi on 2016/8/10.
 */

public class WriteFile {
    private static final String TAG = GrideyeActivity.class.getName();
    private static final String FILENAME = "grideye_output.txt";
    public static final String FILE_PATH = "/sdcard/";


    public static final String FILE_NAME_FORMAT = "yyyyMMdd-HHmmss";
    private static final String TIME_FORMAT = "yyyyMMdd HHmmss.SSS";
    private static final int FILE_SIZE_THRESH = 10000; //KB

    private static String FileName = "";


    private FTPService ftp;

    private int writeDataCount = 0;

    public WriteFile(){

    }



    /* ------------------------------------------- /
     *   write setting value to shared preference *
    / ------------------------------------------- */
    public static void writeSharedPref(Context context, byte[] bytes){
        SharedPreferences mPrefs =
                context.getSharedPreferences(GrideyeConstants.GRIDEYE_PREFERENCE, Context.MODE_PRIVATE);
        SharedPreferences.Editor PE = mPrefs.edit();
        if(0x01 == bytes[0] && 14 <= bytes.length) {
            PE.putString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH1, String.format("%s", bytes[1]));
            PE.putString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH2, String.format("%s", bytes[2]));
            PE.putString(GrideyeConstants.settingsPrefName.LABELING_THRESH, String.format("%s", bytes[3]));
            PE.putString(GrideyeConstants.settingsPrefName.FNMV_THRESH, String.format("%s", bytes[4]));
            PE.putString(GrideyeConstants.settingsPrefName.EDGE_THRESH1, String.format("%s", bytes[5]));
            PE.putString(GrideyeConstants.settingsPrefName.EDGE_THRESH2, String.format("%s", bytes[6]));
            PE.putString(GrideyeConstants.settingsPrefName.HUMAN_FRAME_THRESH, String.format("%s", bytes[7]));
            PE.putString(GrideyeConstants.settingsPrefName.BED_LEFT, String.format("%s", bytes[8]));
            PE.putString(GrideyeConstants.settingsPrefName.BED_RIGHT, String.format("%s", bytes[9]));
            PE.putString(GrideyeConstants.settingsPrefName.LEFT_RANGE, String.format("%s", bytes[10]));
            PE.putString(GrideyeConstants.settingsPrefName.RIGHT_RANGE, String.format("%s", bytes[11]));
            PE.putString(GrideyeConstants.settingsPrefName.TOP_RANGE, String.format("%s", bytes[12]));
            PE.putString(GrideyeConstants.settingsPrefName.BOTTOM_RANGE, String.format("%s", bytes[13]));
        } else if(0x02 == bytes[0] && 12 <= bytes.length) {
            PE.putString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_1, String.format("%s", bytes[1]));
            PE.putString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_2, String.format("%s", bytes[2]));
            PE.putString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_3, String.format("%s", bytes[3]));
            PE.putString(GrideyeConstants.settingsPrefName.IP_CAM_ADDR_4, String.format("%s", bytes[4]));
            PE.putString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_1, String.format("%s", bytes[5]));
            PE.putString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_2, String.format("%s", bytes[6]));
            PE.putString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_3, String.format("%s", bytes[7]));
            PE.putString(GrideyeConstants.settingsPrefName.FTP_IP_ADDR_4, String.format("%s", bytes[8]));
            PE.putString(GrideyeConstants.settingsPrefName.RESET, String.format("%s", bytes[9]));
            PE.putString(GrideyeConstants.settingsPrefName.RESET_SETTINGS, String.format("%s", bytes[10]));
            PE.putString(GrideyeConstants.settingsPrefName.DELETE_LOGS, String.format("%s", bytes[11]));
        } else {
            Log.d("MIN", String.format("writeSharedPref():[0x%02x]%d", bytes[0], bytes.length));
            return;
        }

        PE.commit();
    }

    /* ------------------------------------------- /
     *   read setting value from shared preference *
    / ------------------------------------------- */
    public static byte[] readSharedPref(Context context) {

        byte[] settingDatas = new byte[GrideyeConstants.SETTING_BYTE_SIZE];

        SharedPreferences sharedPref = context.getSharedPreferences(
                GrideyeConstants.GRIDEYE_PREFERENCE, Context.MODE_PRIVATE);

        settingDatas[GrideyeConstants.settingsPrefIndex.DIFF_TEMP_THRESH1] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH1,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH1)));
        Log.d(TAG, "[shared pref] DIFF_TEMP_THRESH1 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH1, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.DIFF_TEMP_THRESH2] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH2,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_DIFF_TEMP_THRESH2)));
        Log.d(TAG, "[shared pref] DIFF_TEMP_THRESH2 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.DIFF_TEMP_THRESH2, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.LABELING_THRESH] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.LABELING_THRESH,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_LABELING_THRESH)));
        Log.d(TAG, "[shared pref] LABELING_THRESH : " + sharedPref.getString(GrideyeConstants.settingsPrefName.LABELING_THRESH, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.FNMV_THRESH] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.FNMV_THRESH,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_FNMV_THRESH)));
        Log.d(TAG, "[shared pref] FNMV_THRESH : " + sharedPref.getString(GrideyeConstants.settingsPrefName.FNMV_THRESH, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.EDGE_THRESH1] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH1,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH1)));
        Log.d(TAG, "[shared pref] EDGE_THRESH1 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH1, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.EDGE_THRESH2] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH2,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_EDGE_THRESH2)));
        Log.d(TAG, "[shared pref] EDGE_THRESH2 : " + sharedPref.getString(GrideyeConstants.settingsPrefName.EDGE_THRESH2, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.HUMAN_FRAME_THRESH] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.HUMAN_FRAME_THRESH,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_HUMAN_FRAME_THRESH)));
        Log.d(TAG, "[shared pref] HUMAN_FRAME_THRESH : " + sharedPref.getString(GrideyeConstants.settingsPrefName.HUMAN_FRAME_THRESH, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.BED_LEFT] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.BED_LEFT,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_BED_LEFT)));
        Log.d(TAG, "[shared pref] BED_LEFT : " + sharedPref.getString(GrideyeConstants.settingsPrefName.BED_LEFT, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.BED_RIGHT] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.BED_RIGHT,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_BED_RIGHT)));
        Log.d(TAG, "[shared pref] BED_RIGHT : " + sharedPref.getString(GrideyeConstants.settingsPrefName.BED_RIGHT, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.LEFT_RANGE] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.LEFT_RANGE,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_LEFT_RANGE)));
        Log.d(TAG, "[shared pref] LEFT_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.LEFT_RANGE, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.RIGHT_RANGE] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.RIGHT_RANGE,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_RIGHT_RANGE)));
        Log.d(TAG, "[shared pref] RIGHT_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.RIGHT_RANGE, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.TOP_RANGE] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.TOP_RANGE,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_TOP_RANGE)));
        Log.d(TAG, "[shared pref] TOP_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.TOP_RANGE, "0"));

        settingDatas[GrideyeConstants.settingsPrefIndex.BOTTOM_RANGE] = Byte.parseByte(sharedPref.getString(GrideyeConstants.settingsPrefName.BOTTOM_RANGE,
                String.valueOf(GrideyeConstants.settingPrefDefault.DEFAULT_BOTTOM_RANGE)));
        Log.d(TAG, "[shared pref] BOTTOM_RANGE : " + sharedPref.getString(GrideyeConstants.settingsPrefName.BOTTOM_RANGE, "0"));

        Log.d(TAG, "[ble setting] response data:");
        for (int i=0;i<13;i++){
            Log.d(TAG, "[ble setting] response data[" + i + "]:" + "= " + settingDatas[i] );
        }

        return settingDatas;
    }


    public void writeToFile(String data, Context context) {
        try {
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(context.openFileOutput(FILENAME, context.MODE_APPEND));
            outputStreamWriter.write(data);
            outputStreamWriter.close();
        }
        catch (IOException e) {
            Log.e(TAG, "File write failed: " + e.toString());
        }

    }

    public String readFromFile(Context context) {

        String ret = "";

        try {
            InputStream inputStream = context.openFileInput(FILENAME);

            if ( inputStream != null ) {
                InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
                BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
                String receiveString = "";
                StringBuilder stringBuilder = new StringBuilder();

                while ( (receiveString = bufferedReader.readLine()) != null ) {
                    stringBuilder.append(receiveString);
                }

                inputStream.close();
                ret = stringBuilder.toString();
            }
        }
        catch (FileNotFoundException e) {
            Log.e(TAG, "File not found: " + e.toString());
        } catch (IOException e) {
            Log.e(TAG, "Can not read file: " + e.toString());
        }

        return ret;
    }

    public void createTextFile(Context context, String fileName){
        // Create a directory in the Internal Storage
        try {
            File mydir = context.getDir("mydir", Context.MODE_PRIVATE); //Creating an internal dir;
            File fileWithinMyDir = new File(mydir, "myfile"); //Getting a file within the dir.
            FileOutputStream out = new FileOutputStream(fileWithinMyDir); //Use the stream as usual to write into the file.
        } catch (Exception ex){
            ex.printStackTrace();
        }

        // Create a file in the Internal Storage
        String content = "hello world";

        FileOutputStream outputStream = null;
        try {
            outputStream = context.openFileOutput(fileName, Context.MODE_PRIVATE);
            outputStream.write(content.getBytes());
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }



    public void createTextFile(String fileName){
        // Create a dir in the External Storage
        try {

            File directory = new File(Environment.getExternalStorageDirectory() + File.separator + fileName);

            boolean isMkDirSuccess = true;
            if (!directory.exists()) {
                isMkDirSuccess = directory.mkdirs();
            }
            if (isMkDirSuccess) {

            } else {
                Log.e(TAG, "To make directory failed");
            }
        } catch (Exception ex){
            Log.e(TAG, ex.getStackTrace().toString());
        }
    }

    public void writeTxtFile(String text){
        try{
            SimpleDateFormat sdFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss.SSS");
            Date date = new Date();
            String strDate = sdFormat.format(date);

            FileWriter fw = new FileWriter(FILE_PATH + File.separator + FileName, true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write(strDate + ": " + text);
            bw.newLine();
            bw.close();
        }catch(IOException e){
            e.printStackTrace();
        }
    }

    public boolean writeTxtFile(String text, String fileName){
        try{
            SimpleDateFormat sdFormat = new SimpleDateFormat(TIME_FORMAT);
            Date date = new Date();
            String strDate = sdFormat.format(date);

            String localFilePath = FILE_PATH + File.separator + fileName + File.separator + fileName + ".txt";


            FileWriter fw = new FileWriter(localFilePath, true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write(strDate + ": " + text);
            bw.newLine();
            bw.close();


            return true;

        }catch(IOException e){
            e.printStackTrace();
            return false;
        }
    }


    public String writeImageFile(Bitmap b, String fileName) {
        if (b != null) {
            try {
                //File sdCardFile = Environment.getExternalStorageDirectory();

                File sdCardFile = new File(Environment.getExternalStorageDirectory() + File.separator + fileName);

                if (sdCardFile.canWrite() == true) {
                    File viewerFile = new File(sdCardFile, Utils.IMG_DIRECTORY_NAME);
                    viewerFile.mkdirs();

                    SimpleDateFormat sdFormat = new SimpleDateFormat(TIME_FORMAT);
                    Date date = new Date();
                    String imgName = sdFormat.format(date);

                    File imageFile = new File(viewerFile, imgName + ".jpg");
                    FileOutputStream fileStream = new FileOutputStream(imageFile);
                    b.compress(Bitmap.CompressFormat.JPEG, 100, fileStream);
                    fileStream.close();

                    return imageFile.toString();
                } else {
                    Log.e(TAG, "IOUtility - Cannot write to SD Card");
                }
                return "";
            } catch (Exception e) {
                Log.e(TAG, "IOUtility - Error - " + e);
                e.printStackTrace();
            }
        }
        return "";
    }



    public void writeFliesToFTP(String text, Bitmap bImg, String fileName){

        if (writeTxtFile(text, fileName)) {

            String localImgFilePath = writeImageFile(bImg, fileName);

            String localTextFilePath = FILE_PATH + File.separator + fileName + File.separator + fileName + ".text";

            String localZipfilePath = FILE_PATH + File.separator + fileName + ".zip";


            //String localImgFilePath = FILE_PATH + File.separator + fileName + File.separator + fileName + ".jpg";

            ftp = new FTPService();

            if (ftp.getStatus() != AsyncTask.Status.RUNNING) {
                //ftp.uploadFile(localFilePath, FileName, FileName+".txt");
                ftp.setFTPClient(localTextFilePath, localImgFilePath, fileName, fileName);

                // start to transfer file to FTP Server
                ftp.execute();

            }
        }
    }


    public void removeFile(String directoryName){

        if (directoryName == "") return;

        try {
            File dir = new File(Environment.getExternalStorageDirectory() + directoryName);
            if (dir.isDirectory())
            {
                String[] children = dir.list();
                for (int i = 0; i < children.length; i++)
                {
                    new File(dir, children[i]).delete();
                }
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }


    public boolean checkFileSize(String fileName){
        String localFilePath = FILE_PATH + File.separator + fileName + File.separator + fileName + ".txt";

        File file = new File(localFilePath);
        int file_size = Integer.parseInt(String.valueOf(file.length()/1024));


        Log.d(TAG, "check the file size = " + file_size);

        if (file_size > FILE_SIZE_THRESH) {
            return true;
        }

        return false;
    }

    /*
 *
 * Zips a file at a location and places the resulting zip file at the toLocation
 * Example: zipFileAtPath("downloads/myfolder", "downloads/myFolder.zip");
 */

    public static void zipSubFolder(String fileName) {

        String inputFolderPath = FILE_PATH + File.separator + fileName;
        String outZipPath = FILE_PATH + File.separator + fileName + ".zip";


        ZipFile zipfile = null;
        try {
            zipfile = new ZipFile(outZipPath);
            ZipParameters parameters = new ZipParameters();
            parameters.setCompressionMethod(Zip4jConstants.COMP_DEFLATE);
            parameters.setCompressionLevel(Zip4jConstants.DEFLATE_LEVEL_NORMAL);
            zipfile.addFolder(inputFolderPath, parameters);
        } catch (ZipException e) {
            e.printStackTrace();
        }

    }

}

