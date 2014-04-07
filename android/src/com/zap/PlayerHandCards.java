package com.zap;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

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
    private ArrayList<Card> cards;
    private ArrayList<Integer> images;
    private HashMap<Integer, Opponent> opponents;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerHandCards = inflater.inflate(R.layout.fragment_player_hand_cards, container, false);

        ((PlayerActivity) getActivity()).setTabHandCards(getTag());
        buildCards();

        return this.playerHandCards;
    }

    public void buildCards() {
        GridView cardGrid = (GridView) this.playerHandCards.findViewById(R.id.handCardGrid);
        PlayerActivity parentActivity = (PlayerActivity) getActivity();

        this.images = new ArrayList<Integer>();
        this.playerCurrent = parentActivity.getPlayer();
        this.cards = this.playerCurrent.getHandCards();
        this.opponents = this.playerCurrent.getOpponents();

        Log.v("TONY", "# of players: " + this.opponents.size());
        Log.v("TONY", "# of hand cards: " + this.cards.size());
        for (Card card : this.cards) {
            Log.v("TONY", "A hand card exists: " + card.name + " image: " + card.image);
            this.images.add(getResources().getIdentifier(card.image, "drawable", parentActivity.getPackageName()));
        }

        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), this.images));
        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int cardPosition, long id) {
                final Card cardPlayed = PlayerHandCards.this.cards.get(cardPosition);
                ArrayList<String> names = new ArrayList<String>();

                // TODO: this loop is purely for debug so you can confirm other
                // player's names
                for (Opponent opponent : PlayerHandCards.this.opponents.values()) {
                    Log.v("TONY", "A player named: " + opponent.getName() + " exists.");
                }

                if (cardPlayed.allPlayersIncSelf()) {
                    confirmCard(cardPosition);
                } else if (cardPlayed.allPlayersNotSelf()) {
                    confirmCard(cardPosition);
                } else if (cardPlayed.onePlayerNotSelf()) {
                    for (Opponent opponent : opponents.values()) {
                        Integer pid = opponent.getPid();
                        if (pid != PlayerHandCards.this.playerCurrent.getPid() && !opponent.getDead() && PlayerHandCards.this.playerCurrent.checkRange(pid)) {
                            names.add("Player " + pid + " (range " + PlayerHandCards.this.playerCurrent.getRangeFromOpponent(pid) + " away)");
                        }
                    }
                    choosePlayer(cardPosition, names);
                } else {
                    confirmCard(cardPosition);
                }
            }

            public void playCard(int cardPosition, Integer target) {
                // TODO: TONY/AMITOJ somehow get card feedback and do extra
                // things if necessary?
                PlayerHandCards.this.playerCurrent.playCard(PlayerHandCards.this.cards.get(cardPosition).cid, target);
                PlayerHandCards.this.cards.remove(cardPosition);
                buildCards();
            }

            public void confirmCard(final int cardPosition) {
                AlertDialog.Builder confirmDialog = new AlertDialog.Builder(getActivity());
                confirmDialog.setTitle("Are you sure you want to use the " + PlayerHandCards.this.cards.get(cardPosition).name + " card?");
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
                        String selectedItem = arrayAdapter.getItem(which);
                        
                        String pattern = "([0-9]+)";
                        Pattern regex = Pattern.compile(pattern);
                        Matcher match = regex.matcher(selectedItem);
                        playerSelected = Integer.parseInt(match.group(0));
                        
                        AlertDialog.Builder cardPlayedDialog = new AlertDialog.Builder(getActivity());
                        // TODO: this is pid only, not the name...
                        cardPlayedDialog.setTitle(PlayerHandCards.this.cards.get(cardPosition).name + " Played On pid: " + playerSelected);
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
