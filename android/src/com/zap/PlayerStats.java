package com.zap;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

public class PlayerStats extends Fragment {

    TextView statsResults;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View playerStats = inflater.inflate(R.layout.fragment_player_stats, container, false);

        statsResults = (TextView) playerStats.findViewById(R.id.statsResults);
        //WHAT IS THIS?
        String data = getTag();
        ((PlayerActivity) getActivity()).setTabStats(data);

        Toast.makeText(getActivity(), "stats: " + data, Toast.LENGTH_SHORT).show();

        //((TextView) playerStats.findViewById(R.id.playerStatsHealthValue)).setText("4");
        //((TextView) playerStats.findViewById(R.id.playerStatsRangeValue)).setText("2");
        //((TextView) playerStats.findViewById(R.id.playerStatsHiddenValue)).setText("0");
        //((TextView) playerStats.findViewById(R.id.playerStatsSpecialValue)).setText("Some Status");
        return playerStats;
    }

    public void updateStats(String data) {
        statsResults.setText(data);
    }
}
