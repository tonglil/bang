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

typedef struct CardCtrl {
    Card deck[MAX_CARDS];
    Card discard[MAX_CARDS];
    Card store[NUM_PLAYERS];
    int deckIndex;
    int discardIndex;
} CardCtrl;

void updateStore(PlayerCtrl *playerCtrl, CardCtrl* cardCtrl) {
    int i;
    for (i = 0; i < NUM_PLAYERS); i++) {
        cardCtrl->store[i] = 0;
    }
    for (i = 0; i < getNumAlivePlayers(playerCtrl); i++) {
        cardCtrl->store[i] = cardCtrl->deck[cardCtrl->deckIndex];
        cardCtrl->deckIndex++;
    }
}

void removeCardFromStore(CardCtrl* cardCtrl, Card card) {
    int i;
    for (i = 0; i < NUM_PLAYERS); i++) {
        if (cardCtrl->store[i] == card)
            cardCtrl->store[i] = 0;
            return;
    }
}

typedef struct CardInfo {
    Card card;
    int fromId;
    int toId;
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

void drawCardsForId(int id, CardCtrl* cardCtrl, int count) {
    int i;
    for (i = 0; i < count; i++) {
        sendCard(id, cardCtrl->deck[cardCtrl->deckIndex]);
        cardCtrl->deckIndex++;
    }
}

void sendCard(int id, Card card){
    return;
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

void getLife(int id){
    return;
}

typedef struct Player {
    int id;
    int position;
    role role;
    int lives;
    Card hand[MAX_CARDS];
    Card blueCards[MAX_BLUE_CARDS];
} Player;

typedef struct PlayersInfo {
    Player players[NUM_PLAYERS]
    Player distance[NUM_PLAYERS]
} PlayerInfo;

typedef struct PlayerCtrl {
    Player players[NUM_PLAYERS];
    int turn;
    int subTurn;
} PlayerCtrl;

typedef int Card;

void updateBlueCardsForId(PlayerCtrl* playerCtrl, int id, Card cards[]) {
    playerCtrl->players[id].blueCards = cards;
}

void updateHandForId(PlayerCtrl* playerCtrl, int id, Card cards[]) {
    playerCtrl->players[id].hand = cards;
}

Player getPlayerWithId(PlayerCtrl* playerCtrl, int id) {
    return playerCtrl->players[id];
}

PlayersInfo getPlayersInfoForId(PlayerCtrl* playerCtrl, int id) {
    PlayersInfo playersInfo;
    playersInfo.players = playerCtrl->players;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (playersInfo.players[i].position < 0) {
            playersInfo.distance[i] = -1;
            continue;
        }

        tempDistance =  playersInfo.players[i].position - playersInfo.players[id].position;
        //Get absolute distance
        if (tempDistance < 0)
            tempDistance = -tempDistance;
        //If 7 players are alive
        //eg if position1 = 0 and position2 = 6 distance is 1
        //eg if position1 = 1 and position2 = 6 distance is 2
        //If 3 players are alive
        //eg if position1 = 0 and position2 = 2 distance is 1
        if (tempDistance > getNumAlivePlayers(playerCtrl)/2)
            tempDistance = getNumAlivePlayers(playerCtrl) - tempDistance
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
    } while (playerCtrl->players[playerCtrl->turn].lives <= 0)
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
    } while (playerCtrl->players[playerCtrl->subTurn].lives <= 0)
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

int getNumAlivePlayers(PlayerCtrl* playerCtrl) {
    int count = 0;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (playerCtrl->players[i].lives > 0)
            count++;
    }
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


