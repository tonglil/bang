#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <system.h>
#include <string.h>

#include "queue_type.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "io.h"

#define HEIGHT 240
#define WIDTH 320
#define X_OFFSET 0 //6
#define Y_OFFSET 0
#define SLEEP 15625
#define COLOR_MAX 65535
#define COLOR_INC 546

alt_up_char_buffer_dev *initCharBuffer();
alt_up_pixel_buffer_dma_dev *initPixelBuffer();

int refresh(alt_up_pixel_buffer_dma_dev *pixel_buffer);

int draw_pixel_fast(alt_up_pixel_buffer_dma_dev *pixel_buffer,
        unsigned int color, unsigned int x, unsigned int y);

#endif
