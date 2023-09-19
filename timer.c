#include <pic32mx.h>

// Intitiazing timer 2
void timerInit() {
  // Setting period scaling to 256 
  T2CONSET = 0x70; // 80 000 000 / 250 = 312500
  PR2 = 31250;
  // Enable Timer2
  IEC(0) = 1 << 8;
  // Start timer 2
  T2CONSET = 0x8000;
  // Set priority timer 2
  IPC(2) = 4;
}

int timerGet() {
    
  
  return TMR2;
}
