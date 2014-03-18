#ifndef TYPES_H_
#define TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PLAYERS 7
#define MAX_NAME 20
#define MAX_CARDS 80
#define MAX_BLUE_CARDS 80

typedef enum { false, true } bool;

typedef enum role {
    SHERIFF,
    DEPUTY,
    OUTLAW,
    RENEGADE
} role;

typedef enum cardStatus {
    IN_DECK,
    IN_HAND,
    PLAYED,
    DISCARDED
} cardStatus;

typedef enum action{
    DISCARD,
    CHOOSE,
    LOSE_LIFE,
    TRANSFER_CARDS
} action;

typedef int Card;

typedef struct CardInfo {
    Card card;
    int fromId;
    int toId;
} CardInfo;

typedef struct CardCtrl {
    Card deck[MAX_CARDS];
    Card discard[MAX_CARDS];
    Card store[NUM_PLAYERS];
    int deckIndex;
    int discardIndex;
} CardCtrl;

typedef enum messageType{
    DRAW_CARDS,
    UPDATE_HAND,
    UPDATE_BLUE,
    UPDATE_LIVES,
    GATLING,
    ALIENS,
    BEER,
    GENERAL_STORE,
    SALOON,
    ZAP,
    PANIC,
    CAT_BALOU,
    DUEL,
    JAIL,
    END_TURN
} messageType;

typedef struct Message{
    messageType type;
    action action;
    int id;
    int count;
    Card cards[MAX_CARDS];
} Message;

typedef struct Player {
    int id;
    int position;
    role role;
    int lives;
    Card hand[MAX_CARDS];
    Card blueCards[MAX_BLUE_CARDS];
} Player;

typedef struct PlayersInfo {
    Player players[NUM_PLAYERS];
    int distance[NUM_PLAYERS];
} PlayersInfo;

typedef struct PlayerCtrl {
    Player players[NUM_PLAYERS];
    int turn;
    int subTurn;
} PlayerCtrl;

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

void initCards(CardCtrl* cardCtrl) {
    int i;
    for (i = 0; i < MAX_CARDS; i++) {
        cardCtrl->deck[i] = i + 1;
        cardCtrl->discard[i] = 0;
    }
    for (i = 0; i < MAX_CARDS; i++)
    {
        int j = i + rand() % (MAX_CARDS - i);
        if (j >= MAX_CARDS)
            printf("Randomizer exceeded maximum index\n");
        int temp = cardCtrl->deck[j];
        cardCtrl->deck[j] = cardCtrl->deck[i];
        cardCtrl->deck[i] = temp;
    }
    printf("Printing deck order:\n");
    for (i = 0; i < MAX_CARDS; i++) {
        printf("%d, ", cardCtrl->deck[i]);
    }
    printf("\n");
    cardCtrl->deckIndex = 0;
    cardCtrl->discardIndex = 0;
}

void sendCard(int id, Card card){
    return;
}

void drawCardsForId(int id, CardCtrl* cardCtrl, int count) {
    int i;
    for (i = 0; i < count; i++) {
        sendCard(id, cardCtrl->deck[cardCtrl->deckIndex]);
        cardCtrl->deckIndex++;
    }
}

void sendStore(int id, CardCtrl* cardCtrl){
    return;
}

void sendPanic(int id) {
    return;
}

void sendCatBalou(int id) {
    return;
}

void sendJail(int id){
    return;
}

void discardCard(int id, Card card){
    return;
}

void getLife(int id){
    return;
}

void startTurn(int id){
    return;
}

void missOrLose(int id){
    return;
}

void zapOrLose(int id){
    return;
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

void useBeerForId(PlayerCtrl* playerCtrl, int id) {
    playerCtrl->players[id].lives++;
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

Message receivedFromAndroid() {
    Message message;
    return message;
}

void startGatling(PlayerCtrl* playerCtrl, int id) {
    setSubTurn(playerCtrl,id);
    endSubTurn(playerCtrl);
    missOrLose(getSubTurn(playerCtrl));
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.action) {
        case DISCARD:
            updateHandForId(playerCtrl, message.id, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                missOrLose(getSubTurn(playerCtrl));
            break;
        case LOSE_LIFE:
            updateLivesForId(playerCtrl, message.id, message.count);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                missOrLose(getSubTurn(playerCtrl));
            break;
        default:
            break;
        }
    }
}

void startDuel(PlayerCtrl* playerCtrl, int to, int from) {
    int turn = to;
    int loop = 1;
    while (loop) {
        missOrLose(turn);
        while (1) {
            Message message = receivedFromAndroid();
            if (message.action == DISCARD) {
                updateHandForId(playerCtrl, message.id, message.cards);
                if (turn == to)
                    turn = from;
                else
                    turn = to;
                break;
            } else if (message.action == LOSE_LIFE) {
                updateLivesForId(playerCtrl, message.id, message.count);
                loop = 0;
                break;
            }
        }
    }
}

void startZap(PlayerCtrl* playerCtrl, int to) {
    missOrLose(to);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.action == DISCARD) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.action == LOSE_LIFE) {
            updateLivesForId(playerCtrl, message.id, message.count);
            break;
        }
    }
}

void startAliens(PlayerCtrl* playerCtrl, int id) {
    setSubTurn(playerCtrl,id);
    endSubTurn(playerCtrl);
    zapOrLose(getSubTurn(playerCtrl));
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.action) {
        case DISCARD:
            updateHandForId(playerCtrl, message.id, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                zapOrLose(getSubTurn(playerCtrl));
            break;
        case LOSE_LIFE:
            updateLivesForId(playerCtrl, message.id, message.count);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                zapOrLose(getSubTurn(playerCtrl));
            break;
        default:
            break;
        }
    }
}

void startStore(PlayerCtrl* playerCtrl, int id, CardCtrl* cardCtrl) {
    updateStore(playerCtrl, cardCtrl);
    setSubTurn(playerCtrl,id);
    sendStore(getSubTurn(playerCtrl), cardCtrl);
    int loop = 1;
    while (loop) {
        Message message = receivedFromAndroid();
        switch (message.action) {
        case CHOOSE:
            sendCard(getSubTurn(playerCtrl), message.cards[0]);
            removeCardFromStore(cardCtrl, message.cards[0]);
            endSubTurn(playerCtrl);
            sendStore(getSubTurn(playerCtrl), cardCtrl);
            loop = 0;
        default:
            break;
        }
    }
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.action) {
        case CHOOSE:
            sendCard(getSubTurn(playerCtrl), message.cards[0]);
            removeCardFromStore(cardCtrl, message.cards[0]);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                sendStore(getSubTurn(playerCtrl), cardCtrl);
            loop = 0;
        default:
            break;
        }
    }
}

void startSaloon(PlayerCtrl* playerCtrl) {
    int i;
    for (i = 0; i < NUM_PLAYERS; ++i) {
        if (playerCtrl->players[i].lives > 0)
            getLife(i);
        while (1) {
            Message message = receivedFromAndroid();
            if (message.type == UPDATE_LIVES) {
                updateLivesForId(playerCtrl, message.id, message.count);
                break;
            }
        }
    }
}

void startPanic(PlayerCtrl* playerCtrl, int to, int from) {
    Card transfer;
    sendPanic(to);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.action == TRANSFER_CARDS) {
            transfer = message.cards[0];
            break;
        }
    }
    while (1) {
        Message message = receivedFromAndroid();
        if (message.action == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.action == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.id, message.cards);
            break;
        }
    }
    sendCard(from, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.action == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.action == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.id, message.cards);
            break;
        }
    }
}

void startCatBalou(PlayerCtrl* playerCtrl, int to) {
    sendCatBalou(to);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.action == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.action == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.id, message.cards);
            break;
        }
    }
}

#endif /* PLAYER_H_ */

