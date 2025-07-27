#ifndef MAIN_H
#define MAIN_H

#define STORY_TEXT_COLOR YELLOW
#define SHAPE_ANIMATION_BASELINE_1 45
#define SHAPE_ANIMATION_BASELINE_2 80
#define ANIMATION_DELAY_MS 100
#define SECONDS_TO_MS(seconds) (seconds * 1000)

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

typedef struct titleAnimationImage {
    int row;
    int col;
    int height;
    int width;
    int prevRow;
    int prevCol;
    int playerSpeed;
    int isFinishedMoving;
    int isDisplayed;
} titleAnimationImage;

// function that pauses the game in seconds
void pause(int seconds);

// Initializes title animation everytime START state is triggered
void initializeTitleAnimation(titleAnimationImage *circle,
    titleAnimationImage *square,
    titleAnimationImage *triangle,
    titleAnimationImage *titleLogo,
    titleAnimationImage *titleOtherLogo);

#endif
