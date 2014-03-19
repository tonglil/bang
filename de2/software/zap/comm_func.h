#ifndef COMM_FUNC_H_
#define COMM_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_rs232.h"
#include <string.h>

#include "Player.h"
#include "comm.h"

extern alt_up_rs232_dev* uart;
extern Comm_data* cd;

void tell_user_pid_role(int pid, Player* p);

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi, int alive_players);

void tell_user_all_opponent_blue_lives(int pid, PlayersInfo* pi, int alive_players);

void tell_user_new_card(int pid, int cid);

void tell_user_lost_card(int pid, int cid);

void tell_user_their_turn(int pid);

void tell_user_play_or_lose_life(int pid);

void tell_user_zap_or_lose_life(int pid);

void tell_user_get_life(int pid);

void receive_interpret_android(void);

void tell_de2_user_cards(int pid, int ncards, int* cards);

void tell_de2_blue_cards(int pid, int nbcards, int* bcards);

void tell_de2_user_beer(int pid);

void tell_de2_user_gatling(int pid);

void tell_de2_user_aliens(int pid);

void tell_de2_user_general_store(int pid);

void tell_de2_user_zap(int pid, int pid1);

void tell_de2_user_panic(int pid, int pid1);

void tell_de2_user_cat(int pid, int pid1);

void tell_de2_user_duel(int pid, int pid1);

void tell_de2_user_jail(int pid, int pid1);

void tell_de2_end_turn(int pid);

void tell_de2_new_card(int pid);

#endif
