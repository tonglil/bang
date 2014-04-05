#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Card.h"

#define MAX_NAME 20

extern int connected_count;

typedef enum { false, true } bool;

typedef enum role {
    NONE,
    SHERIFF,
    DEPUTY,
    OUTLAW,
    RENEGADE,
} role;

typedef struct Player {
    int id;
    int pos;
    role role;
    int lives;
    Card hand[MAX_CARDS];
    Card blueCards[MAX_BLUE_CARDS];
    int num_blue;
    int num_hand;
} Player;

typedef struct PlayersInfo {
    Player players[NUM_PLAYERS];
    int distance[NUM_PLAYERS];
} PlayersInfo;

typedef struct PlayerCtrl {
    Player players[NUM_PLAYERS];
    int numPlayers;
    int turn;
    int subTurn;
} PlayerCtrl;

void initPlayers(PlayerCtrl* playerCtrl, int count);
int getNumAlivePlayers(PlayerCtrl* playerCtrl);
void updateStore(PlayerCtrl *playerCtrl, CardCtrl* cardCtrl);
void removeCardFromStore(CardCtrl* cardCtrl, Card card);
void updateBlueCardsForId(PlayerCtrl* playerCtrl, int id, int nbcards, Card cards[]);
void updateHandForId(PlayerCtrl* playerCtrl, int id, int ncards, Card cards[]);
void addCard(PlayerCtrl* playerCtrl, int id, int cid);
Player getPlayerWithId(PlayerCtrl* playerCtrl, int id);
PlayersInfo getPlayersInfoForId(PlayerCtrl* playerCtrl, int id);
void endTurn(PlayerCtrl* playerCtrl);
void setSubTurn(PlayerCtrl* playerCtrl, int id);
int getSubTurn(PlayerCtrl* playerCtrl);
void endSubTurn(PlayerCtrl* playerCtrl);
int getPlayerIdAtpos(PlayerCtrl* playerCtrl, int pos);
void updateLivesForId(PlayerCtrl* playerCtrl, int id, int lives);
const char * roleToString(role r);

#endif /* PLAYER_H_ */

