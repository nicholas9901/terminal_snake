#include "prototypes.h"

#include <time.h>
#include <unistd.h>
#include <getopt.h>

static inline void usage_and_exit()
{
	printf(
    "Usage: terminal_snake [OPTIONS]...\n\n"
    "Option        Long Option           Description\n"
    "-h            --help                Display this message\n"
    "-k <keymap>   --keymap=<keymap>     Choose the keymap (qwerty|azerty|dvorak)\n"
    "-g <gradient> --gradient=<gradient> Choose the gradient\n\n"
    "Gradients:\n");
  for (int i = 0; i < NUM_GRADIENTS; i++) {
    printf("%-*s", SIZE_NAME, gradient_table[i].name);
    for (int j = SIZE_GRADIENT - 1; j >= 0; j--) {
      printf(ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, gradient_table[i].colors[j]);
    }
    printf(ESC SPRITE_SNAKE_HEAD"\n");
  }
  exit(0);
}

int main(int argc, char** argv) 
{
  char opt;
  byte keymap_chosen = QWERTY;
  char (*gradient_chosen)[SIZE_GRADIENT][SIZE_COLOR] = &(gradient_table[2].colors);
  const struct option long_options[] = {
    {"help",     0, NULL, 'h'},
    {"keymap",   1, NULL, 'k'},
    {"gradient", 1, NULL, 'g'},
    {NULL,       0, NULL,   0}
  };
    
	while ((opt = getopt_long(argc, argv, "hk:g:", long_options, NULL)) > 0) {
	  switch (opt) {
	    case 'h':
	      usage_and_exit();
	    case 'k':
	      if      (!strcmp(optarg, "qwerty")) break;
	      else if (!strcmp(optarg, "azerty")) keymap_chosen = AZERTY;
	      else if (!strcmp(optarg, "dvorak")) keymap_chosen = DVORAK;
	      else {
	        printf("Invalid argument for keymap: %s\n", optarg);
          usage_and_exit();
	      }
	      break;
      case 'g':
        for (int i = 0; i < NUM_GRADIENTS; i++) {
          if (!strcmp(optarg, gradient_table[i].name)) {
            gradient_chosen = &(gradient_table[i].colors);
            goto found;
          }
        }
        printf("Invalid argument for gradient: %s\n", optarg);
        usage_and_exit();
        found:
        break;
      case '?':
        usage_and_exit();
	  }
  }
  
  snake snake;
  point apple;
  point bounds[PERIMETER_SIZE];
  byte collision[BOUNDARY_WIDTH_SNAKE][BOUNDARY_HEIGHT] = {0};
  input_buffer input_buffer = {{KEY_RIGHT, KEY_RIGHT, KEY_RIGHT}, 0};
  unsigned char key = KEY_NONE;
  
  srand(time(NULL));
  set_keymap(keymap_chosen);
  init_draw_buffers();
  init_term();
  init_canvas();
  init_signal();
  init_bounds(bounds, collision);
  init_snake(&snake, collision, gradient_chosen);
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
        init_snake(&snake, collision, gradient_chosen);
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


