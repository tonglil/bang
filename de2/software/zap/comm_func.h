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

void tell_user_pid_role(int pid, Player* p);

void tell_user_all_opponent_range_role(int pid, PlayersInfo* pi);

void tell_user_all_opponent_blue_lives(int pid, PlayersInfo* pi);

void tell_user_new_card(int pid, int cid);

void tell_user_lost_card(int pid, int cid);

void tell_user_their_turn(int pid);

void tell_user_miss_or_lose_life(int pid);

void tell_user_zap_or_lose_life(int pid);

void tell_user_get_life(int pid);

Message receive_interpret_android(void);

Message create_message(messageType type, int fromId, int toId, int count, Card cards[]);

#endif
