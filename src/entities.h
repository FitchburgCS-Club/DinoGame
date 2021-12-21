#ifndef ENTITIES_H
#define ENTITIES_H
#include <SDL2/SDL.h>
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
