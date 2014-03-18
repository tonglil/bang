#ifndef CARD_H_
#define CARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Types.h"

#define MAX_CARDS 80
#define MAX_BLUE_CARDS 80

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

void initCards(CardCtrl* cardCtrl);

#endif /* CARD_H_ */
