#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include "Player.h"
#include "comm_func.h"
#include "Types.h"

Message receivedFromAndroid();
void sendStore(int id, CardCtrl* cardCtrl);
void sendPanic(int id, Card hand[], Card blueCards[]);
void sendCatBalou(int from, Card hand[], Card blueCards[]);
void sendJail(int id);
void discardCard(int id, Card card);
void getLife(int id);
void startTurn(int id);
void missOrLose(int id);
void zapOrLose(int id);
void sendCard(int id, Card card);
void drawCardsForId(int id, CardCtrl* cardCtrl, int count, PlayerCtrl* playerCtrl);
void startGatling(PlayerCtrl* playerCtrl, int id);
void startDuel(PlayerCtrl* playerCtrl, int to, int from, int self);
void startZap(PlayerCtrl* playerCtrl, int to, int self);
void startAliens(PlayerCtrl* playerCtrl, int id);
void startStore(PlayerCtrl* playerCtrl, int id, CardCtrl* cardCtrl);
void startSaloon(PlayerCtrl* playerCtrl);
void startPanic(PlayerCtrl* playerCtrl, int to, int from, int self);
void startCatBalou(PlayerCtrl* playerCtrl, int to, int from, int self);

#endif /* COMMUNICATION_H_ */
