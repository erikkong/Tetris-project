#include <pic32mx.h>
#include "tetris_movement.h"
#include "mipslab.h"

Tetromino tetrominoes[7] = {
   // T
  {
    4, 4,
    {
      0, 0, 0, 0,
      1, 1, 1, 0,
      0, 1, 0, 0,
      0, 0, 0, 0
    }
  },
  // Square
  {
    4, 4,
    {
      0, 0, 0, 0,
      0, 1, 1, 0,
      0, 1, 1, 0,
      0, 0, 0, 0
    }
  },
    // L1
  {
    4, 4,
    {
      0, 1, 0, 0,
      0, 1, 0, 0,
      0, 1, 1, 0,
      0, 0, 0, 0
    }
  },
  // L2
  {
    4, 4,
    {
      0, 0, 1, 0,
      0, 0, 1, 0,
      0, 1, 1, 0,
      0, 0, 0, 0
    }
  },
  // Line
  {
    4, 4,
    {
      0, 1, 0, 0,
      0, 1, 0, 0,
      0, 1, 0, 0,
      0, 1, 0, 0
    }
  },
  // Z1
  {
    4, 4,
    {
      0, 0, 1, 0,
      0, 1, 1, 0,
      0, 1, 0, 0,
      0, 0, 0, 0
    }
  },
  // Z2
  {
    4, 4,
    {
      0, 1, 0, 0,
      0, 1, 1, 0,
      0, 0, 1, 0,
      0, 0, 0, 0
    }
  }
};

int getsw(){
  // Gets Switch data from bit 8 to 11
  return (PORTD >> 8) & 0xf;
}
int getbtns(){
  // Get button data from bit 5 to 7 in PORTD and bit 1 in PORTF
  int btn_data = ((PORTD >> 4) & 0xe) | ((PORTF >> 1) & 1);
  return btn_data;
}
void createBlock(Block *block, int x, int y, int type) {
  block->x = x;
  block->y = y;
  const Tetromino *def = &tetrominoes[type];
  block->width = def->width;
  block->height = def->height;
  int i, j;
  for (i = 0; i < block->height; ++i) {
    for (j = 0; j < block->width; ++j) {
      block->data[i * block->width + j] = def->data[i * def->width + j];
    }
  }
}
void moveRight(Block *block) {
  int oldX = block->x;
  ++block->x;
  int i, j;
  // Check if the new position is free. Otherwise, move it back.
  for (i = 0; i < block->height; ++i) {
    for (j = 0; j < block->width; ++j) {
      if (block->data[i * block->width + j]) {
        if (block->x + j > 9 || boardLogic[(block->y + i) * 10 + block->x + j]) {
          block->x = oldX;
          return;
        }
      }
    }
  }
}
void moveLeft(Block *block) {
  int oldX = block->x;
  --block->x;
  int i, j;
  // Check if the new position is free. Otherwise, move it back.
  for (i = 0; i < block->height; ++i) {
    for (j = 0; j < block->width; ++j) {
      if (block->data[i * block->width + j]) {
        if (block->x + j < 0 || boardLogic[(block->y + i) * 10 + block->x + j]) {
          block->x = oldX;
          return;
        }
      }
    }
  }
}
void rotateRight(Block *block){
  // Create a temporary array to store the original state of the block
  int i, j;
  uint8_t tmp[block->width * block->height];
  for(i = 0; i < block->height; ++i){
    for(j = 0; j < block->width; ++j){
      tmp[i * 4 + j] = block->data[i * 4 + j];
    }
  }
  for(i = 0; i < block->height; ++i){
    for(j = 0; j < block->width; ++j){
      block->data[i * 4 + j] = tmp[4 * (3 - j) + i];
    }
  }
}
void rotateLeft(Block *block){
  // Create a temporary array to store the original state of the block
  int i, j;
  uint8_t tmp[block->width * block->height];
  for(i = 0; i < block->height; ++i){
    for(j = 0; j < block->width; ++j){
      tmp[i * 4 + j] = block->data[i * 4 + j];
    }
  }

  for(i = 0; i < block->height; ++i){
    for(j = 0; j < block->width; ++j){
      block->data[i * 4 + j] = tmp[3 - i + 4 * j];
    }
  }
}

