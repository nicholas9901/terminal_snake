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

  draw_all(&snake, &apple, bounds);

#if DEBUG
  printf(ESC "2;0Hwidth:%d height:%d %d %d", width, height, mid_x, mid_y);
  printf(ESC "3;0Htba:%d lba:%d", top_bound_adj, left_bound_adj);
#else
  fcntl(0, F_SETFL, O_NONBLOCK);
#endif

  /* The snake starts by going to the right */
  char 
    key_curr = 'd',
    key_prev = 'd';

  /* Main loop */    
  while(1) {
    read(0, &key_curr, 1);

    /* Scan for each possible input */
    switch(key_curr) {
      
      /* Movement-related inputs */
      case '\x1b':            // Arrow Keys
        read(0, & key_curr, 2); // Skip to the important part of the key code
        switch(key_curr) {
          case 'A':           // Up
            key_curr = 'w';
            break;
          case 'B':           // Down
            key_curr = 's';
            break;
          case 'C':           // Right
            key_curr = 'd';
            break;
          case 'D':           // Left
            key_curr = 'a';
            break;
        }

      /* Menu-related inputs */
      case 'q': /* Quit */
        exit(0);
      case 'e': /* Pause */
        key_curr = KEY_NONE;
        printf(ESC YX FMT_INFO "Paused" FMT_CLEAR,  TOP_BOUND, width - 6);
        while(key_curr != 'e') {
          read(0, &key_curr, 1);
          if (key_curr == 'q') exit(0);
          usleep(POLLING_RATE);
        }
        printf(ESC YX FMT_INFO "      " FMT_CLEAR, TOP_BOUND, width - 6); 
        key_curr = KEY_NONE;
        break;
      case ' ': /* Work pause (pause and switch to the alternate buffer) */
        key_curr = KEY_NONE;
        puts(ALT_BUF OFF);
        while(key_curr == KEY_NONE) {
          read(0, &key_curr, 1);
          if (key_curr == 'q') exit(0);
          usleep(POLLING_RATE);
        }
        puts(ALT_BUF ON);
        draw_all(&snake, &apple, bounds);
        if (key_curr == 'e') continue;
        else if (key_curr == ' ') key_curr = KEY_NONE;
        continue;
    }

#if DEBUG
    switch (key_curr) {
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
  printf(ESC YX FMT_INFO "Game Over" FMT_CLEAR, TOP_BOUND, width - 9);
  while (key_curr != 'q' && key_curr != '\n') {
    read(0, &key_curr, 1);
    usleep(POLLING_RATE);
  }
  clean();
}
