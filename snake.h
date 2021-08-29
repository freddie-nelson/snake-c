#ifndef SNAKEH
#define SNAKEH

#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT
} Direction;

typedef struct
{
  int x;
  int y;
  Direction direction;
} Piece;

typedef struct
{
  Piece *array;
  size_t used;
  size_t size;
} Snake;

void initSnake(Snake *a, size_t initialSize);
void insertSnake(Snake *a, Piece element);
void freeSnake(Snake *a);

void snakeEat(Snake *snake);
void snakeMove(Snake *snake, bool *gameOver);
void drawSnake(Snake *snake, SDL_Window *window, SDL_Renderer *renderer);

#endif