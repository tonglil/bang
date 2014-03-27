package com.zap;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class PlayerStats extends Fragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View player1 = inflater.inflate(R.layout.fragment_player_stats, container, false);
        ((TextView) player1.findViewById(R.id.playerStatsHealthValue)).setText("4");
        ((TextView) player1.findViewById(R.id.playerStatsRangeValue)).setText("2");
        ((TextView) player1.findViewById(R.id.playerStatsHiddenValue)).setText("0");
        ((TextView) player1.findViewById(R.id.playerStatsSpecialValue)).setText("Some Status");
        return player1;
    }
}