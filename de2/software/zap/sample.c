#include "comm_func.h"

alt_up_rs232_dev* uart;
Comm_data* cd;

int main() {
    cd = (Comm_data*) malloc(sizeof(Comm_data));

    uart = init_clear_uart(cd);

    tell_user_pid_role(2, 1);

    return 0;
}
