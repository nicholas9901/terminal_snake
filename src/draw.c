#include "prototypes.h"

static char msg_controls_formatted[sizeof(MSG_CONTROLS) - 4];

void init_draw_buffers()
{
  sprintf(
    msg_controls_formatted, 
    MSG_CONTROLS, 
    keymap[KEY_UP],
    keymap[KEY_LEFT],
    keymap[KEY_DOWN],
    keymap[KEY_RIGHT],
    keymap[KEY_PAUSE]);
}

void draw_redraw(snake* s, point* a, point* b) 
{
  if (width_terminal < width_terminal_min || height_terminal < height_terminal_min) {
    paused = TRUE;
    printf(
      TERM_CLEAR YX "Window too small!\n" 
      YX "Required: %u, %u\n"
      YX "Current: %u, %u\n" FMT_CLEAR,
      mid_height_terminal - 1, mid_width_terminal - 9,
      mid_height_terminal, mid_width_terminal - 9, width_terminal_min, height_terminal_min,
      mid_height_terminal + 1, mid_width_terminal - 8, width_terminal, height_terminal);
#ifndef _WIN32
      pause();
#endif
    return;
  }
  draw_snake_all(s);               
  draw_apple(a);                   
  draw_bounds(b);                 
  draw_ribbons();                   
  draw_score(s->score);
  if (paused) {                        
    if (game_over) {
      if (win) {
        draw_win_ribbon();
        return;
      }
      draw_game_over_ribbon(s);
    } else {
      draw_pause();
    }
  }                                    
}

void draw_snake(snake* s, byte clear) 
{
  if (clear) {
    printf(
      YX SPRITE_CLEAR, 
      s->segments[s->ghost_pointer].y + top_bound_terminal, 
      (s->segments[s->ghost_pointer].x * 2) + left_bound_terminal
    );    
}
  printf(
    YX SPRITE_SNAKE_BODY, 
    s->segments[1].y + top_bound_terminal, 
    (s->segments[1].x * 2) + left_bound_terminal
  );
  for (size_t i = NUM_GRADIENTS - 1; i-- != 0;) {
    printf(
      YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[s->gradient_indices[i]].y + top_bound_terminal, 
      (s->segments[s->gradient_indices[i]].x * 2) + left_bound_terminal,
      (*s->gradient_chosen)[i]
    );
  }
  printf(
    YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_terminal, 
    (s->segments[0].x * 2) + left_bound_terminal
  );
}

void draw_snake_all(snake* s) /* For redrawing the entire snake */
{
  /* Draw the body of the snake */
  size_t j = s->ghost_pointer - 1;
  for (size_t i = NUM_GRADIENTS; i-- != 0;) {
    for (; j >= s->gradient_indices[i]; j--) {
      printf(
        YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
        s->segments[j].y + top_bound_terminal, 
        (s->segments[j].x * 2) + left_bound_terminal,
        (*s->gradient_chosen)[i]);
    }
  }
  printf(
    YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_terminal, 
    (s->segments[0].x * 2) + left_bound_terminal
  );
}

void draw_snake_victory(snake* s)
{
  for (size_t i = NUM_GRADIENTS - 1; i-- != 0;) {
    printf(
      YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[s->gradient_indices[i]].y + top_bound_terminal, 
      (s->segments[s->gradient_indices[i]].x * 2) + left_bound_terminal,
      (*s->gradient_chosen)[i]
    );
  }
}

void draw_apple(point* a)
{
  printf(YX SPRITE_APPLE, a->y + top_bound_terminal, (a->x * 2) + left_bound_terminal);
}

void draw_bounds(point* bounds) 
{
  for (size_t i = 0; i < boundary_context.perimeter_size; i++) {
    printf(
      YX "%s",
      bounds[i].y + top_bound_terminal,
      (bounds[i].x * 2) + left_bound_terminal,
      SPRITE_BOUNDARY);
  }
}

void draw_score(unsigned int score)
{
  printf(YX FMT_INFO "%u" FMT_CLEAR, TOP_BOUND, (int) sizeof(MSG_SCORE), score);
}

void draw_ribbons()
{
  printf(Y FMT_INFO "%-*s" FMT_CLEAR, TOP_BOUND, width_terminal, MSG_SCORE);
  printf(Y FMT_INFO "%-*s" FMT_CLEAR, height_terminal, width_terminal, msg_controls_formatted);
}

void draw_pause() {
  printf(YX FMT_INFO MSG_PAUSE FMT_CLEAR, TOP_BOUND, width_terminal - (int) sizeof(MSG_PAUSE) + 1);
}

void draw_unpause() {
  printf(YX FMT_INFO MSG_PAUSE_CLEAR FMT_CLEAR, TOP_BOUND, width_terminal - (int) sizeof(MSG_PAUSE) + 1); 
}

void draw_game_over_ribbon(snake* s)
{
  printf(
    YX "%s",
    s->segments[0].y + top_bound_terminal,
    (s->segments[0].x * 2) + left_bound_terminal,
    SPRITE_CRASH);
  
  printf(
    YX FMT_INFO MSG_GAME_OVER FMT_CLEAR, 
    TOP_BOUND, 
    width_terminal - (int) sizeof(MSG_GAME_OVER) + 3,
    keymap[KEY_RESTART]);
}

void draw_win_ribbon()
{
  printf(
    YX FMT_INFO MSG_WIN FMT_CLEAR, 
    TOP_BOUND, 
    width_terminal - (int) sizeof(MSG_WIN) + 3,
    keymap[KEY_RESTART]);
}
