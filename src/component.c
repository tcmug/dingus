

#include <SDL2/SDL.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "component.h"

component *_NODE(component init) {
  component *this = (component *)malloc(sizeof(component));
  memcpy(this, &init, sizeof(component));
  return this;
}

component **_LIST(int count, ...) {
  int i = 0;
  va_list args;
  va_start(args, count);
  component **children =
      (component **)malloc((count + 1) * sizeof(component *));
  while (i < count) {
    children[i++] = va_arg(args, component *);
  }
  children[i] = 0;
  va_end(args);
  return children;
}

void DESTROY(component *this) {
  if (this->children) {
    int i = 0;
    while (this->children[i]) {
      DESTROY(this->children[i++]);
    }
  }
  free(this->children);
  free(this);
}
