#include "animations.h"

alt_up_char_buffer_dev *initCharBuffer() {
    //Character Buffer
    alt_up_char_buffer_dev *char_buffer;
    char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
    alt_up_char_buffer_init(char_buffer);

    return char_buffer;
}
