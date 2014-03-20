package com.zap;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

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
                EditText player1NameValue = (EditText) findViewById(R.id.player1NameValue);
                String player1Name = player1NameValue.getText().toString();
                
                Log.v("ZAP_TAG", "init new game info...");
                Log.v("ZAP_TAG", player1Name);

                setupGame(view);
            }
        });
    }

    public void setupGame(View view) {
        Intent setupIntent = new Intent(getBaseContext(), PlayerActivity.class);
        // Pass over player information
//      actionBar.addTab(actionBar.newTab().setText("Names").setTabListener(tabListener));
//      actionBar.addTab(actionBar.newTab().setText("Roles").setTabListener(tabListener));
//      actionBar.addTab(actionBar.newTab().setText("Characters").setTabListener(tabListener));
//      actionBar.addTab(actionBar.newTab().setText("Done").setTabListener(tabListener));
        
//        Intent setupIntent = new Intent(getBaseContext(), PlayerActivity.class);
//        String value = "A string value to pass.";
//        setupIntent.putExtra("key", value);
        startActivity(setupIntent);
    }

}