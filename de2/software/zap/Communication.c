#include "Communication.h"

Message receivedFromAndroid() {
    Message message = receive_interpret_android();
    return message;
}

void sendStore(int id, CardCtrl* cardCtrl){
    return;
}

void sendPanic(int id, Card hand[], Card blueCards[]) {
    return;
}

void sendCatBalou(int from, Card hand[], Card blueCards[]) {
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

void sendCard(int id, Card card){
    return;
}

void drawCardsForId(int id, CardCtrl* cardCtrl, int count) {
    int i;
    for (i = 0; i < count; i++) {
        tell_user_new_card(id, cardCtrl->deck[cardCtrl->deckIndex]);
        cardCtrl->deckIndex++;
    }
}

void startGatling(PlayerCtrl* playerCtrl, int id) {
    setSubTurn(playerCtrl,id);
    endSubTurn(playerCtrl);
    tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.type) {
        case UPDATE_BLUE:
            updateBlueCardsForId(playerCtrl, message.fromId, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
            break;
        case UPDATE_HAND:
            updateHandForId(playerCtrl, message.fromId, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
            break;
        case UPDATE_LIVES:
            updateLivesForId(playerCtrl, message.fromId, message.count);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
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
        tell_user_zap_or_lose_life(turn);
        while (1) {
            Message message = receivedFromAndroid();
            if (message.type == UPDATE_HAND) {
                updateHandForId(playerCtrl, message.fromId, message.cards);
                if (turn == to)
                    turn = from;
                else
                    turn = to;
                break;
            } else if (message.type == UPDATE_LIVES) {
                updateLivesForId(playerCtrl, message.fromId, message.count);
                loop = 0;
                break;
            }
        }
    }
}

void startZap(PlayerCtrl* playerCtrl, int to) {
    tell_user_miss_or_lose_life(to);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.cards);
            break;
        } else if (message.type == UPDATE_LIVES) {
            updateLivesForId(playerCtrl, message.fromId, message.count);
            break;
        }
    }
}

void startAliens(PlayerCtrl* playerCtrl, int id) {
    setSubTurn(playerCtrl,id);
    endSubTurn(playerCtrl);
    tell_user_zap_or_lose_life(getSubTurn(playerCtrl));
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.type) {
        case UPDATE_HAND:
            updateHandForId(playerCtrl, message.fromId, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_zap_or_lose_life(getSubTurn(playerCtrl));
            break;
        case UPDATE_LIVES:
            updateLivesForId(playerCtrl, message.fromId, message.count);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_zap_or_lose_life(getSubTurn(playerCtrl));
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
    int loop = 1;
    while (loop) {
        Message message = receivedFromAndroid();
        switch (message.type) {
        case CHOOSE:
            tell_user_new_card(getSubTurn(playerCtrl), message.cards[0]);
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
        switch (message.type) {
        case CHOOSE:
            tell_user_new_card(getSubTurn(playerCtrl), message.cards[0]);
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
            tell_user_get_life(i);
        while (1) {
            Message message = receivedFromAndroid();
            if (message.type == UPDATE_LIVES) {
                updateLivesForId(playerCtrl, message.fromId, message.count);
                break;
            }
        }
    }
}

void startPanic(PlayerCtrl* playerCtrl, int to, int from) {
    Card transfer;
    sendPanic(from, playerCtrl->players[to].hand, playerCtrl->players[to].blueCards);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == TRANSFER) {
            transfer = message.cards[0];
            break;
        }
    }
    tell_user_lost_card(to, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.cards);
            break;
        } else if (message.type == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.fromId, message.cards);
            break;
        }
    }
    tell_user_new_card(from, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.cards);
            break;
        } else if (message.type == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.fromId, message.cards);
            break;
        }
    }
}

void startCatBalou(PlayerCtrl* playerCtrl, int to, int from) {
    Card transfer;
    sendCatBalou(from, playerCtrl->players[to].hand, playerCtrl->players[to].blueCards);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == TRANSFER) {
            transfer = message.cards[0];
            break;
        }
    }
    tell_user_lost_card(to, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.cards);
            break;
        } else if (message.type == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.fromId, message.cards);
            break;
        }
    }
}
