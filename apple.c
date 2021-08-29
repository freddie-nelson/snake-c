#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "globals.h"
#include "snake.h"

typedef struct
{
  int x;
  int y;
} Apple;

void randomApple(Apple *apple, Snake *snake)
{
  bool overlaps = false;
  do
  {
    apple->x = rand() % GRID_SIZE;
    apple->y = rand() % GRID_SIZE;

    for (size_t i = 0; i < snake->used; i++)
    {
      if (apple->x == snake->array[i].x && apple->y == snake->array[i].y)
      {
        overlaps = true;
        break;
      }
    }
  } while (overlaps);
}

Apple newApple(Snake *snake)
{
  srand(time(NULL));

  Apple a;
  randomApple(&a, snake);

  return a;
}

void drawApple(Apple *apple, SDL_Window *window, SDL_Renderer *renderer)
{
  SDL_Rect rect;
  rect.w = SCREEN_SIZE / GRID_SIZE;
  rect.h = SCREEN_SIZE / GRID_SIZE;
  rect.x = apple->x * rect.w;
  rect.y = apple->y * rect.h;

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &rect);
}