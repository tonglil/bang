package com.zap;

import java.util.ArrayList;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.zap.main.Player;

public class PlayerStats extends Fragment {

    private Player player;
    
    private TextView statsResults;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View playerStats = inflater.inflate(R.layout.fragment_player_stats, container, false);
        
        PlayerActivity parentActivity = (PlayerActivity) getActivity();
        player = parentActivity.getPlayer();
        ArrayList<String> status = new ArrayList<String>();

        Toast.makeText(getActivity(), "stats for player: " + player.getName(), Toast.LENGTH_SHORT).show();
        
        ((TextView) playerStats.findViewById(R.id.playerStatsHealthValue)).setText("" + player.getLives());
        ((TextView) playerStats.findViewById(R.id.playerStatsRangeValue)).setText("" + player.getRange());
        //TODO: TONY for mustang
        ((TextView) playerStats.findViewById(R.id.playerStatsHiddenValue)).setText("0");
        if (player.hasBarrel()) status.add("Barrel");
        if (player.hasDynamite()) status.add("Dynamite");
        if (player.hasJail()) status.add("Jail");
        if (status.size() == 0) status.add("N/A");
        ((TextView) playerStats.findViewById(R.id.playerStatsSpecialValue)).setText(TextUtils.join(" / ", status));
        
        statsResults = (TextView) playerStats.findViewById(R.id.statsResults);
        // TODO: WHAT IS THIS FOR?
        String data = getTag();
        ((PlayerActivity) getActivity()).setTabStats(data);

        return playerStats;
    }

    public void updateStats(String data) {
        statsResults.setText(data);
    }
}
