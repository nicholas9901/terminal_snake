#include "prototypes.h"

#define    SIZE_TABLE 6
#define    SIZE_COLOR 4
#define   COLOR_START 213
#define    COLOR_STEP 36

static char color_table[SIZE_TABLE][SIZE_COLOR];

void init_color_table()
{
  int color = COLOR_START;
  
  for (int i = 0; i < COLOR_STEP; i++) {
    sprintf(color_table[i], "%d", color);
    color -= COLOR_STEP;
  }
}

void draw_redraw(snake* s, point* a, point* b) 
{
  draw_snake_all(s);               
  draw_apple(a);                   
  draw_bounds(b);                 
  draw_score(s);                   
  draw_controls();
  if (paused) {                        
    draw_pause();
  }                                    
}

void draw_snake(snake* s, byte clear) 
{
#if DEBUG /* Print the location of each segment */
  printf(ESC "0;0H");
  for (int i = 0; i <= s->ghost_pointer; i++) {
    printf("(%d,%d)", s->segments[i].x, s->segments[i].y);
  }
#endif
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
  for (int i = 0; i < NUM_GRADIENT; i++) {
    printf(
      ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[s->gradient_indices[i]].y + top_bound_adj, 
      s->segments[s->gradient_indices[i]].x + left_bound_adj,
      color_table[i]
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
  int j = s->gradient_indices[0];
  for (int i = 0; i < NUM_GRADIENT - 1; i++) {
    for (; j < s->gradient_indices[i + 1]; j++) {
      printf(
        ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
        s->segments[j].y + top_bound_adj, 
        s->segments[j].x + left_bound_adj,
        color_table[i]
      );
    }
  }
  
  /* Draw the tail of the snake */
  for (; j < s->ghost_pointer; j++) {
    printf(
      ESC YX ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, 
      s->segments[j].y + top_bound_adj, 
      s->segments[j].x + left_bound_adj,
      color_table[NUM_GRADIENT - 1]
    );
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

void draw_score(snake* s)
{
  printf(ESC "%d;1H" FMT_INFO " Score: %d", 1, s->score);

  for (int i = 0; i < width - 9; i++) { /* Length of the top ribbon */
    printf(" ");
  }
  printf(FMT_CLEAR); 
}

void draw_controls() 
{
  printf(ESC "%d;1H" FMT_INFO " Quit: [Ctrl-c] Pause: [e] Work: [Space]", height);
  
  for (int i = 0; i < width - 40; i++) { /* Length of the bottom ribbon */
    printf(" ");
  }
  
  printf(FMT_CLEAR); 
}

void draw_pause() {
  printf(ESC YX FMT_INFO "Paused" FMT_CLEAR,  TOP_BOUND, width - 6);
}

void draw_unpause() {
  printf(ESC YX FMT_INFO "      " FMT_CLEAR, TOP_BOUND, width - 6); 
}

void draw_game_over(snake* s)
{
  printf(
    ESC YX "%s",
    s->segments[0].y + top_bound_adj,
    s->segments[0].x + left_bound_adj,
    SPRITE_CRASH);
  
  printf(ESC YX FMT_INFO "Game Over! Restart: [%c]" FMT_CLEAR, 
    TOP_BOUND, 
    width - 23,
    keymap[KEY_RESTART]);
}
