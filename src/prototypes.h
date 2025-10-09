#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#include "definitions.h"

/* External variables */
#ifndef _WIN32
extern struct termios initial;
#endif

extern u8 
  paused,
  hidden,
  redraw,
  retry,
  game_over,
  win,
  quit;
    
extern unsigned char* keymap;

extern u16 
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

/* compatibility.c */
void get_key(unsigned char*);
void get_terminal_size(u16* width, u16* height);
void terminal_switch_alternate();
void terminal_switch_main();

/* startup.c */
void clean();
void init_canvas();
void init_term();
void init_signal();
void handle_exit(int);

/* input.c */
void set_keymap(u8);
u8 get_action(unsigned char* key);
u8 parse_action(unsigned char* key);
u8 parse_state_game_over(unsigned char*, u8*);
u8 parse_state_hidden(unsigned char*, u8*);
u8 parse_state_paused(unsigned char*, u8*);
u8 parse_state_movement(unsigned char*);
void queue_input(input_buffer* input_buffer, u8 direction);
u8 dequeue_input(input_buffer* input_buffer);


/* point.c */
u8 allocate_all(snake*, point**, u8***);
void init_point(point*, u16, u16);
void init_bounds(point*, u8**);
void init_snake(snake*, u8**, char (*)[SIZE_GRADIENT][SIZE_COLOR]);
void init_apple(point*, u8**);

void reset_bounds(u8**);
void update_apple(point*, u8**);
u8 add_segment(snake*);
u8 move_snake(
  snake*, 
  point*, 
  u8**, 
  u8);
void free_all(snake*, point*, u8**);

/* draw.c */
void init_draw_buffers();
void draw_redraw(snake*, point*, point*);
void draw_snake(snake*, u8);
void draw_snake_all(snake*);
void draw_snake_victory(snake* s);
void draw_apple(point*);
void draw_bounds(point*);
void draw_score(u16);
void draw_ribbons();
void draw_pause();
void draw_unpause();
void draw_game_over_ribbon(snake*);
void draw_win_ribbon();

#endif
