#ifndef TETRIS_H
#define TETRIS_H

#include <stdint.h>
#include "tetris_movement.h"
#include "tetrominoes.h"

extern int gamestate;
// The current falling block
typedef struct {
	int level;
	int highScore;
	int score;
	int gameState;
	int nextBlockType;
	Block nextBlock;
	Block currentBlock;
} State;
// The current game state
State currentGame;
// Initialize the state to default values
void state_init(State *state);
// Reset the state
void state_reset(State *state);
// Interrupt callback for timer 2
void user_isr();
// Get random value for chosing next tetromino type
int getRand();
// Called from tetris_main, used for home screen logic
void title_display(int btns, int sws);
// Called from tetris_main, used for in-game logic
void tetris_logic(int btns, int sws);
// Initialize the game
void tetris_init();
// The main game loop
void tetris_main();
// Start tetris game from home screen
void tetris_begin();
// Called upon game over
void gameover();
// Check if a the current falling block collide with another block or hits the bottom of the playing field
int block_check();
// Permantely place the current falling block on the playing field
void place_on_board();
// Check if ther are any rows with 10 blocks in it
void check_rows();

#endif
