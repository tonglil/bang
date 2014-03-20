package com.zap;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

import com.zap.main.Player;

public class GameActivity extends Activity {

    Button makeGame;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);
        addButtonListenerNewGame();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.game, menu);
        return true;
    }

    public void addButtonListenerNewGame() {
        makeGame = (Button) findViewById(R.id.buttonMakeGame);
        makeGame.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {

                setupGame(view);
            }
        });
    }

    public void setupGame(View view) {
        ArrayList<EditText> names = new ArrayList<EditText>();
        // Get names
        names.add((EditText) findViewById(R.id.player1NameValue));
        names.add((EditText) findViewById(R.id.player2NameValue));
        names.add((EditText) findViewById(R.id.player3NameValue));
        names.add((EditText) findViewById(R.id.player4NameValue));
        names.add((EditText) findViewById(R.id.player5NameValue));
        names.add((EditText) findViewById(R.id.player6NameValue));
        names.add((EditText) findViewById(R.id.player7NameValue));

        ArrayList<Player> players = new ArrayList<Player>();
        // Make players
        for (EditText nameValue : names) {
            String name = nameValue.getText().toString();

            if (name.trim().length() > 0) {
                players.add(new Player(name));
            }
        }

        int pCount = players.size();
        Log.v("MY_TAG", "number of players to initiate: " + pCount);

        // Set roles randomly
        // if 7 players: 1 sheriff, 2 deputies, 3 outlaws, 1 renegade
        // if 6 players: 1 sheriff, 2 deputies, 2 outlaws, 1 renegade
        // if 5 players: 1 sheriff, 1 deputy, 2 outlaws, 1 renegade
        // if 4 players: 1 sheriff, 1 deputy, 2 outlaws
        // if 3 players: 1 sheriff, 1 outlaw, 1 renegade

        int role = (int) Math.random() * 4;
        Log.v("MY_TAG", "rand role num: " + role);

        // Assign characters

        Intent setupIntent = new Intent(getBaseContext(), PlayerActivity.class);

        // Pass players to the player activity
        // String value = "A string value to pass.";
        // setupIntent.putExtra("key", value);

        // Start the activity
        startActivity(setupIntent);
    }

}
