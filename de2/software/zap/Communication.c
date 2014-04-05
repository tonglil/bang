#include "Communication.h"

Message receivedFromAndroid() {
    Message message = receive_interpret_android();
    return message;
}

void drawCardsForId(int id, CardCtrl* cardCtrl, int count, PlayerCtrl* playerCtrl) {
    int i;
    for (i = 0; i < count; i++) {
        tell_user_new_card(id, cardCtrl->deck[cardCtrl->deckIndex]);
		printf("Waiting for Updated Hand\n");
		Message message = receivedFromAndroid();
		printf("received/interpreted\n");
		printf("message.type: %d\n", message.type);
		switch (message.type) {
		case UPDATE_HAND:
			updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
			break;
		default:
			printf("garbage\n");
			break;
		}
        cardCtrl->deckIndex++;
    }
}

void startGatling(PlayerCtrl* playerCtrl, int id) {
    setSubTurn(playerCtrl,id);
    endSubTurn(playerCtrl);
    tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
    while (getSubTurn(playerCtrl) != id) {
    	printf("%d,%d\n", getSubTurn(playerCtrl), id);
        Message message = receivedFromAndroid();
        switch (message.type) {
        case UPDATE_BLUE:
            updateBlueCardsForId(playerCtrl, message.fromId, message.count, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
            break;
        case UPDATE_HAND:
            updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
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
    printf("GATLING ENDED\n");
}

void startDuel(PlayerCtrl* playerCtrl, int to, int from) {
    int turn = to;
    int loop = 1;
    while (loop) {
        tell_user_zap_or_lose_life(turn, 0);
        while (1) {
            Message message = receivedFromAndroid();
            if (message.type == UPDATE_HAND) {
                updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
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

void startZap(PlayerCtrl* playerCtrl, int to, int self) {
	if (self == 1) {
		tell_user_ok(to);
		Message message = receivedFromAndroid();
	}
    tell_user_miss_or_lose_life(to);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
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
    tell_user_zap_or_lose_life(getSubTurn(playerCtrl), 1);
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.type) {
        case UPDATE_HAND:
            updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_zap_or_lose_life(getSubTurn(playerCtrl), 1);
            break;
        case UPDATE_LIVES:
            updateLivesForId(playerCtrl, message.fromId, message.count);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
                tell_user_zap_or_lose_life(getSubTurn(playerCtrl), 1);
            break;
        default:
            break;
        }
    }
}

void startStore(PlayerCtrl* playerCtrl, int id, CardCtrl* cardCtrl) {
    updateStore(playerCtrl, cardCtrl);
    setSubTurn(playerCtrl,id);
    int count = getNumAlivePlayers(playerCtrl);
    tell_user_store(getSubTurn(playerCtrl), count--, cardCtrl->store);
    int loop = 1;
    while (loop) {
        Message message = receivedFromAndroid();
        switch (message.type) {
        case CHOOSE:
        	addCard(playerCtrl, getSubTurn(playerCtrl),message.cards[0]);
            removeCardFromStore(cardCtrl, message.cards[0]);
            endSubTurn(playerCtrl);
            tell_user_store(getSubTurn(playerCtrl), count--, cardCtrl->store);
            loop = 0;
        default:
            break;
        }
    }
    while (getSubTurn(playerCtrl) != id) {
        Message message = receivedFromAndroid();
        switch (message.type) {
        case CHOOSE:
        	addCard(playerCtrl, getSubTurn(playerCtrl),message.cards[0]);
            removeCardFromStore(cardCtrl, message.cards[0]);
            endSubTurn(playerCtrl);
            if (getSubTurn(playerCtrl) != id)
            	tell_user_store(getSubTurn(playerCtrl), count--, cardCtrl->store);
            loop = 0;
        default:
            break;
        }
    }
}

void startSaloon(PlayerCtrl* playerCtrl) {
    int i;
    for (i = 0; i < connected_count; ++i) {
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

void startPanic(PlayerCtrl* playerCtrl, int to, int from, int self) {
	if (self == 1) {
		tell_user_ok(to);
		Message message = receivedFromAndroid();
	}
    Card transfer;
    tell_user_panic(from, to, playerCtrl->players[to].num_hand, playerCtrl->players[to].hand, playerCtrl->players[to].num_blue, playerCtrl->players[to].blueCards);
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
            updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
            break;
        } else if (message.type == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.fromId, message.count, message.cards);
            break;
        }
    }
    tell_user_new_card(from, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
            break;
        }
    }
}

void startCatBalou(PlayerCtrl* playerCtrl, int to, int from) {
    Card transfer;
    tell_user_cat_balou(from, to, playerCtrl->players[to].num_hand, playerCtrl->players[to].hand, playerCtrl->players[to].num_blue, playerCtrl->players[to].blueCards);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == CHOOSE) {
            transfer = message.cards[0];
            break;
        }
    }
    tell_user_lost_card(to, transfer);
    while (1) {
        Message message = receivedFromAndroid();
        if (message.type == UPDATE_HAND) {
            updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
            break;
        } else if (message.type == UPDATE_BLUE) {
            updateBlueCardsForId(playerCtrl, message.fromId, message.count, message.cards);
            break;
        }
    }
}
