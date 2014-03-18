#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include Player.h

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
