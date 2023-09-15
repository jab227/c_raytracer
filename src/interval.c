#include "interval.h"

double interval_clamp(Interval i, double x) {
  if (x < i.tmin) {
    return i.tmin;
  }
  if (x > i.tmax) {
    return i.tmax;
  }

  return x;
}
