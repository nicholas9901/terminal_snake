#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* External variables */
extern struct termios initial;

extern byte 
  paused,
  hidden,
  redraw,
  new_segment_added;
    
extern unsigned char
  keymap_qwerty[NUM_KEYS],
  keymap_dvorak[NUM_KEYS],
  keymap_azerty[NUM_KEYS];

extern int 
  width, 
  height, 
  mid_x,
  mid_y,
  apple_x_start,
  perimeter,
  left_bound_adj,
  right_bound_adj,
  top_bound_adj,
  bottom_bound_adj;

/* startup.c */
void clean();
void init_canvas();
void init_term();
void init_signal();

/* input.c */
void set_keymap(unsigned char*);
byte get_input(unsigned char* key);
void queue_input(input_buffer* input_buffer, byte direction);
byte dequeue_input(input_buffer* input_buffer);


/* point.c */
void init_point(point*, int, int);
void init_bounds(point*, byte[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);
void init_snake(snake*, byte[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);
void init_apple(point*, byte[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);

void update_apple(point*, byte[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT]);
void add_segment(snake*);
int move_snake(
  snake*, 
  point*, 
  byte[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT], 
  byte);

/* draw.c */
#define draw_all(snake, apple, bounds) \
  draw_snake_all(snake);               \
  draw_apple(apple);                   \
  draw_bounds(bounds);                 \
  draw_score(snake);                   \
  draw_controls()                      \
  
void init_color_table();
void draw_snake(snake*, byte);
void draw_snake_all(snake* s);
void draw_apple(point*);
void draw_bounds(point*);
void draw_controls();
void draw_score(snake*);

#endif
