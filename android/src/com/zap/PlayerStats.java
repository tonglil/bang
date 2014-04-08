package com.zap;

import java.util.ArrayList;
import java.util.HashMap;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.zap.main.Opponent;
import com.zap.main.Player;

public class PlayerStats extends Fragment {

    private View playerStats;
    
    private TextView textHealth;
    private TextView textRange;
    private TextView textPlayers;
    private TextView textSpecial;
    private Button doneTurn;

    private Player player;
    private HashMap<Integer, Opponent> opponents;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerStats = inflater.inflate(R.layout.fragment_player_stats, container, false);

        this.textHealth = (TextView) this.playerStats.findViewById(R.id.playerStatsHealthValue);
        this.textRange = (TextView) this.playerStats.findViewById(R.id.playerStatsRangeValue);
        this.textPlayers = (TextView) this.playerStats.findViewById(R.id.playerStatsPlayersValue);
        this.textSpecial = (TextView) this.playerStats.findViewById(R.id.playerStatsSpecialValue);
        this.doneTurn = (Button) this.playerStats.findViewById(R.id.playerDone);

        buildStats();
        
        addButtonListenerDone();
        
        return this.playerStats;
    }

    public void buildStats() {
        PlayerActivity parentActivity = (PlayerActivity) getActivity();
        this.player = parentActivity.getPlayer();
        this.opponents = player.getOpponents();
        ArrayList<String> status = new ArrayList<String>();
        
        Toast.makeText(getActivity(), "stats for player: " + player.getName(), Toast.LENGTH_SHORT).show();

        this.textHealth.setText("" + player.getLives());
        this.textRange.setText("" + player.getRange());
        
        String players = "";
        for (Opponent opponent : this.opponents.values()) {
            Integer pid = opponent.getPid();
            players += "Player " + pid + " is " + this.player.getRangeFromOpponent(pid) + " away\n";
        }
                
        if (player.hasMustang())
            status.add("Mustang");
        if (player.hasBarrel())
            status.add("Barrel");
        if (player.hasDynamite())
            status.add("Dynamite");
        if (player.hasJail())
            status.add("Jailed");
        if (status.size() == 0)
            status.add("N/A");
        this.textSpecial.setText(TextUtils.join(" / ", status));

        if (PlayerStats.this.player.isTurn()) {
            this.doneTurn.setEnabled(true);
            this.textPlayers.setText(players);
        } else {
            this.doneTurn.setEnabled(false);
        }
        
        // NOTE: this allows this tab to be referenced by a tag and updated by other tabs
        ((PlayerActivity) getActivity()).setTabStats(getTag());
    }
        
    public void addButtonListenerDone() {
        this.doneTurn = (Button) this.playerStats.findViewById(R.id.playerDone);
        this.doneTurn.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                AlertDialog.Builder nextConfirmationDialog = new AlertDialog.Builder(getActivity());
                nextConfirmationDialog.setTitle("Are you sure you want to end your turn?");
                nextConfirmationDialog.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        PlayerStats.this.player.endTurn();

                        // TODO:
                        // GO TO INTERMEDIATE VIEW
                        AlertDialog.Builder passOrWaitDialog = new AlertDialog.Builder(getActivity());
                        passOrWaitDialog.setTitle("Please Wait For Your Turn");
                        if (PlayerStats.this.player.isTurn()) {

                            passOrWaitDialog.setPositiveButton("Okay", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface dialog, int which) {

                                    // UPDATE PLAYER #
                                    // REFRESH/REBUILD ALL TABS
                                    PlayerStats playerStats = (PlayerStats) getActivity().getSupportFragmentManager().findFragmentByTag(((PlayerActivity) getActivity()).getTabStats());
                                    playerStats.buildStats();
                                    dialog.dismiss();
                                }
                            });
                            passOrWaitDialog.show();
                        }
                        passOrWaitDialog.show();
                    }
                });
                nextConfirmationDialog.setNegativeButton("No", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
                nextConfirmationDialog.show();
            }
        });
    }

}
