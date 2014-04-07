package com.zap;

import java.util.ArrayList;

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

import com.zap.main.Player;

public class PlayerStats extends Fragment {

    private View playerStats;
    
    private TextView textHealth;
    private TextView textRange;
    private TextView textHidden;
    private TextView textSpecial;
    private Button doneTurn;

    private Player player;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerStats = inflater.inflate(R.layout.fragment_player_stats, container, false);

        textHealth = (TextView) this.playerStats.findViewById(R.id.playerStatsHealthValue);
        textRange = (TextView) this.playerStats.findViewById(R.id.playerStatsRangeValue);
        textHidden = (TextView) this.playerStats.findViewById(R.id.playerStatsHiddenValue);
        textSpecial = (TextView) this.playerStats.findViewById(R.id.playerStatsSpecialValue);
        doneTurn = (Button) this.playerStats.findViewById(R.id.playerDone);

        buildStats();
        addButtonListenerDone();
        
        return this.playerStats;
    }

    public void buildStats() {
        PlayerActivity parentActivity = (PlayerActivity) getActivity();
        player = parentActivity.getPlayer();
        ArrayList<String> status = new ArrayList<String>();

        Toast.makeText(getActivity(), "stats for player: " + player.getName(), Toast.LENGTH_SHORT).show();

        textHealth.setText("" + player.getLives());
        textRange.setText("" + player.getRange());
        // TODO: TONY for mustang
        textHidden.setText("0");
        if (player.hasBarrel())
            status.add("Barrel");
        if (player.hasDynamite())
            status.add("Dynamite");
        if (player.hasJail())
            status.add("Jailed");
        if (status.size() == 0)
            status.add("N/A");
        textSpecial.setText(TextUtils.join(" / ", status));

        // NOTE: this allows this tab to be referenced by a tag and updated by other tabs
        ((PlayerActivity) getActivity()).setTabStats(getTag());
    }
        
    public void addButtonListenerDone() {
        doneTurn = (Button) this.playerStats.findViewById(R.id.playerDone);
        doneTurn.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                AlertDialog.Builder nextConfirmationDialog = new AlertDialog.Builder(getActivity());
                nextConfirmationDialog.setTitle("Are you sure you want to end your turn?");
                nextConfirmationDialog.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        Log.v("TONY", "GO TO INTERMEDIATE VIEW");
                        // TODO: could blank bg screen here?
                        AlertDialog.Builder passOrWaitDialog = new AlertDialog.Builder(getActivity());
                        // TODO: based on if next player is on same or different device...
                        passOrWaitDialog.setTitle("Please Wait / Pass The Device To: NEXT PLAYER");
                        passOrWaitDialog.setPositiveButton("Okay", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                Log.v("TONY", "NEXT PLAYER CONFIRMS => CHANGE TO THAT PLAYER AND UPDATE TABS");
                                // TODO:
                                // GO TO INTERMEDIATE VIEW
                                // UPDATE PLAYER #
                                // REFRESH/REBUILD TABS
                                PlayerStats playerStats = (PlayerStats) getActivity().getSupportFragmentManager().findFragmentByTag(((PlayerActivity) getActivity()).getTabStats());
                                playerStats.buildStats();
                                dialog.dismiss();
                            }
                        });
                        passOrWaitDialog.show();
                    }
                });
                nextConfirmationDialog.setNegativeButton("No", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
                PlayerStats.this.player.endTurn();
                nextConfirmationDialog.show();
            }
        });
    }

}
