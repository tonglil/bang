package com.zap;

import java.util.ArrayList;

import android.app.AlertDialog;
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

import com.zap.main.Card;
import com.zap.main.CardController;
import com.zap.main.Player;

public class PlayerHandCards extends Fragment {
    
    private Player player;

    private EditText cardInput;
    private Button cardButton;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View playerHandCards = inflater.inflate(R.layout.fragment_player_hand_cards, container, false);
        
        PlayerActivity parentActivity = (PlayerActivity) getActivity();
        player = parentActivity.getPlayer();
        final ArrayList<Card> cards = player.getHandCards();
        ArrayList<Integer> images = new ArrayList<Integer>();
        
        Log.v("TONY", "# of hand cards: " + cards.size());
        for (Card card : cards) {
            Log.v("TONY", "A hand card exists: " + card.name + " image: " + card.image);
            images.add(getResources().getIdentifier(card.image , "drawable", parentActivity.getPackageName()));
        }
   
        GridView cardGrid = (GridView) playerHandCards.findViewById(R.id.handCardGrid);
        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), images));
        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
                Toast.makeText(getActivity(), "What we would actually do is zoom on card position: " + position + " card: " + cards.get(position).name + " for actions and targets", Toast.LENGTH_SHORT).show();
                Log.v("TONY", "The hand card choosen is: " + cards.get(position).name);

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
        
        cardInput = (EditText) playerHandCards.findViewById(R.id.cardInput);
        cardButton = (Button) playerHandCards.findViewById(R.id.cardButton);
        cardButton.setOnClickListener(cardButtonListener);
        
        return playerHandCards;
    }

    OnClickListener cardButtonListener = new OnClickListener() {
        @Override
        public void onClick(View args) {
            String dataPassToStats = cardInput.getText().toString();
            String tabOfStats = ((PlayerActivity) getActivity()).getTabStats();
            PlayerStats playerStats = (PlayerStats) getActivity().getSupportFragmentManager().findFragmentByTag(tabOfStats);
            playerStats.updateStats(dataPassToStats);

            Toast.makeText(getActivity(), "data sent to stats fragment (" + tabOfStats + ")", Toast.LENGTH_SHORT).show();
        }
    };
}
