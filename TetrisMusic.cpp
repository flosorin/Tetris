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
const long B_NOTE[2] = {494, 988};
const long C_NOTE[2] = {523, 1047};
const long D_NOTE[2]= {587, 1175};
const long E_NOTE[2] = {659, 1319};
const long F_NOTE = 1397;
const long G_NOTE = 1568;
const long G_NOTE_SHARP[2] = {415, 830};
const long notesFreq[NUMBER_OF_NOTES + 1] = { 
  E_NOTE[1], B_NOTE[1], C_NOTE[1], D_NOTE[1], C_NOTE[1], B_NOTE[1], A_NOTE[1], A_NOTE[1], C_NOTE[1],
  E_NOTE[1], D_NOTE[1], C_NOTE[1], B_NOTE[1], B_NOTE[1], C_NOTE[1], D_NOTE[1], E_NOTE[1], C_NOTE[1], 
  A_NOTE[1], A_NOTE[1], 0, D_NOTE[1], F_NOTE, A_NOTE[2], G_NOTE, F_NOTE, E_NOTE[1], C_NOTE[1], E_NOTE[1],
  D_NOTE[1], C_NOTE[1], B_NOTE[1], B_NOTE[1], C_NOTE[1], D_NOTE[1], E_NOTE[1], C_NOTE[1], A_NOTE[1],
  A_NOTE[1], 0, E_NOTE[0], C_NOTE[0], D_NOTE[0], B_NOTE[0], C_NOTE[0], A_NOTE[0], G_NOTE_SHARP[0], B_NOTE[0], 
  E_NOTE[0], C_NOTE[0], D_NOTE[0], B_NOTE[0], C_NOTE[0], E_NOTE[0], A_NOTE[1], A_NOTE[1], G_NOTE_SHARP[1] };

/* Other variables */
volatile int pinBuzzer = 0;
volatile int pinBuzzerState = LOW;
volatile unsigned int noteIndex = 0;
volatile unsigned long numberOfCycles = 0;
volatile unsigned long cycleCounter = 0;

void initMusic(int pinBuzzerInit)
{
  pinBuzzer = pinBuzzerInit;
  pinMode(pinBuzzer, OUTPUT);
  noteIndex = NUMBER_OF_NOTES;
  nextNote();
  initIntTimer();
}

void initIntTimer() {
  cli(); // Stop interrupts
  TCCR1A = 0; // Set entire TCCR1A register to 0
  TCCR1B = 0; // Same for TCCR1B
  TCNT1  = 0; // Initialize counter value to 0
  OCR1A = (F_CPU / (8 * (2 * notesFreq[0]))) - 1; // Set compare match register according to first note frequency
  TCCR1B |= (1 << WGM12); // Turn on CTC mode
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10); // Set CS11 to use 1/8 prescaler
  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
  sei(); // Allow interrupts
}

ISR(TIMER1_COMPA_vect)
{
  // Manage next note
  if (cycleCounter >= numberOfCycles) {
    cycleCounter = 0;
    nextNote();
  }

  if (notesFreq[noteIndex] != 0) { // Oscillate the pin at the right frequency if a note should be played
    pinBuzzerState = (pinBuzzerState == HIGH) ? LOW : HIGH;
    digitalWrite(pinBuzzer, pinBuzzerState);
    OCR1A = (F_CPU / (8 * (2 * notesFreq[noteIndex]))) - 1;
  } else { // Define next overflow to 10 ms (100 Hz) if we need to wait before next note
    OCR1A = (F_CPU / (8 * 100)) - 1;
  }

  cycleCounter++;
}

void nextNote()
{
  noteIndex = (noteIndex < NUMBER_OF_NOTES) ? (noteIndex + 1) : 0;
  if (notesFreq[noteIndex] != 0) {
    // Number of cycles is frequency * cycle duration in seconds, doubled here becaused the 2 steps are managed in a single call
    numberOfCycles = (2 * (notesFreq[noteIndex] * notesDuration[noteIndex] / 1000));
  } else {
    // The interrupt is triggered every 10 ms to check if the duration has been reached
    numberOfCycles = notesDuration[noteIndex] / 10;
  }
}

