package com.evenwell.grideye;

import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by CrabChen on 7/12/16.
 */
public class TempAdapter extends BaseAdapter {
    Context mContext;
    byte[] byteData;
    String sb_rawdata;
    double maxTemp = 0.0;
    double minTemp = 0.0;


    public TempAdapter(Context context){
        mContext = context;
    }

    @Override
    public int getCount() {
        return (8*8);
    }

    @Override
    public Object getItem(int position) {
        return position;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        Log.i("Crab","getView: " + position);
        int temperture = 0;
        if(convertView == null){
            LayoutInflater inflater = LayoutInflater.from(mContext);
            convertView = inflater.inflate(R.layout.temperturelayout, null);

        }
        TextView tv = (TextView)convertView.findViewById(R.id.textView);
//        tv.setBackgroundColor(getColor(drawTempLayout(position)));
        tv.setBackgroundColor(getColorwithRange(drawTempLayout(position)));
        tv.setText(String.valueOf(drawTempLayout(position)));
//        LinearLayout tempLayout = (LinearLayout)convertView.findViewById(R.id.temp);
//        tempLayout.setBackgroundColor(getColor(temperture));
        return convertView;
    }

//    private String getColor(double temperture){
    private int getColor(double temperture){
        if(temperture > 88.0){
            return Color.rgb(232,52,0);
//            return "e83400";
        }else if(temperture > 72.0 && temperture <= 88.0){
            return Color.rgb(201,120,2);
//            return "c97802";
        }else if(temperture > 56.0 && temperture <= 72.0){
            return Color.rgb(172,188,1);
//            return "acbc01";
        }else if(temperture > 40.0 && temperture <= 56.0){
            return Color.rgb(143,254,2);
//            return "8ffe02";
        }else if(temperture > 24.0 && temperture <= 40.0){
            if(temperture > 24.0 && temperture <= 26.0){
                return Color.rgb(103,186,70);
            }else if(temperture > 26.0 && temperture <= 28.0){
                return Color.rgb(108,194,59);
            }else if(temperture > 28.0 && temperture <= 30.0){
                return Color.rgb(115,201,52);
            }else if(temperture > 30.0 && temperture <= 32.0){
                return Color.rgb(118,211,45);
            }else if(temperture > 32.0 && temperture <= 34.0){
                return Color.rgb(124,220,34);
            }else if(temperture > 34.0 && temperture <= 36.0){
                return Color.rgb(127,228,26);
            }else if(temperture > 36.0 && temperture <= 38.0){
                return Color.rgb(132,236,19);
            }else if(temperture > 38.0 && temperture <= 40.0){
                return Color.rgb(137,243,10);
            }else{
                return Color.rgb(137,243,10);
            }
//            return "67ba46";
        }else if(temperture > 0.0 && temperture <= 24.0){
            return Color.rgb(67,118,137);
//            return "437689";
        }else{
//            return "0xffffff";
            return Color.WHITE;
        }
    }

    public int getColorwithRange(double temperture){
//        final double MAX_TEMP = 35.0;
//        final double MIN_TEMP = 30.0;
        if(temperture <= minTemp){
            return Color.rgb(0,0,254);
        }else if(temperture > maxTemp){
            return Color.rgb(232,52,0);
        }else{
            // temp in range, so calculate
            double unit = (maxTemp - minTemp)/8;
            int whichcolor = (int) Math.ceil((maxTemp - temperture)/unit);
            switch (whichcolor){
                case 0:
                    return Color.rgb(232,52,0);
                case 1:
                    return Color.rgb(201,121,0);
                case 2:
                    return Color.rgb(172,189,0);
                case 3:
                    return Color.rgb(143,254,2);
                case 4:
                    return Color.rgb(103,186,70);
                case 5:
                    return Color.rgb(67,118,137);
                case 6:
                    return Color.rgb(27,50,205);
                case 7:
                default:
                    return Color.rgb(0,0,254);
            }
        }
    }

    public void setRawData(String data){
        Log.i("Crab","setRawData: " + data);
//        sb_rawdata = null;
        sb_rawdata = data;
//        this.notifyDataSetChanged();
    }

    public double drawTempLayout(int position){
        if(sb_rawdata == null) return 0.0;
//        String packetStartSymbol = "2A2A2A31";
//        String packetEndSymbol = "0D0A";
//
//        //remove end symbol
//        dataString = dataString.substring(0, dataString.indexOf(packetEndSymbol));
        String dataString = sb_rawdata.toString();
        List<String> items = new ArrayList<>();
        for(int k=0; k<dataString.length(); k+=2){
            items.add(dataString.substring(k,k+2));
        }
        int i = 1;

        /*ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);
        for (String element : items) {
            try {
                out.writeUTF(element);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        byte[] bytes = baos.toByteArray();*/

        //
        /*String temp = "";
        for(int j=items.size()-1; j>0; j--){
            temp = temp + "-" + (Integer.parseInt(items.get(j).trim(), 16) * 0.25);
            if((i % 8) == 0) {
                Log.i("Crab", "result: " + temp);
                temp = "";
            }
            i++;
        }*/
//        notifyDataSetChanged();
        if (items.size() == 0){return 0.0;}
        return Integer.parseInt(items.get(items.size()-1 - (position%items.size())), 16) * 0.25;
    }
    //
    public void setMaxMinTemp(String maxtemp, String mintemp){
        maxTemp = Double.valueOf(maxtemp);
        minTemp = Double.valueOf(mintemp);
    }
    //
}
