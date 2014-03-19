#include "comm_func.h"

void tell_user_pid_role(int pid, Player p) {
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

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x02;

    int i;
    for (i = 0; i < 3; i++) {
        cd->s_message[3*i + 1] = pi->players[i]->id;
        cd->s_message[3*i + 2] = pi->distance[i];
        // switch(pi->players[i]->role) {
        //     case NONE:
        //         cd->s_message[3*i + 3] = 0x00;
        //         break;
        //     case SHERIFF:
        //         cd->s_message[3*i + 3] = 0x01;
        //         break;
        //     case DEPUTY:
        //         cd->s_message[3*i + 3] = 0x02;
        //         break;
        //     case OUTLAW:
        //         cd->s_message[3*i + 3] = 0x03;
        //         break;
        //     case RENEGADE:
        //         cd->s_message[3*i + 3] = 0x04;
        //         break;
        // }
        cd->s_message[3*i + 3] = pi->distance[i];
    }

    cd->s_len = 1 + 3*i;

    send_data_to_middleman(uart, cd);
}

void tell_user_all_opponent_blue_lives(int pid, PlayersInfo* pi) {
    return;
}

void tell_user_new_card(int pid, int cid) {
    return;
}

void tell_user_lost_card(int pid, int cid) {
    return;
}

void tell_user_their_turn(int pid) {
    return;
}

void tell_user_play_or_lose_life(int pid) {
    return;
}

void tell_user_zap_or_lose_life(int pid) {
    return;
}

void tell_user_get_life(int pid) {
    return;
}
