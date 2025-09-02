#include "prototypes.h"

void draw_snake(snake* s, uint8_t clear) 
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
      s->segments[s->ghost_pointer].x + left_bound_adj_snk
    );    
}
  printf(
    ESC YX SPRITE_SNAKE_BODY, 
    s->segments[1].y + top_bound_adj, 
    s->segments[1].x + left_bound_adj_snk
  );
  printf(
    ESC YX SPRITE_SNAKE_HEAD, 
    s->segments[0].y + top_bound_adj, 
    s->segments[0].x + left_bound_adj_snk
  );
}

void draw_apple(point* a)
{
  printf(ESC YX SPRITE_APPLE, a->y + top_bound_adj, a->x + left_bound_adj_snk);
}

void draw_bounds(point* bounds) 
{
  for (int i = 0; i < perimeter; i++) {
    printf(ESC YX "%s", bounds[i].y, bounds[i].x, SPRITE_BOUNDARY);
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
  printf(ESC "%d;1H" FMT_INFO " Quit: [q] Pause: [e] Work: [Space]", height);
  
  for (int i = 0; i < width - 35; i++) { /* Length of the bottom ribbon */
    printf(" ");
  }
  
  printf(FMT_CLEAR); 
}

