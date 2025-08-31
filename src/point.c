#include "prototypes.h"

void init_point(point* p, int x, int y)
{
  p->x = x;
  p->y = y;
}

void init_point_u(point_u* p, int x, int y, char* icon)
{
  p->x = x;
  p->y = y;
  strcpy(p->icon, icon);
}

void init_bounds(point* p, uint8_t c[BOUNDARY_WIDTH][BOUNDARY_HEIGHT])
{
  /* stored as relative coordinates since boundaries don't change */
  int points_index = 0;
  
  for (int i = left_bound_adj; i <= right_bound_adj; i += 2) {
    /* Top Border */
    init_point(&p[++points_index], i, top_bound_adj);

    /* Bottom Border */
    init_point(&p[++points_index], i, bottom_bound_adj);
  } 

  for (int i = top_bound_adj + 1; i < bottom_bound_adj; i++) {
    /* Left border */
    init_point(&p[++points_index], left_bound_adj, i);

    /* Right border */
    init_point(&p[++points_index], right_bound_adj, i);
  }

  /*
  Shift collision fields aside from the left boundary by one to account for
  the snake which is 2 characters wide    
  */
   
  for (int i = 2; i < BOUNDARY_WIDTH; i += 2) {
    /* Top Border */
    c[i][0] = COLLISION_BAD;

    /* Bottom Border */
    c[i][BOUNDARY_HEIGHT - 1] = COLLISION_BAD;
  }

  for (int i = 1; i < BOUNDARY_HEIGHT - 1; i++) {
    /* Left border */
    c[0][i] = COLLISION_BAD;

    /* Right border */
    c[BOUNDARY_WIDTH][i] = COLLISION_BAD;
  }
}

void init_snake(snake* s, uint8_t c[BOUNDARY_WIDTH][BOUNDARY_HEIGHT])
{
  int offset       = (PARTS_START * 2);
  s->ghost_pointer = PARTS_START;
  s->score         = 0;

  init_point(&s->segments[0], offset, BOUNDARY_HEIGHT / 2);

  for (int i = 1; i <= s->ghost_pointer; i++) {
    s->segments[i].x = s->segments[i - 1].x - 2;
    s->segments[i].y = s->segments[i - 1].y;
  }
  
  for (int i = 0; i < s->ghost_pointer; i++) {
    c[s->segments[i].x][s->segments[i].y] = COLLISION_BAD;
  }
}

void init_apple(point* a, uint8_t c[BOUNDARY_WIDTH][BOUNDARY_HEIGHT])
{
  int adjusted_x = (BOUNDARY_WIDTH / 2) % 2 == 1 ? (BOUNDARY_WIDTH / 2) + 1 : (BOUNDARY_WIDTH / 2);
  init_point(a, adjusted_x, BOUNDARY_HEIGHT / 2);
  c[adjusted_x][BOUNDARY_HEIGHT / 2] = COLLISION_APPLE;
}

void update_apple(point* a, uint8_t c[BOUNDARY_WIDTH][BOUNDARY_HEIGHT])
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
  new_segment_added = 1;
  s->ghost_pointer++;
  point* tail_1 = &(s->segments[s->ghost_pointer]);
  point* tail_2 = &(s->segments[s->ghost_pointer - 1]);
  point* tail_3 = &(s->segments[s->ghost_pointer - 2]);
  tail_1->x = tail_2->x;
  tail_1->y = tail_2->y;
  tail_2->x = tail_3->x;
  tail_2->y = tail_3->y;
}

char get_movement_key(char* key_curr)
{
  switch(*key_curr) {
    case '\x1b': // Arrow Keys
      read(0, key_curr, 1);
      read(0, key_curr, 1);
      switch(*key_curr) {
        case 'A': // Up
          return 'w';
        case 'B': // Down
          return 's';
        case 'C': // Right
          return 'd';
        case 'D': // Left
          return 'a';
      }
      break;

    case 'w':
      return 'w';
    case 's':
      return 's';
    case 'd':
      return 'd';
    case 'a':
      return 'a';
  }
  return '0';
}

int move_snake(
  snake* s, 
  point* a, 
  uint8_t c[BOUNDARY_WIDTH][BOUNDARY_HEIGHT], 
  char key_curr, 
  char* key_prev
) 
{
  /*
  The tail of the snake is cleared in each call of `draw_sprites()` so it 
  does not have collision
  */
  uint8_t* collision_pos;
  uint8_t  done  = 0;
  uint8_t  clear = 1;

  for (int i = s->ghost_pointer; i > 0; i--) {
    s->segments[i].x = s->segments[i - 1].x;
    s->segments[i].y = s->segments[i - 1].y;
  }

  if (!new_segment_added) {
    c[s->segments[s->ghost_pointer].x]
     [s->segments[s->ghost_pointer].y] = COLLISION_NONE;
  }
  else {
    new_segment_added = 0;
    clear = 0;
  }
  
  while (!done) {
    switch(key_curr) {
      case 'w':
        s->segments[0].y--;
        done = 1;
        break;
      case 's':
        s->segments[0].y++;
        done = 1;
        break;
      case 'a':
        s->segments[0].x -= 2;
        done = 1;
        break;
      case 'd':
        s->segments[0].x += 2;
        done = 1;
        break;
      default:
        key_curr = *key_prev;
        continue;
      }
  }

  *key_prev          = key_curr;
  collision_pos      = &(c[s->segments[0].x][s->segments[0].y]);

  switch (*collision_pos) {
    case COLLISION_NONE:
      *collision_pos = COLLISION_BAD;
      draw_sprites(s, clear);
      break;
    case COLLISION_APPLE:   
      *collision_pos = COLLISION_BAD;
      s->score++;
      add_segment(s);
      draw_sprites(s, clear);
      update_apple(a, c);
      break; 
    case COLLISION_BAD:
      draw_sprites(s, clear);
      return 0;
    default:
      exit(1);
  }

  return 1;
}
