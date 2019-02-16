

#include "../core/log.h"
#include "math.h"

int TW_RectangleIncludesPoint(TW_Rectangle r, TW_Point p) {
  TW_Point temp = {p.x - r.x, p.y - r.y};
  int x = temp.x >= 0 && temp.x <= r.w && temp.y >= 0 && temp.y <= r.h;
  app_log("%3.f %3.f in %3.f %3.f %3.f %3.f = %i", p.x, p.y, r.x, r.y, r.w, r.h,
          x);
  return x;
}