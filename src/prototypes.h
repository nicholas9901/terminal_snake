#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* External variables */
extern struct termios initial;

extern byte 
  paused,
  hidden,
  redraw,
  retry,
  game_over,
  win,
  quit;
    
extern unsigned char* keymap;

extern int 
  width_terminal, 
  height_terminal, 
  mid_width_terminal,
  mid_height_terminal,
  width_terminal_min,
  height_terminal_min,
  left_bound_terminal,
  right_bound_terminal,
  top_bound_terminal,
  bottom_bound_terminal;
  
extern gradient_pair gradient_table[NUM_GRADIENTS];
extern context boundary_context;

/* main.c */
static inline void usage_and_exit();

/* startup.c */
void clean();
void init_canvas();
void init_term();
void init_signal();
void handle_exit();

/* input.c */
void set_keymap(byte);
byte get_action(unsigned char* key);
byte parse_keypress(unsigned char* key);
static inline byte parse_state_game_over(unsigned char*, byte*);
static inline byte parse_state_hidden(unsigned char*, byte*);
static inline byte parse_state_paused(unsigned char*, byte*);
static inline byte parse_state_movement(unsigned char*);
void queue_input(input_buffer* input_buffer, byte direction);
byte dequeue_input(input_buffer* input_buffer);


/* point.c */
int allocate_all(snake*, point**, byte***);
void init_point(point*, int, int);
void init_bounds(point*, byte**);
void init_snake(snake*, byte**, char (*)[SIZE_GRADIENT][SIZE_COLOR]);
void init_apple(point*, byte**);

void reset_bounds(byte**);
void update_apple(point*, byte**);
int add_segment(snake*);
int move_snake(
  snake*, 
  point*, 
  byte**, 
  byte);
void free_all(snake*, point*, byte**);

/* draw.c */
void init_draw_buffers();
void draw_redraw(snake*, point*, point*);
void draw_snake(snake*, byte);
void draw_snake_all(snake*);
void draw_snake_victory(snake* s);
void draw_apple(point*);
void draw_bounds(point*, int);
void draw_score(int);
void draw_ribbons();
void draw_pause();
void draw_unpause();
void draw_game_over_ribbon(snake*);
void draw_win_ribbon();

#endif
