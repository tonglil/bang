package com.zap;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class PlayerCards extends Fragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View player = inflater.inflate(R.layout.fragment_player_cards, container, false);
        //((TextView) player.findViewById(R.id.textView2)).setText("Your Cards");
        return player;
    }
}
