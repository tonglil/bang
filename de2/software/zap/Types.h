#ifndef TYPES_H_
#define TYPES_H_

#define NUM_PLAYERS 7

#define MAX_CARDS 80
#define MAX_BLUE_CARDS 80

typedef int Card;

typedef enum messageType{
	NO_TYPE,
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
    END_TURN,
    CHOOSE,
    TRANSFER,
    DRAW_CARDS
} messageType;

typedef struct Message{
    messageType type;
    int fromId;
    int toId;
    int count;
    Card cards[MAX_CARDS];
} Message;

#endif /* TYPES_H_ */
