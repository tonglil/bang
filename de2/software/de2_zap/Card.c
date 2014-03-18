#include "Card.h"

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
