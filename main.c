#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdlib.h>
#include "globals.h"
#include "snake.h"
#include "apple.h"

TTF_Font *FONT;

bool gameOver = false;

bool isInGrid(uint8_t r, uint8_t c)
{
  return r >= 0 && r < GRID_SIZE && c >= 0 && c < GRID_SIZE;
}

SDL_Texture *numTextures[8];

void setupNumbers(SDL_Renderer *renderer)
{
  SDL_Surface *surface;
  SDL_Color textColor = {0, 0, 255, 0};

  for (uint8_t i = 0; i < 8; i++)
  {
    char text[1];
    sprintf(text, "%u", i + 1);

    surface = TTF_RenderText_Solid(FONT, text, textColor);
    if (surface == NULL)
    {
      printf(" %s ", SDL_GetError());
    }

    numTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
    if (numTextures[i] == NULL)
    {
      printf(" %s ", SDL_GetError());
    }
  }

  SDL_FreeSurface(surface);
}

Snake snake;

int main()
{
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  SDL_Surface *screenSurface = NULL;

  // init png loading
  IMG_Init(IMG_INIT_PNG);

  // load font
  TTF_Init();
  FONT = TTF_OpenFont("font.ttf", 16);
  if (FONT == NULL)
  {
    printf(" could not find font %s ", stderr);
    exit(EXIT_FAILURE);
  }

  bool quit = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    // create window
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
    }
    else
    {
      // get window surface
      screenSurface = SDL_GetWindowSurface(window);
      renderer = SDL_GetRenderer(window);

      quit = false;
    }
  }

  // create starter snake
  initSnake(&snake, GRID_SIZE * GRID_SIZE);

  Piece head;
  head.x = GRID_SIZE / 2;
  head.y = GRID_SIZE / 2;
  head.direction = UP;
  insertSnake(&snake, head);

  snakeEat(&snake);
  snakeEat(&snake);

  // create apple
  Apple apple = newApple(&snake);

  // game loop
  SDL_Event e;

  struct timeval lastMove_t;
  gettimeofday(&lastMove_t, NULL);

  while (!quit && !gameOver)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      switch (e.type)
      {
      case SDL_QUIT:
      {
        quit = true;
        break;
      }
      case SDL_KEYDOWN:
      {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
          snake.array[0].direction = UP;
          break;
        case SDLK_DOWN:
          snake.array[0].direction = DOWN;
          break;
        case SDLK_RIGHT:
          snake.array[0].direction = RIGHT;
          break;
        case SDLK_LEFT:
          snake.array[0].direction = LEFT;
          break;
        default:
          break;
        }

        break;
      }
      }
    }

    // clear renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // move every 500ms
    struct timeval now_t;
    gettimeofday(&now_t, NULL);

    if ((now_t.tv_sec * 1000000 + now_t.tv_usec) - (lastMove_t.tv_sec * 1000000 + lastMove_t.tv_usec) >= 200 * 1000)
    {
      snakeMove(&snake, &gameOver);
      if (snake.array[0].x == apple.x && snake.array[0].y == apple.y)
      {
        snakeEat(&snake);
        randomApple(&apple, &snake);
      }

      gettimeofday(&lastMove_t, NULL);
    }

    drawSnake(&snake, window, renderer);
    drawApple(&apple, window, renderer);

    SDL_RenderPresent(renderer);
  }

  freeSnake(&snake);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}