#include "TetrisMusic.h"

#define NUMBER_OF_NOTES 57

/* Notes durations (ms) */
#define EIGHT_NOTE 250
#define QUARTER_NOTE 500
#define DOTTED_QUARTER_NOTE 750
#define HALF_NOTE 1000
#define WHOLE_NOTE 2000
const long notesDuration[NUMBER_OF_NOTES + 1] = { 
  QUARTER_NOTE, EIGHT_NOTE, EIGHT_NOTE, QUARTER_NOTE, EIGHT_NOTE, EIGHT_NOTE, QUARTER_NOTE,
  EIGHT_NOTE, EIGHT_NOTE, QUARTER_NOTE, EIGHT_NOTE, EIGHT_NOTE, QUARTER_NOTE, EIGHT_NOTE,
  EIGHT_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, 
  DOTTED_QUARTER_NOTE, EIGHT_NOTE, QUARTER_NOTE, EIGHT_NOTE, EIGHT_NOTE,DOTTED_QUARTER_NOTE,
  EIGHT_NOTE, QUARTER_NOTE, EIGHT_NOTE, EIGHT_NOTE, QUARTER_NOTE, EIGHT_NOTE, EIGHT_NOTE,
  QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, HALF_NOTE,
  HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
  HALF_NOTE, HALF_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, QUARTER_NOTE, WHOLE_NOTE };

/* Notes frequencies (Hz) */
const long A_NOTE[3] = {440, 880, 1760};
const long B_NOTE[2] = {493.88, 987.77};
const long C_NOTE[2] = {523.25, 1046.50};
const long D_NOTE[2]= {587.33, 1174.66};
const long E_NOTE[2] = {659.26, 1318.51};
const long F_NOTE = 1396.91;
const long G_NOTE = 1567.98;
const long G_NOTE_SHARP[2] = {415.30, 830.61};
const long notesFreq[NUMBER_OF_NOTES + 1] = { 
  E_NOTE[1], B_NOTE[1], C_NOTE[1], D_NOTE[1], C_NOTE[1], B_NOTE[1], A_NOTE[1], A_NOTE[1], C_NOTE[1],
  E_NOTE[1], D_NOTE[1], C_NOTE[1], B_NOTE[1], B_NOTE[1], C_NOTE[1], D_NOTE[1], E_NOTE[1], C_NOTE[1], 
  A_NOTE[1], A_NOTE[1], 0, D_NOTE[1], F_NOTE, A_NOTE[2], G_NOTE, F_NOTE, E_NOTE[1], C_NOTE[1], E_NOTE[1],
  D_NOTE[1], C_NOTE[1], B_NOTE[1], B_NOTE[1], C_NOTE[1], D_NOTE[1], E_NOTE[1], C_NOTE[1], A_NOTE[1],
  A_NOTE[1], 0, E_NOTE[0], C_NOTE[0], D_NOTE[0], B_NOTE[0], C_NOTE[0], A_NOTE[0], G_NOTE_SHARP[0], B_NOTE[0], 
  E_NOTE[0], C_NOTE[0], D_NOTE[0], B_NOTE[0], C_NOTE[0], E_NOTE[0], A_NOTE[1], A_NOTE[1], G_NOTE_SHARP[1] };

TetrisMusic::TetrisMusic(const int pinBuzzerInit)
{
  pinBuzzer = pinBuzzerInit;
  pinMode(pinBuzzer, OUTPUT);
  noteIndex = NUMBER_OF_NOTES;
  NextNote();
}

/* Must be called as frequently as possible! */
void TetrisMusic::Update()
{
  static unsigned long lastTime = 0;
  static unsigned long cycleNb = 0;

  if (notesFreq[noteIndex] != 0)
  {
    // Switch pin at the right frequency for the right duration
    if (cycleNb < numberOfCycles)
    {
      if ((micros() - lastTime) >= delayValueUs)
      {
        lastTime = micros();
        cycleNb++;
        digitalWrite(pinBuzzer, (digitalRead(pinBuzzer) == LOW) ? HIGH : LOW);
      }
    }
    else
    {
      cycleNb = 0;
      lastTime = micros();
      NextNote();
    }
  }
  else if ((micros() - lastTime) >= delayValueUs) // No note to be played, just wait for a bit
  {
    lastTime = micros();
    NextNote();
  }
}

void TetrisMusic::NextNote()
{
  noteIndex = (noteIndex < NUMBER_OF_NOTES) ? (noteIndex + 1) : 0;
  // Delay is 1s / freq / 2 because a cycle has 2 steps (LOW / HIGH), or just the duration if no note should be played
  delayValueUs = (notesFreq[noteIndex] != 0) ? (1000000 / notesFreq[noteIndex] / 2) : (notesDuration[noteIndex] * 1000);
  // Number of cycles is frequency * cycle duration in seconds, doubled here becaused the 2 steps are managed in a single if
  numberOfCycles = 2 * (notesFreq[noteIndex] * notesDuration[noteIndex] / 1000);
}
