#include "comm_func.h"

alt_up_rs232_dev* uart;
Comm_data* cd;

void tell_user_pid_role(int pid, Player* p) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x01;
    cd->s_message[1] = p->role;

    cd->s_len = 2;

    send_data_to_middleman(uart, cd);
}

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x02;

    int i;
    //TODO: change to numplayers
    for (i = 0; i < NUM_PLAYERS; i++) {
        cd->s_message[3*i + 1] = pi->players[i].id;
        cd->s_message[3*i + 2] = pi->distance[i];
        cd->s_message[3*i + 3] = pi->players[i].role;
    }

    cd->s_len = 1 + 3*i;

    send_data_to_middleman(uart, cd);
}

//TODO: need to know how many blue cards player has
void tell_user_all_opponent_blue_lives(int pid, PlayersInfo* pi) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x03;

    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        cd->s_message[3*i + 1] = pi->players[i].id;
        cd->s_message[3*i + 2] = pi->players[i].lives;
        cd->s_message[3*i + 3] = pi->players[i].num_blue;
    }

    int j = 1 + 3*i;
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
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x04;
    cd->s_message[1] = cid;

    cd->s_len = 2;

    send_data_to_middleman(uart, cd);
}

void tell_user_lost_card(int pid, int cid) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x05;
    cd->s_message[1] = cid;

    cd->s_len = 2;

    send_data_to_middleman(uart, cd);
}

void tell_user_their_turn(int pid) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x06;

    cd->s_len = 1;

    send_data_to_middleman(uart, cd);
}

void tell_user_miss_or_lose_life(int pid) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x07;

    cd->s_len = 1;

    send_data_to_middleman(uart, cd);
}

void tell_user_zap_or_lose_life(int pid) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x08;

    cd->s_len = 1;

    send_data_to_middleman(uart, cd);
}

void tell_user_get_life(int pid) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x09;

    cd->s_len = 1;

    send_data_to_middleman(uart, cd);
}

Message receive_interpret_android(void) {
    receive_data_from_middleman(uart, cd);
    int cards[MAX_CARDS] = {0};
    int msg_type = cd->r_message[0];
    switch(msg_type) {
        case 0x11:
        {
            int pid = cd->r_message[1];
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
            int pid = cd->r_message[1];
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
            int pid = cd->r_message[1];
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
            int pid = cd->r_message[1];
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
            int pid = cd->r_message[1];
            return create_message(END_TURN, pid, pid, 0, cards);
            break;
        }
        case 0x16:
        {
            int pid = cd->r_message[1];
            int ncards = cd->r_message[2];
            return create_message(DRAW_CARDS, pid, pid, ncards, cards);
            break;
        }
        case 0x17:
        {
            int pid = cd->r_message[1];
            int lives = cd->r_message[2];
            return create_message(UPDATE_LIVES, pid, pid, lives, cards);
        }
        case 0x18:
        {
            int pid = cd->r_message[1];
            cards[0] = cd->r_message[2];
            return create_message(CHOOSE, pid, pid, 0, cards);
        }
        case 0x19:
        {
            int pid = cd->r_message[1];
            cards[0] = cd->r_message[2];
            return create_message(TRANSFER, pid, pid, 0, cards);
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
