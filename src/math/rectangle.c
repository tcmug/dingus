

#include "../core/log.h"
#include "math.h"

int TW_RectangleIncludesPoint(TW_Rectangle r, TW_Vector2 p) {
  TW_Vector2 temp = {p.x - r.x, p.y - r.y};
  int x = temp.x >= 0 && temp.x <= r.w && temp.y >= 0 && temp.y <= r.h;
  return x;
}
