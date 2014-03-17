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

typedef enum suit {
    DIAMONDS,
    CLUBS,
    HEARTS,
    SPADES
} suit;

typedef enum color {
    BROWN,
    BLUE
}

typedef struct Player {
    int id;
    int position;
    role role;
    int lives;
    Card[MAX_CARDS] hand;
    Card[MAX_BLUE_CARDS] blueCards;
} Player;

typedef struct PlayersInfo {
    Player players[NUM_PLAYERS]
    Player distance[NUM_PLAYERS]
} PlayerInfo;

typedef struct PlayerCtrl {
    Player players[NUM_PLAYERS];
    int turn;
} PlayerCtrl;

typedef int Card;

void updateBlueCardsForPlayerWithId(PlayerCtrl* playerCtrl, int id, Card cards[]) {
    playerCtrl->players[id].blueCards = blueCards;
}

void updateHandForPlayerWithId(PlayerCtrl* playerCtrl, int id, Card cards[]) {
    playerCtrl->players[id].blueCards = blueCards;
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

void endTurn(PlayerCtrl* playerCtrl) {
    do {
        playerCtrl->turn++;
        if (playerCtrl->turn > NUM_PLAYERS)
            playerCtrl->turn = 0;
    } while (playerCtrl->players[playerCtrl->turn].lives <= 0)
}

void loseLifeForPlayerWithId(PlayerCtrl* playerCtrl, int id) {
    if (playerCtrl->players[id].lives <= 0)
        return;
    if (playerCtrl->players[id].lives == 1) {
        int i;
        for (i = playerCtrl->players[id].position + 1; i < getNumAlivePlayers(playerCtrl); i++) {
            playerCtrl->players[getPlayerIdAtPosition(playerCtrl, i)].position--;
        }
        playerCtrl->players[id].lives--;
        playerCtrl->players[id].position = -1;
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
