package com.zap;

import java.util.ArrayList;
import java.util.HashMap;

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

import com.zap.main.Card;
import com.zap.main.Opponent;
import com.zap.main.Player;

public class PlayerHandCards extends Fragment {

    private View playerHandCards;

    private Player playerCurrent;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerHandCards = inflater.inflate(R.layout.fragment_player_hand_cards, container, false);

        buildCards();

        return this.playerHandCards;
    }

    public void buildCards() {
        GridView cardGrid = (GridView) this.playerHandCards.findViewById(R.id.handCardGrid);
        PlayerActivity parentActivity = (PlayerActivity) getActivity();

        playerCurrent = parentActivity.getPlayer();
        final ArrayList<Card> cards = playerCurrent.getHandCards();
        final HashMap<Integer, Opponent> opponents = playerCurrent.getOpponents();
        ArrayList<Integer> images = new ArrayList<Integer>();

        Log.v("TONY", "# of players: " + opponents.size());
        Log.v("TONY", "# of hand cards: " + cards.size());
        for (Card card : cards) {
            Log.v("TONY", "A hand card exists: " + card.name + " image: " + card.image);
            images.add(getResources().getIdentifier(card.image, "drawable", parentActivity.getPackageName()));
        }

        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), images));
        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int cardPosition, long id) {
                final Card cardPlayed = cards.get(cardPosition);
                ArrayList<String> names = new ArrayList<String>();

                // TODO: this is purely for debug
                for (Opponent opponent : opponents.values()) {
                    Log.v("TONY", "A player named: " + opponent.getName() + " exists.");
                }

                if (cardPlayed.allPlayersIncSelf()) {
                    Log.v("TONY", "All players inc self (saloon)");
                    confirmCard(cardPosition);
                } else if (cardPlayed.allPlayersNotSelf()) {
                    Log.v("TONY", "All players not self (indians)");
                    confirmCard(cardPosition);
                } else if (cardPlayed.onePlayerNotSelf()) {
                    for (Opponent opponent : opponents.values()) {
                        names.add(opponent.getName());
                    }
                    Log.v("TONY", "one players not self (jail)");
                    choosePlayer(cardPosition, names);
                } else {
                    Log.v("TONY", "self (beer)");
                    confirmCard(cardPosition);
                }
            }

            public void playCard(int cardPosition, Integer target) {
                // TODO: TONY/AMITOJ somehow get card feedback and do extra
                // things if necessary?
                playerCurrent.playCard(cards.get(cardPosition).cid, target);
                cards.remove(cardPosition);
                buildCards();
            }

            public void confirmCard(final int cardPosition) {
                AlertDialog.Builder confirmDialog = new AlertDialog.Builder(getActivity());
                confirmDialog.setTitle("Are you sure you want to use the " + cards.get(cardPosition).name + " card?");
                confirmDialog.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        playCard(cardPosition, null);
                        PlayerStats playerStats = (PlayerStats) getActivity().getSupportFragmentManager().findFragmentByTag(((PlayerActivity) getActivity()).getTabStats());
                        playerStats.buildStats();
                        dialog.dismiss();
                    }
                });
                confirmDialog.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                    }
                });
                confirmDialog.show();
            }

            public void choosePlayer(final int cardPosition, ArrayList<String> names) {
                AlertDialog.Builder cardActionDialog = new AlertDialog.Builder(getActivity());
                cardActionDialog.setTitle("Choose A Player");
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
                        Integer playerSelected = null;
                        String selectedName = arrayAdapter.getItem(which);
                        for (Opponent opponent : opponents.values()) {
                            if (opponent.getName() == selectedName) {
                                playerSelected = opponent.getPid();
                            }
                        }
                        AlertDialog.Builder cardPlayedDialog = new AlertDialog.Builder(getActivity());
                        cardPlayedDialog.setTitle(cards.get(cardPosition).name + " Played On pid: " + playerSelected);
                        // TODO: TONY/AMITOJ: if we want to display a custom
                        // message based on the card played, set that message as
                        // a part of: card.message for example
                        // "can't play beer card, max health"
                        // cardPlayedDialog.setMessage(cards.get(position).message);
                        cardPlayedDialog.setPositiveButton("Continue", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                            }
                        });
                        playCard(cardPosition, playerSelected);

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
