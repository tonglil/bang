package com.zap.main;

public class Card {
	public final int cid;
	public final String name;
	public final char border;
	public final char number;
	public final char suit;
	public final boolean zap;
	public final boolean missed;
	public final int life;
	public final boolean forceDiscard;
	public final int draw;
	public final boolean onePlayer;
	public final boolean allPlayers;
	public final boolean onePlayerReachable;
	public final int onePlayerFixed;
	//space duel, indians, general store
	//space jail, dynamite, barrel, scope, mustang
	Card(int cid, String name, char border, char number, char suit, boolean zap, 
			boolean missed, int life, boolean forceDiscard, int draw, boolean onePlayer, 
			boolean allPlayers, boolean onePlayerReachable, int onePlayerFixed) {
		this.cid = cid;
		this.name = name;
		this.border = border;
		this.number = number;
		this.suit = suit;
		this.zap = zap;
		this.missed = missed;
		this.life = life;
		this.forceDiscard = forceDiscard;
		this.draw = draw;
		this.onePlayer = onePlayer;
		this.allPlayers = allPlayers;
		this.onePlayerReachable = onePlayerReachable;
		this.onePlayerFixed = onePlayerFixed;
	}	
	
	public boolean isGunCard() {
		if (border == 'L' && onePlayerFixed != 0) {
			return true;
		} else {
			return false;
        }
    }
}
