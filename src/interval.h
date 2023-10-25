#ifndef INTERVAL_H_
#define INTERVAL_H_

#define INTERVAL_CONTAINS(interval, x)                                         \
    (interval.tmin <= x && x <= interval.tmax)
#define INTERVAL_SURROUNDS(interval, x) (interval.tmin < x && x < interval.tmax)
typedef struct
{
    double tmin;
    double tmax;
} Interval;

double interval_clamp(Interval i, double x);
#endif
