#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>

#include "Player.h"
#include "Field.h"
#include "Draw.h"
#include "Card.h"
#include "Communication.h"
#include "sys/alt_timestamp.h"

#define switches (volatile char *) 0x0004430
#define leds (char *) 0x0004420

#define FPS 2.0
#define TICKS_PER_FRAME (1000.0 / FPS)
#define PS 256

int main() {
    // INIT COMS
    cd = (Comm_data*) malloc(sizeof(Comm_data));
    uart = init_clear_uart(cd);

    alt_timestamp_start();
    srand(alt_timestamp());
    CardCtrl *cardCtrl = (CardCtrl*)malloc(sizeof(CardCtrl));
    PlayerCtrl* playerCtrl = (PlayerCtrl*)malloc(sizeof(PlayerCtrl));
    initCards(cardCtrl);

    printf("Waiting for players\n");
    while(connected_count < 3) {
    	receivedFromAndroid();
    }
    printf("Got all players\n");

    initPlayers(playerCtrl, connected_count);

    int y;
    for (y = 0; y < connected_count; y++) {
    	if (pid_connected[y] == 1) {
    		tell_user_pid_role(y, playerCtrl->players[y]);
    		Message message = receivedFromAndroid();
			if (message.type == ACKNOWLEDGE);
    	}
    }

    int i, j;
    for (i = 0; i < connected_count; i++) {
    	drawCardsForId(i, cardCtrl, 2, playerCtrl);
    }

    field = malloc(sizeof(Field));
    alt_up_char_buffer_dev *charBuffer = initCharBuffer();
    alt_up_pixel_buffer_dma_dev *pixelBuffer = initPixelBuffer();
    initField(field, playerCtrl, cardCtrl, charBuffer);

	*leds = *switches;
	alt_timestamp_start();
	srand(alt_timestamp());

    Message message;

	tell_user_all_opponent_range_role(playerCtrl->turn, getPlayersInfoForId(playerCtrl, playerCtrl->turn));
	message = receivedFromAndroid();
	if (message.type == ACKNOWLEDGE);
	tell_user_all_opponent_blue_lives(playerCtrl->turn, getPlayersInfoForId(playerCtrl, playerCtrl->turn));
	message = receivedFromAndroid();
	if (message.type == ACKNOWLEDGE);
    tell_user_their_turn(0);
    while (1){
        int listening = 1;
        while (listening) {
            Message message = receivedFromAndroid();
            switch (message.type) {
            case DRAW_CARDS:
                drawCardsForId(message.fromId, cardCtrl, message.count, playerCtrl);
                tell_user_ok(message.fromId);
                break;
            case UPDATE_HAND:
                updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
                break;
            case UPDATE_BLUE:
                updateBlueCardsForId(playerCtrl, message.fromId, message.count, message.cards);
                tell_user_ok(message.fromId);
                break;
            case UPDATE_LIVES:
                updateLivesForId(playerCtrl, message.fromId, message.count);
                break;
            case GATLING:
                startGatling(playerCtrl, message.fromId);
                tell_user_ok(message.fromId);
                break;
            case ALIENS:
                startAliens(playerCtrl, message.fromId);
        		tell_user_ok(message.fromId);
                break;
            case BEER:
            {
            	//TODO
                //updateLivesForId(playerCtrl, message.fromId, message.count);
            	Message message = receivedFromAndroid();
            	if (message.type == UPDATE_LIVES)
            		updateLivesForId(playerCtrl, message.fromId, message.count);
            	updateHand(message.fromId, playerCtrl);
                break;
            }
            case GENERAL_STORE:
                startStore(playerCtrl, message.fromId, cardCtrl);
                break;
            case SALOON:
                startSaloon(playerCtrl, message.fromId);
                tell_user_ok(message.fromId);
                break;
            case ZAP: {
            	startZap(playerCtrl, message.toId, message.fromId, message.self);
            	if (message.self == 0) {
            		tell_user_ok(message.fromId);
            	}
				break;
            }
            case PANIC:
                startPanic(playerCtrl, message.toId, message.fromId, message.self);
            	if (message.self == 0) {
            		tell_user_ok(message.fromId);
            	}
                break;
            case CAT_BALOU:
                startCatBalou(playerCtrl, message.toId, message.fromId, message.self);
            	if (message.self == 0) {
            		tell_user_ok(message.fromId);
            	}
                break;
            case DUEL:
                startDuel(playerCtrl, message.toId, message.fromId, message.self);
            	if (message.self == 0) {
            		tell_user_ok(message.fromId);
            	}
                break;
            case JAIL:
            	//TODO
                tell_user_jail(message.toId, message.cards);
                break;
            case END_TURN:
                listening = 0;
                break;
            default:
                break;
            }
        	alt_up_char_buffer_clear(charBuffer);
        	runField(field, isGameEnd, winningPlayer);
        }
        endTurn(playerCtrl);
    	tell_user_all_opponent_range_role(playerCtrl->turn, getPlayersInfoForId(playerCtrl, playerCtrl->turn));
		message = receivedFromAndroid();
		if (message.type == ACKNOWLEDGE);
    	tell_user_all_opponent_blue_lives(playerCtrl->turn, getPlayersInfoForId(playerCtrl, playerCtrl->turn));
		message = receivedFromAndroid();
		if (message.type == ACKNOWLEDGE);
		int y;
		for (y = 0; y < MAX_CARDS; y++ ) {
			if (playerCtrl->players[playerCtrl->turn].blueCards[y] >= 72 && playerCtrl->players[playerCtrl->turn].blueCards[y] <= 74) {
				drawCardsForId(playerCtrl->turn, cardCtrl, 1, playerCtrl);
				break;
			}
		}
    	tell_user_their_turn(playerCtrl->turn);
        alt_up_char_buffer_clear(charBuffer);
        runField(field, isGameEnd, winningPlayer);
    }
    return 0;
}
