package com.zap;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridView;
import android.widget.Toast;

public class PlayerCards extends Fragment {

    EditText cardInput;
    Button cardButton;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View playerCards = inflater.inflate(R.layout.fragment_player_cards, container, false);

        cardInput = (EditText) playerCards.findViewById(R.id.cardInput);
        cardButton = (Button) playerCards.findViewById(R.id.cardButton);
        cardButton.setOnClickListener(cardButtonListener);

        GridView cardGrid = (GridView) playerCards.findViewById(R.id.cardGrid);
        cardGrid.setAdapter(new ImageAdapter(getActivity()));

        cardGrid.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
                Toast.makeText(getActivity(), "What we would actually do is zoom on card " + position + "for actions and targets", Toast.LENGTH_SHORT).show();
                Log.v("MY_TAG", "CLICKED. Position: " + position + ". Id: " + id);
            }
        });

        return playerCards;
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
