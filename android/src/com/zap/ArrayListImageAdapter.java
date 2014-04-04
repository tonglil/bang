package com.zap;

import java.util.ArrayList;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;

public class ArrayListImageAdapter extends BaseAdapter {
    private Context context;
    private ArrayList<Integer> cards;

    public ArrayListImageAdapter(Context c, ArrayList<Integer> cards) {
        this.context = c;
        this.cards = cards;
    }

    public int getCount() {
        return cards.size();
    }

    public Object getItem(int position) {
        return null;
    }

    public long getItemId(int position) {
        return cards.get(position);
    }

    // create a new ImageView for each item referenced by the Adapter
    public View getView(int position, View convertView, ViewGroup parent) {
        ImageView imageView;
        if (convertView == null) {
            // if it's not recycled, initialize some attributes
            imageView = new ImageView(context);
            imageView.setAdjustViewBounds(true);
            imageView.setScaleType(ImageView.ScaleType.CENTER_CROP);
            imageView.setPadding(4, 4, 4, 4);
        } else {
            imageView = (ImageView) convertView;
        }

        imageView.setImageResource(cards.get(position));
        return imageView;
    }
}
