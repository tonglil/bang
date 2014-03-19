#include "comm_func.h"

void tell_user_pid_role(int pid, Player* p) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x01;
    cd->s_message[1] = pid;
    cd->s_message[2] = p->role;

    cd->s_len = 3;

    send_data_to_middleman(uart, cd);
}

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi) {
    cd->client_id = pid;

    // Clear message buffer
    memset(cd->s_message, 0, 128*sizeof(*cd->s_message));

    cd->s_message[0] = 0x02;

    int i;
    //TODO: change to numplayers
    for (i = 0; i < 7; i++) {
        cd->s_message[3*i + 1] = pi->players[i]->id;
        cd->s_message[3*i + 2] = pi->distance[i];
        cd->s_message[3*i + 3] = pi->players[i]->role;
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

void receive_interpret_android(void) {
    receive_data_from_middleman(uart, cd);
    int msg_type = cd->r_message[0];
    switch(msg_type) {
        case 0x11:
            int pid = cd->r_message[1];
            int ncards = cd->r_message[2];
            int i, j = 0;
            int cards[ncards];
            for (i = 3; i < ncards; i++) {
                card[j++] = r_message[i];
            }
            tell_de2_user_cards(pid, ncards, cards);
            break;
        case 0x12:
            int pid = cd->r_message[1];
            int nbcards = cd->r_message[2];
            int i, j = 0;
            int bcards[nbcards];
            for (i = 3; i < nbcards; i++) {
                bcard[j++] = r_message[i];
            }
            tell_de2_blue_cards(pid, nbcards, bcards);
            break;
        case 0x13:
            int pid = cd->r_message[1];
            int card_type = cd->r_message[2];
            switch (card_type) {
                case 0x01:
                    tell_de2_user_beer(pid);
                    break;
                case 0x02:
                    tell_de2_user_gatling(pid);
                    break;
                case 0x03:
                    tell_de2_user_aliens(pid);
                    break;
                case 0x04:
                    tell_de2_user_general_store(pid);
                    break;
                case 0x05:
                    tell_de2_user_saloon(pid);
                    break;
                default:
                    break;
            }
            break;
        case 0x14:
            int pid = cd->r_message[1];
            int pid1 = cd->r_message[2];
            int card_type = cd->r_message[3];
            switch (card_type) {
                case 0x01:
                    tell_de2_user_zap(pid, pid1);
                    break;
                case 0x02:
                    tell_de2_user_panic(pid, pid1);
                    break;
                case 0x03:
                    tell_de2_user_cat(pid, pid1);
                    break;
                case 0x04:
                    tell_de2_user_duel(pid, pid1);
                    break;
                case 0x05:
                    tell_de2_user_jail(pid, pid1);
                    break;
                default:
                    break;
            }
            break;
        case 0x15:
            int pid = cd->r_message[1];
            tell_de2_end_turn(pid);
            break;
        case 0x16:
            int pid = cd->r_message[1];
            int ncards = cd->r_message[2];
            tell_de2_new_card(pid, ncards);
            break;
        default:
            break;
    }
}
