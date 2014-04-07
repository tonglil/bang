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
import android.widget.Toast;

import com.zap.main.Card;
import com.zap.main.Opponent;
import com.zap.main.Player;

public class PlayerHandCards extends Fragment {

    private View playerHandCards;

    private Player playerCurrent;

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

        playerCurrent = parentActivity.getPlayer();
        final ArrayList<Card> cards = playerCurrent.getHandCards();
        final HashMap<Integer, Opponent> opponents = playerCurrent.getOpponents();
        ArrayList<Integer> images = new ArrayList<Integer>();

        Log.v("TONY", "# of hand cards: " + cards.size());
        for (Card card : cards) {
            Log.v("TONY", "A hand card exists: " + card.name + " image: " + card.image);
            images.add(getResources().getIdentifier(card.image, "drawable", parentActivity.getPackageName()));
        }

        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), images));
        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int cardPosition, long id) {
                final Card cardPlayed = cards.get(cardPosition);
                Toast.makeText(getActivity(), "The hand card choosen: " + cardPlayed.name, Toast.LENGTH_SHORT).show();
                Log.v("TONY", "The hand card choosen: " + cardPlayed.name);

                // TODO: determine if that card needs to be able to select
                // (other) users => then show dialog
                ArrayList<String> names = new ArrayList<String>();
                Log.v("TONY", "# of players: " + opponents.size());

                // TODO: add in a confirmation screen if no player needs to be
                // selected....
                if (cardPlayed.allPlayersIncSelf()) {
                    playCard(cardPosition, null);
                } else if (cardPlayed.allPlayersNotSelf()) {
                    playCard(cardPosition, null);
                } else if (cardPlayed.onePlayerNotSelf()) {
                    for (Opponent opponent : opponents.values()) {
                        Integer pid = opponent.getPid();
                        if (pid != PlayerHandCards.this.playerCurrent.getPid() && !opponent.getDead() && PlayerHandCards.this.playerCurrent.checkRange(pid)) {
                            names.add("Player " + pid + " (range " + PlayerHandCards.this.playerCurrent.getRangeFromOpponent(pid) + " away)");
                        }
                    }
                    choosePlayer(cardPosition, names);
                } else {
                    playCard(cardPosition, playerCurrent.getPid());
                }
            }

            public void playCard(int cardPosition, Integer target) {
                // TODO: DO CARD ACTION HERE
                // TODO: somehow get card feedback to see if it can proceed, and
                // then do extra things if necessary?
                playerCurrent.playCard(cards.get(cardPosition).cid, target);
                cards.remove(cardPosition);
                buildCards();
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
                        cardPlayedDialog.setTitle(cards.get(cardPosition).name + " Played On: " + playerSelected);
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
