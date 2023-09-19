#ifndef TETROMINOES_H
#define TETROMINOES_H

#include <stdint.h>

// A Bitmap struct
typedef struct {
  int width;
  int height;
  const uint8_t data[];
} Bitmap;
// A block definition. Will be copied when a block is created
typedef struct {
  const int width; // The width of a tetris block
  const int height; // The height of a tetris block
  const uint8_t data[16]; // The actual block definitioo
} Tetromino;
// A tetris block.
typedef struct{
  int x; // X-coordinate in bitmap
  int y; // Y-coordinate in bitmap
  int width; // The width of a tetris block. Always 4 subblocks 
  int height; // The height of a tetris block. Always 4 subblocks
  uint8_t data[16]; // The actual block definition. 1 = subblock, 0 = nothing
} Block;
// Square container for nextblock
Bitmap nextContainer;
// Title KTH
Bitmap kthTitle;
// 3x3 subblock
Bitmap block;
// 0-9 digits
Bitmap digits;
// Block definitions for all 7 Tetrominoes
extern Tetromino tetrominoes[7];


#endif