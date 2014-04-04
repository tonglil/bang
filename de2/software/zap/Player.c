#include "Player.h"

void initPlayers(PlayerCtrl* playerCtrl, int count) {
    if (count < 4){
        printf("There has to be at least 4 players");
        return;
    }

    playerCtrl->turn = 0;

    Player defaultPlayer;
    defaultPlayer.id = -1;
    defaultPlayer.pos = -1;
    defaultPlayer.role = NONE;
    defaultPlayer.lives = 0;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        playerCtrl->players[i] = defaultPlayer;
    }

    playerCtrl->players[0].role = SHERIFF;
    playerCtrl->players[1].role = DEPUTY;
    playerCtrl->players[2].role = OUTLAW;
    playerCtrl->players[3].role = OUTLAW;

    if (count > 4)
        playerCtrl->players[4].role = DEPUTY;
    if (count > 5)
        playerCtrl->players[5].role = OUTLAW;
    if (count == 7)
        playerCtrl->players[6].role = RENEGADE;

    //Shuffle roles
    for (i = 0; i < count; i++)
    {
        int j = i + rand() % (count - i);
        if (j >= MAX_CARDS)
            printf("Randomizer exceeded maximum index\n");
        role temp = playerCtrl->players[j].role;
        playerCtrl->players[j].role = playerCtrl->players[i].role;
        playerCtrl->players[i].role = temp;
        playerCtrl->players[i].num_blue = 0;
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        Player *p = &(playerCtrl->players[i]);
        memset(p->blueCards, 0, sizeof(Card)*MAX_CARDS);
        memset(p->hand, 0, sizeof(Card)*MAX_CARDS);
        if (p->role == NONE)
            continue;
        p->id = i;
        p->pos = i;
        if (p->role == SHERIFF)
            p->lives = 5;
        else
            p->lives = 4;

        printf("ID %d, Role %d, Pos %d, Lives %d\n", p->id, p->role, p->pos, p->lives);
    }
}

const char * roleToString(role r) {
    switch (r) {
    case SHERIFF:
        return "Sheriff";
    case DEPUTY:
        return "Deputy";
    case OUTLAW:
        return "Outlaw";
    case RENEGADE:
        return "Renegade";
    default:
        return "None";
    }
}

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

void updateBlueCardsForId(PlayerCtrl* playerCtrl, int id, int nbcards, Card cards[]) {
    memcpy(playerCtrl->players[id].blueCards, cards, sizeof(Card)*MAX_BLUE_CARDS);
    playerCtrl->players[id].num_blue = nbcards;
}

void updateHandForId(PlayerCtrl* playerCtrl, int id, int ncards, Card cards[]) {
    memcpy(playerCtrl->players[id].hand, cards, sizeof(Card)*MAX_CARDS);
    playerCtrl->players[id].num_hand = ncards;
}

void addCard(PlayerCtrl* playerCtrl, int id, int cid) {
	int num_hand = playerCtrl->players[id].num_hand;
	playerCtrl->players[id].hand[num_hand] = cid;
	playerCtrl->players[id].num_hand++;
}

Player getPlayerWithId(PlayerCtrl* playerCtrl, int id) {
    return playerCtrl->players[id];
}

PlayersInfo getPlayersInfoForId(PlayerCtrl* playerCtrl, int id) {
    PlayersInfo playersInfo;
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
    playersInfo.players[i] = playerCtrl->players[i];
        if (playersInfo.players[i].pos < 0) {
            playersInfo.distance[i] = -1;
            continue;
        }
        int tempDistance =  playersInfo.players[i].pos - playersInfo.players[id].pos;
        //Get absolute distance
        if (tempDistance < 0)
            tempDistance = -tempDistance;
        //If 7 players are alive
        //eg if pos1 = 0 and pos2 = 6 distance is 1
        //eg if pos1 = 1 and pos2 = 6 distance is 2
        //If 3 players are alive
        //eg if pos1 = 0 and pos2 = 2 distance is 1
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

int getPlayerIdAtpos(PlayerCtrl* playerCtrl, int pos) {
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (playerCtrl->players[i].pos == pos)
            return i;
    }
    printf("Could not find player with that pos\n");
    return -1;
}

void updateLivesForId(PlayerCtrl* playerCtrl, int id, int lives) {
    if (playerCtrl->players[id].lives <= 0)
        return;
    if (lives == 0) {
        int i;
        for (i = playerCtrl->players[id].pos + 1; i < getNumAlivePlayers(playerCtrl); i++) {
            playerCtrl->players[getPlayerIdAtpos(playerCtrl, i)].pos--;
        }
        playerCtrl->players[id].lives = 0;
        playerCtrl->players[id].pos = -1;
    } else {
        playerCtrl->players[id].lives = lives;
    }
}
