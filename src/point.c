#include "prototypes.h"

int allocate_all(snake* s, point** bounds, byte*** collision)
{
  s->segments = malloc(boundary_context.active_area * sizeof(point));
  *bounds = malloc(boundary_context.perimeter_size * sizeof(point));
  *collision = calloc(boundary_context.width, sizeof(byte*));
  
  for (size_t i = 0; i < boundary_context.width; i++) { 
    (*collision)[i] = calloc(boundary_context.height, sizeof(byte));
  }

  if (!s->segments || !bounds || !collision) { return 1; }
  return 0;
}

void init_point(point* p, unsigned int x, unsigned int y)
{
  p->x = x;
  p->y = y;
}

void init_bounds(point* p, byte** c)
{
  size_t points_index = 0;
  
  for (size_t i = 1; i < boundary_context.width - 1; i++) { 
    
    /* Top Border */
    init_point(&p[points_index++], i, 0);

    /* Bottom Border */
    init_point(&p[points_index++], i, boundary_context.height - 1);
    
    /* Top Border */
    c[i][0] = COLLISION_BAD;

    /* Bottom Border */
    c[i][boundary_context.height - 1] = COLLISION_BAD;
  }

  for (size_t i = 1; i < boundary_context.height - 1; i++) {
    
    /* Left border */
    init_point(&p[points_index++], 0, i);

    /* Right border */
    init_point(&p[points_index++], boundary_context.width - 1, i);
    
    /* Left border */
    c[0][i] = COLLISION_BAD;

    /* Right border */
    c[boundary_context.width - 1][i] = COLLISION_BAD;
  }

  /* Draw the four corners which aren't accounted for by the collision */
  init_point(&p[points_index++], 0, 0);
  init_point(&p[points_index++], boundary_context.width - 1, 0);
  init_point(&p[points_index++], 0, boundary_context.height - 1);
  init_point(&p[points_index++], boundary_context.width - 1, boundary_context.height - 1);
}

void init_snake(
  snake* s, 
  byte** c, 
  char (*gradient_chosen)[SIZE_GRADIENT][SIZE_COLOR])
{
  s->ghost_pointer       = PARTS_START;
  s->score               = 0;
  s->direction           = KEY_RIGHT;
  s->new_segment_added   = FALSE;
  s->gradient_chosen     = gradient_chosen;
  s->gradient_pointer    = PARTS_START - 1;
  s->gradient_indices[0] = 1;
  for (size_t i = 1; i < NUM_GRADIENTS; i++) {
    s->gradient_indices[i] = 2;
  }

  init_point(&s->segments[0], s->ghost_pointer, boundary_context.height / 2);

  for (size_t i = 1; i <= s->ghost_pointer; i++) {
    s->segments[i].x = s->segments[i - 1].x - 1;
    s->segments[i].y = s->segments[i - 1].y;
  }
  
  for (size_t i = 0; i < s->ghost_pointer; i++) {
    c[s->segments[i].x][s->segments[i].y] = COLLISION_BAD;
  }
}

void init_apple(point* a, byte** c)
{
  unsigned int x_adjusted = (boundary_context.width + 1) / 2;
  init_point(a, x_adjusted, boundary_context.height / 2);
  c[x_adjusted][boundary_context.height / 2] = COLLISION_APPLE;
}

void reset_bounds(byte** collision)
{
  for (size_t i = 1; i <= boundary_context.active_height; i++) {
    for (size_t j = 1; j <= boundary_context.active_width; j++) {
      collision[j][i] = COLLISION_NONE;
    }
  }
}

void update_apple(point* a, byte** c)
{
  unsigned int
    x = (rand() % boundary_context.active_width) + 1,
    y = (rand() % boundary_context.active_height) + 1;

  while(c[x][y] == COLLISION_BAD) {
    if ((x = (x + 1) % (boundary_context.width - 1)) == 0) {
      x++;
      if ((y = (y + 1) % (boundary_context.height - 1)) == 0) { 
        y++; 
      }
    }
  }

  a->x = x;
  a->y = y;
  c[x][y] = COLLISION_APPLE;
  draw_apple(a);
}

int add_segment(snake* s)
{
  if (s->ghost_pointer == boundary_context.active_area) { 
    return TRUE;
  }
  
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
  for (size_t i = s->gradient_pointer; i < NUM_GRADIENTS; i++) {
    s->gradient_indices[i]++;
  }
  s->gradient_pointer = (s->gradient_pointer % NUM_GRADIENTS) + 1;
  
  return FALSE;
}

int move_snake(
  snake* s, 
  point* a, 
  byte** c, 
  byte direction
) 
{
  byte* collision_pos;
  byte  clear = TRUE;

  for (size_t i = s->ghost_pointer; i > 0; i--) {
    s->segments[i].x = s->segments[i - 1].x;
    s->segments[i].y = s->segments[i - 1].y;
  }

  if (!s->new_segment_added) {
    c[s->segments[s->ghost_pointer].x]
     [s->segments[s->ghost_pointer].y] = COLLISION_NONE;
  } else {
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
      s->segments[0].x++;        
      break;
    case KEY_DOWN:
      s->segments[0].y++;        
      break;
    case KEY_LEFT:
      s->segments[0].x--;        
      break;
  }

  collision_pos = &c[s->segments[0].x][s->segments[0].y];
  
  switch (*collision_pos) {
    case COLLISION_NONE:
      *collision_pos = COLLISION_BAD;
      draw_snake(s, clear);
      return STATUS_NONE;
    case COLLISION_APPLE:   
      *collision_pos = COLLISION_BAD;
      s->score++;
      if (add_segment(s)) {
        draw_snake(s, clear);
        return STATUS_WIN;
      }
      draw_snake(s, clear);
      draw_score(s->score);
      update_apple(a, c);
      return STATUS_NONE; 
    case COLLISION_BAD:
      draw_snake(s, clear);
      return STATUS_BAD;
  }

  return STATUS_BAD;
}

void free_all(snake* s, point* b, byte** c)
{
  free(s->segments);
  free(b);
  for (size_t i = 0; i < boundary_context.width; i++) { 
    free(c[i]);
  }
  free(c);
}
