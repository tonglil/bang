#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "Player.h"

typedef enum action{
    DISCARD,
    CHOOSE,
    LOSE_LIFE,
    TRANSFER_CARDS
} action;

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

Message receivedFromAndroid();
void sendStore(int id, CardCtrl* cardCtrl);
void sendPanic(int id);
void sendCatBalou(int id);
void sendJail(int id);
void discardCard(int id, Card card);
void getLife(int id);
void startTurn(int id);
void missOrLose(int id);
void zapOrLose(int id);
void sendCard(int id, Card card);
void drawCardsForId(int id, CardCtrl* cardCtrl, int count);
void startGatling(PlayerCtrl* playerCtrl, int id);
void startDuel(PlayerCtrl* playerCtrl, int to, int from);
void startZap(PlayerCtrl* playerCtrl, int to);
void startAliens(PlayerCtrl* playerCtrl, int id);
void startStore(PlayerCtrl* playerCtrl, int id, CardCtrl* cardCtrl);
void startSaloon(PlayerCtrl* playerCtrl);
void startPanic(PlayerCtrl* playerCtrl, int to, int from);
void startCatBalou(PlayerCtrl* playerCtrl, int to);

#endif /* COMMUNICATION_H_ */