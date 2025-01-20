#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>

#include "defs.h"
#include "dtypes.h"

void draw_snake(snake* s, point_wide* a) 
{
    #if DEBUG /* Print the location of each segment */
        printf(esc "0;0H");
        for (int i = 0; i < s->num_segments; i++) {
            printf("(%d,%d)", s->segments[i].x, s->segments[i].y);
        }
    #endif

    printf(esc yx "%s", a->y, a->x, a->icon);
    for (int i = s->num_segments - 1; i >= 0; i--) {
        printf(esc yx "%s", s->segments[i].y, s->segments[i].x, s->segments[i].icon);
    }
    printf(esc yx "  ", s->segments[s->num_segments - 1].y, s->segments[s->num_segments - 1].x);
    printf(esc yx "Score: %d", top_bound_adj - 2, left_bound_adj, s->score);
}

void draw_bounds(point_wide* bounds, int perimeter) 
{
    for (int i = 0; i < perimeter; i++) {
        printf(esc yx "%s", bounds[i].y, bounds[i].x, bounds[i].icon);
    }
}

void draw_controls() {
    printf(esc "%d;1H" esc "38;5;0;48;5;251m" "Quit: [q] Pause: [e]" fmt_clear, height);
}

#endif