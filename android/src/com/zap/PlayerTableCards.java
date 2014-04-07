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
import android.widget.Toast;

import com.zap.main.Card;
import com.zap.main.Opponent;
import com.zap.main.Player;

public class PlayerTableCards extends Fragment {

    private View playerTableCards;
    
    private Player playerCurrent;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerTableCards = inflater.inflate(R.layout.fragment_player_table_cards, container, false);

        buildCards();

        return this.playerTableCards;
    }

    public void buildCards() {
        GridView cardGrid = (GridView) this.playerTableCards.findViewById(R.id.tableCardGrid);
        PlayerActivity parentActivity = (PlayerActivity) getActivity();

        playerCurrent = parentActivity.getPlayer();
        final ArrayList<Card> cards = playerCurrent.getBlueCards();
        ArrayList<Integer> images = new ArrayList<Integer>();

        Log.v("TONY", "# of table cards: " + cards.size());
        for (Card card : cards) {
            Log.v("TONY", "A table card exists: " + card.name + " image: " + card.image);
            images.add(getResources().getIdentifier(card.image, "drawable", parentActivity.getPackageName()));
        }

        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), images));

    }
}
