package com.evenwell.grideye;

import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

/**
 * Created by HowardJHLi on 10/20/16.
 */
public class quadrantAdapter extends BaseAdapter {
    Context mContext;

    int quadrant[] = {0,0,0,0};

    public quadrantAdapter(Context context){
        mContext = context;
    }

    @Override
    public int getCount() {
        return (2*2);
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
        Log.i("Howard","getView: " + position);
        int temperture = 0;
        if(convertView == null){
            LayoutInflater inflater = LayoutInflater.from(mContext);
            convertView = inflater.inflate(R.layout.quadrantlayout, null);

        }

        TextView tv = (TextView)convertView.findViewById(R.id.textView);
        tv.setBackgroundColor(getColorwithQuadrant(this.quadrant[position]));

        return convertView;
    }



    private int getColorwithQuadrant(int quadrant){
        if (quadrant == 1) {
            return Color.RED;
        } else {
            return Color.GRAY;
        }
    }


    public void setQuadrant(String quadrant1, String quadrant2, String quadrant3, String quadrant4){
        this.quadrant[0] = Integer.parseInt(quadrant4);
        this.quadrant[1] = Integer.parseInt(quadrant3);
        this.quadrant[2] = Integer.parseInt(quadrant2);
        this.quadrant[3] = Integer.parseInt(quadrant1);
    }

}
