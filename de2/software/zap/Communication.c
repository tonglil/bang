#include "Communication.h"

int isGameEnd = 0;
int winningPlayer = 0;

Message receivedFromAndroid() {
	Message message = receive_interpret_android();
	return message;
}

void checkDead(int to, int from, PlayerCtrl* playerCtrl){
	if (playerCtrl->players[to].lives <= 0) {
		if (playerCtrl->players[to].role == OUTLAW &&
				playerCtrl->players[from].role == RENEGADE){
			isGameEnd = 1;
			winningPlayer = from;
			while(0) {
				printf("The game ended, %d won\n", from);
			}
		} else if (playerCtrl->players[to].role == RENEGADE &&
				playerCtrl->players[from].role == SHERIFF) {
			isGameEnd = 1;
			winningPlayer = from;
			while(0) {
				printf("The game ended, %d won\n", from);
			}
		} else if (playerCtrl->players[to].role == SHERIFF &&
				playerCtrl->players[from].role == OUTLAW) {
			isGameEnd = 1;
			winningPlayer = from;
			while(0) {
				printf("The game ended, %d won\n", from);
			}
		} else if (getNumAlivePlayers(playerCtrl) == 1) {
			isGameEnd = 1;
			winningPlayer = from;
			while(0) {
				printf("The game ended %d won\n", from);
			}
		}
	}
}

void updateHand(int id, PlayerCtrl* playerCtrl){
	tell_update_hand(id);
	while (1) {
		Message message = receivedFromAndroid();
		if (message.type == UPDATE_HAND) {
			updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
			break;
		}
	}
}

void drawCardsForId(int id, CardCtrl* cardCtrl, int count, PlayerCtrl* playerCtrl) {
	int i;
	for (i = 0; i < count; i++) {
		tell_user_new_card(id, cardCtrl->deck[cardCtrl->deckIndex]);
		Message message = receivedFromAndroid();
		switch (message.type) {
		case UPDATE_HAND:
			updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
			break;
		default:
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
			checkDead(message.fromId, id, playerCtrl);
			endSubTurn(playerCtrl);
			if (getSubTurn(playerCtrl) != id)
				tell_user_miss_or_lose_life(getSubTurn(playerCtrl));
			break;
		default:
			break;
		}
	}
	updateHand(id, playerCtrl);
}

void startDuel(PlayerCtrl* playerCtrl, int to, int from, int self) {
	if (self == 1) {
		tell_user_ok(to);
		Message message = receivedFromAndroid();
	}
	int turn = to;
	int killer = from;
	int loop = 1;
	while (loop) {
		tell_user_zap_or_lose_life(turn, 0);
		while (1) {
			Message message = receivedFromAndroid();
			if (message.type == UPDATE_HAND) {
				updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
				if (turn == to) {
					turn = from;
					killer = to;
				} else {
					turn = to;
					killer = from;
				}
				break;
			} else if (message.type == UPDATE_LIVES) {
				updateLivesForId(playerCtrl, message.fromId, message.count);
				checkDead(message.fromId, killer, playerCtrl);
				loop = 0;
				break;
			}
		}
	}
	updateHand(from, playerCtrl);
}

void startZap(PlayerCtrl* playerCtrl, int to, int from, int self) {
	int init = from;
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
			printf("%d's live should be updated to %d\n", message.fromId, message.count);
			checkDead(to, from, playerCtrl);
			break;
		}
	}
	updateHand(from, playerCtrl);
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
			checkDead(message.fromId, id, playerCtrl);
			endSubTurn(playerCtrl);
			if (getSubTurn(playerCtrl) != id)
				tell_user_zap_or_lose_life(getSubTurn(playerCtrl), 1);
			break;
		default:
			break;
		}
	}
	updateHand(id, playerCtrl);
}

void startStore(PlayerCtrl* playerCtrl, int id, CardCtrl* cardCtrl) {
	tell_user_ok(id);
	Message message1 = receivedFromAndroid();

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
			tell_user_new_card(getSubTurn(playerCtrl), message.cards[0]);
			removeCardFromStore(cardCtrl, message.cards[0]);
			receivedFromAndroid();
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
			tell_user_new_card(getSubTurn(playerCtrl), message.cards[0]);
			removeCardFromStore(cardCtrl, message.cards[0]);
			receivedFromAndroid();
			endSubTurn(playerCtrl);
			if (getSubTurn(playerCtrl) != id)
				tell_user_store(getSubTurn(playerCtrl), count--, cardCtrl->store);
			loop = 0;
		default:
			break;
		}
	}
	updateHand(id, playerCtrl);
}

void startSaloon(PlayerCtrl* playerCtrl, int id) {
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
	updateHand(id, playerCtrl);
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

void startCatBalou(PlayerCtrl* playerCtrl, int to, int from, int self) {
	if (self == 1) {
		tell_user_ok(to);
		Message message = receivedFromAndroid();
	}
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
	updateHand(from, playerCtrl);
}

