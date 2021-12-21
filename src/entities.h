#ifndef ENTITIES_H
#define ENTITIES_H
#ifdef __unix__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <stdbool.h>

typedef struct {
  unsigned char fps;
  unsigned char nFrames;
  SDL_Rect *frames;
} Animation;

typedef struct {
  Animation *anims;
  SDL_Rect pos;
  Animation *cAnim;
  bool jumping;
} Dino;
#endif
