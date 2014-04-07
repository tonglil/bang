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
        Player.activity = this;
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
        MyApplication app = (MyApplication) getApplication();
        EditText name = (EditText) findViewById(R.id.playerNameValue);
        Log.v("TONY", name.toString());
        app.setPlayer(new Player(name.toString()));

        Intent setupIntent = new Intent(getBaseContext(), PlayerActivity.class);
        startActivity(setupIntent);
    }

}
