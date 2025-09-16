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
      TERM_CLEAR ESC YX "Window too small!\n" 
      ESC YX "Required: %d, %d\n"
      ESC YX "Current: %d, %d\n" FMT_CLEAR,
      mid_height_terminal - 1, mid_width_terminal - 9,
      mid_height_terminal, mid_width_terminal - 9, width_terminal_min, height_terminal_min,
      mid_height_terminal + 1, mid_width_terminal - 8, width_terminal, height_terminal);
    pause();
    return;
  }
  draw_snake_all(s);               
  draw_apple(a);                   
  draw_bounds(b, boundary_context.perimeter_size);                 
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
      ESC YX SPRITE_CLEAR, 
      s->segments[s->ghost_pointer].y + top_bound_terminal, 
      (s->segments[s->ghost_pointer].x * 2) + left_bound_terminal
    );    
}
  printf(
    ESC YX SPRITE_SNAKE_BODY, 
    s->segments[1].y + top_bound_terminal, 
    (s->segments[1].x * 2) + left_bound_terminal
  );
  for (int i = NUM_GRADIENTS - 1; i >= 0; i--) {
    printf(
      ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[s->gradient_indices[i]].y + top_bound_terminal, 
      (s->segments[s->gradient_indices[i]].x * 2) + left_bound_terminal,
      (*s->gradient_chosen)[i]
    );
  }
  printf(
    ESC YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_terminal, 
    (s->segments[0].x * 2) + left_bound_terminal
  );
}

void draw_snake_all(snake* s) /* For redrawing the entire snake */
{
  /* Draw the body of the snake */
  int j = s->ghost_pointer - 1;
  for (int i = NUM_GRADIENTS - 1; i >= 0; i--) {
    for (; j >= s->gradient_indices[i]; j--) {
      printf(
        ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
        s->segments[j].y + top_bound_terminal, 
        (s->segments[j].x * 2) + left_bound_terminal,
        (*s->gradient_chosen)[i]);
    }
  }
  printf(
    ESC YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_terminal, 
    (s->segments[0].x * 2) + left_bound_terminal
  );
}

void draw_snake_victory(snake* s)
{
  for (int i = NUM_GRADIENTS - 1; i >= 0; i--) {
    printf(
      ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[s->gradient_indices[i]].y + top_bound_terminal, 
      (s->segments[s->gradient_indices[i]].x * 2) + left_bound_terminal,
      (*s->gradient_chosen)[i]
    );
  }
}

void draw_apple(point* a)
{
  printf(ESC YX SPRITE_APPLE, a->y + top_bound_terminal, (a->x * 2) + left_bound_terminal);
}

void draw_bounds(point* bounds, int perimeter_size) 
{
  for (int i = 0; i < perimeter_size; i++) {
    printf(
      ESC YX "%s",
      bounds[i].y + top_bound_terminal,
      (bounds[i].x * 2) + left_bound_terminal,
      SPRITE_BOUNDARY);
  }
}

void draw_score(int score)
{
  printf(ESC YX FMT_INFO "%d" FMT_CLEAR, TOP_BOUND, (int) sizeof(MSG_SCORE), score);
}

void draw_ribbons()
{
  printf(ESC Y FMT_INFO "%-*s" FMT_CLEAR, TOP_BOUND, width_terminal, MSG_SCORE);
  printf(ESC Y FMT_INFO "%-*s" FMT_CLEAR, height_terminal, width_terminal, msg_controls_formatted);
}

void draw_pause() {
  printf(ESC YX FMT_INFO MSG_PAUSE FMT_CLEAR, TOP_BOUND, width_terminal - (int) sizeof(MSG_PAUSE) + 1);
}

void draw_unpause() {
  printf(ESC YX FMT_INFO MSG_PAUSE_CLEAR FMT_CLEAR, TOP_BOUND, width_terminal - (int) sizeof(MSG_PAUSE) + 1); 
}

void draw_game_over_ribbon(snake* s)
{
  printf(
    ESC YX "%s",
    s->segments[0].y + top_bound_terminal,
    (s->segments[0].x * 2) + left_bound_terminal,
    SPRITE_CRASH);
  
  printf(
    ESC YX FMT_INFO MSG_GAME_OVER FMT_CLEAR, 
    TOP_BOUND, 
    width_terminal - (int) sizeof(MSG_GAME_OVER) + 3,
    keymap[KEY_RESTART]);
}

void draw_win_ribbon()
{
  printf(
    ESC YX FMT_INFO MSG_WIN FMT_CLEAR, 
    TOP_BOUND, 
    width_terminal - (int) sizeof(MSG_WIN) + 3,
    keymap[KEY_RESTART]);
}
