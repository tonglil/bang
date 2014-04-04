package com.zap;

import java.util.ArrayList;

import com.zap.main.Card;

import android.app.AlertDialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

public class PlayerTableCards extends Fragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View playerTableCards = inflater.inflate(R.layout.fragment_player_table_cards, container, false);

        // Need to pass in a list of the player's cards
        // ArrayList<Card> cards = Player.getTableCards
        // Map cards to an integer array of cards
        Integer[] cards = { R.drawable.jail_1, R.drawable.mustang_1, R.drawable.schofield_1 };
        GridView cardGrid = (GridView) playerTableCards.findViewById(R.id.tableCardGrid);
        cardGrid.setAdapter(new ImageAdapter(getActivity(), cards));

        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
                Toast.makeText(getActivity(), "What we would actually do is zoom on card position: " + position + " id: " + id + " for actions and targets", Toast.LENGTH_SHORT).show();

                // GET 
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
