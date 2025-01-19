#ifndef POINT_H
#define POINT_H 

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include "startup.h"
#include "dtypes.h"
#include "collision.h"


void init_point(point* p, char icon)
{
    p->x = 40;
    p->y = 20;
    p->icon = icon;
}

void init_point_u(point_u* p, int x, int y, char* icon)
{
    p->x = x;
    p->y = y;
    strcpy(p->icon, icon);
}

void init_snake(snake* s)
{
    s->num_segments = PARTS_START;
    s->score        = 0;
    init_point_u(&s->segments[0], mid_x, mid_y, "█");
    for (int i = 1; i < s->num_segments; i++) {
        s->segments[i].x = s->segments[i - 1].x + 1;
        s->segments[i].y = s->segments[i - 1].y;
        strcpy(s->segments[i].icon, "█");
    }
}

void init_bounds(point_u* points)
{
    int points_index = 3;
    char
        box_vertical_bar[]   = "┃",
        box_horizontal_bar[] = "━",
        box_top_left[]       = "┏",
        box_top_right[]      = "┓",
        box_bottom_left[]    = "┗",
        box_bottom_right[]   = "┛";

    // Top left
    init_point_u(&points[0], left_bound_adj, top_bound_adj, box_top_left);
    
    // Top right
    init_point_u(&points[1], right_bound_adj, top_bound_adj, box_top_right);
    
    // Bottom left
    init_point_u(&points[2], left_bound_adj, bottom_bound_adj, box_bottom_left);
    
    // Bottom right
    init_point_u(&points[3], right_bound_adj, bottom_bound_adj, box_bottom_right);
    
    // Top border
    for (int i = left_bound_adj + 1; i < right_bound_adj; i++) {
        init_point_u(&points[++points_index], i, top_bound_adj, box_horizontal_bar);
    }

    // Left border
    for (int i = top_bound_adj + 1; i < bottom_bound_adj; i++) {
        init_point_u(&points[++points_index], left_bound_adj, i, box_vertical_bar);
    }

    // Right border
    for (int i = top_bound_adj + 1; i < bottom_bound_adj; i++) {
        init_point_u(&points[++points_index], right_bound_adj, i, box_vertical_bar);
    }

    // Bottom border
    for (int i = left_bound_adj + 1; i < right_bound_adj; i++) {
        init_point_u(&points[++points_index], i, bottom_bound_adj, box_horizontal_bar);
    }
}

void rand_point_u(point_u* p, bitmap* b, snake* s)
{
    int rand_bit = rand() % BITMAP_LENGTH;
    while (get_bit_linear(b, rand_bit)) {
        rand_bit %= BITMAP_LENGTH;
        rand_bit++; 
    }

    p->x = (rand_bit % ACTIVE_WIDTH) + left_bound_adj + 1;
    p->y = (rand_bit / ACTIVE_WIDTH) + top_bound_adj + 1;

    #if DEBUG
        // for (int i = top_bound_adj+1; i <= bottom_bound_adj-1; i++) {
        //     for (int j = left_bound_adj+1; j <= right_bound_adj-1; j++) {
        //         printf(esc "%d;%dHa", p->y+10,p->x);
        //     }
        // }
        printf(esc "4;0Haw:%d", ACTIVE_WIDTH);
        printf(esc "5;0Hcx:%d cy:%d", (rand_bit % ACTIVE_WIDTH) + left_bound_adj, (rand_bit / ACTIVE_WIDTH) + top_bound_adj);
        printf(esc "6;0Hrb:%d", rand_bit);
        printf(esc "7;0Hpx:%d", p->x);
        printf(esc "8;0Hpy:%d", p->y);
        printf(esc "9;0Hr%%aw:%d", rand_bit % ACTIVE_WIDTH);
        printf(esc "10;0Hr+aw:%d", rand_bit + ACTIVE_WIDTH);
        printf(esc "11;0H%d", rand_bit % ACTIVE_WIDTH);
        printf(esc "13;0Hrb:%d", rand_bit);
        printf(esc "12;0Haw:%d", ACTIVE_WIDTH);
    # endif
}

void add_segment(snake* s)
{
    point_u* curr = &s->segments[s->num_segments];
    point_u  prev = s->segments[s->num_segments - 1];
    curr->x = prev.x;
    curr->y = prev.y;
    strcpy(curr->icon, "█");
    s->num_segments++;
}

char get_movement_key(char* key_curr)
{
    switch(*key_curr) {
        case '\x1b': // Arrow Keys
            read(0, key_curr, 1);
            read(0, key_curr, 1);
            switch(*key_curr) {
                case 'A': // Up
                    return 'w';
                case 'B': // Down
                    return 's';
                case 'C': // Right
                    return 'd';
                case 'D': // Left
                    return 'a';
            }
        break;

        case 'w':
            return 'w';
        case 's':
            return 's';
        case 'd':
            return 'd';
        case 'a':
            return 'a';
    }
    return '0';
}

int out_of_bounds(int x, int y) {
    if (
        x <= left_bound_adj  || 
        x >= right_bound_adj ||
        y <= top_bound_adj   || 
        y >= bottom_bound_adj
    ) {
        return 1;
    }
    return 0;
}

int move(snake* s, point_u* a, bitmap* b, char key_curr, char* key_prev)
{
    /*
    The tail of the snake is cleared in each call of `draw_snake()` so it does
    not have collision
    */
    bool done = false;

    clear_bit(
        b, 
        s->segments[s->num_segments - 1].x, 
        s->segments[s->num_segments - 1].y
    );

    for (int i = s->num_segments - 1; i > 0; i--) {
        s->segments[i].x = s->segments[i - 1].x;
        s->segments[i].y = s->segments[i - 1].y;
    }
    
    while (!done) {
        switch(key_curr) {
            case 'w':
                s->segments[0].y--;
                done = true;
                break;
            case 's':
                s->segments[0].y++;
                done = true;
                break;
            case 'a':
                s->segments[0].x--;
                done = true;
                break;
            case 'd':
                s->segments[0].x++;
                done = true;
                break;
            default:
                key_curr = *key_prev;
                continue;
        }
    }
    *key_prev = key_curr;

    if (a->x == s->segments[0].x && a->y == s->segments[0].y) {
        add_segment(s);
        rand_point_u(a, b, s);
        s->score++;
    }

    if (
        get_bit(b, s->segments[0].x, s->segments[0].y) || 
        out_of_bounds(s->segments[0].x, s->segments[0].y)
    ) {
        strcpy(s->segments[0].icon, "X");
        return 0;
    }
    
    set_bit(b, s->segments[0].x, s->segments[0].y);

    return 1;
}

#endif