#include "Player.h"

int getNumAlivePlayers(PlayerCtrl* playerCtrl) {
    int count = 0;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (playerCtrl->players[i].lives > 0)
            count++;
    }
    return count;
}

void updateStore(PlayerCtrl *playerCtrl, CardCtrl* cardCtrl) {
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        cardCtrl->store[i] = 0;
    }
    for (i = 0; i < getNumAlivePlayers(playerCtrl); i++) {
        cardCtrl->store[i] = cardCtrl->deck[cardCtrl->deckIndex];
        cardCtrl->deckIndex++;
    }
}

void removeCardFromStore(CardCtrl* cardCtrl, Card card) {
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (cardCtrl->store[i] == card)
            cardCtrl->store[i] = 0;
            return;
    }
}

void updateBlueCardsForId(PlayerCtrl* playerCtrl, int id, Card cards[]) {
    memcpy(playerCtrl->players[id].blueCards, cards, sizeof(Card)*MAX_BLUE_CARDS);
}

void updateHandForId(PlayerCtrl* playerCtrl, int id, Card cards[]) {
    memcpy(playerCtrl->players[id].hand, cards, sizeof(Card)*MAX_CARDS);
}

Player getPlayerWithId(PlayerCtrl* playerCtrl, int id) {
    return playerCtrl->players[id];
}

PlayersInfo getPlayersInfoForId(PlayerCtrl* playerCtrl, int id) {
    PlayersInfo playersInfo;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
    playersInfo.players[i] = playerCtrl->players[i];
        if (playersInfo.players[i].position < 0) {
            playersInfo.distance[i] = -1;
            continue;
        }
        int tempDistance =  playersInfo.players[i].position - playersInfo.players[id].position;
        //Get absolute distance
        if (tempDistance < 0)
            tempDistance = -tempDistance;
        //If 7 players are alive
        //eg if position1 = 0 and position2 = 6 distance is 1
        //eg if position1 = 1 and position2 = 6 distance is 2
        //If 3 players are alive
        //eg if position1 = 0 and position2 = 2 distance is 1
        if (tempDistance > getNumAlivePlayers(playerCtrl)/2)
            tempDistance = getNumAlivePlayers(playerCtrl) - tempDistance;
        playersInfo.distance[i] = tempDistance;
    }
    return playersInfo;
}

void endTurn(PlayerCtrl* playerCtrl) {
    do {
        playerCtrl->turn++;
        if (playerCtrl->turn >= NUM_PLAYERS)
            playerCtrl->turn = 0;
    } while (playerCtrl->players[playerCtrl->turn].lives <= 0);
}

void setSubTurn(PlayerCtrl* playerCtrl, int id) {
    playerCtrl->subTurn = id;
}

int getSubTurn(PlayerCtrl* playerCtrl) {
    return playerCtrl->subTurn;
}

void endSubTurn(PlayerCtrl* playerCtrl) {
    do {
        playerCtrl->subTurn++;
        if (playerCtrl->subTurn >= NUM_PLAYERS)
            playerCtrl->subTurn = 0;
    } while (playerCtrl->players[playerCtrl->subTurn].lives <= 0);
}

int getPlayerIdAtPosition(PlayerCtrl* playerCtrl, int pos) {
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (playerCtrl->players[i].position == pos)
            return i;
    }
    printf("Could not find player with that position\n");
    return -1;
}

void updateLivesForId(PlayerCtrl* playerCtrl, int id, int lives) {
    if (playerCtrl->players[id].lives <= 0)
        return;
    if (lives == 0) {
        int i;
        for (i = playerCtrl->players[id].position + 1; i < getNumAlivePlayers(playerCtrl); i++) {
            playerCtrl->players[getPlayerIdAtPosition(playerCtrl, i)].position--;
        }
        playerCtrl->players[id].lives = 0;
        playerCtrl->players[id].position = -1;
    } else {
        playerCtrl->players[id].lives = lives;
    }
}
