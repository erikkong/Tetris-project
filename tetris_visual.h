#ifndef TETRIS_VISUAL_H
#define TETRIS_VISUAL_H

#include <stdint.h>
#include "tetrominoes.h"


// Display that will hold all pixel data - 127 x 32 or 4096 pixels - GLOBAL
extern uint8_t display[128 * 32];
// Used to keep track of the blocks on the game board - GLOBAL
extern uint8_t boardLogic[10 * 20];
// Clear the temporary display array
void display_clear(void);
// Writes the temporary display array data to the actual display on the Basic I/O Shield
void display_update(void);
// Draw a bitmap with additional parameters for width, height, origin x and origin y
void display_bitmap_pos(int x, int y, const Bitmap *bitmap, int w, int h, int ox, int oy);
// Draw a bitmap at the specified location. Calls display_bitmap_ex
void display_bitmap(int x, int y, const Bitmap *bitmap);
// Draw a number string
void display_score(int x, int y, unsigned int score);
// Draw a block using its internal position
void display_block(const Block *block);
// Draw a block at the specified position
void display_block_pos(const Block *block, int x, int y);

#endif