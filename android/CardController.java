package com.zap.cards;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Scanner;

public class CardController {
	private Map<Character, Card> validCards;
	private static final String CARD_FILE = "cards.txt";
	private List<Card> playerCards;
	
	public CardController() {
		playerCards = new ArrayList<Card>();
		
		initCards(CARD_FILE);
 
		for (Entry<Character, Card> e : validCards.entrySet())
		{
			Card c = e.getValue();
		    System.out.println(c.name);
		}
	}
	
	public void receiveCard(char id) {
		Card c = validCards.get(Character.valueOf(id));
		playerCards.add(c);
	}
	
	public void discardCard(char id) {
		for (int i = 0; i < playerCards.size(); i++) {
			if (playerCards.get(i).id == id) {
				playerCards.remove(i);
				return;
			}
		}
	}
	
	public List<Card> getCards() {
		List<Card> r = new ArrayList<Card>();
		for (Card c : playerCards) {
			r.add(c);
		}
		return r;
	}
	
	public int numberOfCards() {
		return playerCards.size();
	}
	
	private void initCards(String filename) {
		validCards = new HashMap<Character, Card>();
		File file = new File(filename);
		try {
			Scanner input = new Scanner(file);
			while(input.hasNext()) {
				String nextLine = input.nextLine();
				if (nextLine.charAt(0) != '-') {
					Card card = readCard(nextLine);
					validCards.put(Character.valueOf(card.id), card);
				}
			}
			input.close();
		} catch(FileNotFoundException fnfe) {
			System.err.println(CARD_FILE + " not found");
		}
	}
	
	private Card readCard(String s) {
		String[] l = s.split(" ");		
		char id = (char) Integer.parseInt(l[0]);
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
		char life = l[i].charAt(5);
		boolean forceDiscard;
		if (l[i].charAt(6) == '1') {
			forceDiscard = true;
		} else {
			forceDiscard = false;
		}
		char draw = l[i].charAt(7);
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
		char onePlayerFixed = l[i].charAt(11);
		return new Card(id, name, border, number, suit, 
				zap, missed, life, forceDiscard, draw, 
				onePlayer, allPlayers, onePlayerReachable, 
				onePlayerFixed);
	}
	
	public static void main(String[] args) {
		new CardController();
	}
}