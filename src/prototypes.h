#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* External variables */
extern struct termios initial;

extern int 
    width, 
    height, 
    mid_x, 
    mid_y,
    left_bound_adj,
    right_bound_adj,
    top_bound_adj,
    bottom_bound_adj;

/* startup.c */
void clean();
void sig_exit(int); 
void init_canvas();
void init_term();

/* point.c */
void init_point(point*, int, int, char*);
void init_point_wide(point_wide*, int, int, char*);
void init_snake(snake*, uint8_t[BOUND_WIDTH][BOUND_HEIGHT]);
void init_bounds(point*, uint8_t[BOUND_WIDTH][BOUND_HEIGHT]);

void random_pos(point_wide*, uint8_t[BOUND_WIDTH][BOUND_HEIGHT], snake*);
void add_segment(snake*);
char get_movement_key(char* key_curr);
int move(
    snake*, 
    point_wide*, 
    uint8_t[BOUND_WIDTH][BOUND_HEIGHT], 
    char, char* 
);

/* draw.c */
void draw_sprites(snake*, point_wide*);
void draw_bounds(point*, int);
void draw_controls();

#endif