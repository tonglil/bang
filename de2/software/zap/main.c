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
	printf("Started, waiting 5 secs\n");
	usleep(5000000);
    cd = (Comm_data*) malloc(sizeof(Comm_data));
    uart = init_clear_uart(cd);

    alt_timestamp_start();
    srand(alt_timestamp());
    CardCtrl *cardCtrl = (CardCtrl*)malloc(sizeof(CardCtrl));
    PlayerCtrl* playerCtrl = (PlayerCtrl*)malloc(sizeof(PlayerCtrl));
    initCards(cardCtrl);
    initPlayers(playerCtrl, 7);

    printf("Waiting for players\n");
    while(connected_count < 2) {
    	receivedFromAndroid();
    }
    printf("Got all players\n");

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
        for (j = 0; j < 2; j++) {
            playerCtrl->players[i].hand[j] = testDrawCard(cardCtrl);
            printf("The drawn card is %d\n", playerCtrl->players[i].hand[j]);
            tell_user_new_card(i, playerCtrl->players[i].hand[j]);
            Message message = receivedFromAndroid();
			if (message.type == UPDATE_HAND) {
				updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
			}
        }
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
        	printf("Waiting for Command\n");
            Message message = receivedFromAndroid();
        	printf("received/interpreted\n");
        	printf("message.type: %d\n", message.type);
            switch (message.type) {
            case DRAW_CARDS:
                drawCardsForId(message.fromId, cardCtrl, message.count, playerCtrl);
                tell_user_ok(message.fromId);
                break;
            case UPDATE_HAND:
                updateHandForId(playerCtrl, message.fromId, message.count, message.cards);
                break;
            case UPDATE_BLUE:
            {
            	printf("message.fromId:%d %d %d %d %d %d", message.fromId, message.cards[0], message.cards[1], message.cards[2], message.cards[3], message.cards[4]);
                updateBlueCardsForId(playerCtrl, message.fromId, message.count, message.cards);
                printf("Came back from bluecards\n");
                break;
            }
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
            	//TODO
                //updateLivesForId(playerCtrl, message.fromId, message.count);
                tell_user_ok(message.fromId);
                break;
            case GENERAL_STORE:
                startStore(playerCtrl, message.fromId, cardCtrl);
                break;
            case SALOON:
                startSaloon(playerCtrl);
                tell_user_ok(message.fromId);
                break;
            case ZAP: {
            	startZap(playerCtrl, message.toId, message.self);
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
            	printf("garbage\n");
                break;
            }
        	alt_up_char_buffer_clear(charBuffer);
        	runField(field);
        }
        printf("exited while loops\n");
        endTurn(playerCtrl);
    	tell_user_all_opponent_range_role(playerCtrl->turn, getPlayersInfoForId(playerCtrl, playerCtrl->turn));
		message = receivedFromAndroid();
		if (message.type == ACKNOWLEDGE);
    	tell_user_all_opponent_blue_lives(playerCtrl->turn, getPlayersInfoForId(playerCtrl, playerCtrl->turn));
		message = receivedFromAndroid();
		if (message.type == ACKNOWLEDGE);
    	tell_user_their_turn(playerCtrl->turn);
        alt_up_char_buffer_clear(charBuffer);
        runField(field);
    }
    return 0;
}
