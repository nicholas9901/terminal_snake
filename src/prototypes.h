#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* External variables */
extern struct termios initial;

extern uint8_t new_segment_added;

extern int 
  width, 
  height, 
  mid_x,
  mid_y,
  apple_x_start,
  perimeter,
  left_bound_adj,
  left_bound_adj_snk,
  right_bound_adj,
  top_bound_adj,
  bottom_bound_adj;

/* startup.c */
void clean();
void sig_exit(int); 
void init_canvas();
void init_term();

/* point.c */
void init_point(point*, int, int);
void init_point_u(point_u*, int, int, char*);
void init_bounds(point*, uint8_t[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);
void init_snake(snake*, uint8_t[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);
void init_apple(point*, uint8_t[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);

void update_apple(point*, uint8_t[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);
void add_segment(snake*);
char get_movement_key(char* key_curr);
int move_snake(
  snake*, 
  point*, 
  uint8_t[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT], 
  char, char* 
);

/* draw.c */
#define draw_all(snake, apple, bounds) \
  draw_snake(snake, 1);                \
  draw_apple(apple);                   \
  draw_bounds(bounds);                 \
  draw_score(snake);                   \
  draw_controls()                      \
  
void draw_snake(snake*, uint8_t);
void draw_apple(point*);
void draw_bounds(point*);
void draw_controls();
void draw_score(snake*);

#endif
