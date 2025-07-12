#ifndef MAIN_H
#define MAIN_H

#define STORY_TEXT_COLOR YELLOW

#include "gba.h"


// Struct that defines the game state
struct state {
    int currentState;
    int nextState;
};

// Defines properties that the user will have
struct player {
    int row;
    int col;
    int height;
    int width;
    int prevRow;
    int prevCol;
    int playerSpeed;
};

#endif
