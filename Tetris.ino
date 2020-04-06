/************************************************************************
* Classic Tetris game displayed on LED matrices by an Arduino UNO
* https://github.com/flosorin/Tetris.git
*
* Using Simon Monk's Timer library
* http://www.doctormonk.com/2012/01/arduino-timer-library.html
* 
* Using Eberhard Fahle's LedControl library  
* http://wayoda.github.io/LedControl/
* 
* Using Peter Knight's TrueRandom library
* http://code.google.com/p/tinkerit
* 
* Multi7Seg library by Florian Sorin (2018) 
* https://github.com/flosorin/Multi7Seg.git
* 
************************************************************************/

#include "Tetris.h"

/* Buttons management */
int buttonPressed = 0;
int buttonsValue = NONE;

/* Game variables */
bool goDown = false; // Force the piece to go to its final position immediatly
int nbMvtDown = 0; // Count the number of mouvement which occured before the collision (used to detect game over)
bool isGameOver = false; // Tell if the game is running or over
int level = 0; // Current game level (+1 each 10 lines completed)
int score = 0; // Current score
int nbLines = 0, lastNbLines = 0; // Number of lines currently and previously completed, useful to calculate level and score

/* Game matrix management */
bool leds[8][16] = {0}; // All LEDs
LedControl gameMatrix = LedControl(PIN_DATA_IN_GAME, PIN_CLK_MAX7219_GAME, PIN_LOAD_GAME, 2); // MAX7219 control provided by its library

/* Next piece matrix management */
LedControl nextPieceMatrix = LedControl(PIN_DATA_IN_SHAPE, PIN_CLK_MAX7219_SHAPE, PIN_LOAD_SHAPE, 1); // MAX7219 control provided by its library

/* 7 segment displays (level and score) */
int displayerPins[5] = {PIN_DIGIT0, PIN_DIGIT1, PIN_DIGIT2, PIN_DIGIT3, PIN_DIGIT4};
Multi7Seg displayers = Multi7Seg(NB_7_SEG, displayerPins, PIN_CLK_74HC595, PIN_LATCH, PIN_DATA);
displayerValue dispValues[2] = {{0, 1}, {0, 4}}; // Initial level 0 with 1 digit, initial score 0 with 4 digits

/* Timer to make the piece go down periodically */
Timer movingTimer;

/* Tetris current and next piece */
TetrisPiece piece = TetrisPiece();
TetrisPiece nextPiece = TetrisPiece();

void setup() {
  
  Serial.begin(9600);

  initMatrices(); // Initialize LEDs

  pinMode(PIN_BUTTONS, INPUT_PULLUP); // Initialize buttons

  displayPiece(true);
  refreshNextPiece();

  movingTimer.every(1000, movePieceDown); // Initialize timer (refresh every second)
}

void loop() {
   if (!isGameOver) {
    if (!goDown) {
      readButtons();
      switch (buttonsValue) {
        case LEFT: Serial.println("Left");
          if (!willBeCollisionSide(1)) {
            displayPiece(false);
            piece.moveTetrisPieceX(1);
            displayPiece(true);
          }
          break;
        case RIGHT: Serial.println("Right");
          if (!willBeCollisionSide(-1)) {
            displayPiece(false);
            piece.moveTetrisPieceX(-1);
            displayPiece(true);
          }
          break;
        case UP: Serial.println("Up");
          displayPiece(false);
          piece.rotateTetrisPiece();
          displayPiece(true);
          break;
        case DOWN: Serial.println("Down");
          goDown = true;
          movingTimer.stop(0);
          break;
      }

      movingTimer.update();
    } else {
      movePieceDown();
    }
  }

  dispValues[0].value = level;
  dispValues[1].value = score;
  displayers.updateDisplay(dispValues, 2);
}

void initMatrices() {
  
  /* Initialize game matrices */
  /* Address 0 for the first MAX7219 */
  gameMatrix.shutdown(0, false); // Power saving off, activate chip
  gameMatrix.setIntensity(0, 1); // Luminosity to 1 (values 0~15)
  gameMatrix.clearDisplay(0); // Shutdown all LEDS

  /* Address 1 for the second MAX7219 */
  gameMatrix.shutdown(1, false); // Power saving off, activate chip
  gameMatrix.setIntensity(1, 1); // Luminosity to 1 (values 0~15)
  gameMatrix.clearDisplay(1); // Shutdown all LEDS

  /* Initialize next piece matrix */
  nextPieceMatrix.shutdown(0, false); // Power saving off, activate chip
  nextPieceMatrix.setIntensity(0, 1); // Luminosity to 1 (values 0~15)
  nextPieceMatrix.clearDisplay(0); // Shutdown all LEDS
}

void readButtons() {
  
  buttonsValue = analogRead(PIN_BUTTONS); // Read raw value
  /* Calibrate raw value */
  //buttonsValue =  map (buttonsValue, 220, 600, 1, 5);
  buttonsValue /= 100;
  buttonsValue --;
  
  // After calibration, buttons return a number between 1 and 4
  if (buttonsValue < 5) {
    if (buttonPressed < 2) {
      buttonPressed ++;
    }
  } else {
    buttonPressed = 0;
  }

  /* Debouncing */
  if (buttonPressed != 1) {
    buttonsValue = NONE;
  }
}

/* Turn on or off leds at piece coordinates */
void displayPiece(bool on) {
  
  for (int i = 0; i < 4; i ++) {
    if (piece.getCoordinate(i).y < 8) {
      gameMatrix.setLed(1, piece.getCoordinate(i).x, piece.getCoordinate(i).y, on);
    } else {
      gameMatrix.setLed(0, piece.getCoordinate(i).x, piece.getCoordinate(i).y - 8, on);
    }
  }
}

/* Turn on LEDs memorized in the array */
void refreshMatrix() {
  
  for (int x = 0; x < 8; x ++) {
    for (int y = 0; y < 8; y ++) {
      gameMatrix.setLed(1, x, y, leds[x][y]);
    }
    for (int y = 8; y < 16; y ++) {
      gameMatrix.setLed(0, x, y - 8, leds[x][y]);
    }
  }
}

/* Display the new next piece */
void refreshNextPiece() {
  
  /* Erase current display (except the outlines) */
  for (int x = 0; x < 8; x ++) {
    for (int y = 0; y < 8; y ++) {
      if (x == 0 || x == 7 || y == 0 || y == 7) {
        nextPieceMatrix.setLed(0, x, y, true);
      } else {
        nextPieceMatrix.setLed(0, x, y, false);
      }
    }
  }

  /* Center the piece, display it, and then go back to its initial coordinates */
  nextPiece.centerTetrisPiece();
  for (int i = 0; i < 4; i ++) {
    nextPieceMatrix.setLed(0, nextPiece.getCoordinate(i).x, nextPiece.getCoordinate(i).y, true);
  }
  nextPiece.reinitTetrisPiece();
}

/* Periodical movement of the piece management */
void movePieceDown() {
  
  displayPiece(false);
  piece.moveTetrisPieceDown();
  displayPiece(true);

  nbMvtDown ++;

  if (isCollisionDown()) {
    collisionManager();
  }
}

/* Tells if there will be a collision in case of a side movement of the piece */
bool willBeCollisionSide(int mvt) {
  
  for (int i = 0; i < 4; i ++) { // Check each piece coordinate 
    // If there is a led ON next to a piece coordinate in the movement side, then there will be a collision
    if (leds[piece.getCoordinate(i).x + mvt][piece.getCoordinate(i).y]) {
      return true;
    }
  }

  return false;
}

/* Detect collision when the piece goes down */
bool isCollisionDown() {
  
  for (int i = 0; i < 4; i ++) { // Check each piece coordinate
    // If there is a led ON on the bottom of a coordinate, or if a coordinate is at the bottom of the screen, there is a collision
    if (leds[piece.getCoordinate(i).x][piece.getCoordinate(i).y + 1] || piece.getCoordinate(i).y == 15) {
      return true;
    }
  }

  return false;
}

/* Collision management */
void collisionManager() {

  /* Memorize piece coordinates in the screen leds array in order to fix its position */
  for (int i = 0; i < 4; i ++) {
    leds[piece.getCoordinate(i).x][piece.getCoordinate(i).y] = true;
  }

  /* Turn off the piece displaying and refresh the screen */
  displayPiece(false);
  refreshMatrix();

  /* Check if lines have been completed and store the first full line found if existing (16 overwise) */
  int line = detectFullLine();
  if (line < 16) {
    nbMvtDown ++;
  }
  
  /* Supress the lines and count them (16 is return when there is no full line anymore) */
  while (line < 16) {
    suppressFullLine(line);
    nbLines ++;
    line = detectFullLine();
  }

  /* Next level if 10 lines have been completed */
  if (nbLines >= 10 && level < 9) {
     level ++;
  }

  /* Score management, according to the number of lines completed at a time */
  switch(nbLines - lastNbLines) {
    case 1: score += 40 * (level + 1);
      break;
    case 2: score += 100 * (level + 1);
      break;
    case 3: score += 300 * (level + 1);
      break;
    case 4: score += 1200 * (level + 1);
      break;
    default :
      break;
  }

  /* Update number of lines completed (current reset if 10 was reached and last set to current) */
  if (nbLines >= 10) {
    nbLines -= 10;
  }
  lastNbLines = nbLines;
  
  /* If only one movement occured before the collision, then the game is over */
  if (nbMvtDown == 1) {
    movingTimer.stop(1);
    gameOver();
  } else {
    nbMvtDown = 0;
  }

  if (!isGameOver) {
    /* Display next piece on top screen and get a random new next piece */
    piece = nextPiece;
    nextPiece.initTetrisPiece();
    displayPiece(true);
    refreshNextPiece();

    /* Reset periodical movement variable */
    if (goDown) {
      goDown = false;
      movingTimer.every(1000, movePieceDown); // Reset timer (refresh every second)
    }
  }
}

/* Return the first full line found, 16 if there is no full line */
int detectFullLine() {
 
  int nbLedsOn = 0;

  /* Check each line */
  for (int y = 0; y < 16; y++) {
    /* Count the number of LEDs ON in this line */
    for (int x = 0; x < 8; x++) {
      if (leds[x][y]) {
        nbLedsOn ++;
      }
    }
    /* If all LEDs are ON, return the line number, else reset the counter and check next line */
    if (nbLedsOn == 8) {
      return y;
    } else {
      nbLedsOn = 0;
    }
  }

  /* If there is no full line, return 16 */
  return 16;
}

/* Suppress a full line and move everything on the screen to the bottom */
void suppressFullLine(int line) {

  for (int y = line; y >= 0; y --) {
    for (int x = 0; x < 8; x++) {
      if (y != 0) {
        leds[x][y] = leds[x][y - 1];
      } else {
        leds[x][y] = false;
      }
    }
  }

  refreshMatrix();
}

/* Game over management */
void gameOver() {
  
  isGameOver = true;
  Serial.println("Game over!");
  
  /* Outlines of the smiley */
  for (int y = 0; y < 8; y++) {
    if (y == 0 || y == 7) {
      leds[0][y] = false;
      leds[1][y] = false;
      leds[6][y] = false;
      leds[7][y] = false;

      for (int x = 2; x < 6; x++) {
        leds[x][y] = true;
      }
    } else {
      if (y != 1 && y != 6) {
        leds[0][y] = true;
        leds[7][y] = true;
      } else {
        leds[0][y] = false;
        leds[7][y] = false;
      }  
    }
  }

  /* Erase the inside */
  for (int y = 1; y < 7; y++) {
    for (int x = 1; x < 7; x++) {
      leds[x][y] = false;
    }
  }
  leds[1][1] = true;
  leds[1][6] = true;
  leds[6][1] = true;
  leds[6][6] = true;
  
  /* Eyes of the smiley */
  leds[2][2] = true;
  leds[5][2] = true;

  /* Mouth of the smiley */
  leds[3][4] = true;
  leds[4][4] = true;
  leds[2][5] = true;
  leds[5][5] = true;

  refreshMatrix();
}
