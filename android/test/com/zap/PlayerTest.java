package com.zap;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class PlayerTest {

	@Test
	public void test_ReceiveDiscardCards() {
		Player player = new Player();
		
		player.receiveCard(1);
		assertTrue(player.getNumberOfHandCards() == 1);
		
		player.receiveCard(2);
		assertTrue(player.getNumberOfHandCards() == 2);
		assertTrue(player.getNumberOfBlueCards() == 0);
		
		player.discardCard(3);
		assertTrue(player.getNumberOfHandCards() == 2);

		player.discardCard(2);
		assertTrue(player.getNumberOfHandCards() == 1);
	}
	
	@Test
	public void test_PlayOutOfTurnCards() {
		Player player = new Player();
	
		player.receiveCard(1);
		player.playCard(1);
		assertTrue(player.test_call.compareTo("Not turn") == 0);
		assertTrue(player.getNumberOfHandCards() == 1);
		assertTrue(player.getNumberOfBlueCards() == 0);
	}
	
	@Test
	public void test_PlayNotInHandCards() {
		Player p = new Player();
		p.startTurn();
		p.playCard(1);
		assertTrue(p.test_call.compareTo("card not in hand") == 0);
	}
	
	@Test
	public void test_PlayBlueCards() {
		Player player = new Player();
		
		player.startTurn();
		
		player.receiveCard(1);
		player.playCard(1);
		assertTrue(player.test_call.compareTo("blue card") == 0);
		assertTrue(player.getNumberOfBlueCards() == 1);
		assertTrue(player.getNumberOfHandCards() == 0);
		
		player.receiveCard(2);
		player.playCard(2);
		assertTrue(player.getNumberOfBlueCards() == 1);
		assertTrue(player.getNumberOfHandCards() == 0);
		for (Card c : player.getBlueCards()) {
			assertTrue(c.cid == 2);
			assertTrue(c.name.compareTo("Schofield") == 0);
		}
	}
	
	@Test
	public void test_PlayGunCards() {
		Player player = new Player();
		player.startTurn();
		
		assertTrue(player.getRange() == 1);
		
		player.receiveCard(1);
		player.playCard(1);
		assertTrue(player.getRange() == 2);
		
		player.receiveCard(4);
		player.receiveCard(3);
		player.playCard(4);
		assertTrue(player.getRange() == 5);
		player.playCard(3);
		assertTrue(player.getRange() == 4);
		
		player.receiveCard(5);
		player.playCard(5);
		assertTrue(player.getRange() == 1);
		assertTrue(player.getNumberOfBlueCards() == 1);
		assertTrue(player.getNumberOfHandCards() == 0);
	}
	
	@Test
	public void test_Scope() {
		Player player = new Player();
		player.startTurn();
		
		player.receiveCard(1);
		player.receiveCard(78);
		player.playCard(1);
		player.playCard(78);

		assertTrue(player.getNumberOfBlueCards() == 2);
		assertTrue(player.getRange() == 3);
		
		player.receiveCard(4);
		player.playCard(4);
		assertTrue(player.getNumberOfBlueCards() == 2);
		assertTrue(player.getRange() == 6);
	}
	
	@Test
	public void test_LivesBeersSaloon() {
		Player player = new Player();
		player.startTurn();
		assertTrue(player.getLives() == 4);
		assertFalse(player.isDead());
		
		player.setLives(2);
		assertTrue(player.getLives() == 2);
		assertFalse(player.isDead());

		player.receiveCard(46);		
		player.playCard(46);
		assertTrue(player.test_call.compareTo("drinkBeer") == 0);
		
		player.receiveCard(71);
		player.playCard(71);
		assertTrue(player.test_call.compareTo("goToSaloon") == 0);
		
		player.setLives(6);
		assertTrue(player.getLives() == 4);
		assertFalse(player.isDead());

		player.setLives(0);
		assertTrue(player.getLives() == 0);
		assertTrue(player.isDead());
		
		player.setLives(1);
		assertTrue(player.getLives() == 0);
		assertTrue(player.isDead());
	}
	
	@Test
	public void test_Roles() {
		Player p = new Player();
		p.startTurn();
		p.setRole("Sheriff");
		assertTrue(p.getLives() == 5);
		p.setRole("Deputy");
		assertTrue(p.getLives() == 4);
	}
	
	@Test
	public void test_ZapInRange() {
		Player p = new Player();
		p.initOpponent(0, 1, "Deputy");
		p.startTurn();
		p.receiveCard(20);
		p.playCard(20);
		assertTrue(p.test_call.compareTo("zapOpponent") == 0);
	}
	
	@Test
	public void test_ZapOutOfRange() {
		Player p = new Player();
		p.initOpponent(0, 2, "Deputy");
		p.startTurn();
		p.receiveCard(21);
		p.playCard(21);
		assertTrue(p.test_call.compareTo("target not in range") == 0);
		
		p.receiveCard(1);
		p.playCard(1);
		p.playCard(21);
		assertTrue(p.test_call.compareTo("zapOpponent") == 0);
	}
	
	@Test
	public void test_ZapTwice() {
		Player p = new Player();
		p.initOpponent(0, 1, "Deputy");
		p.startTurn();
		p.receiveCard(21);
		p.playCard(21);
		assertTrue(p.test_call.compareTo("zapOpponent") == 0);
		
		p.receiveCard(22);
		p.playCard(22);
		assertTrue(p.test_call.compareTo("cant zap twice") == 0);
		
		p.endTurn();
		p.startTurn();
		p.playCard(22);
		assertTrue(p.test_call.compareTo("zapOpponent") == 0);
	}
	
	@Test
	public void test_Missed() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(35);
		p.playCard(35);
		assertTrue(p.test_call.compareTo("cant play missed") == 0);
	}
	
	@Test
	public void test_PanicInRange() {
		Player p = new Player();
		p.initOpponent(0, 1, "Deputy");
		p.startTurn();
		p.receiveCard(52);
		p.playCard(52);
		assertTrue(p.test_call.compareTo("panicOpponent") == 0);
	}
	
	@Test
	public void test_PanicOutOfRange() {
		Player p = new Player();
		p.initOpponent(0, 2, "Deputy");
		p.startTurn();
		p.receiveCard(52);
		p.playCard(52);
		assertTrue(p.test_call.compareTo("cant panic out of range") == 0);
		
		p.receiveCard(78);
		p.playCard(78);
		p.playCard(52);
		assertTrue(p.test_call.compareTo("panicOpponent") == 0);
	}
	
	@Test
	public void test_CatBalou() {
		Player p = new Player();
		p.initOpponent(0, 3, "Deputy");
		p.startTurn();
		p.receiveCard(56);
		p.playCard(56);
		assertTrue(p.test_call.compareTo("discardOpponentCard") == 0);
	}
	
	@Test
	public void test_StagecoachWellsFargo() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(60);
		p.receiveCard(62);
		p.playCard(60);
		assertTrue(p.test_call.compareTo("drawCards") == 0);
		p.playCard(62);
		assertTrue(p.test_call.compareTo("drawCards") == 0);
		assertTrue(p.getNumberOfHandCards() == 0);
	}
	
	@Test
	public void test_Gatling() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(63);
		p.playCard(63);
		assertTrue(p.test_call.compareTo("zapAll") == 0);
	}
	
	@Test
	public void test_Duel() {
		Player p = new Player();
		p.initOpponent(0, 3, "Deputy");
		p.startTurn();
		p.receiveCard(64);
		p.playCard(64);
		assertTrue(p.test_call.compareTo("duelOpponent") == 0);
	}
	
	@Test
	public void test_Aliens() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(67);
		p.playCard(67);
		assertTrue(p.test_call.compareTo("releaseTheAliens") == 0);
	}
	
	@Test
	public void test_GeneralStore() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(69);
		p.playCard(69);
		assertTrue(p.test_call.compareTo("generalStore") == 0);
	}
	
	@Test
	public void test_JailSheriff() {
		Player p = new Player();
		p.initOpponent(0, 2, Player.SHERIFF);
		p.startTurn();
		p.receiveCard(72);
		p.playCard(72);
		assertTrue(p.test_call.compareTo("cant jail sheriff") == 0);
	}
	
	@Test
	public void test_JailDeputy() {
		Player p = new Player();
		p.initOpponent(0, 2, "Deputy");
		p.startTurn();
		p.receiveCard(72);
		p.playCard(72);
		assertTrue(p.test_call.compareTo("throwInJail") == 0);
	}
	
	@Test
	public void test_Dynamite() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(75);
		p.playCard(75);
		assertTrue(p.test_call.compareTo("blue card") == 0);
		assertTrue(p.getNumberOfBlueCards() == 1);
	}
	
	@Test
	public void test_Barrel() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(76);
		p.playCard(76);
		assertTrue(p.test_call.compareTo("blue card") == 0);
		assertTrue(p.getNumberOfBlueCards() == 1);
	}
	
	@Test
	public void test_Mustang() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(79);
		p.playCard(79);
		assertTrue(p.test_call.compareTo("blue card") == 0);
		assertTrue(p.getNumberOfBlueCards() == 1);
	}
	
	@Test
	public void test_PlayerDead() {
		Player p = new Player();
		p.startTurn();
		assertFalse(p.isDead());
		p.receiveCard(1);
		p.receiveCard(3);
		p.playCard(1);
		assertTrue(p.getNumberOfBlueCards() == 1);
		assertTrue(p.getNumberOfHandCards() == 1);
		p.setLives(0);
		assertTrue(p.isDead());
		assertTrue(p.getNumberOfBlueCards() == 0);
		assertTrue(p.getNumberOfHandCards() == 0);
	}
	
	@Test
	public void test_EndTurn() {
		Player p = new Player();
		p.startTurn();
		p.receiveCard(1);
		p.receiveCard(2);
		p.receiveCard(3);
		p.receiveCard(4);
		p.receiveCard(5);
		assertTrue(p.isTurn());
		p.endTurn();
		assertTrue(p.test_call.compareTo("cant end turn") == 0);
		assertTrue(p.isTurn());
		p.discardCard(3);
		assertTrue(p.getNumberOfHandCards() == 4);
		p.endTurn();
		assertFalse(p.isTurn());
	}
}
