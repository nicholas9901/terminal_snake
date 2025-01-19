#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>

#include "defs.h"
#include "dtypes.h"

void draw_snake(snake* s, point_u* a) 
{
    #if DEBUG
        printf(esc "0;0H");
        for (int i = 0; i < s->num_segments; i++) {
            printf("(%d,%d)", s->segments[i].x, s->segments[i].y);
        }
    #endif
    printf(esc "%d;%dH%s", a->y, a->x, a->icon);
    for (int i = s->num_segments - 1; i >= 0; i--) {
        printf(esc "%d;%dH%s", s->segments[i].y, s->segments[i].x, s->segments[i].icon);
    }
    printf(esc "%d;%dH ", s->segments[s->num_segments - 1].y, s->segments[s->num_segments - 1].x);
    printf(esc "%d;%dHScore: %d", top_bound_adj - 2, left_bound_adj, s->score);
}

void draw_bounds(point_u* bounds, int perimeter) 
{
    for (int i = 0; i < perimeter; i++) {
        printf(esc "%d;%dH%s", bounds[i].y, bounds[i].x, bounds[i].icon);
    }
}

void draw_controls() {
    printf(esc "%d;1H" esc "38;5;0;48;5;251m" "Quit: [q] Pause: [e]" fmt_clear, height);
}

#endif