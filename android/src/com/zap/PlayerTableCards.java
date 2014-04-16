package com.zap;

import java.util.ArrayList;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridView;

import com.zap.main.Card;
import com.zap.main.Player;

public class PlayerTableCards extends Fragment {

    private View playerTableCards;

    private Player playerCurrent;
    private ArrayList<Card> cards;
    private ArrayList<Integer> images;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        this.playerTableCards = inflater.inflate(R.layout.fragment_player_table_cards, container, false);

        ((PlayerActivity) getActivity()).setTabTableCards(getTag());
        buildCards();

        return this.playerTableCards;
    }

    public void buildCards() {
        GridView cardGrid = (GridView) this.playerTableCards.findViewById(R.id.tableCardGrid);
        PlayerActivity parentActivity = (PlayerActivity) getActivity();

        this.images = new ArrayList<Integer>();
        this.playerCurrent = parentActivity.getPlayer();
        this.cards = this.playerCurrent.getBlueCards();

        for (Card card : this.cards) {
            this.images.add(getResources().getIdentifier(card.image, "drawable", parentActivity.getPackageName()));
        }

        cardGrid.setAdapter(new ArrayListImageAdapter(getActivity(), this.images));
    }
}
