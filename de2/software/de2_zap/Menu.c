#include "Menu.h"

Menu* menu;

int getCursorPosition(Menu* menu) {
    return MENU_TITLE_Y + menu->optionSelected + 1;
}

void runMenu(Menu* menu, alt_up_char_buffer_dev* char_buffer) {
    char buffer[64];
    while(1) {
        if (menu->hasChanged == 0)
            continue;

        if (menu->hasChanged == 2) {
            alt_up_char_buffer_clear(char_buffer);
        }

        menu->hasChanged = 0;

        if (menu->mode == MAIN_MENU) {
            int i;
            int partitionWidth = CHAR_ROW_SIZE/NUM_PLAYERS;
            int lineLength, padding;
            for (i = 0; i < NUM_PLAYERS; i++) {
                lineLength = 8;
                sprintf(buffer, "Player %d", i+1);
                padding = (partitionWidth - lineLength)/2;
                alt_up_char_buffer_string(char_buffer, buffer, padding + partitionWidth*i, 0);
            }

            sprintf(buffer, "Game Start");
            alt_up_char_buffer_string(char_buffer, buffer, 24, MENU_TITLE_Y+1);

            sprintf(buffer, "Instructions");
            alt_up_char_buffer_string(char_buffer, buffer, 24, MENU_TITLE_Y+2);

            sprintf(buffer, "Options");
            alt_up_char_buffer_string(char_buffer, buffer, 24, MENU_TITLE_Y+3);

            sprintf(buffer, "   ");
            alt_up_char_buffer_string(char_buffer, buffer, 20, MENU_TITLE_Y+1);
            alt_up_char_buffer_string(char_buffer, buffer, 20, MENU_TITLE_Y+2);
            alt_up_char_buffer_string(char_buffer, buffer, 20, MENU_TITLE_Y+3);
            sprintf(buffer, ">>>");
            alt_up_char_buffer_string(char_buffer, buffer, 20, getCursorPosition(menu));
        }
    }
}

void initMenu(Menu* menu) {
    menu->optionsMax = 3;
    menu->mode = MAIN_MENU;
    menu->optionSelected = 0;
    menu->hasChanged = 1;
    menu->p1Mode = BANANA;
    menu->p2Mode = CHERRY;
    menu->aiMode = AI_OFF;
    menu->timeMode = MIN_1;
}

void toggleP1(){
    menu->p1Mode++;
    if (menu->p1Mode > WATERMELON)
        menu->p1Mode = 0;
}

void toggleP2(){
    menu->p2Mode++;
    if (menu->p2Mode > WATERMELON)
        menu->p2Mode = 0;
}

void toggleAI(){
    menu->aiMode++;
    if (menu->aiMode > AI_2)
        menu->aiMode = 0;
}

void toggleTime(){
    menu->timeMode++;
    if (menu->timeMode > MIN_10)
        menu->timeMode = 0;
}

void handleMenu(char ascii) {
    if (menu->mode == GAMEOVER) {
        menu->mode == MAIN_MENU;
    }

    if (ascii == '8' && menu->optionSelected > 0) {
        menu->optionSelected--;
        menu->hasChanged = 1;
    } else if (ascii == '2' && menu->optionSelected < menu->optionsMax - 1) {
        menu->optionSelected++;
        menu->hasChanged = 1;
    } else if (ascii == 'Y') {
        menu->hasChanged = 2;
        if (menu->mode == MAIN_MENU) {
            switch (menu->optionSelected) {
            case(0):
        menu->optionSelected = 0;
            menu->mode = GAME;
            menu->optionsMax = 0;
            break;
            case(1):
        menu->optionSelected = 0;
            menu->mode = INSTRUCTIONS;
            menu->optionsMax = 1;
            break;
            case(2):
        menu->optionSelected = 0;
            menu->mode = OPTIONS;
            menu->optionsMax = 5;
            break;
            default:
                break;
            }
        } else if (menu->mode == INSTRUCTIONS) {
            switch (menu->optionSelected) {
            case(0):
        menu->optionSelected = 0;
            menu->mode = MAIN_MENU;
            menu->optionsMax = 3;
            break;

            default:
                break;
            }
        } else if (menu->mode == OPTIONS) {
            switch (menu->optionSelected) {
            case(0):
        menu->optionSelected = 0;
            menu->mode = MAIN_MENU;
            menu->optionsMax = 3;
            break;
            case(1):
        toggleP1();
            break;
            case(2):
        toggleP2();
            break;
            case(3):
        toggleAI();
            break;
            case(4):
        toggleTime();
            break;
            default:
                break;
            }
        }
    }
}
