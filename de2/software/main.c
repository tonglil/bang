#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>

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
    actionType action;
    int id;
    int count;
    Card cards[MAX_CARDS];
} Message;

int main() {
    CardCtrl *cardCtrl = (CardCtrl*)malloc(sizeof(CardCtrl));
    PlayerCtrl* playerCtrl = (PlayerCtrl*)malloc(sizeof(PlayerCtrl));
    initCards(cardCtrl);
    initPlayers(playerCtrl);

    while (1){
        int listening = 1;
        while (listening) {
            Message message = receiveFromAndroid();
            switch (message.messageType) {
            case DRAW_CARDS:
                drawCardsForId(message.id, cardCtrl, message.count);
                break;
            case UPDATE_HAND:
                updateHandForId(playerCtrl, message.id, message.cards);
                break;
            case UPDATE_BLUE:
                updateBlueCardsForId(playerCtrl, message.id, message.cards);
                break;
            case UPDATE_LIVES:
                updateLivesForId(playerCtrl, message.id, message.count);
                break;
            case GATLING:
                startGatling(playerCtrl, messageType.id);
                break;
            case ALIENS:
                startAliens(playerCtrl, messageType.id);
                break;
            case BEER:
                updateLivesForId(playerCtrl, message.id, message.count);
                break;
            case GENERAL_STORE:
                startStore(playerCtrl, message.id,cardCtrl);
                break;
            case SALOON:
                startSaloon(playerCtrl);
                break;
            case ZAP:
                startZap(playerCtrl, message.id);
                break;
            case PANIC:
                startPanic(playerCtrl, message.id, playerCtrl->turn);
                break;
            case CAT_BALOU:
                startCatBalou(playerCtrl, message.id);
                break;
            case DUEL:
                startDuel(playerCtrl, message.id, playerCtrl->turn);
                break;
            case JAIL:
                sendJail(message.id);
                break;
            case END_TURN:
                listening = 0;
            default:
                break;
            }
        }
        endTurn(playerCtrl);
    }
    return 0;
}

typedef enum actionType{
    DISCARD,
    CHOOSE,
    LOSE_LIFE,
    TRANSFER_CARDS
} actionType;

void startGatling(PlayerCtrl* playerCtrl, int id) {
    setSubTurn(playerCtrl,id);
    endSubTurn(playerCtrl);
    missOrLose(getSubTurn(playerCtrl));
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.actionType) {
        case DISCARD:
            updateHandForId(PlayerCtrl, message.id, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                missOrLose(getSubTurn(playerCtrl));
            break;
        case LOSE_LIFE:
            updateLivesForId(playerCtrl, message.id, message.lives);
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
            if (message.actionType == DISCARD) {
                updateHandForId(PlayerCtrl, message.id, message.cards);
                if (turn == to)
                    turn = from;
                else
                    turn = to;
                break;
            } else if (message.actionType = LOSE_LIFE) {
                updateLivesForId(playerCtrl, message.id, message.lives);
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
        if (message.actionType == DISCARD) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.actionType == LOSE_LIFE) {
            updateLivesForId(playerCtrl, message.id, message.lives);
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
        switch (message.actionType) {
        case DISCARD:
            updateHandForId(PlayerCtrl, message.id, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                zapOrLose(getSubTurn(playerCtrl));
            break;
        case LOSE_LIFE:
            updateLivesForId(playerCtrl, message.id, message.lives);
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
    int loop
    while (loop) {
        Message message = receivedFromAndroid();
        switch (message.actionType) {
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
        switch (message.actionType) {
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
            if (message.messageType == UPDATE_LIVES) {
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
        if (message.actionType == TRANSFER_CARDS) {
            transfer = message.cards[0];
            break;
        }
    }
    while (1) {
        Message message = receivedFromAndroid();
        if (message.actionType == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.actionType == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.id, message.cards);
            break;
        }
    }
    sendCard(from, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.actionType == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.actionType == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.id, message.cards);
            break;
        }
    }
}

void startCatBalou(PlayerCtrl* playerCtrl, int to) {
    sendCatBalou(to);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.actionType == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.id, message.cards);
            break;
        } else if (message.actionType == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.id, message.cards);
            break;
        }
    }
}
