#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

/*
  Wait until the start of the next VBlank. This is useful to avoid tearing.
  Completing this function is required.
*/
void waitForVBlank(void) {

  // (1)
  //  while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)
  while (SCANLINECOUNTER > 160){}

  // (2)
  // while loop that keeps going until we're in vBlank:
  while (SCANLINECOUNTER < 160){}

  // (3)
  // increment vBlank counter:
  vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

/*
  Sets a pixel in the video buffer to a given color.
  Using DMA is NOT recommended. (In fact, using DMA with this function would be really slow!)
*/
void setPixel(int row, int col, u16 color) {
  // TODO: IMPLEMENT
  videoBuffer[OFFSET(row, col, WIDTH)] = color;
}

/*
  Draws a rectangle of a given color to the video buffer.
  The width and height, as well as the top left corner of the rectangle, are passed as parameters.
  This function can be completed using `height` DMA calls. 
*/
void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  // TODO: IMPLEMENT
  volatile u16 lcolor = color;
  for (int r = row; r < row + height; r++) {
    DMA[3].src = &lcolor;
    DMA[3].dst = videoBuffer + OFFSET(r, col, WIDTH);
    DMA[3].cnt = DMA_ON | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | width;
  }
}

/*
  Draws a fullscreen image to the video buffer.
  The image passed in must be of size WIDTH * HEIGHT.
  This function can be completed using a single DMA call.
*/
void drawFullScreenImageDMA(const u16 *image) {
  DMA[3].src = image;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = DMA_ON | DMA_32 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | (WIDTH * HEIGHT) / 2;
}

/*
  Draws an image to the video buffer.
  The width and height, as well as the top left corner of the image, are passed as parameters.
  The image passed in must be of size width * height.
  Completing this function is required.
  This function can be completed using `height` DMA calls. Solutions that use more DMA calls will not get credit.
*/
void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  for (int y = 0; y < height; y++) {
    const u16 *src = image + y * width;
    volatile u16 *dst = videoBuffer + OFFSET(row + y, col, WIDTH);
    DMA[3].src = src;
    DMA[3].dst = dst;
    DMA[3].cnt = DMA_ON | DMA_16 | DMA_NOW | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | width;
  }
}

/*
  Draws a rectangular chunk of a fullscreen image to the video buffer.
  The width and height, as well as the top left corner of the chunk to be drawn, are passed as parameters.
  The image passed in must be of size WIDTH * HEIGHT.
  This function can be completed using `height` DMA calls.
*/
void undrawImageDMA(int row, int col, int width, int height, const u16 *image) {
  for (int y = 0; y < height; y++) {
    const u16 *src = image + OFFSET(row + y, col, WIDTH);
    volatile u16 *dst = videoBuffer + OFFSET(row + y, col, WIDTH);
    DMA[3].src = src;
    DMA[3].dst = dst;
    DMA[3].cnt = DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | width;
  }
}

/*
  Fills the video buffer with a given color.
  This function can be completed using a single DMA call.
*/
void fillScreenDMA(volatile u16 color) {
  volatile u16 lcolor = color;
  DMA[3].src = &lcolor;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = DMA_ON | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | (WIDTH * HEIGHT);
}

/* STRING-DRAWING FUNCTIONS (provided) */
void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str, u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}
