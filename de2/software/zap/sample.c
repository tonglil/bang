#include "comm.h"
#include <unistd.h>

int main() {
	printf("Wait 5 seconds\n");
	usleep(5000000);

    Comm_data* cd = (Comm_data*) malloc(sizeof(Comm_data));

    alt_up_rs232_dev* uart = init_clear_uart(cd);

    while(1) {

    receive_data_from_middleman(uart, cd);

    send_data_to_middleman(uart, cd);

    }

    return 0;
}
