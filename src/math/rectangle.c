

#include "math.h"

int TW_RectangleIncludesPoint(TW_Rectangle r, TW_Point p) {
  TW_Point temp = {p.x - r.x, p.y - r.y};
  return temp.x >= 0 && temp.x <= r.w && temp.y >= 0 && temp.y <= r.h;
}