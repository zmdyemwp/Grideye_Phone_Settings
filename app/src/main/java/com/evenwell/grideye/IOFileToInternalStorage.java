package com.evenwell.grideye;

import android.app.ActivityManager;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.util.Log;

import net.lingala.zip4j.core.ZipFile;
import net.lingala.zip4j.exception.ZipException;
import net.lingala.zip4j.model.ZipParameters;
import net.lingala.zip4j.util.Zip4jConstants;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class IOFileToInternalStorage{
    private static final String TAG = IOFileToInternalStorage.class.getName();
    public static final String TIME_FORMAT = "yyyyMMdd HHmmss.SSS";
    public static final String FILE_NAME_FORMAT = "yyyyMMdd-HHmmss";
    public static final int FILE_SIZE_THRESH = 1000*1024; //bytes
    public static final int FILE_NUMBER_THRESH = 10;

    public static String FileName = "";
    public static FileOutputStream writeStream;
    private static FTPService ftp;
    public static List<String> missFileNames = new ArrayList<String>();

    public static void createLogDirectory(Context context) {
//        // Create a dir in the External Storage
//        Date date = new Date();
//        SimpleDateFormat dFormat = new SimpleDateFormat(FILE_NAME_FORMAT);
//        FileName = dFormat.format(date);
//
//        // Create a directory in the Internal Storage
//        try {
//            File mydir = context.getDir(FileName, Context.MODE_PRIVATE); //Creating an internal dir;
//            File fileWithinMyDir = new File(mydir, FileName); //Getting a file within the dir.
//            writeStream = new FileOutputStream(fileWithinMyDir); //Use the stream as usual to write into the file.
//
//        } catch (Exception ex) {
//            ex.printStackTrace();
//        }


        // Create a dir in the Internal Storage
        if (!FileName.equals("")) {
            try {
                File directory = new File(context.getFilesDir() + File.separator + FileName);

                boolean isMkDirSuccess = true;
                if (!directory.exists()) {
                    isMkDirSuccess = directory.mkdirs();
                }
                if (isMkDirSuccess) {

                } else {
                    Log.e(TAG, "To make directory failed");
                }
            } catch (Exception ex) {
                Log.e(TAG, ex.getStackTrace().toString());
            }
        }
    }

    public static boolean writeTxtFile(String logText, String logTime,  Context context) {
//        // write text file in Internal Storage
//        SimpleDateFormat sdFormat = new SimpleDateFormat(TIME_FORMAT);
//        Date date = new Date();
//        String strDate = sdFormat.format(date);
//
//        String writeLogText = strDate + " " + logText;
//
//        // Create a file in the Internal Storage
//        try {
//            writeStream.write(writeLogText.getBytes());
//            writeStream.close();
//        } catch (Exception e) {
//            e.printStackTrace();
//            return false;
//        }
//
//        return true;


        // write text file in External Storage
        try{

            String localFilePath = context.getFilesDir() + File.separator + FileName + File.separator + FileName + ".txt";

            FileWriter fw = new FileWriter(localFilePath, true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write(logTime + " " + logText);
            bw.newLine();
            bw.close();

            return true;

        }catch(IOException e){
            e.printStackTrace();
            return false;
        }
    }

    public static String writeImageFile(Bitmap b, String ImgName, Context context) {
        Log.e(TAG, "write image file to internal");
        if (b != null) {
            try {
                File sdCardFile = new File(context.getFilesDir() + File.separator + FileName);

                if (sdCardFile.canWrite() == true) {
                    File viewerFile = new File(sdCardFile, Utils.IMG_DIRECTORY_NAME);
                    viewerFile.mkdirs();

                    File imageFile = new File(viewerFile, ImgName + ".jpg");
                    FileOutputStream fileStream = new FileOutputStream(imageFile);
                    b.compress(Bitmap.CompressFormat.JPEG, 50, fileStream);
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

    public static void writePreImageFile(StoredBitmap[] sbmaps, Context context) {
        Log.d(TAG, "write Pre ImageFile");
        if (sbmaps != null) {
            for (int i = 0; i < sbmaps.length; i++) {
                writeImageFile(sbmaps[i].bitmap, sbmaps[i].fileName, context);
            }
        }
    }


    public static void zipSubFolder(Context context) {

        String inputFolderPath = context.getFilesDir() + File.separator + FileName;
        String outZipPath = context.getFilesDir() + File.separator + FileName + ".zip";


        // if zip number over than Thresh, then delete the first zip file
        //if(checkFileNumber(context))
          //  removeFile(findFirstZipFile(context), context);
        if (isAvaliableFull(context))
            removeFile(findFirstZipFile(context), context);


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

    private static String findFirstZipFile(Context context) {
        String firstFileName = "";
        try {
            File dir = new File(context.getFilesDir() + "");
            if (dir.isDirectory())
            {
                String[] children = dir.list();

                for (int i = 1; i < children.length; i++)
                {
                    if (children[i].contains(".zip")) {

                        String bbb = children[i];

                        String[] aaa = bbb.split("\\.");

                        //String childFileName = children[i].split("\\.")[0].replace("-", "");
                        String childFileName = children[i];
                        if (firstFileName.equals("")) firstFileName = childFileName;

                        // find miminum string name(first zip file) for zip file
                        int comp = childFileName.compareTo(firstFileName);
                        if (childFileName.compareTo(firstFileName) < 0)
                            firstFileName = childFileName;
                    }
                }
            }
        }catch (Exception e){
            e.printStackTrace();
            firstFileName = "";
        }


        return firstFileName;
    }

    public static void uploadFliesToFTP(String text, Bitmap bImg, String fileName, Context context){

        //if (writeTxtFile(text, fileName, context)) {

            //String localImgFilePath = writeImageFile(bImg, fileName, context);

            //String localTextFilePath = context.getFilesDir() + File.separator + FileName + File.separator + FileName + ".text";

            String localZipfilePath = context.getFilesDir() + File.separator + FileName + ".zip";

            ftp = new FTPService();

            if (ftp.getStatus() != AsyncTask.Status.RUNNING) {

                //ftp.uploadFile(localFilePath, FileName, FileName+".txt");
                //ftp.setFTPClient(localTextFilePath, localImgFilePath, FileName, FileName);

                ftp.setFTPClient(localZipfilePath, FileName, context);

                // start to transfer file to FTP Server
                ftp.execute();
            }
        //}
    }


    public static boolean checkFileSize(Context context){
        String localFilePath = context.getFilesDir() + File.separator + FileName + File.separator + FileName + ".txt";

        File file = new File(localFilePath);
        long file_size = Integer.parseInt(String.valueOf(file.length()));

        if (file_size > FILE_SIZE_THRESH) {
            Log.d(TAG, "check the file size = " + file_size);
            return true;
        }

        return false;
    }

    public static boolean checkFileNumber(Context context){
        try {
            File dir = new File(context.getFilesDir()+"");
            if (dir.isDirectory())
            {
                String[] children = dir.list();
                if (children.length > FILE_NUMBER_THRESH){
                    return true;
                }
            }
        }catch (Exception e){
            e.printStackTrace();
        }

        return false;
    }

    public static int getFileNumber(Context context){
        int result = 0;

        try {
            File dir = new File(context.getFilesDir()+"");
            if (dir.isDirectory())
            {
                String[] children = dir.list();
                result = children.length;
            }
        }catch (Exception e){
            e.printStackTrace();
        }

        return result;
    }

    public static boolean isAvaliableFull(Context context){
        if (getAvailableMemory(context) < FILE_SIZE_THRESH * 7 * 5)
            return true;
        else
            return false;
    }


    public static void removeFile(String fileName, Context context){

        if (fileName == "") return;

        try {
            File dir = new File(context.getFilesDir() + File.separator + fileName);
            if (dir.isDirectory())
            {
                String[] children = dir.list();
                for (int i = 0; i < children.length; i++)
                {
                    new File(dir, children[i]).delete();
                }
                String test = dir.getName();
                if (!dir.getName().equals("files")) dir.delete();
            } else{
                dir.delete();
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public static void removeAllFile(Context context, File dir){
        try {
            if (dir == null) {
                dir = new File(context.getFilesDir() + "");
            }
            if (dir.isDirectory())
            {
                String[] children = dir.list();
                for (int i = 0; i < children.length; i++)
                {
                    File childFile = new File(dir, children[i]);

                    if (childFile.isDirectory()){
                        removeAllFile(context, childFile);
                    } else {
                        childFile.delete();
                    }
                }
                String test = dir.getName();
                if (!dir.getName().equals("files")) dir.delete();
            }
            else{
                dir.delete();
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public static void removeAllDirectory(Context context, File dir){
        try {
            if (dir == null) {
                dir = new File(context.getFilesDir() + "");
            }
            if (!dir.getName().contains(".zip")) {
                if (dir.isDirectory()) {
                    String[] children = dir.list();
                    for (int i = 0; i < children.length; i++) {
                        File childFile = new File(dir, children[i]);

                        if (childFile.isDirectory()) {
                            removeAllDirectory(context, childFile);
                        } else {
                            if (!childFile.getName().contains(".zip")) childFile.delete();
                        }

                    }
                    String test = dir.getName();
                    if (!dir.getName().equals("files")) dir.delete();
                } else {
                    if (!dir.getName().contains(".zip")) dir.delete();
                }
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    /**

     * 獲取當前可用內存，返回數據以字節為單位。
     *
     * @param context 可傳入應用程式上下文。
     * @return 當前可用內存。
     */
    private static long getAvailableMemory(Context context) {
        ActivityManager.MemoryInfo info = new ActivityManager.MemoryInfo();
        getActivityManager(context).getMemoryInfo(info); //利用ActivityManager的getMemoryInfo獲取到內存的一些信息賦給info
        long availableSize = info.availMem; //availMem為系統可用內存
        return availableSize;
    }
    private static ActivityManager mActivityManager;
    private static ActivityManager getActivityManager(Context context) {

        if (mActivityManager == null) {
            mActivityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        }
        return mActivityManager;
    }

}
