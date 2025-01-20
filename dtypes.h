#ifndef DTYPES_H
#define DTYPES_H

#include "defs.h"

#include <stdint.h>

typedef struct point {
    int x;
    int y;
    char icon;
} point;

typedef struct point_wide {
    int x;
    int y;
    char icon[POINT_WIDE_LENGTH];
} point_wide;

typedef struct snake {
    int num_segments;
    int score;
    point_wide segments[PARTS_MAX];
} snake;

typedef struct bitmap {
    uint8_t storage[BITMAP_LENGTH];
} bitmap;

#endif