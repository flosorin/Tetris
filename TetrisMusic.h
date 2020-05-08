#include "Arduino.h"

class TetrisMusic {
  
public:
  TetrisMusic(const int pinBuzzerInit);
  void Update();

private:
  void NextNote();
  
  int pinBuzzer;
  unsigned int noteIndex;
  unsigned long delayValueUs;
  unsigned long numberOfCycles;
};
