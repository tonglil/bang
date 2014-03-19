#include "comm_func.h"

void tell_user_pid_role(int pid, int role) {
    cd->client_id = pid;

    s_message[128] = {0};

    s_message[0] = role;

    send_data_to_middleman(alt_up_rs232_dev* uart, Comm_data* cd);
}

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi) {
    return;
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
