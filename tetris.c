#include <pic32mx.h>
#include <stdint.h>
#include <stdlib.h>
#include "mipslab.h"
#include "tetris.h"
#include "timer.h"
#include "tetris_visual.h"
#include "tetris_movement.h"

// The current frame for the animation of 'KTH' at title screen
int titleFraming = 0;
// If 1, draw a frame and update the current game state
int updateFrame = 0;
// A small delay for when the current block will be dropped 1 unit.
int dropDelay = 75;
// When dropDelay = dropCounter, the current block will be dropped 1 unit
int dropCounter = 0;
// Cheat button
int stop = 1;
//Keep track of how many units the current block has fallen
int currentBlockDrops = 0;
//Keep track of how many units the last 2 blocks have fallen(used for checking if the game is over)
int lastTwoDrops[] = {20, 20};
// If 1, the game is over
int gameOver = 0;

// Get Random number for nextBlock - middle-square
int getRand(void) {

  static int next = 3251;

  next = ((next * next) / 100) % 10000;
  
  return next;
}

// Initialize state
void state_init(State *state) {
  // Initialize the state of the game, set gamestate, score and next block
  state->gameState = 1;
  state->score = 0;
  state->nextBlockType = getRand() % 7;
}

// Reset state after losing
void state_reset(State *state) {
  // Reset the state of the game, set gamestate, highscore, score and next block
  state->gameState = 1;
  if(state->score > state->highScore) {
    state->highScore = state->score;

  }
  state->score = 0;
  state->nextBlockType = getRand() % 7;
}

// Interrupt service routine
void user_isr( void ){
  // Reset the interrupt flag
  IFS(0) &= 0xfeff;

  // There is a new frame ready for the game to be displayed
  updateFrame = 0;
}

// Initialize tetris
void tetris_init() {
  // Set the 8 LEDs as outputs
  volatile int * trise = (volatile int *) 0xbf886100;
  *trise &= ~0xff;
  volatile int * porte = (volatile int *) 0xbf886110;
  *porte &= ~0xff;
  // Set push buttons 2-4  and switch 4 as inputs
  TRISDSET = 0xfe0;
  // Set push button 1 as input
  TRISFSET = 2;


  // Enable interrupts globally
  enable_interrupt();

  // Initialize and starts timer 2 for handling frame update of the game
  timerInit();

  // Initialize the playing field
  int i, j;
  for (i = 0; i < 20; ++i) {
    for (j = 0; j < 10; ++j) {
      boardLogic[i * 10 + j] = 0;
    }
  }

  // Initialize the game state
  state_init(&currentGame);
}

// Check if block is colliding
int block_check() {
  // Move the current block down 1 unit
  Block *block = &currentGame.currentBlock;
  int newY = block->y + 1;
  // Check if the block intersects witch another block on the playing field or hits the bottom of them playing field
  int i, j;
  for (i = 0; i < block->height; ++i) {
    for (j = 0; j < block->width; ++j) {
      if (block->data[i * block->width + j]) {
        if (newY + i > 19 || boardLogic[(newY + i) * 10 + block->x + j]) {
          return 1;
        }
      }
    }
  }
  return 0;
}

// Gameover
void gameover(){
  //Set gameOver to 1, reset the game state and turn of the LEDs
  titleFraming = 0;
  state_reset(&currentGame),
  gameOver = 1;
  PORTE = 0;

  // Reset the game playing field
  int i, j;
  for (i = 0; i < 20; ++i) {
    for (j = 0; j < 10; ++j) {
      boardLogic[i * 10 + j] = 0;
    }
  }
}

// Copy to boardLogic
void place_on_board() {
  Block *block = &currentGame.currentBlock;
  // Place the current block on the current postion in the playing field
  int i, j;
  for (i = 0; i < block->height; ++i) {
    for (j = 0; j < block->width; ++j) {
      if (block->data[i * block->height + j])
        boardLogic[(block->y + i) * 10 + block->x + j] = 1;
    }
  }
}

// Check if 10 in a line
void check_rows() {
  // For each row on the playing field, check if the rows have 10 blocks
  int i, j;
  for (i = 0; i < 20; ++i) {
    int count = 0;
    for (j = 0; j < 10; ++j) {
      count += boardLogic[i * 10 + j];
    }
    // If there are 10 blocks in a row, remove the current row and move all the rows above down
    if (count == 10) {
      int k;
      for (k = i - 1; k >= 0; --k) {
        for (j = 0; j < 10; ++j) {
          boardLogic[(k + 1) * 10 + j] = boardLogic[k * 10 + j];
        }
      }
      for (j = 0; j < 10; ++j) {
        boardLogic[j] = 0;
      }
      // Once a row has been cleared, increase the score by 10
      currentGame.score += 10;
      if (currentGame.level < 7) {
        // Increase the game level by 1
        currentGame.level += 1;
        // Set the LEDs to match the current game level
        PORTE = (1 << (currentGame.level + 1)) - 1;
      }
      else {
        currentGame.level += 1;
      }
    }
  }
}

// Start tetris
void tetris_begin() {
  // Generate a new block
  currentGame.nextBlockType = getRand() % 7;

  //Reset the dropcount for the last two blocks
  lastTwoDrops[0] = 20;
  lastTwoDrops[1] = 20;

  currentBlockDrops = 0;
  // Create the current block
  createBlock(&currentGame.currentBlock, 4, -2, currentGame.nextBlockType);


  currentGame.nextBlockType = getRand() % 7;
  // Create the next expected block, which will be displayed above the playing field
  createBlock(&currentGame.nextBlock, 18, 23, currentGame.nextBlockType);

  // Reset the gameOver, level and LEDs
  gameOver = 0;
  currentGame.level = 0;
  PORTE = 1;
}

// Display title
void title_display(int btns, int sws) {
  //If either of buttons 1-4 is pushed, start the game
  if (btns) {
    currentGame.gameState = 2;
    tetris_begin();
  }
  //Animation for the title
  if (titleFraming < 60)
    titleFraming += 3;
  display_bitmap_pos(6, 0, &kthTitle, 20, titleFraming, 0, 0);
}

// Speed of the falling tetrominoes
void dropSpeedDelay(int delay) {
    dropDelay = delay;
    dropCounter += (10 + (currentGame.level * 10) > dropDelay ? dropDelay : 10 + currentGame.level);
}

// Main game logic of tetris 
void tetris_logic(int btns, int sws) {

  if (btns & 1) {
    //Rotate right if button 1 is pushed
    rotateRight(&currentGame.currentBlock);
  } else if (btns & 2) {
    //Rotate left if button 2 is pushed
    rotateLeft(&currentGame.currentBlock);
  } else if (btns & 4) {
    //Move left if button 3 is pushed
    moveLeft(&currentGame.currentBlock);
  } else if (btns & 8) {
    //Move right if button 4 is pushed
    moveRight(&currentGame.currentBlock);
  }

  //If the current falling block collides with another block or hits the bottom of the playingfield
  if (block_check()) {
    // Copy it to the boardLogic
    place_on_board();
    // Check if there are any rows with 10 blocks
    check_rows();
    //Update the dropcount for the last two blocks
    lastTwoDrops[1] = lastTwoDrops[0];
    lastTwoDrops[0] = currentBlockDrops;
    //If the last two blocks have not fallen any units, then they cannot move because the top row is occupied by blocks = gameover
    if (lastTwoDrops[0] <= 0 && lastTwoDrops[1] <= 0 ) {
      gameover();
    }
    //Create the new current block and next block
    createBlock(&currentGame.currentBlock, 4, -2, currentGame.nextBlockType);
    currentGame.nextBlockType = getRand() % 7;
    createBlock(&currentGame.nextBlock, 18, 22, currentGame.nextBlockType);
    //Reset the currentBlockDropsdrops and drop counter
    currentBlockDrops = 0;
    dropCounter = 0;

  } else if (!gameOver) {
    if (sws & 1) {
        dropSpeedDelay(0);
    } else if (sws & 2) {
        dropSpeedDelay(25);
    } else if (sws & 4) {
        dropSpeedDelay(50);
    } else if (sws & 8) {
        dropCounter = stop;
    } else {
        dropSpeedDelay(70);
    }
    if (dropCounter >= dropDelay) {
      //When dropCounter = dropDelay, the current falling block will fall 1 unit
      ++currentGame.currentBlock.y;
      dropCounter = 0;
      ++currentBlockDrops;
    }
  }

  //Draw the score and high score on the display
  display_score(0, 8, currentGame.score);
  display_score(0, 0, currentGame.highScore);

  //Draw the next block expcted to fall on the display
  display_bitmap(16, 16, &nextContainer);
  display_block_pos(&currentGame.nextBlock, 18, 24);

  //Draw the current falling block and the playing field on the display
  display_block(&currentGame.currentBlock);
  int i, j;
  for (i = 0; i < 20; ++i) {
    for (j = 0; j < 10; ++j) {
      if (boardLogic[i * 10 + j])
        display_bitmap(1 + j * 3, 68 + i * 3, &block);
    }
  }
}

// Main tetris engine
void tetris_main() {
  int btns = 0, sws = 0;
  //Get user input from buttons and switches while waiting for frame to update
  //When timer 2 rolls over, reset updateFrame
  while (updateFrame) {
    btns = getbtns();
    sws = getsw();
  }
  updateFrame = 1;

  //Clear the display, render the title state or the in-game state
  display_clear();
  switch (currentGame.gameState) {
  case 1:
    title_display(btns, sws);
    break;
  case 2:
    tetris_logic(btns, sws);
    break;
  }
  // Update the display
  display_update();
}
