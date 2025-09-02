#include "prototypes.h"

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

int main() 
{
  snake snake;
  point apple;
  point bounds[PERIMETER_SIZE];
  uint8_t collision[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT] = {0};

  srand(time(NULL));

  init_canvas();
  init_term();
  
  init_bounds(bounds, collision);
  init_snake(&snake, collision);
  init_apple(&apple, collision);

  draw_snake(&snake, 1);
  draw_apple(&apple);
  draw_bounds(bounds, PERIMETER_SIZE);
  draw_score(&snake);
  draw_controls();

#if DEBUG
  printf(ESC "2;0Hwidth:%d height:%d %d %d", width, height, mid_x, mid_y);
  printf(ESC "3;0Htba:%d lba:%d", top_bound_adj, left_bound_adj);
#else
  fcntl(0, F_SETFL, O_NONBLOCK);
#endif

  char 
    key_main = '0',
    key_curr = 'd',
    key_prev = 'd';
    
  while(1) {
    read(0, &key_main, 1);
    key_curr = get_movement_key(&key_main);

    switch(key_main) {
      case 'q':
        exit(0);
      case 'e':
        key_main = '0';
        printf(ESC YX FMT_INFO "Paused" FMT_CLEAR,  1, width - 6);
        while(key_main != 'e') {
            read(0, &key_main, 1);
            if (key_main == 'q') {
                exit(0);
            }
            usleep(10000);
        }
        printf(ESC YX FMT_INFO "      " FMT_CLEAR, 1, width - 6); 
        key_main = '0';
        break;
    }

#if DEBUG
    switch (key_main) {
      case 'n':
        add_segment(&snake);
        break;
      case 'z':
        update_apple(&apple, collision);
        break;
    }
#endif
   if (!move_snake(&snake, &apple, collision, key_curr, &key_prev)) {
      printf(ESC YX "%s", snake.segments[0].y + top_bound_adj, snake.segments[0].x + left_bound_adj_snk, SPRITE_CRASH);
      break;
      
    }

#if DEBUG
    for (int i = 0; i < BOUNDARY_WIDTH_SNAKE; i += 2) {
      for (int j = 0; j < BOUNDARY_HEIGHT; j++) {
        printf(ESC "%d;%dH%d", j+top_bound_adj, i-BOUNDARY_WIDTH_SNAKE+left_bound_adj_snk,collision[i][j]);
      }
    }
    printf(ESC YX "X", snake.segments[0].y+top_bound_adj, snake.segments[0].x-BOUNDARY_WIDTH_SNAKE+left_bound_adj_snk);
#endif

#if !DEBUG
    usleep(GAME_WAIT);
#endif
  }
  printf(ESC "%d;%dHGame Over!", bottom_bound_adj + 2, left_bound_adj);
  while (key_main != 'q' && key_main != '\n') {
    read(0, &key_main, 1);
    usleep(10000);
  }
  clean();
}
