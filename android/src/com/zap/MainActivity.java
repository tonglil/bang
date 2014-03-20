package com.zap;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

    Button newGame;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        addButtonListenerNewGame();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    public void addButtonListenerNewGame() {
        newGame = (Button) findViewById(R.id.buttonNewGame);
        newGame.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.v("MY_TAG", "init new view...");
                setupGame(view);
            }
        });
    }

    public void setupGame(View view) {
        Intent setupIntent = new Intent(getBaseContext(), GameActivity.class);
        String value = "A string value to pass.";
        setupIntent.putExtra("key", value);
        startActivity(setupIntent);
    }

}
