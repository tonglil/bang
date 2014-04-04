package com.zap.main;

import java.util.ArrayList;

public class Opponent {
	private int pid;
	private int lives;
	private int maxLives;
	private int range;
	private String role;
	private ArrayList<Card> blueCards;
	private boolean dead;
	
	public Opponent(int pid, int range, String role) {
		this.pid = pid;
		this.range = range;
		this.role = role;
		this.dead = false;
		blueCards = new ArrayList<Card>();
		if (role.compareTo(Player.SHERIFF) == 0) {
			lives = 5;
			maxLives = 5;
		} else {
			lives = 4;
			maxLives = 4;
		}
	}
	
	public int getPid() {
		return pid;
	}

	public String getRole() {
		return role;
	}
	
	public void setLives(int lives) {
		if (!dead) {
			if (lives <= 0) {
				dead = true;
				discardAll();
			} else if (lives > 0 && lives <= maxLives) {
				this.lives = lives;
			} else if (lives > maxLives) {
				this.lives = maxLives;
			}
		}
	}
	
	public ArrayList<Card> getBlueCards() {
		ArrayList<Card> r = new ArrayList<Card>();
		for (Card c : blueCards) {
			r.add(c);
		}
		return r;
	}
	
	public void setRange(int range) {
		this.range = range;
	}
	
	public int getFixedRange() {
		return this.range;
	}
	
	public int getLives() {
		return lives;
	}
	
	public boolean getDead() {
		return dead;
	}
	
	public int getRange() {
		int range = this.range;
		for (Card c : blueCards) {
			if(c.name.compareTo(Player.MUSTANG) == 0) {
				range++;
			}
		}
		return range;
	}
	
	public void playBlueCard(int cid) {
		Card c = CardController.getValidCard(cid);
		if (c.border == 'L') {
			blueCards.add(c);
		}
	}
	
	public void discardBlueCard(int cid) {
		for (int i = 0; i < blueCards.size(); i++) {
			Card c = blueCards.get(i);
			if (c.cid == cid) {
				blueCards.remove(i);
				return;
			}
		}
	}
	
	public void discardAll() {
		blueCards.clear();
	}
}
