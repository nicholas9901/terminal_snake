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
  draw_snake_all(s);               
  draw_apple(a);                   
  draw_bounds(b);                 
  draw_ribbons();                   
  draw_score(s->score);
  if (paused) {                        
    if (game_over) {
      draw_game_over(s);
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
      s->segments[s->ghost_pointer].y + top_bound_adj, 
      s->segments[s->ghost_pointer].x + left_bound_adj
    );    
}
  printf(
    ESC YX SPRITE_SNAKE_BODY, 
    s->segments[1].y + top_bound_adj, 
    s->segments[1].x + left_bound_adj
  );
  for (int i = NUM_GRADIENT - 1; i >= 0; i--) {
    printf(
      ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[s->gradient_indices[i]].y + top_bound_adj, 
      s->segments[s->gradient_indices[i]].x + left_bound_adj,
      (*s->gradient_chosen)[i]
    );
  }
  printf(
    ESC YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_adj, 
    s->segments[0].x + left_bound_adj
  );
}

void draw_snake_all(snake* s) /* For redrawing the entire snake */
{
  /* Draw the body of the snake */
  int j = s->ghost_pointer - 1;
  for (int i = NUM_GRADIENT - 1; i >= 0; i--) {
    for (; j >= s->gradient_indices[i]; j--) {
      printf(
        ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
        s->segments[j].y + top_bound_adj, 
        s->segments[j].x + left_bound_adj,
        (*s->gradient_chosen)[i]);
    }
  }
  printf(
    ESC YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_adj, 
    s->segments[0].x + left_bound_adj
  );
}

void draw_apple(point* a)
{
  printf(ESC YX SPRITE_APPLE, a->y + top_bound_adj, a->x + left_bound_adj);
}

void draw_bounds(point* bounds) 
{
  for (int i = 0; i < perimeter; i++) {
    printf(
      ESC YX "%s",
      bounds[i].y + top_bound_adj,
      bounds[i].x + left_bound_adj,
      SPRITE_BOUNDARY);
  }
}

void draw_score(int score)
{
  printf(ESC YX FMT_INFO "%d" FMT_CLEAR, TOP_BOUND, (int) sizeof(MSG_SCORE), score);
}

void draw_ribbons()
{
  printf(ESC Y FMT_INFO "%-*s" FMT_CLEAR, TOP_BOUND, width, MSG_SCORE);
  printf(ESC Y FMT_INFO "%-*s" FMT_CLEAR, height, width, msg_controls_formatted);
}

void draw_pause() {
  printf(ESC YX FMT_INFO MSG_PAUSE FMT_CLEAR, TOP_BOUND, width - (int) sizeof(MSG_PAUSE) + 1);
}

void draw_unpause() {
  printf(ESC YX FMT_INFO MSG_PAUSE_CLEAR FMT_CLEAR, TOP_BOUND, width - (int) sizeof(MSG_PAUSE) + 1); 
}

void draw_game_over(snake* s)
{
  printf(
    ESC YX "%s",
    s->segments[0].y + top_bound_adj,
    s->segments[0].x + left_bound_adj,
    SPRITE_CRASH);
  
  printf(ESC YX FMT_INFO MSG_GAME_OVER FMT_CLEAR, 
    TOP_BOUND, 
    width - (int) sizeof(MSG_GAME_OVER) + 3,
    keymap[KEY_RESTART]);
}
