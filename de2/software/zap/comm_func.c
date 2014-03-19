#include "comm_func.h"

void tell_user_pid_role(int pid, Player* p) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x01;
    switch(p->role) {
        case NONE:
            cd->s_message[1] = 0x00;
            break;
        case SHERIFF:
            cd->s_message[1] = 0x01;
            break;
        case DEPUTY:
            cd->s_message[1] = 0x02;
            break;
        case OUTLAW:
            cd->s_message[1] = 0x03;
            break;
        case RENEGADE:
            cd->s_message[1] = 0x04;
            break;
    }

    cd->s_len = 2;

    send_data_to_middleman(uart, cd);
}

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi, int alive_players) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x02;

    int i;
    for (i = 0; i < alive_players; i++) {
        // if (pi->players[i]->lives > 0) {
            cd->s_message[3*i + 1] = pi->players[i]->id;
            cd->s_message[3*i + 2] = pi->distance[i];
            switch(pi->players[i]->role) {
                case NONE:
                    cd->s_message[3*i + 3] = 0x00;
                    break;
                case SHERIFF:
                    cd->s_message[3*i + 3] = 0x01;
                    break;
                case DEPUTY:
                    cd->s_message[3*i + 3] = 0x02;
                    break;
                case OUTLAW:
                    cd->s_message[3*i + 3] = 0x03;
                    break;
                case RENEGADE:
                    cd->s_message[3*i + 3] = 0x04;
                    break;
            }
        // }
    }

    cd->s_len = 1 + 3*i;

    send_data_to_middleman(uart, cd);
}

//TODO: need to know how many blue cards player has
void tell_user_all_opponent_blue_lives(int pid, PlayersInfo* pi, int alive_players) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x03;

    int i;
    for (i = 0; i < alive_players; i++) {
        // if (pi->players[i]->lives > 0) {
            cd->s_message[3*i + 1] = pi->players[i]->id;
            cd->s_message[3*i + 2] = pi->players[i]->lives;
            cd->s_message[3*i + 3] = pi->players[i]->num_blue;
        // }
    }

    int j = 1 + 3*i;
    int k;
    for (i = 0; i < alive_players; i++) {
        for (k = 0; i < pi->players[i]->num_blue; k++)
        {
            cd->s_message[j++] = pi->players[i]->blueCards[k];
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

void tell_user_play_or_lose_life(int pid) {
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
