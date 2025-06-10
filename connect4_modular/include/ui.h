
#ifndef UI_H
#define UI_H

#include <allegro5/allegro_font.h>

void draw_board(ALLEGRO_FONT* font, int hover_col);
void start_game();
bool is_board_full(void);
bool play_turn(int col);


#endif
