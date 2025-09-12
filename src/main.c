#include "prototypes.h"

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) 
{
  snake snake;
  point apple;
  point bounds[PERIMETER_SIZE];
  byte collision[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT] = {0};
  input_buffer input_buffer = {{KEY_RIGHT, KEY_RIGHT, KEY_RIGHT}, 0};
  unsigned char key = KEY_NONE;
  
  srand(time(NULL));
  fcntl(0, F_SETFL, O_NONBLOCK);
  set_keymap(QWERTY);
  init_term();
  init_canvas();
  init_signal();
  init_color_table();
  init_bounds(bounds, collision);
  init_snake(&snake, collision);
  init_apple(&apple, collision);
  
  for (;;) {
    for (int times_to_poll = 0; times_to_poll < GAME_WAIT / POLLING_RATE; times_to_poll++) {
      queue_input(&input_buffer, get_action(&key));
      if (redraw) {
        redraw = FALSE;
        draw_redraw(&snake, &apple, bounds);
      } else if (retry) {
        retry     = FALSE;
        paused    = FALSE;
        game_over = FALSE;
        input_buffer.inputs[0] = KEY_RIGHT;
        input_buffer.inputs[1] = KEY_RIGHT;
        memset(collision, 0, sizeof(collision));
        init_bounds(bounds, collision);
        init_snake(&snake, collision);
        init_apple(&apple, collision);
        printf(TERM_CLEAR);
        draw_redraw(&snake, &apple, bounds);
      }
      usleep(POLLING_RATE);
    }

    if (!paused) {
      if (!move_snake(&snake, &apple, collision, dequeue_input(&input_buffer))) {
        game_over = TRUE;
        paused    = TRUE;
        draw_game_over(&snake);
      }
    }
  }
}
