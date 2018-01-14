#include "Arduino.h"
#include <TrueRandom.h>
#include <stdlib.h> 

/* Tetris existing pieces list */
typedef enum pieceName {SQUARE, BAR, L, J, T, S, Z};

/* Coordinate definition */
typedef struct {
  int x = 0;
  int y = 0;
} coordinate;

class TetrisPiece {

public:
     TetrisPiece(); // Constructor: calls initTetrisPiece()
     void initTetrisPiece(); // Create a random Tetris piece
     void defineTetrisPiece(); // Modify the piece coordinates according to its state (rotation)
     void createTetrisPiece(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3); // Create a piece with given coordinates
     void rotateTetrisPiece(); // Define the new state of the piece
     void moveTetrisPieceX(int mvt); // Move the piece to the side
     void moveTetrisPieceDown(); // Move the piece down
     void centerTetrisPiece(); // Center the piece on the screen
     void reinitTetrisPiece(); // Get back to the piece initial coordinates
     coordinate getCoordinate(int coord);  // Get a piece coordinate
private:
     /* Piece coordinates */
     coordinate base;
     coordinate coord1;
     coordinate coord2;
     coordinate coord3;
     /* Piece state managemement */
     int nbStates;
     int state;
     pieceName randomTetrisPiece;
};
