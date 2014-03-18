package com.zap.main;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class CardController {
	private static HashMap<Integer, Card> validCards = null;
	private static final String CARD_FILE = "cards.txt";
	private ArrayList<Card> handCards;
	private ArrayList<Card> blueCards;
	
	public CardController() {
		handCards = new ArrayList<Card>();
		blueCards = new ArrayList<Card>();
		
		if (validCards == null) {
			initValidCards(CARD_FILE);
		}
	}
	
	public void receiveCard(int cid) {
		Card c = validCards.get(Integer.valueOf(cid));
		handCards.add(c);
	}
	
	public void discardCard(int cid) {
		for (int i = 0; i < handCards.size(); i++) {
			if (handCards.get(i).cid == cid) {
				handCards.remove(i);
				return;
			}
		}
		for (int i = 0; i < blueCards.size(); i++) {
			if (blueCards.get(i).cid == cid) {
				blueCards.remove(i);
				return;
			}
		}
	}
	
	public void discardAll() {
		handCards.clear();
		blueCards.clear();
	}
	
	public void placeBlueCard(int cid) {
		Card c = getValidCard(cid);
		if (c.isGunCard()) {
			placeGun(cid);
			return;
		}
		for (int i = 0; i < blueCards.size(); i++) {
			Card tmpCard = blueCards.get(i);
			if (tmpCard.name.compareTo(c.name) == 0) {
				blueCards.remove(i);
				break;
			}
		}
		for (int i = 0; i < handCards.size(); i++) {
			Card tmpCard = handCards.get(i);
			if (tmpCard.cid == cid && tmpCard.border == 'L') {
				handCards.remove(i);
				blueCards.add(tmpCard);
				return;
			}
		}
	}

	private void placeGun(int cid) {
		for (int i = 0; i < blueCards.size(); i++) {
			Card c = blueCards.get(i);
			if (c.isGunCard()) {
				blueCards.remove(i);
				break;
			}
		}
		for (int i = 0; i  < handCards.size(); i++) {
			Card c = handCards.get(i);
			if (c.cid == cid && c.border == 'L') {
				handCards.remove(i);
				blueCards.add(c);
				return;
			}
		}
	}
	
	public Card getHandCard(int cid) {
		for (Card c : handCards) {
			if (c.cid == cid) {
				return c;
			}
		}
		return null;
	}
	
	public ArrayList<Card> getHandCards() {
		ArrayList<Card> r = new ArrayList<Card>();
		for (Card c : handCards) {
			r.add(c);
		}
		return r;
	}
	
	public ArrayList<Card> getBlueCards() {
		ArrayList<Card> r = new ArrayList<Card>();
		for (Card c : blueCards) {
			r.add(c);
		}
		return r;
	}
	
	public int numberOfHandCards() {
		return handCards.size();
	}
	
	public int numberOfBlueCards() {
		return blueCards.size();
	}
	
	public static Card getValidCard(int cid) {
		return validCards.get(new Integer(cid));
	}
	
	private void initValidCards(String filename) {
		validCards = new HashMap<Integer, Card>();
		File file = new File(filename);
		try {
			Scanner input = new Scanner(file);
			while(input.hasNext()) {
				String nextLine = input.nextLine();
				if (nextLine.charAt(0) != '-') {
					Card card = readCard(nextLine);
					validCards.put(Integer.valueOf(card.cid), card);
				}
			}
			input.close();
		} catch(FileNotFoundException fnfe) {
			System.err.println(CARD_FILE + " not found");
		}
	}
	
	private Card readCard(String s) {
		String[] l = s.split(" ");		
		int id = Integer.parseInt(l[0]);
		String name = l[1];
		int i;
		for(i = 2; i < l.length-1; i++) {
			name += " " + l[i];
		}
		char border = l[i].charAt(0);
		char number = l[i].charAt(1);
		char suit = l[i].charAt(2);
		boolean zap;
		if (l[i].charAt(3) == '1') {
			zap = true;
		} else {
			zap = false;
		}
		boolean missed;
		if (l[i].charAt(4) == '1') {
			missed = true;
		} else {
			missed = false;
		}
		int life = Integer.parseInt(String.valueOf(l[i].charAt(5)));
		boolean forceDiscard;
		if (l[i].charAt(6) == '1') {
			forceDiscard = true;
		} else {
			forceDiscard = false;
		}
		int draw = Integer.parseInt(String.valueOf(l[i].charAt(7)));
		boolean onePlayer;
		if (l[i].charAt(8) == '1') {
			onePlayer = true;
		} else {
			onePlayer = false;
		}
		boolean allPlayers;
		if (l[i].charAt(9) == '1') {
			allPlayers = true;
		} else {
			allPlayers = false;
		}
		boolean onePlayerReachable;
		if (l[i].charAt(10) == '1') {
			onePlayerReachable = true;
		} else {
			onePlayerReachable = false;
		}
		int onePlayerFixed = Integer.parseInt(String.valueOf(l[i].charAt(11)));
		return new Card(id, name, border, number, suit, 
				zap, missed, life, forceDiscard, draw, 
				onePlayer, allPlayers, onePlayerReachable, 
				onePlayerFixed);
	}
}