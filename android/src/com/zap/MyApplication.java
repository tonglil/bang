package com.zap;

import java.net.Socket;

import com.zap.main.Player;

import android.app.Application;

public class MyApplication extends Application {
    public Socket sock = null;
    private Player player;

    public void setPlayer(Player player) {
        this.player = player;
    }

    public Player getPlayer() {
        return this.player;
    }
}