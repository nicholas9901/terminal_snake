#include "prototypes.h"

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

int main() 
{
  snake snake;
  point apple;
  point bounds[PERIMETER_SIZE];
  byte collision[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT] = {0};
  input_buffer input_buffer = {{RIGHT, RIGHT, RIGHT}, 0};
  unsigned char key = KEY_NONE;
  
  set_keymap(keymap_qwerty);
  srand(time(NULL));

  init_canvas();
  init_term();
  init_signal();
  init_color_table();
  
  init_bounds(bounds, collision);
  init_snake(&snake, collision);
  init_apple(&apple, collision);

  fcntl(0, F_SETFL, O_NONBLOCK);
  
  for (;;) {
    for (int times_to_poll = 0; times_to_poll < GAME_WAIT / POLLING_RATE; times_to_poll++) {
      queue_input(&input_buffer, get_input(&key));
      if (redraw) {
        redraw = FALSE;
        draw_all(&snake, &apple, bounds);
        printf(ESC YX FMT_INFO "Paused" FMT_CLEAR,  TOP_BOUND, width - 6);
      }
      usleep(POLLING_RATE);
    }

    if (!paused) {
      if (!move_snake(&snake, &apple, collision, dequeue_input(&input_buffer))) {
        printf(ESC YX "%s", snake.segments[0].y + top_bound_adj, snake.segments[0].x + left_bound_adj, SPRITE_CRASH);
        printf(ESC YX FMT_INFO "Game Over! Restart: [r]" FMT_CLEAR, TOP_BOUND, width - 23);
        key = KEY_NONE;
        while (key != 'r') {
          read(0, &key, 1);
          usleep(POLLING_RATE);
        }    
        puts(TERM_CLEAR);
        key = 'd';
        memset(collision, 0, sizeof(collision));
        init_bounds(bounds, collision);
        init_snake(&snake, collision);
        init_apple(&apple, collision);
        draw_all(&snake, &apple, bounds);
      }
    }
  }
}
