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

Card testDrawCard(CardCtrl* cardCtrl) {
    cardCtrl->deckIndex++;
    return cardCtrl->deck[cardCtrl->deckIndex - 1];
}

const char * cardToString(Card id) {
    if (id > 0 && id <= 2)
        return "Schofield";
    else if (id <= 3)
        return "RevCarabine";
    else if (id <= 4)
        return "Winchester";
    else if (id <= 6)
        return "Volcanic ";
    else if (id <= 7)
        return "Schofield ";
    else if (id <= 8)
        return "Remington ";
    else if (id <= 33)
        return "Zap!";
    else if (id <= 45)
        return "Missed!";
    else if (id <= 51)
        return "Beer";
    else if (id <= 55)
        return "Panic";
    else if (id <= 59)
        return "Cat Balou";
    else if (id <= 61)
        return "Stagecoach";
    else if (id <= 62)
        return "Wells Fargo";
    else if (id <= 63)
        return "Gatling";
    else if (id <= 66)
        return "Duel";
    else if (id <= 68)
        return "Aliens";
    else if (id <= 70)
        return "Gen. Store";
    else if (id <= 71)
        return "Saloon";
    else if (id <= 74)
        return "Space Jail";
    else if (id <= 75)
        return "Dynamite";
    else if (id <= 77)
        return "Barrel";
    else if (id <= 78)
        return "Scope";
    else if (id <= 80)
        return "Mustang";
    else
        return "Invalid";
}
