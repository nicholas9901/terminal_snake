#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* External variables */
extern struct termios initial;

extern byte 
  paused,
  hidden,
  game_over,
  retry,
  redraw;
    
extern unsigned char* keymap;

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
void set_keymap(byte);
byte parse_keypress(unsigned char* key);
static inline byte parse_state_game_over(unsigned char*, byte*);
static inline byte parse_state_hidden(unsigned char*, byte*);
static inline byte parse_state_paused(unsigned char*, byte*);
static inline byte parse_state_movement(unsigned char*);
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
void init_color_table();
void draw_redraw(snake*, point*, point*);
void draw_snake(snake*, byte);
void draw_snake_all(snake* s);
void draw_apple(point*);
void draw_bounds(point*);
void draw_controls();
void draw_score(snake*);
void draw_pause();
void draw_unpause();
void draw_game_over(snake*);

#endif
