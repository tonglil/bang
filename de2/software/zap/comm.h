#ifndef COMM_H_
#define COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_rs232.h"
#include <string.h>
#include <unistd.h>

typedef struct Comm_data {
    unsigned char data;
    unsigned char parity;
    unsigned char r_message[128];
    unsigned char s_message[128];
    int r_len;
    int s_len;
    int client_id;
    int num_to_receive;
} Comm_data;

alt_up_rs232_dev* init_clear_uart(Comm_data* cd);

void receive_data_from_middleman(alt_up_rs232_dev* uart, Comm_data* cd);

void send_data_to_middleman(alt_up_rs232_dev* uart, Comm_data* cd);

#endif
