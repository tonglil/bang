#include "comm.h"

alt_up_rs232_dev* init_clear_uart(Comm_data* cd) {
    printf("UART Initialization\n");
    alt_up_rs232_dev* uart = alt_up_rs232_open_dev("/dev/rs232");

    printf("Clearing read buffer to start\n");
    while (alt_up_rs232_get_used_space_in_read_FIFO(uart)) {
        alt_up_rs232_read_data(uart, &(cd->data), &(cd->parity));
    }

    return uart;
}

void receive_data_from_middleman(alt_up_rs232_dev* uart, Comm_data* cd) {
    // Now receive the message from the Middleman
    printf("Waiting for data to from the Middleman\n");

    // First byte is the client id received
    while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
    alt_up_rs232_read_data(uart, &(cd->data), &(cd->parity));
    cd->client_id = (int)cd->data;
    printf("Received client ID %d.\n", cd->client_id);
    // Second byte is the number of characters passed by Middleman, don't store
    while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
    alt_up_rs232_read_data(uart, &(cd->data), &(cd->parity));
    // Third byte is the number of characters in our message
    while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
    alt_up_rs232_read_data(uart, &(cd->data), &(cd->parity));
    cd->num_to_receive = (int)cd->data;
    cd->r_len = cd->num_to_receive;
    printf("Received %d characters.\n", cd->num_to_receive);

    int i;
    for (i = 1; i < cd->num_to_receive; i++) {
        while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
        alt_up_rs232_read_data(uart, &(cd->data), &(cd->parity));

        cd->r_message[i] = cd->data;
    }
    for (i = 0; i < cd->num_to_receive; i++) {
        printf("%c", cd->r_message[i]);
    }
    printf("\n");
    printf("Message Complete\n");

    // Acknowledge message received
    if (cd->r_message[0] == 0x2e) {
		alt_up_rs232_write_data(uart, (unsigned char) cd->client_id);
		alt_up_rs232_write_data(uart, 1);
		alt_up_rs232_write_data(uart, 0x2a);
		usleep(1000000);
    } else if (cd->r_message[0] == 0x2f) {
    	// do nothing
    }
}

void send_data_to_middleman(alt_up_rs232_dev* uart, Comm_data* cd) {
    // Reply to the message
    printf("Send the message to the Middleman\n");
    // Write the client id
    alt_up_rs232_write_data(uart, (unsigned char) cd->client_id);
    // Write the message length
    alt_up_rs232_write_data(uart, cd->s_len);
    // If final message
    if (send_last_msg == 1) {
    	alt_up_rs232_write_data(uart, 0x2f);
    } else if (send_last_msg == 0){
    	alt_up_rs232_write_data(uart, 0x2e);
    }
    // Write the message
    int i;
    for (i = 0; i < cd->s_len; i++) {
        alt_up_rs232_write_data(uart, cd->s_message[i]);
    }
    // wait for ack
    // messages to DE2 with type NOT 0x2a will be lost
    if (send_last_msg == 0) {
    	do {
    		receive_data_from_middleman(uart, cd);
    	} while (cd->r_message[1] != 0x2a);
    } else {
    	// I just sent my last msg, don't care about ack
    }
}
