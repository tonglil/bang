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

public class PlayerHandCards extends Fragment {

    private View playerHandCards;
    
    private Player player;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerHandCards = inflater.inflate(R.layout.fragment_player_hand_cards, container, false);

        buildCards();

        return this.playerHandCards;
    }

    public void buildCards() {
        GridView cardGrid = (GridView) this.playerHandCards.findViewById(R.id.handCardGrid);
        PlayerActivity parentActivity = (PlayerActivity) getActivity();
        
        player = parentActivity.getPlayer();
        final ArrayList<Card> cards = player.getHandCards();
        ArrayList<Integer> images = new ArrayList<Integer>();

        Log.v("TONY", "# of hand cards: " + cards.size());
        for (Card card : cards) {
            Log.v("TONY", "A hand card exists: " + card.name + " image: " + card.image);
            images.add(getResources().getIdentifier(card.image, "drawable", parentActivity.getPackageName()));
        }

        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), images));
        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, final int position, long id) {
                final Card cardPlayed = cards.get(position);
                Toast.makeText(getActivity(), "This card: " + cardPlayed.name + " does actions on targets", Toast.LENGTH_SHORT).show();
                Log.v("TONY", "The hand card choosen is: " + cardPlayed.name);

                // TODO: determine if that card needs to be able to select
                // (other) users => then show dialog

                AlertDialog.Builder cardActionDialog = new AlertDialog.Builder(getActivity());
                cardActionDialog.setTitle("Choose A Player");
                // TODO: get list of player names
                String names[] = { "Player 1", "Player 2", "Player 3", "Player 4", "Player 5", "Player 6", "Player 7" };
                final ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getActivity(), android.R.layout.select_dialog_singlechoice, names);
                cardActionDialog.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });

                cardActionDialog.setAdapter(arrayAdapter, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String selectedPlayer = arrayAdapter.getItem(which);
                        AlertDialog.Builder cardPlayedDialog = new AlertDialog.Builder(getActivity());
                        cardPlayedDialog.setTitle(cards.get(position).name + " Played On " + selectedPlayer);
                        // TODO: TONY/AMITOJ: if we want to display a custom message based on the card played, set that message as a part of: card.message for example "can't play beer card, max health"
                        // cardPlayedDialog.setMessage(cards.get(position).message);
                        cardPlayedDialog.setPositiveButton("Continue", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                            }
                        });

                        // TODO: DO CARD ACTION HERE
                        // TODO: either check what kind of card and do accordingly, or somehow get card feedback to see if it can proceed, and then do extra things if necessary?
                        player.playCard(cards.get(position).cid);
                        cards.remove(position);
                        buildCards();

                        PlayerStats playerStats = (PlayerStats) getActivity().getSupportFragmentManager().findFragmentByTag(((PlayerActivity) getActivity()).getTabStats());
                        playerStats.buildStats();

                        cardPlayedDialog.show();
                    }
                });
                cardActionDialog.show();
            }
        });
    }
    
}
