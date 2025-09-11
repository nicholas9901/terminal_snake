#include "prototypes.h"

void init_point(point* p, int x, int y)
{
  p->x = x;
  p->y = y;
}

void init_bounds(point* p, byte c[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT])
{
  int points_index = 0;
  
  
  /* Shift collision fields aside from the left boundary by one to account for
   * the snake which is 2 characters wide */   
   
  for (int i = 2; i < BOUNDARY_WIDTH; i += 2) {
    
    /* Top Border */
    init_point(&p[points_index++], i, 0);

    /* Bottom Border */
    init_point(&p[points_index++], i, BOUNDARY_HEIGHT - 1);
    
    /* Top Border */
    c[i][0] = COLLISION_BAD;

    /* Bottom Border */
    c[i][BOUNDARY_HEIGHT - 1] = COLLISION_BAD;
  }

  for (int i = 1; i < BOUNDARY_HEIGHT - 1; i++) {
    
    /* Left border */
    init_point(&p[points_index++], 0, i);

    /* Right border */
    init_point(&p[points_index++], BOUNDARY_WIDTH, i);
    
    /* Left border */
    c[0][i] = COLLISION_BAD;

    /* Right border */
    c[BOUNDARY_WIDTH][i] = COLLISION_BAD;
  }

  /* Draw the four corners which aren't accounted for by the collision */
  init_point(&p[points_index++], 0, 0);
  init_point(&p[points_index++], BOUNDARY_WIDTH, 0);
  init_point(&p[points_index++], 0, BOUNDARY_HEIGHT - 1);
  init_point(&p[points_index++], BOUNDARY_WIDTH, BOUNDARY_HEIGHT - 1);
}

void init_snake(snake* s, byte c[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT])
{
  int offset             = (PARTS_START * 2);
  s->ghost_pointer       = PARTS_START;
  s->score               = 0;
  s->direction           = KEY_RIGHT;
  s->gradient_pointer    = PARTS_START - 1;
  s->gradient_indices[0] = 1;
  for (int i = 1; i < NUM_GRADIENT; i++) {
    s->gradient_indices[i] = 2;
  }

  init_point(&s->segments[0], offset, BOUNDARY_HEIGHT / 2);

  for (int i = 1; i <= s->ghost_pointer; i++) {
    s->segments[i].x = s->segments[i - 1].x - 2;
    s->segments[i].y = s->segments[i - 1].y;
  }
  
  for (int i = 0; i < s->ghost_pointer; i++) {
    c[s->segments[i].x][s->segments[i].y] = COLLISION_BAD;
  }
}

void init_apple(point* a, byte c[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT])
{
  int adjusted_x = (BOUNDARY_WIDTH / 2) % 2 == 1 ? (BOUNDARY_WIDTH / 2) + 1 : (BOUNDARY_WIDTH / 2);
  init_point(a, adjusted_x, BOUNDARY_HEIGHT / 2);
  c[adjusted_x][BOUNDARY_HEIGHT / 2] = COLLISION_APPLE;
}

void update_apple(point* a, byte c[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT])
{
  int x = ((rand() % ACTIVE_WIDTH) * 2) + 2;
  int y = (rand() % ACTIVE_HEIGHT) + 1;

  while(c[x][y] == COLLISION_BAD) {
    if ((x = (x + 2) % BOUNDARY_WIDTH) == 0) {
      x += 2;
      if ((y = (y + 1) % BOUNDARY_HEIGHT) == 0) { 
          y++; 
      }
    }
  }

  a->x = x;
  a->y = y;
  c[x][y] = COLLISION_APPLE;
  draw_apple(a);
}

void add_segment(snake* s)
{
  if (s->ghost_pointer == ACTIVE_AREA) { exit(0); }
  s->new_segment_added = TRUE;
  s->ghost_pointer++;
  point* tail_1 = &(s->segments[s->ghost_pointer]);
  point* tail_2 = &(s->segments[s->ghost_pointer - 1]);
  point* tail_3 = &(s->segments[s->ghost_pointer - 2]);
  tail_1->x = tail_2->x;
  tail_1->y = tail_2->y;
  tail_2->x = tail_3->x;
  tail_2->y = tail_3->y;

  /* Also update the gradient indices */
  for (int i = s->gradient_pointer; i < NUM_GRADIENT; i++) {
    s->gradient_indices[i]++;
  }
  s->gradient_pointer = (s->gradient_pointer % NUM_GRADIENT) + 1;
}

int move_snake(
  snake* s, 
  point* a, 
  byte c[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT], 
  byte direction
) 
{
  /* The tail of the snake is cleared in each call of `draw_snake()` so it 
   * does not have collision */
  byte* collision_pos;
  byte  clear = TRUE;

  for (int i = s->ghost_pointer; i > 0; i--) {
    s->segments[i].x = s->segments[i - 1].x;
    s->segments[i].y = s->segments[i - 1].y;
  }

  if (!s->new_segment_added) {
    c[s->segments[s->ghost_pointer].x]
     [s->segments[s->ghost_pointer].y] = COLLISION_NONE;
  }
  else {
    s->new_segment_added = FALSE;
    clear = FALSE;
  }
  
  /* Validate the proposed direction before deciding the new one
   * (the snake shouldn't be able to move into itself) */ 
  if (!(((direction + 2) % NUM_DIRECTIONS) == s->direction) && direction != ACTION_NONE) s->direction = direction;
  
  switch(s->direction) {
    case KEY_UP:
      s->segments[0].y--;
      break;
    case KEY_RIGHT:
      s->segments[0].x += 2;        
      break;
    case KEY_DOWN:
      s->segments[0].y++;        
      break;
    case KEY_LEFT:
      s->segments[0].x -= 2;        
      break;
  }

  collision_pos = &c[s->segments[0].x][s->segments[0].y];
  
  switch (*collision_pos) {
    case COLLISION_NONE:
      *collision_pos = COLLISION_BAD;
      draw_snake(s, clear);
      break;
    case COLLISION_APPLE:   
      *collision_pos = COLLISION_BAD;
      s->score++;
      add_segment(s);
      draw_snake(s, clear);
      printf(ESC YX FMT_INFO "%d", 1, 9,  s->score);
      update_apple(a, c);
      break; 
    case COLLISION_BAD:
      draw_snake(s, clear);
      return 0;
    default:
      exit(1);
  }

  return 1;
}
