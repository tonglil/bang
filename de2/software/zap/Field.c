#include "Field.h"

Field* field;

int getCursorPosition(Field* field) {
	return MENU_TITLE_Y + field->optionSelected + 1;
}

void runField(Field* field, int isGameEnd, int winningPlayer) {
	char buffer[64];

	if (isGameEnd){
		Player *p = &(field->playerCtrl->players[winningPlayer]);
		sprintf(buffer, "The winner of Bang! is the %s", roleToString(p->role));
		int lineLength = strlen(buffer);
		int padding = (CHAR_ROW_SIZE - lineLength)/2;
		int padding_y = (CHAR_COL_SIZE)/2;
		alt_up_char_buffer_string(field->charBuffer, buffer, padding, padding_y);
		while(1);
	}

	if (field->hasChanged == 2) {
		alt_up_char_buffer_clear(field->charBuffer);
	}

	field->hasChanged = 0;

	int i;
	int partitionWidth = CHAR_ROW_SIZE/NUM_PLAYERS;
	int lineLength, padding;
	for (i = 0; i < NUM_PLAYERS; i++) {
		Player *p = &(field->playerCtrl->players[i]);

		lineLength = 8;
		sprintf(buffer, "Player %d", p->id);
		padding = (partitionWidth - lineLength)/2;
		alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 0);

		if (p->lives <= 0 || p->role == SHERIFF) {
			lineLength = strlen(roleToString(p->role));
			sprintf(buffer, "%s", roleToString(p->role));
			padding = (partitionWidth - lineLength)/2;
			alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 1);
		} else {
			lineLength = strlen("Role: Unknown");
			sprintf(buffer, "Role: Unknown");
			padding = (partitionWidth - lineLength)/2;
			alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 1);
		}

		if (p->lives > 0) {
			lineLength = 7;
			sprintf(buffer, "Lives %d", p->lives);
		} else {
			lineLength = 5;
			sprintf(buffer, "Dead!");
		}
		padding = (partitionWidth - lineLength)/2;
		alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 2);

		int count = 0;
		int j;

//		lineLength = 10;
//		sprintf(buffer, "Hand Cards");
//		padding = (partitionWidth - lineLength)/2;
//		alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 4 + count);
//
//		for (j = 0; j < MAX_CARDS; j++) {
//			if (p->hand[j] == 0)
//				continue;
//			lineLength = strlen(cardToString(p->hand[j]));
//			sprintf(buffer, "%s", cardToString(p->hand[j]));
//			padding = (partitionWidth - lineLength)/2;
//			alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 6 + count);
//			count++;
//		}

		if (field->playerCtrl->turn == i){
			lineLength = 9;
			sprintf(buffer, "Your Turn");
			padding = (partitionWidth - lineLength)/2;
			alt_up_char_buffer_string(field->charBuffer, buffer, padding + partitionWidth*i, 3);
		}

	}
}

void initField(Field* field,
		PlayerCtrl* playerCtrl,
		CardCtrl* cardCtrl,
		alt_up_char_buffer_dev* charBuffer) {
	field->playerCtrl = playerCtrl;
	field->cardCtrl = cardCtrl;
	field->charBuffer = charBuffer;
}

void toggleP1(){
	field->p1Mode++;
	if (field->p1Mode > WATERMELON)
		field->p1Mode = 0;
}

void toggleP2(){
	field->p2Mode++;
	if (field->p2Mode > WATERMELON)
		field->p2Mode = 0;
}

void toggleAI(){
	field->aiMode++;
	if (field->aiMode > AI_2)
		field->aiMode = 0;
}

void toggleTime(){
	field->timeMode++;
	if (field->timeMode > MIN_10)
		field->timeMode = 0;
}

void handleField(char ascii) {
	if (field->mode == GAMEOVER) {
		field->mode == MAIN_MENU;
	}

	if (ascii == '8' && field->optionSelected > 0) {
		field->optionSelected--;
		field->hasChanged = 1;
	} else if (ascii == '2' && field->optionSelected < field->optionsMax - 1) {
		field->optionSelected++;
		field->hasChanged = 1;
	} else if (ascii == 'Y') {
		field->hasChanged = 2;
		if (field->mode == MAIN_MENU) {
			switch (field->optionSelected) {
			case(0):
        		field->optionSelected = 0;
			field->mode = GAME;
			field->optionsMax = 0;
			break;
			case(1):
        		field->optionSelected = 0;
			field->mode = INSTRUCTIONS;
			field->optionsMax = 1;
			break;
			case(2):
        		field->optionSelected = 0;
			field->mode = OPTIONS;
			field->optionsMax = 5;
			break;
			default:
				break;
			}
		} else if (field->mode == INSTRUCTIONS) {
			switch (field->optionSelected) {
			case(0):
        		field->optionSelected = 0;
			field->mode = MAIN_MENU;
			field->optionsMax = 3;
			break;

			default:
				break;
			}
		} else if (field->mode == OPTIONS) {
			switch (field->optionSelected) {
			case(0):
        		field->optionSelected = 0;
			field->mode = MAIN_MENU;
			field->optionsMax = 3;
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
