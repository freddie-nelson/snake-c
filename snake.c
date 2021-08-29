#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "globals.h"

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

void initSnake(Snake *a, size_t initialSize)
{
  a->array = malloc(initialSize * sizeof(int));
  a->used = 0;
  a->size = initialSize;
}

void insertSnake(Snake *a, Piece element)
{
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size
  if (a->used == a->size)
  {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}

void freeSnake(Snake *a)
{
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

void snakeEat(Snake *snake)
{
  Piece *tail = &snake->array[snake->used - 1];
  Piece newTail;
  newTail.x = tail->x;
  newTail.y = tail->y;
  newTail.direction = tail->direction;

  switch (tail->direction)
  {
  case UP:
    newTail.y += 1;
    break;
  case DOWN:
    newTail.y -= 1;
    break;
  case LEFT:
    newTail.x += 1;
    break;
  case RIGHT:
    newTail.x -= 1;
    break;
  default:
    break;
  }

  insertSnake(snake, newTail);
}

void snakeMove(Snake *snake, bool *gameOver)
{
  for (int i = snake->used - 1; i >= 0; i--)
  {
    Piece *p = &snake->array[i];

    switch (p->direction)
    {
    case UP:
      p->y -= 1;
      break;
    case DOWN:
      p->y += 1;
      break;
    case LEFT:
      p->x -= 1;
      break;
    case RIGHT:
      p->x += 1;
      break;
    default:
      break;
    }

    if (i == 0)
    {
      if (p->x < 0 || p->x >= GRID_SIZE || p->y < 0 || p->y >= GRID_SIZE)
      {
        *gameOver = true;
        return;
      }

      for (size_t i = 1; i < snake->used; i++)
      {
        if (p->x == snake->array[i].x && p->y == snake->array[i].y)
        {
          *gameOver = true;
          return;
        }
      }
    }
    else
    {
      p->direction = snake->array[i - 1].direction;
    }
  }
}

void drawSnake(Snake *snake, SDL_Window *window, SDL_Renderer *renderer)
{
  for (size_t i = 0; i < snake->used; i++)
  {
    SDL_Rect rect;
    rect.w = SCREEN_SIZE / GRID_SIZE;
    rect.h = SCREEN_SIZE / GRID_SIZE;
    rect.x = snake->array[i].x * rect.w;
    rect.y = snake->array[i].y * rect.h;

    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
  }
}