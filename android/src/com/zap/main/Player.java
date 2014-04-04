package com.zap.main;

import java.util.ArrayList;
import java.util.HashMap;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.util.Log;

//TODO Amitoj: Implement dying mechanics (3 cards for killing outlaw, etc)
//TODO Amitoj: Implement onEvent functions, test dynamite and jail

public class Player {
    public static Activity activity;

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
    public static final String JAIL = "Space Jail";
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
            Comm.tellDE2UserUpdateLives(pid, this.lives);
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
        if (opponents.get(Integer.valueOf(pid)) == null) {
            opponents.put(Integer.valueOf(pid), new Opponent(pid, range, role));
        }
    }

    public void setOpponentRole(int pid, String role) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        if (o != null) {
            o.setRole(role);
        }
    }

    public void setOpponentRange(int pid, int range) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        if (o != null) {
            o.setRange(range);
        }
    }

    public void setOpponentLives(int pid, int lives) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        o.setLives(lives);
    }

    // TEST
    public void setOpponentBlueCards(int pid, ArrayList<Integer> cids) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        o.discardAll();
        for (Integer i : cids) {
            o.playBlueCard(i.intValue());
        }
    }

    public void opponentPlayBlueCard(int pid, int cid) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        o.playBlueCard(cid);
    }

    public void opponentDiscardBlueCard(int pid, int cid) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        o.discardBlueCard(cid);
    }

    // Call this function to give player a card. For example, when he draws a
    // card
    public void receiveCard(int cid) {
        cc.receiveCard(cid);
    }

    // TODO AMITOJ: add unit tests
    public void receiveBlueCard(int cid) {
        cc.receiveBlueCard(cid);
    }

    public void discardCard(int cid) {
        cc.discardCard(cid);
    }

    public void startTurn() {
        turn = true;
        zappedThisTurn = false;

        // Draw for jail, if in jail
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

        // Draw for dynamite, if there is dynamite
        for (Card c : cc.getBlueCards()) {
            if (c.name.compareTo(DYNAMITE) == 0) {
                Card t = drawOneCard();
                if (t.suit == 'S' && t.number >= '2' && t.number <= '9') {
                    // TODO: take 3 hits here
                    cc.discardCard(c.cid);
                } else {
                    // TODO: pass dynamite to next player
                    cc.discardCard(c.cid);
                }
                break;
            }
        }

        drawCards(2);
    }

    public void endTurn() {
        if (cc.numberOfHandCards() > lives) {
            // TODO Tony: Disallow, force player to discard cards
            test_call = "cant end turn";
        } else {
            // TODO: tell de2 that my turn is over
            turn = false;
            Comm.tellDE2UserEndedTurn(pid);
        }
    }

    public void forceEndTurn() {
        // TODO: tell de2 that my turn is over
        test_call = "forceEndTurn";
        turn = false;
        Comm.tellDE2UserEndedTurn(pid);
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

    public void playCard(int cid) {
        if (turn) {
            Card c = cc.getHandCard(cid);
            if (c == null) {
                // This should never occur
                test_call = "card not in hand";
            } else {
                if (c.zap) {
                    if (c.onePlayerReachable) { // zap
                        if (!zappedThisTurn) {
                            int pid = 1; // TODO Tony: prompt user for target
                            Log.i("colin", "going to checkRange");
                            if (checkRange(pid)) {
                                Log.i("colin", "zapOpponent");
                                zapOpponent(pid);
                                zappedThisTurn = true;
                                cc.discardCard(cid);
                            } else {
                                // TODO Tony: target not in range, give player
                                // appropriate message
                                test_call = "target not in range";
                            }
                        } else {
                            // TODO Tony: tell user he has already zapped
                            test_call = "cant zap twice";
                        }
                    } else if (c.allPlayers) { // gatling
                        zapAll();
                        cc.discardCard(cid);
                    }
                } else if (c.life == 1) {
                    if (c.allPlayers) { // saloon
                        goToSaloon();
                    } else if (lives < maxLives) { // beer
                        drinkBeer();
                    } else {
                        // TODO Tony: tell user he has full lives, so he can't
                        // play beer
                        test_call = "cant beer";
                    }
                } else if (c.border == 'L') {
                    if (c.name.compareTo(JAIL) == 0) { // space jail
                        int pid = 1; // TODO Tony: prompt user for target
                        Opponent o = opponents.get(Integer.valueOf(pid));
                        if (o.getRole().compareTo(SHERIFF) == 0) {
                            // TODO Tony: tell user he can't jail the sheriff
                            test_call = "cant jail sheriff";
                        } else {
                            for (Card t : o.getBlueCards()) {
                                if (t.name.compareTo(JAIL) == 0) {
                                    // TODO Tony: tell user he can't jail
                                    // someone who is already in jail
                                    test_call = "cant jail jailed";
                                    return;
                                }
                            }
                            throwInJail(pid, cid);
                            cc.discardCard(cid);
                        }
                    } else { // the other blue cards
                        cc.placeBlueCard(cid);
                        test_call = "blue card";
                    }
                } else if (c.missed) { // missed
                    // TODO Tony: tell user he can't play a missed card during
                    // his turn (it has no effect)
                    test_call = "cant play missed";
                } else if (c.draw > 0) {
                    if (c.onePlayerFixed == 1) { // panic
                        int pid = 1; // TODO Tony: prompt user for target
                        if (checkFixedRange(pid)) {
                            panicOpponent(pid);
                            cc.discardCard(cid);
                        } else {
                            // TODO Tony: target not in range, give player
                            // appropriate message
                            test_call = "cant panic out of range";
                        }
                    } else { // draw card
                        drawCards(c.draw);
                        cc.discardCard(cid);
                    }
                } else if (c.forceDiscard) {
                    if (c.onePlayer) { // cat balou
                        catBalouOpponentCard(1);
                        cc.discardCard(cid);
                    }
                } else if (c.name.compareTo(DUEL) == 0) {
                    int pid = 1; // TODO Tony: prompt user for target
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
            Log.i("colin", "sending tellDE2CardsInHand");
            Comm.tellDE2CardsInHand(pid, getNumberOfHandCards(), getHandCards());
            Comm.tellDE2BlueCardsInFront(pid, getNumberOfBlueCards(), getBlueCards());
        } else {
            // TODO Tony: tell player it isn't his turn
            test_call = "Not turn";
        }
        Log.i("colin", test_call);
    }

    // //////////////////////////////////////////////////
    // THE BELOW PUBLIC FUNCTIONS ARE CALLED WHEN AN OPPONENT INITIATES AN
    // ACTION

    // TEST THEM ALL
    public void onZap() {
        // TODO: user has choice of playing miss or taking a life (player
        // getting zapped)
        // TODO: let him use a beer if this is a lethal hit
        int missed_cid = 0;
        for (Card c : getHandCards()) {
            if (c.missed) {
                missed_cid = c.cid;
                break;
            }
        }

        CharSequence choices[];
        if (missed_cid != 0) {
            choices = new CharSequence[] { "Take hit", "Use miss" };
        } else {
            choices = new CharSequence[] { "Take hit" };
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(Player.activity);
        builder.setTitle("Select an option");
        builder.setItems(choices, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Boolean userplayedmiss = false;

                int missed_cid = 0;
                for (Card c : getHandCards()) {
                    if (c.missed) {
                        missed_cid = c.cid;
                        break;
                    }
                }

                if (which == 0) {
                    userplayedmiss = false;
                } else {
                    userplayedmiss = true;
                }

                if (userplayedmiss) {
                    discardCard(missed_cid);
                    Comm.tellDE2CardsInHand(pid, getNumberOfHandCards(), getHandCards());
                } else {
                    setLives(lives - 1);
                    Comm.tellDE2UserUpdateLives(pid, lives);
                }
            }
        });
        builder.show();
    }

    public void onAliens() {
        // TODO: user has choice of playing zap or taking a life (player getting
        // dueled or aliens)
        // TODO: let him use a beer if this is a lethal hit
        int zap_cid = 0;
        for (Card c : getHandCards()) {
            if (c.zap) {
                zap_cid = c.cid;
                break;
            }
        }

        CharSequence choices[];
        if (zap_cid != 0) {
            choices = new CharSequence[] { "Take hit", "Use zap" };
        } else {
            choices = new CharSequence[] { "Take hit" };
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(Player.activity);
        builder.setTitle("Select an option");
        builder.setItems(choices, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Boolean userplayedzap = false;

                int zap_cid = 0;
                for (Card c : getHandCards()) {
                    if (c.zap) {
                        zap_cid = c.cid;
                        break;
                    }
                }

                if (which == 0) {
                    userplayedzap = false;
                } else {
                    userplayedzap = true;
                }

                if (userplayedzap) {
                    discardCard(zap_cid);
                    Comm.tellDE2CardsInHand(pid, getNumberOfHandCards(), getHandCards());
                } else {
                    setLives(lives - 1);
                    Comm.tellDE2UserUpdateLives(pid, lives);
                }
            }
        });
        builder.show();
    }

    public void onSaloon() {
        if (lives < maxLives) {
            setLives(lives + 1);
            ;
        }
        Comm.tellDE2UserUpdateLives(this.pid, lives);
    }

    public void onPanic() {
        // show dialog to pick
        // cards are in:
        CharSequence choices[] = new CharSequence[DE2Message.getR_cinfo().get(0).size()];
        int i = 0;
        for (Integer cid : DE2Message.getR_cinfo().get(0)) {
            choices[i++] = CardController.getValidCard(cid.intValue()).name;
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(Player.activity);
        builder.setTitle("Select an card");
        builder.setItems(choices, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                int cid = DE2Message.getR_cinfo().get(0).get(which);
                Comm.tellDE2UserTransferCard(pid, cid);
            }
        });
        builder.show();
    }

    public void onCatBalou() {
        // show dialog to discard
        // cards are in:
        CharSequence choices[] = new CharSequence[DE2Message.getR_cinfo().get(0).size()];
        int i = 0;
        for (Integer cid : DE2Message.getR_cinfo().get(0)) {
            choices[i++] = CardController.getValidCard(cid.intValue()).name;
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(Player.activity);
        builder.setTitle("Select an card");
        builder.setItems(choices, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                int cid = DE2Message.getR_cinfo().get(0).get(which);
                Comm.tellDE2UserPickedCard(pid, cid);
            }
        });
        builder.show();
    }

    public void onJail(int cid) {
        // Nothing needs to be done
        receiveBlueCard(cid);
        Comm.tellDE2BlueCardsInFront(cid, getNumberOfBlueCards(), getBlueCards());
    }

    public void onDuel() {
        // TODO: user has choice of playing zap or taking a life (player getting
        // dueled or aliens)
        // TODO: let him use a beer if this is a lethal hit
        int zap_cid = 0;
        for (Card c : getHandCards()) {
            if (c.zap) {
                zap_cid = c.cid;
                break;
            }
        }

        CharSequence choices[];
        if (zap_cid != 0) {
            choices = new CharSequence[] { "Take hit", "Use zap" };
        } else {
            choices = new CharSequence[] { "Take hit" };
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(Player.activity);
        builder.setTitle("Select an option");
        builder.setItems(choices, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Boolean userplayedzap = false;

                int zap_cid = 0;
                for (Card c : getHandCards()) {
                    if (c.zap) {
                        zap_cid = c.cid;
                        break;
                    }
                }

                if (which == 0) {
                    userplayedzap = false;
                } else {
                    userplayedzap = true;
                }

                if (userplayedzap) {
                    discardCard(zap_cid);
                    Comm.tellDE2CardsInHand(pid, getNumberOfHandCards(), getHandCards());
                } else {
                    setLives(lives - 1);
                    Comm.tellDE2UserUpdateLives(pid, lives);
                }
            }
        });
        builder.show();
    }

    public void onGeneralStore(ArrayList<Integer> card_choices) {
        // pick something from card_choices;
        // show dialog to pick
        // cards are in:
        CharSequence choices[] = new CharSequence[DE2Message.getR_cinfo().get(0).size()];
        int i = 0;
        for (Integer cid : DE2Message.getR_cinfo().get(0)) {
            choices[i++] = CardController.getValidCard(cid.intValue()).name;
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(Player.activity);
        builder.setTitle("Select an card");
        builder.setItems(choices, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                int cid = DE2Message.getR_cinfo().get(0).get(which);
                receiveCard(cid);
                Comm.tellDE2UserPickedCard(pid, cid);
            }
        });
        builder.show();
    }

    public void onLoseCard(int cid) {
        Boolean isHand = false;
        for (Card c : getHandCards()) {
            if (c.cid == cid) {
                isHand = true;
            }
        }
        discardCard(cid);
        Log.i("colin", "sending tellDE2CardsInHand");
        if (isHand) {
            Comm.tellDE2CardsInHand(pid, getNumberOfHandCards(), getHandCards());
        } else {
            Comm.tellDE2BlueCardsInFront(pid, getNumberOfBlueCards(), getBlueCards());
        }
    }

    public void onReceiveCard(int cid) {
        receiveCard(cid);
        Log.i("colin", "sending tellDE2CardsInHand");
        Comm.tellDE2CardsInHand(pid, getNumberOfHandCards(), getHandCards());
    }

    // /////////////////////////////////////////////////////

    private void zapOpponent(int pid) {
        // TODO: tell de2 that this player wants to zap the opponent with
        // playerid=pid
        // This function shouldn't return until either the opponent uses missed,
        // beer, or takes the hit
        test_call = "zapOpponent";
        Comm.tellDE2UserUsedOther(this.pid, pid, "ZAP", 0);
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void zapAll() {
        // TODO: tell de2 that this player wants to zap everyone
        // This function shouldn't return until every opponent uses missed,
        // beer, or takes the hit
        test_call = "zapAll";
        Comm.tellDE2UserUsedSelf(this.pid, "GATLING");
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void goToSaloon() {
        // TODO: tell de2 that everyone gets a life
        // This function shouldn't return until de2 says everything is good
        test_call = "goToSaloon";
        Comm.tellDE2UserUsedSelf(this.pid, "SALOON");
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void drinkBeer() {
        // TODO: tell de2 that you beered
        // This function shouldn't return until de2 says everything is good
        test_call = "drinkBeer";
        Comm.tellDE2UserUsedSelf(this.pid, "BEER");
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void throwInJail(int pid, int cid) {
        // TODO: tell de2 that this opponent should be put in jail
        // This function shouldn't return until de2 says everything is good
        test_call = "throwInJail";
        Comm.tellDE2UserUsedOther(this.pid, pid, "JAIL", cid);
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void drawCards(int numCards) {
        // TODO: tell de2 that this player needs numCards cards
        // This function shouldn't return until de2 says everything is good
        test_call = "drawCards";
        Comm.tellDE2UserNeedsXCards(this.pid, numCards);
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private Card drawOneCard() {
        // TODO: tell de2 that this player needs 1 card
        test_call = "drawOneCard";
        Comm.tellDE2UserNeedsXCards(this.pid, 1);
        // TODOCOLIN: get card
        // while (!DE2Message.isReady())
        // ;
        // DE2Message.setReady(false);
        int card = DE2Message.getR_cinfo().get(0).get(0);
        return CardController.getValidCard(card);
    }

    private void panicOpponent(int pid) {
        // TODO: tell de2 that this player wants to panic opponent
        // This function shouldn't return until de2 says everything is good
        test_call = "panicOpponent";
        Comm.tellDE2UserUsedOther(this.pid, pid, "PANIC", 0);
        while (!DE2Message.isReadyToContinue() && DE2Message.getType() != 0x0d)
            ;
        DE2Message.setReadyToContinue(false);
        // Prompt user to pick a card from blue cards in front of victim or random card from hand of victim
        // The ArrayList<Integer> of blue cards in front is DE2Message.getR_cinfo().get(0);
        // Put cid = 0 to indicate random card from hand
        int cid = 0;
        Comm.tellDE2UserPickedCard(pid, cid);
        return;
    }

    private void catBalouOpponentCard(int pid) {
        // TODO: tell de2 to randomly discard card from all opponents
        // This function shouldn't return until de2 says everything is good
        test_call = "catBalouOpponentCard";
        Comm.tellDE2UserUsedOther(this.pid, pid, "CAT_BALOU", 0);
        while (!DE2Message.isReadyToContinue() && DE2Message.getType() != 0x0e)
            ;
        DE2Message.setReadyToContinue(false);
        // Prompt user to pick a card from blue cards in front of victim or random card from hand of victim
        // The ArrayList<Integer> of blue cards in front is DE2Message.getR_cinfo().get(0);
        // Put cid = 0 to indicate random card from hand
        int cid = 0;
        Comm.tellDE2UserPickedCard(pid, cid);
        return;
    }

    private void duelOpponent(int pid) {
        // TODO: tell de2 to begin duel with opponent
        // This function shouldn't return until de2 says everything is good
        test_call = "duelOpponent";
        Comm.tellDE2UserUsedOther(this.pid, pid, "DUEL", 0);
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void releaseTheAliens() {
        // TODO: tell de2 to send aliens after everyone
        // This function shouldn't return until de2 says everything is good
        test_call = "releaseTheAliens";
        Comm.tellDE2UserUsedSelf(this.pid, "ALIENS");
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private void generalStore() {
        // TODO: tell de2 to use general store
        // This function shouldn't return until de2 says everything is good
        test_call = "generalStore";
        Comm.tellDE2UserUsedSelf(this.pid, "GENERAL_STORE");
        while (!DE2Message.isReadyToContinue())
            ;
        DE2Message.setReadyToContinue(false);
        return;
    }

    private boolean checkFixedRange(int pid) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        if (o.getRange() <= getFixedRange()) {
            return true;
        } else {
            return false;
        }
    }

    // Check if opponent if within range to shoot
    private boolean checkRange(int pid) {
        Opponent o = opponents.get(Integer.valueOf(pid));
        if (o.getRange() <= getRange()) {
            return true;
        } else {
            return false;
        }
    }
}