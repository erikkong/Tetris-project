#ifndef TETRIS_MOVEMENT_H
#define TETRIS_MOVEMENT_H

#include <stdint.h>
#include "tetrominoes.h"
#include "tetris_visual.h"

// Buttons input function
int getbtns(void);
// Rotate block left
void rotateLeft(Block *block);
// Move block left
void moveLeft(Block *block);
// Move block right
void moveRight(Block *block);
// Switch input function
int getsw(void);
// Create a block of a certain type at the specified position (relative to the board)
void createBlock(Block *block, int x, int y, int type);
// Rotate block right
void rotateRight(Block *block);

#endif
