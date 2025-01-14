#ifndef DTYPES_H
#define DTYPES_H

#include "defs.h"

#include <stdint.h>

typedef struct point {
    int x;
    int y;
    char icon;
} point;

typedef struct point_u {
    int x;
    int y;
    char icon[4];
} point_u;

typedef struct snake {
    int num_segments;
    int score;
    point_u segments[PARTS_MAX];
} snake;

typedef struct bitmap {
    uint8_t storage[BITMAP_LENGTH];
} bitmap;

#endif