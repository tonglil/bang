#ifndef COMM_FUNC_H_
#define COMM_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_rs232.h"
#include <string.h>

#include "Player.h"
#include "comm.h"
#include "Types.h"

extern alt_up_rs232_dev* uart;
extern Comm_data* cd;
extern int pid_table[7];

void tell_user_pid_role(int pid, Player* p);

// Don't worry about distinction between player vs opponent
// Send all active players' range/roles and blue/lives
void tell_user_all_opponent_range_role(int pid, PlayersInfo pi);

void tell_user_all_opponent_blue_lives(int pid, PlayersInfo pi);

void tell_user_new_card(int pid, int cid);

void tell_user_lost_card(int pid, int cid);

void tell_user_their_turn(int pid);

void tell_user_miss_or_lose_life(int pid);

void tell_user_zap_or_lose_life(int pid, int aliensOrDuel);

void tell_user_get_life(int pid);

void tell_user_ok(int pid);

// Use this for jail and dynamite
void tell_user_blue_play_infront(int pid, int cid);

void tell_user_store(int pid, int ncards, int* cards);

void tell_user_panic(int pid, int pid1, int ncards, int* cards, int nbcards, int* bcards);

void tell_user_cat_balou(int pid, int pid1, int ncards, int* cards, int nbcards, int* bcards);

void tell_user_jail(int pid, int* cards);

Message receive_interpret_android(void);

Message create_message(messageType type, int fromId, int toId, int count, Card cards[]);

#endif
