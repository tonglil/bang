package com.zap.main;

import java.util.ArrayList;
import java.util.HashMap;

//TODO Amitoj: Implement dying mechanics (3 cards for killing outlaw, etc)
//TODO Amitoj: Implement onEvent functions, test dynamite and jail

public class Player {
    private String name;
	private CardController cc;
	private int lives;
	private int maxLives;
	private HashMap<Integer, Opponent> opponents;
	private String role;
	private int pid;
	private boolean turn;
	private boolean dead;
	private boolean zappedThisTurn;
	
	public String test_call;

	public static final String SHERIFF = "Sheriff";
	public static final String MUSTANG = "Mustang";
	private static final String JAIL = "Space Jail";
	private static final String SCOPE = "Scope";
	private static final String DUEL = "Duel";
	private static final String ALIENS = "Aliens";
	private static final String GENERAL_STORE = "General Store";
	private static final String DYNAMITE = "Dynamite";

    public Player(String newName) {
        name = newName;
        cc = new CardController();
        opponents = new HashMap<Integer, Opponent>();
        lives = 4;
        maxLives = 4;
        turn = false;
        dead = false;
        zappedThisTurn = false;
        test_call = "";
    }	
    
    public Player() {
        this("name");
    }
    
    public String getName() {
        return name;
    }
    
	public void setLives(int lives) {
		if (!dead) {
			if (lives <= 0) {
				this.lives = 0;
				dead = true;
				cc.discardAll();
			} else if (lives > 0 && lives <= maxLives) {
				this.lives = lives;
			} else if (lives > maxLives) {
				this.lives = maxLives;
			}
		}
	}
	
	public int getLives() {
		return lives;
	}
	
	public boolean isDead() {
		return dead;
	}
	
	public boolean isTurn() {
		return turn;
	}
	
	public void setPid(int pid) {
		this.pid = pid;
	}
	
	public int getPid() {
		return pid;
	}
	
	public int getNumberOfHandCards() {
		return cc.numberOfHandCards();
	}
	
	public int getNumberOfBlueCards() {
		return cc.numberOfBlueCards();
	}
	
	public ArrayList<Card> getHandCards() {
		return cc.getHandCards();
	}
	
	public ArrayList<Card> getBlueCards() {
		return cc.getBlueCards();
	}
	
	public void setRole(String role) {
		this.role = role;
		if (role.compareTo(SHERIFF) == 0) {
			maxLives = 5;
			lives = 5;
		} else {
			maxLives = 4;
			lives = 4;
		}
	}
	
	public String getRole() {
		return role;
	}
	
	public void initOpponent(int pid, int range, String role) {
		if (opponents.get(new Integer(pid)) == null) {
			opponents.put(new Integer(pid), new Opponent(pid, range, role));
		}
	}
	
	public void setOpponentRange(int pid, int range) {
		Opponent o = opponents.get(new Integer(pid));
		if (o != null) {
			o.setRange(range);
		}
	}
	
	public void setOpponentLives(int pid, int lives) {
		Opponent o = opponents.get(new Integer(pid));
		o.setLives(lives);
	}
	
	//TEST
	public void setOpponentBlueCards(int pid, ArrayList<Integer> cids) {
		Opponent o = opponents.get(new Integer(pid));
		o.discardAll();
		for (Integer i : cids) {
			o.playBlueCard(i.intValue());
		}
	}
	
	public void opponentPlayBlueCard(int pid, int cid) {
		Opponent o = opponents.get(new Integer(pid));
		o.playBlueCard(cid);
	}
	
	public void opponentDiscardBlueCard(int pid, int cid) {
		Opponent o = opponents.get(new Integer(pid));
		o.discardBlueCard(cid);
	}

	// Call this function to give player a card. For example, when he draws a card
	public void receiveCard(int cid) {
		cc.receiveCard(cid);
	}
	
	public void discardCard(int cid) {
		cc.discardCard(cid);
	}
	
	public void startTurn() {
		turn = true;
		zappedThisTurn = false;
		
		//Draw for jail, if in jail
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(JAIL) == 0) {
				cc.discardCard(c.cid);
				Card t = drawOneCard();
				if (t.suit != 'H') {
					forceEndTurn();
					return;
				}
				break;
			}
		}

		//Draw for dynamite, if there is dynamite
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(DYNAMITE) == 0) {
				Card t = drawOneCard();
				if (t.suit == 'S' && t.number >= '2' && t.number <= '9') {
					//TODO: take 3 hits here
					cc.discardCard(c.cid);
				} else {
					//TODO: pass dynamite to next player
					cc.discardCard(c.cid);
				}
				break;
			}
		}
		
		//TODO: TONY blocks?
//		drawCards(2);
	}

	public void endTurn() {
		if (cc.numberOfHandCards() > lives) {
			//TODO Tony: Disallow, force player to discard cards
			test_call = "cant end turn";
		} else {
			//TODO: tell de2 that my turn is over
			turn = false;
		}
	}
	
	public void forceEndTurn() {
		//TODO: tell de2 that my turn is over
		test_call = "forceEndTurn";
		turn = false;
	}
	
	public int getFixedRange() {
		int range = 1;
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(SCOPE) == 0) {
				range++;
			}
		}
		return range;
	}
	
	// Get maximum range of the payer
	public int getRange() {
		int range = 1;
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(SCOPE) == 0) {
				range++;
			} else if (c.isGunCard()) {
				range += c.onePlayerFixed;
				range--;
			}
		}
		return range;
	}
	
	//TODO AMITOJ: test cases
	public int getRangeFromOpponent(int pid) {
		Opponent o = opponents.get(Integer.valueOf(pid));
		int range = o.getFixedRange();
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(MUSTANG) == 0) {
				range++;
			}
		}
		return range;
	}
	
	public boolean hasJail() {
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(JAIL) == 0) {
				return true;
			}
		}
		return false;
	}
	
	public boolean hasDynamite() {
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(DYNAMITE) == 0) {
				return true;
			}
		}
		return false;
	}
	
	public boolean hasBarrel() {
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(JAIL) == 0) {
				return true;
			}
		}
		return false;
	}
	
	public boolean hasMustang() {
		for (Card c : cc.getBlueCards()) {
			if (c.name.compareTo(MUSTANG) == 0) {
				return true;
			}
		}
		return false;
	}
	
	public void playCard(int cid) {
		if (turn) {
			Card c = cc.getHandCard(cid);
			if (c == null) {
				//This should never occur
				test_call = "card not in hand";
			} else {
				if (c.zap) {
					if(c.onePlayerReachable) { //zap
						if(!zappedThisTurn) {
							int pid = 0; //TODO Tony: prompt user for target
							if (checkRange(pid)) {
								zapOpponent(pid);
								zappedThisTurn = true;
								cc.discardCard(cid);
							} else {
								//TODO Tony: target not in range, give player appropriate message
								test_call = "target not in range";
							}
						} else {
							//TODO Tony: tell user he has already zapped
							test_call = "cant zap twice";
						}
					} else if (c.allPlayers) { //gatling
						zapAll();
						cc.discardCard(cid);
					}
				} else if (c.life == 1) {
					if (c.allPlayers) { //saloon
						goToSaloon();
						cc.discardCard(cid);
					} else if(lives < maxLives) { //beer
						drinkBeer();
                        cc.discardCard(cid);
					} else {
						//TODO Tony: tell user he has full lives, so he can't play beer
						test_call = "cant beer";
					}
				} else if(c.border == 'L') {
					if (c.name.compareTo(JAIL) == 0) { //space jail
						int pid = 0; //TODO Tony: prompt user for target
						Opponent o = opponents.get(new Integer(pid));
						if (o.getRole().compareTo(SHERIFF) == 0) {
							//TODO Tony: tell user he can't jail the sheriff
							test_call = "cant jail sheriff";
						} else {
							for (Card t : o.getBlueCards()) {
								if (t.name.compareTo(JAIL) == 0) {
									//TODO Tony: tell user he can't jail someone who is already in jail
									test_call = "cant jail jailed";
									return;
								}
							}
							throwInJail(pid);
							cc.discardCard(cid);
						}
					} else { //the other blue cards
						cc.placeBlueCard(cid);
						//TODO: tell de2 the player's new set of blue cards
						test_call = "blue card";
					}
				} else if (c.missed) { //missed
					//TODO Tony: tell user he can't play a missed card during his turn (it has no effect)
					test_call = "cant play missed";
				} else if (c.draw > 0) {
					if (c.onePlayerFixed == 1) { //panic
						int pid = 0; //TODO Tony: prompt user for target
						if (checkFixedRange(pid)) {
							panicOpponent(pid);
							cc.discardCard(cid);
						} else {
							//TODO Tony: target not in range, give player appropriate message
							test_call = "cant panic out of range";
						}
					} else { //draw card
						drawCards(c.draw);
						cc.discardCard(cid);
					}
				} else if (c.forceDiscard) {
					if (c.onePlayer) { //cat balou
						discardOpponentCard();
						cc.discardCard(cid);
					}
				} else if (c.name.compareTo(DUEL) == 0) {
					int pid = 0; //TODO Tony: prompt user for target
					duelOpponent(pid);
					cc.discardCard(cid);
				} else if (c.name.compareTo(ALIENS) == 0) {
					releaseTheAliens();
					cc.discardCard(cid);
				} else if (c.name.compareTo(GENERAL_STORE) == 0) {
					generalStore();
					cc.discardCard(cid);
				}
			}
		} else {
			//TODO Tony: tell player it isn't his turn
			test_call = "Not turn";
		}
	}
		
	////////////////////////////////////////////////////
	// THE BELOW PUBLIC FUNCTIONS ARE CALLED WHEN AN OPPONENT INITIATES AN ACTION
	
	//TEST THEM ALL
	public void onZap() {
		//TODO: user has choice of playing miss or taking a life (player getting zapped)
		//TODO: let him use a beer if this is a lethal hit
	}
	
	public void onAliens() {
		//TODO: user has choice of playing zap or taking a life (player getting dueled or aliens)
		//TODO: let him use a beer if this is a lethal hit
	}
	
	public void onSaloon() {
		if (lives < maxLives) {
			setLives(lives + 1);;
		}
	}
	
	public void onPanic(int cid) {
		discardCard(cid);
	}
	
	public void onCatBalou(int cid) {
		discardCard(cid);
	}

	public void onJail() {
		//Nothing needs to be done
	}
	
	public void onDuel() {
		
	}
	
	public void onGeneralStore() {
		
	}
	
///////////////////////////////////////////////////////

	private void zapOpponent(int pid) {
		//TODO: tell de2 that this player wants to zap the opponent with playerid=pid
		//This function shouldn't return until either the opponent uses missed, beer, or takes the hit
		test_call = "zapOpponent";
	}
	
	private void zapAll() {
		//TODO: tell de2 that this player wants to zap everyone
		//This function shouldn't return until every opponent uses missed, beer, or takes the hit	
		test_call = "zapAll";
	}
	
	private void goToSaloon() {
		//TODO: tell de2 that everyone gets a life
		//This function shouldn't return until de2 says everything is good
		test_call = "goToSaloon";
	}

	private void drinkBeer() {
		//TODO: tell de2 that you beered
		//This function shouldn't return until de2 says everything is good
		test_call = "drinkBeer";
	}
	
	private void throwInJail(int pid) {
		//TODO: tell de2 that this opponent should be put in jail
		//This function shouldn't return until de2 says everything is good
		test_call = "throwInJail";
	}
	
	private void drawCards(int numCards) {
		//TODO: tell de2 that this player needs numCards cards
		//This function shouldn't return until de2 says everything is good
		test_call = "drawCards";
	}
	
	private Card drawOneCard() {
		//TODO: tell de2 that this player needs 1 card
		test_call = "drawOneCard";
		return CardController.getValidCard(1);
	}
	
	private void panicOpponent(int pid) {
		//TODO: tell de2 that this player wants to panic opponent
		//This function shouldn't return until de2 says everything is good
		test_call = "panicOpponent";
	}
	
	private void discardOpponentCard() {
		//TODO: tell de2 to randomly discard card from all opponents
		//This function shouldn't return until de2 says everything is good
		test_call = "discardOpponentCard";
	}
	
	private void duelOpponent(int pid) {
		//TODO: tell de2 to begin duel with opponent
		//This function shouldn't return until de2 says everything is good
		test_call = "duelOpponent";
	}
	
	private void releaseTheAliens() {
		//TODO: tell de2 to send aliens after everyone
		//This function shouldn't return until de2 says everything is good
		test_call = "releaseTheAliens";
	}
	
	private void generalStore() {
		//TODO: tell de2 to use general store
		//This function shouldn't return until de2 says everything is good
		test_call = "generalStore";
	}

	private boolean checkFixedRange(int pid) {
		Opponent o = opponents.get(new Integer(pid));
		if (o.getRange() <= getFixedRange()) {
			return true;
		} else {
			return false;
		}
	}
	
	// Check if opponent if within range to shoot
	private boolean checkRange(int pid) {
		Opponent o = opponents.get(new Integer(pid));
		if (o.getRange() <= getRange()) {
			return true;
		} else {
			return false;
		}
	}
}