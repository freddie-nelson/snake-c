#ifndef APPLEH
#define APPLEH

#include "snake.h"

typedef struct
{
  int x;
  int y;
} Apple;

void randomApple(Apple *apple, Snake *snake);
Apple newApple(Snake *snake);
void drawApple(Apple *apple, SDL_Window *window, SDL_Renderer *renderer);

#endif