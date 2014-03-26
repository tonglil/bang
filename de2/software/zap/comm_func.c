#include "comm_func.h"

alt_up_rs232_dev* uart;
Comm_data* cd;
int pid_table[7] = {0, 1, 2, 3, 4, 5, 6};

void tell_user_pid_role(int pid, Player* p) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x01;
    cd->s_message[l++] = p->role;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x02;

    int i;
    //TODO: change to numplayers
    for (i = 0; i < NUM_PLAYERS; i++) {
        cd->s_message[3*i + 1] = pi->players[i].id;
        cd->s_message[3*i + 2] = pi->distance[i];
        cd->s_message[3*i + 3] = pi->players[i].role;
    }

    cd->s_len = l + 3*i;

    send_data_to_middleman(uart, cd);
}

//TODO: need to know how many blue cards player has
void tell_user_all_opponent_blue_lives(int pid, PlayersInfo* pi) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x03;

    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        cd->s_message[3*i + l] = pi->players[i].id;
        cd->s_message[3*i + l + 1] = pi->players[i].lives;
        cd->s_message[3*i + l + 2] = pi->players[i].num_blue;
    }

    int j = l + 3*i;
    int k;
    for (i = 0; i < NUM_PLAYERS; i++) {
        for (k = 0; i < pi->players[i].num_blue; k++)
        {
            cd->s_message[j++] = pi->players[i].blueCards[k];
        }
    }

    cd->s_len = j;

    send_data_to_middleman(uart, cd);
}

void tell_user_new_card(int pid, int cid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x04;
    cd->s_message[l++] = cid;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_lost_card(int pid, int cid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x05;
    cd->s_message[l++] = cid;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_their_turn(int pid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x06;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_miss_or_lose_life(int pid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x07;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_zap_or_lose_life(int pid, int aliensOrDuel) {
    // 1 for Aliens, 0 for Duel
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x08;
    cd->s_message[l++] = aliensOrDuel;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_get_life(int pid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x09;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_ok(int pid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x0a;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_blue_play_infront(int pid, int cid) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x0b;
    cd->s_message[l++] = cid;

    cd->s_len = l;

    send_data_to_middleman(uart, cd);
}

void tell_user_store(int pid, int ncards, int* cards) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x0c;
    cd->s_message[l++] = ncards;

    int i;
    for (i = 0; i < ncards; i++) {
        cd->s_message[i + l] = cards[i];
    }

    cd->s_len = l + i;

    send_data_to_middleman(uart, cd);
}

void tell_user_panic(int pid, int pid1, int nbcards, int* bcards) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x0c;
    cd->s_message[l++] = pid1;
    cd->s_message[l++] = nbcards;

    int i;
    for (i = 0; i < nbcards; i++) {
        cd->s_message[i + l] = bcards[i];
    }

    cd->s_len = l + i;

    send_data_to_middleman(uart, cd);
}

void tel_user_cat_balou(int pid, int pid1, int nbcards, int* bcards) {
    cd->client_id = pid_table[pid];

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    int l = 0;

    cd->s_message[l++] = cd->client_id;
    cd->s_message[l++] = cd->s_len;
    cd->s_message[l++] = 0x0c;
    cd->s_message[l++] = pid1;
    cd->s_message[l++] = nbcards;

    int i;
    for (i = 0; i < nbcards; i++) {
        cd->s_message[i + l] = bcards[i];
    }

    cd->s_len = l + i;

    send_data_to_middleman(uart, cd);
}

Message receive_interpret_android(void) {
    // Message structure:
    // [0] msg_type
    // [1] pid
    receive_data_from_middleman(uart, cd);
    int cards[MAX_CARDS] = {0};
    int msg_type = cd->r_message[0];
    int pid = cd->r_message[1];
    pid_table[pid] = cd->client_id;
    switch(msg_type) {
        case 0x11:
        {
            // tellDE2CardsInHand
            // [2] ncards
            // [3] rest of array is cards
            // ...
            int ncards = cd->r_message[2];
            int i, j = 0;
            for (i = 3; i < ncards + 3; i++) {
                cards[j++] = cd->r_message[i];
            }
            return create_message(UPDATE_HAND, pid, pid, ncards, cards);
            break;
        }
        case 0x12:
        {
            // tellDE2BlueCardsInHand
            // [2] ncards
            // [3] rest of array is bcards
            // ...
            int nbcards = cd->r_message[2];
            int i, j = 0;
            for (i = 3; i < nbcards + 3; i++) {
                cards[j++] = cd->r_message[i];
            }
            return create_message(UPDATE_BLUE, pid, pid, nbcards, cards);
            break;
        }
        case 0x13:
        {
            // tellDE2UserUsedSelf
            // [2] card_type
            //      0x01 BEER
            //      0x02 GATLING
            //      0x03 ALIENS
            //      0x04 GENERAL_STORE
            //      0x05 SALOON
            int card_type = cd->r_message[2];
            switch (card_type) {
                case 0x01:
                    return create_message(BEER, pid, pid, 0, cards);
                    break;
                case 0x02:
                    return create_message(GATLING, pid, pid, 0, cards);
                    break;
                case 0x03:
                    return create_message(ALIENS, pid, pid, 0, cards);
                    break;
                case 0x04:
                    return create_message(GENERAL_STORE, pid, pid, 0, cards);
                    break;
                case 0x05:
                    return create_message(SALOON, pid, pid, 0, cards);
                    break;
                default:
                    break;
            }
            break;
        }
        case 0x14:
        {
            // tellDE2UserUsedOther
            // [2] pid1 (victim)
            // [3] card_type
            //      0x01 ZAP
            //      0x02 PANIC
            //      0x03 CAT_BALOU
            //      0x04 DUEL
            //      0x05 JAIL
            int pid1 = cd->r_message[2];
            int card_type = cd->r_message[3];
            switch (card_type) {
                case 0x01:
                    return create_message(ZAP, pid, pid1, 0, cards);
                    break;
                case 0x02:
                    return create_message(PANIC, pid, pid1, 0, cards);
                    break;
                case 0x03:
                    return create_message(CAT_BALOU, pid, pid1, 0, cards);
                    break;
                case 0x04:
                    return create_message(DUEL, pid, pid1, 0, cards);
                    break;
                case 0x05:
                    return create_message(JAIL, pid, pid1, 0, cards);
                    break;
                default:
                    break;
            }
            break;
        }
        case 0x15:
        {
            // tellDE2UserEndedTurn
            return create_message(END_TURN, pid, pid, 0, cards);
            break;
        }
        case 0x16:
        {
            // tellDE2UserNeedsXCards
            // [2] ncards
            int ncards = cd->r_message[2];
            return create_message(DRAW_CARDS, pid, pid, ncards, cards);
            break;
        }
        case 0x17:
        {
            // tellDE2UserUpdateLives
            // [2] lives
            int lives = cd->r_message[2];
            return create_message(UPDATE_LIVES, pid, pid, lives, cards);
        }
        case 0x18:
        {
            // tellDE2UserPickedCard
            // [2] cid
            cards[0] = cd->r_message[2];
            return create_message(CHOOSE, pid, pid, 0, cards);
        }
        case 0x19:
        {
            // tellDE2UserTransferCard
            // [2] cid
            cards[0] = cd->r_message[2];
            return create_message(TRANSFER, pid, pid, 0, cards);
        }
        case 0x1a:
        {
            // tellDE2OK
            // TODO: custom message for OK
            break;
        }
        default:
            break;
    }
    return create_message(NO_TYPE, 0, 0, 0, cards);
}

Message create_message(messageType type, int fromId, int toId, int count, Card cards[]) {
    Message msg;
    msg.type = type;
    msg.fromId = fromId;
    msg.toId = toId;
    msg.count = count;
    memcpy(msg.cards, cards, sizeof(Card)*MAX_CARDS);

    return msg;
}
