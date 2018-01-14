#include "TetrisPiece.h"

/* Constructor */
TetrisPiece::TetrisPiece() {
  
  initTetrisPiece();
}

/* Create a random Tetris piece */
void TetrisPiece::initTetrisPiece() {
  
  randomTetrisPiece = TrueRandom.random(7); // Use the TrueRandom component to get one of the 7 existing pieces

  state = 0;

  /* Create the shape according to its definition */
  switch (randomTetrisPiece) {
    case (SQUARE): nbStates = 1;
      createTetrisPiece(3, 1, 4, 1, 4, 0, 3, 0);
      break;
    case (BAR): nbStates = 2;
      createTetrisPiece(3, 3, 3, 2, 3, 1, 3, 0);
      break;
    case (T): nbStates = 4;
      createTetrisPiece(3, 2, 3, 1, 4, 1, 3, 0);
      break;
    case (L): nbStates = 4;
      createTetrisPiece(4, 2, 3, 2, 3, 1, 3, 0);
      break;
    case (J): nbStates = 4;
      createTetrisPiece(2, 2, 3, 2, 3, 1, 3, 0);
      break;
    case (S): nbStates = 2;
      createTetrisPiece(4, 2, 4, 1, 3, 1, 3, 0);
      break;
    case (Z): nbStates = 2;
      createTetrisPiece(3, 2, 3, 1, 4, 1, 4, 0);
      break;
  }
}

/* Modify the piece coordinates according to its state (rotation) */
void TetrisPiece::defineTetrisPiece() { 
  
  switch (randomTetrisPiece) {
    case (BAR):
      if (state == 0) { 
        createTetrisPiece(base.x + 2, base.y + 2, coord1.x + 1, coord1.y + 1, coord2.x, coord2.y, coord3.x - 1, coord3.y - 1);
      } else {
        /* Sides of the screen management */
        if (base.x > 1 && base.x < 7) {
          createTetrisPiece(base.x - 2, base.y - 2, coord1.x - 1, coord1.y - 1, coord2.x, coord2.y, coord3.x + 1, coord3.y + 1);
        } else {
          state = 0;
        }
      }
      break;
    case (T):
      if  (state == 0) {
        createTetrisPiece(base.x - 1, base.y + 1, coord1.x, coord1.y, coord2.x + 1, coord2.y + 1, coord3.x + 1, coord3.y - 1);
      } else if (state == 1) {
        /* Sides of the screen management */
        if (base.x > 0) {
          createTetrisPiece(base.x - 1, base.y - 1, coord1.x, coord1.y, coord2.x - 1, coord2.y + 1, coord3.x + 1, coord3.y + 1);
        } else {
          state = 0;
        }
      } else if (state == 2) {
         createTetrisPiece(base.x + 1, base.y - 1, coord1.x, coord1.y, coord2.x - 1, coord2.y - 1, coord3.x - 1, coord3.y + 1);    
      }
      else {
        /* Sides of the screen management */
        if (base.x < 7) {
          createTetrisPiece(base.x + 1, base.y + 1,coord1.x, coord1.y, coord2.x + 1, coord2.y - 1, coord3.x - 1, coord3.y - 1);
        } else {
          state = 2;
        }
      }
      break;
    case (L):
      if  (state == 0) {
        createTetrisPiece(base.x, base.y + 2, coord1.x - 1, coord1.y + 1, coord2.x, coord2.y, coord3.x + 1, coord3.y - 1);
      } else if (state == 1) {
        /* Sides of the screen management */
        if (base.x > 1) {
          createTetrisPiece(base.x - 2, base.y, coord1.x - 1, coord1.y - 1, coord2.x, coord2.y, coord3.x + 1, coord3.y + 1);
        } else {
          state = 0;
        }
      } else if (state == 2) {
        createTetrisPiece(base.x, base.y - 2, coord1.x + 1, coord1.y - 1, coord2.x, coord2.y, coord3.x - 1, coord3.y + 1);
      } else {
        /* Sides of the screen management */
        if (base.x < 6) {
          createTetrisPiece(base.x + 2, base.y, coord1.x + 1, coord1.y + 1, coord2.x, coord2.y, coord3.x - 1, coord3.y - 1);
        } else {
          state = 2;
        }
      }
      break;
    case (J):
      if  (state == 0) {
        createTetrisPiece(base.x - 2, base.y, coord1.x - 1, coord1.y + 1, coord2.x, coord2.y, coord3.x + 1, coord3.y - 1);
      } else if (state == 1) {
        /* Sides of the screen management */
        if (base.x < 6) {
          createTetrisPiece(base.x, base.y - 2, coord1.x - 1, coord1.y - 1, coord2.x, coord2.y, coord3.x + 1, coord3.y + 1);
        } else {
          state = 0;
        }
      }
      else if (state == 2) {
        createTetrisPiece(base.x + 2, base.y, coord1.x + 1, coord1.y - 1, coord2.x, coord2.y, coord3.x - 1, coord3.y + 1);
      } else {
        /* Sides of the screen management */
        if (base.x > 1) {
          createTetrisPiece(base.x, base.y + 2, coord1.x + 1, coord1.y + 1, coord2.x, coord2.y, coord3.x - 1, coord3.y - 1);
        } else {
          state = 2;
        }
      }
      break;
    case (S):
      if (state == 0) { 
        createTetrisPiece(base.x + 1, base.y + 1, coord1.x, coord1.y, coord2.x - 1, coord2.y + 1, coord3.x - 2, coord3.y);
      } else {
        /* Sides of the screen management */
        if (base.x < 7) {
          createTetrisPiece(base.x - 1, base.y - 1, coord1.x, coord1.y, coord2.x + 1, coord2.y - 1, coord3.x + 2, coord3.y);
        } else {
          state = 0;
        }
      }
      break;
    case (Z): 
      if (state == 0) { 
        createTetrisPiece(base.x - 1, base.y + 1, coord1.x, coord1.y, coord2.x + 1, coord2.y + 1, coord3.x + 2, coord3.y);
      } else {
        /* Sides of the screen management */
        if (base.x > 0) {
          createTetrisPiece(base.x + 1, base.y - 1, coord1.x, coord1.y, coord2.x - 1, coord2.y - 1, coord3.x - 2, coord3.y);
        } else {
          state = 0;
        }
      }
      break;
  }
}

/* Create a piece with given coordinates */
void TetrisPiece::createTetrisPiece(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
  
  base.x = x0;
  base.y = y0;
  coord1.x = x1;
  coord1.y = y1;
  coord2.x = x2;
  coord2.y = y2;
  coord3.x = x3;
  coord3.y = y3;
}

/* Define the new state of the piece */
void TetrisPiece::rotateTetrisPiece() {
  
  if (state < nbStates - 1) {
    state ++;
  } else {
    state = 0;
  }

  defineTetrisPiece();
}

/* Move the piece to the side */
void TetrisPiece::moveTetrisPieceX(int mvt) {

  /* Sides of the screen management */
  if ((mvt > 0 && base.x < 7 && coord1.x < 7 && coord2.x < 7 && coord3.x <7) || (mvt < 0 && base.x > 0 && coord1.x > 0 && coord2.x > 0 && coord3.x > 0)) {
    base.x += mvt;
    coord1.x += mvt;
    coord2.x += mvt;
    coord3.x += mvt;
  }
}

/* Move the piece down */
void TetrisPiece::moveTetrisPieceDown() {
  
  base.y ++;
  coord1.y ++;
  coord2.y ++;
  coord3.y ++;
}

/* Functions used to display the next piece */
/* Center the piece on the screen */
void TetrisPiece::centerTetrisPiece() {
  
  base.y += 2;
  coord1.y += 2;
  coord2.y += 2;
  coord3.y += 2;
}
/* Get back to the piece initial coordinates */
void TetrisPiece::reinitTetrisPiece() {
  
  base.y -= 2;
  coord1.y -= 2;
  coord2.y -= 2;
  coord3.y -= 2;
}

/* Get a piece coordinate */
coordinate TetrisPiece::getCoordinate(int coord) {
  
  switch(coord) {
    case 0: return base;
      break;
    case 1: return coord1;
      break;
    case 2: return coord2;
      break;
    case 3: return coord3;
      break;
  }
}
