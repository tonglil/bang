package com.zap;

import java.util.ArrayList;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.GridView;
import android.widget.Toast;

import com.zap.main.Card;
import com.zap.main.Player;

public class PlayerTableCards extends Fragment {

    private Player player;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View playerTableCards = inflater.inflate(R.layout.fragment_player_table_cards, container, false);

        PlayerActivity parentActivity = (PlayerActivity) getActivity();
        player = parentActivity.getPlayer();
        ArrayList<Card> cards = player.getBlueCards();
        ArrayList<Integer> images = new ArrayList<Integer>();
        
        Log.v("TONY", "# of table cards: " + cards.size());
        for (Card card : cards) {
            Log.v("TONY", "A table card exists: " + card.name + " image: " + card.image);
            images.add(getResources().getIdentifier(card.image , "drawable", parentActivity.getPackageName()));
        }
        
        GridView cardGrid = (GridView) playerTableCards.findViewById(R.id.tableCardGrid);
        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), images));
        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
                Toast.makeText(getActivity(), "What we would actually do is zoom on card position: " + position + " id: " + id + " for actions and targets", Toast.LENGTH_SHORT).show();

                // TODO: determine if that card needs to be able to select (other) users => then show dialog

                AlertDialog.Builder builderSingle = new AlertDialog.Builder(getActivity());
                builderSingle.setTitle("Choose One Player");
                String names[] = { "Player 1", "Player 2", "Player 3", "Player 4", "Player 5", "Player 6", "Player 7" };
                final ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getActivity(), android.R.layout.select_dialog_singlechoice, names);
                builderSingle.setNegativeButton("cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });

                builderSingle.setAdapter(arrayAdapter, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String strName = arrayAdapter.getItem(which);
                        AlertDialog.Builder builderInner = new AlertDialog.Builder(getActivity());
                        builderInner.setTitle("Your Selected:");
                        builderInner.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                            }
                        });
                        // TODO: DO SOMETHING TO SELECTED PLAYER HERE
                        builderInner.setMessage(strName);
                        builderInner.show();
                    }
                });
                builderSingle.show();
            }
        });

        return playerTableCards;
    }
}
