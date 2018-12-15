

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

typedef struct s_organic {
  float sx, sy, dx, dy;
  float x, y;
  int age, maturity;
  int depth;
  int num_branches;
  struct s_organic *branches;
} organic;

float easeOut(float t, float b, float c, float d) {
  t /= d;
  return -c * t * (t - 2) + b;
};

void organic_update(organic *o, int time_delta) {
  if (o->age < o->maturity) {
    o->x = o->sx + easeOut(o->age, 0, o->dx - o->sx, o->maturity);
    o->y = o->sy + easeOut(o->age, 0, o->dy - o->sy, o->maturity);
  } else {
    // split
    if (!o->branches) {
      if (o->depth < 5) {
        o->num_branches = (rand() % 2) + 2;
        o->branches = malloc(sizeof(organic) * o->num_branches);
        for (int i = 0; i < o->num_branches; i++) {
          o->branches[i].sx = o->dx;
          o->branches[i].sy = o->dy;
          o->branches[i].x = o->branches[i].sx;
          o->branches[i].y = o->branches[i].sy;
          o->branches[i].dx = o->dx + ((rand() % 100) - 50);
          o->branches[i].dy = o->dy + ((rand() % 100) - 100);
          o->branches[i].depth = o->depth + 1;
          o->branches[i].age = 0;
          o->branches[i].branches = 0;
          o->branches[i].maturity = ((rand() % 3000)) + 1000;
        }
      }
    } else {
      for (int i = 0; i < o->num_branches; i++) {
        organic_update(&o->branches[i], time_delta);
      }
    }
    o->x = o->dx;
    o->y = o->dy;
  }
  o->age += time_delta;
}

void organic_render(SDL_Renderer *renderer, organic *o) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, o->sx, o->sy, o->x, o->y);
  if (o->branches) {
    for (int i = 0; i < o->num_branches; i++) {
      organic_render(renderer, &o->branches[i]);
    }
  }
}