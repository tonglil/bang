#include <stdio.h>
#include "altera_up_avalon_rs232.h"
#include <string.h>

int main() {
    int i;
    unsigned char data;
    unsigned char parity;
    unsigned char r_message[128];
    unsigned char s_message[128];
    int r_len;
    int s_len;

    printf("UART Initialization\n");
    alt_up_rs232_dev* uart = alt_up_rs232_open_dev("/dev/rs232");

    printf("Clearing read buffer to start\n");
    while (alt_up_rs232_get_used_space_in_read_FIFO(uart)) {
        alt_up_rs232_read_data(uart, &data, &parity);
    }

    while(1) {
        // Now receive the message from the Middleman
        printf("Waiting for data to come back from the Middleman\n");

        // First byte is the client id received
        while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
        alt_up_rs232_read_data(uart, &data, &parity);
        int client_id = (int)data;
        printf("Received client ID %d.\n", client_id);
        // Second byte is the number of characters passed by Middleman, don't store
        while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
        alt_up_rs232_read_data(uart, &data, &parity);
        // Third byte is the number of characters in our message
        while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
        alt_up_rs232_read_data(uart, &data, &parity);
        int num_to_receive = (int)data;
        int r_len = num_to_receive;
        printf("Received %d characters.\n", num_to_receive);

        for (i = 0; i < num_to_receive; i++) {
            while (alt_up_rs232_get_used_space_in_read_FIFO(uart) == 0);
            alt_up_rs232_read_data(uart, &data, &parity);

            r_message[i] = data;
        }
        for (i = 0; i < num_to_receive; i++) {
            printf("%c", r_message[i]);
        }
        printf("\n");
        printf("Message Complete\n");

        // Reply to the message
        printf("Return the message to the Middleman and Android client\n");
        for (i = 0; i < r_len; i++) {
            s_message[i] = r_message[i];
        }
        s_len = r_len;
        // Write the client id
        alt_up_rs232_write_data(uart, (unsigned char) client_id);
        // Write the message length
        alt_up_rs232_write_data(uart, s_len);
        // Write the message
        for (i = 0; i < s_len; i++) {
            alt_up_rs232_write_data(uart, s_message[i]);
        }
    }

    return 0;
}
