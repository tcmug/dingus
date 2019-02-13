

#include "math.h"

int rectangle_includes(rectangle r, point p) {
  point temp = {p.x - r.x, p.y - r.y};
  return temp.x >= 0 && temp.x <= r.w && temp.y >= 0 && temp.y <= r.h;
}