#ifndef COLLISION_H
#define COLLISION_H

#include "stdint.h"

#include "defs.h"
#include "dtypes.h"
#include "point.h"

int get_bit_linear(bitmap* b, int n) {
    return b->storage[n / BYTE_SIZE] & ((uint8_t) 1 << (n % BYTE_SIZE)) ? 1 : 0;
}

int get_bit(bitmap* b, int x, int y) {
    x -= left_bound_adj;
    y -= top_bound_adj;
    return b->storage[y * BYTES_PER_ROW + x / BYTE_SIZE] & ((uint8_t) 1 << (x % BYTE_SIZE)) ? 1 : 0;
}

void set_bit(bitmap* b, int x, int y) {
    x -= left_bound_adj;
    y -= top_bound_adj;
    b->storage[y * BYTES_PER_ROW + x / BYTE_SIZE] |= ((uint8_t) 1 << (x % BYTE_SIZE));
}

void clear_bit(bitmap* b, int x, int y) {
    x -= left_bound_adj;
    y -= top_bound_adj;
    b->storage[y * BYTES_PER_ROW + x / BYTE_SIZE] &= ~((uint8_t) 1 << (x % BYTE_SIZE));
}

void init_collision(
    bitmap* b, 
    point_u* bounds, 
    snake* s,
    int perimeter) 
{ 
    for (int i = 0; i < BITMAP_LENGTH; i++) {
        b->storage[i] = 0;
    }
    for (int i = 0; i < s->num_segments - 1; i++) {
        set_bit(b, s->segments[i].x, s->segments[i].y);
    }
}

#endif