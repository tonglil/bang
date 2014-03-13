package com.zap.cards;

public class Card {
	public final char id;
	public final String name;
	public final char border;
	public final char number;
	public final char suit;
	public final boolean zap;
	public final boolean missed;
	public final char life;
	public final boolean forceDiscard;
	public final char draw;
	public final boolean onePlayer;
	public final boolean allPlayers;
	public final boolean onePlayerReachable;
	public final char onePlayerFixed;
	//space duel, indians, general store
	//space jail, dynamite, barrel, scope, mustang
	Card(char id, String name, char border, char number, char suit, boolean zap, boolean missed, char life, boolean forceDiscard, char draw,
			boolean onePlayer, boolean allPlayers, boolean onePlayerReachable, char onePlayerFixed) {
		this.id = id;
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
}
