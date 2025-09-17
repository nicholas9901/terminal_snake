#include "prototypes.h"

#include <time.h>
#include <unistd.h>
#include <getopt.h>

byte 
	paused    = FALSE,
	hidden    = FALSE,
	redraw    = FALSE,
	retry     = FALSE,
	game_over = FALSE,
	win       = FALSE,
	quit      = FALSE;

unsigned int 
  width_terminal, 
  height_terminal, 
  mid_width_terminal,
  mid_height_terminal,
  width_terminal_min,
  height_terminal_min,
  left_bound_terminal,
  right_bound_terminal,
  top_bound_terminal,
  bottom_bound_terminal;

gradient_pair gradient_table[NUM_GRADIENTS] = {
  {"warm",  {"196", "202", "208", "214", "220", "226"}},
  {"cool",  {"21",  "27",  "33",  "39",  "45",  "51"}},
  {"snake", {"34",  "40",  "46",  "82",  "118", "154"}},
  {"16bit", {"9",   "10",  "11",  "12",  "13",  "14"}},
  {"none",  {"239", "239", "239", "239", "239", "239"}}
};

context boundary_context;

static inline void usage_and_exit()
{
	printf(
    "Usage: terminal_snake [OPTIONS]...\n\n"
    "Option        Long Option               Description\n"
    "-h            --help                    Display this message\n"
    "-s            --speed <speed>           Change the speed (default %u)\n"
    "-b            --boundary <width,height> Change the boundary (default %u,%u)\n"
    "-k <keymap>   --keymap <keymap>         Choose the keymap (qwerty|azerty|dvorak)\n"
    "-g <gradient> --gradient <gradient>     Choose the gradient\n\n"
    "Gradients:\n", GAME_SPEED_MILLISECONDS, BOUNDARY_WIDTH, BOUNDARY_HEIGHT);
  for (size_t i = 0; i < NUM_GRADIENTS; i++) {
    printf("%-*s", SIZE_NAME, gradient_table[i].name);
    for (size_t j = SIZE_GRADIENT - 1; j >= 0; j--) {
      printf(ESC BG "%s" FMT_END SPRITE_BLOCK FMT_CLEAR, gradient_table[i].colors[j]);
    }
    printf(ESC SPRITE_SNAKE_HEAD"\n");
  }
  exit(0);
}

static inline void calculate_context(unsigned int width, unsigned int height)
{
  boundary_context.width          = width;
  boundary_context.height         = height;
  boundary_context.active_width   = ACTIVE(boundary_context.width);
  boundary_context.active_height  = ACTIVE(boundary_context.height);
  boundary_context.active_area    = AREA(boundary_context.active_width, boundary_context.active_height);
  boundary_context.perimeter_size = PERIMETER(width, height);
}

int main(int argc, char** argv) 
{
  unsigned int game_speed = GAME_SPEED;
  unsigned int game_speed_arg, width_arg, height_arg;
  char opt;
  char* c;
  char (*gradient_chosen)[SIZE_GRADIENT][SIZE_COLOR] = &(gradient_table[2].colors);
  byte keymap_chosen = QWERTY;
  snake snake;
  point apple;
  point* bounds;
  byte** collision;  
  input_buffer input_buffer = {{KEY_RIGHT, KEY_RIGHT, KEY_RIGHT}, 0};
  unsigned char key = KEY_NONE;
  
  calculate_context(BOUNDARY_WIDTH, BOUNDARY_HEIGHT);
  
  const struct option long_options[] = {
    {"help",     0, NULL, 'h'},
    {"speed",    1, NULL, 's'},
    {"boundary", 1, NULL, 'b'},
    {"keymap",   1, NULL, 'k'},
    {"gradient", 1, NULL, 'g'},
    {NULL,       0, NULL,   0}
  };
    
	while ((opt = getopt_long(argc, argv, "hs:b:k:g:", long_options, NULL)) > 0) {
	  switch (opt) {
	    case 'h':
	      usage_and_exit();
	    case 's':
	      game_speed_arg = atoi(optarg);
	      if (game_speed_arg > GAME_SPEED_MAX) {
	        printf(
            "Chosen speed %u exceeds the maximum value (%u)\n", 
            game_speed_arg, GAME_SPEED_MAX);
  	      exit(0);
	      }
	      game_speed = (int)((float) 1 / game_speed_arg * GAME_SPEED_MUL_FACTOR);
	      break;
	    case 'b':
	      for (c = optarg; *c != ','; c++) {
	        if (*c == '\0') {
	          printf("Invalid format for boundary: %s\n", optarg);
	          usage_and_exit();
	        }
	      }
	      *c = '\0';
	      width_arg  = atoi(optarg);
	      height_arg = atoi(c + 1);
	      if (width_arg < BOUNDARY_WIDTH_MIN || height_arg < BOUNDARY_HEIGHT_MIN) {
	        printf(
            "Chosen boundary dimensions %u,%u are too small (Minimum dimensions: %u,%u)\n", 
            width_arg, height_arg, BOUNDARY_WIDTH_MIN, BOUNDARY_HEIGHT_MIN);
	        exit(0);
	      }
	      calculate_context(width_arg, height_arg);
	      break;
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
        for (size_t i = 0; i < NUM_GRADIENTS; i++) {
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
  
  srand(time(NULL));
  set_keymap(keymap_chosen);
  allocate_all(&snake, &bounds, &collision);
  init_draw_buffers();
  init_term();
  init_canvas();
  init_signal();
  init_bounds(bounds, collision);
  init_snake(&snake, collision, gradient_chosen);
  init_apple(&apple, collision);
  
  for (;;) {
    for (size_t times_to_poll = 0; times_to_poll < game_speed / POLLING_RATE; times_to_poll++) {
      queue_input(&input_buffer, get_action(&key));
      if (quit) {
        goto end;
      } else if (redraw) {
        redraw = FALSE;
        draw_redraw(&snake, &apple, bounds);
      } else if (retry) {
        retry     = FALSE;
        paused    = FALSE;
        game_over = FALSE;
        win       = FALSE;
        input_buffer.inputs[0] = KEY_RIGHT;
        input_buffer.inputs[1] = KEY_RIGHT;
        reset_bounds(collision);
        init_snake(&snake, collision, gradient_chosen);
        init_apple(&apple, collision);
        printf(TERM_CLEAR);
        draw_redraw(&snake, &apple, bounds);
      }
      usleep(POLLING_RATE);
    }
    
    if (!paused) {
      switch(move_snake(&snake, &apple, collision, dequeue_input(&input_buffer))) {
        case (STATUS_NONE):
          break;
        case (STATUS_BAD):
          game_over = TRUE;
          paused    = TRUE;
          draw_game_over_ribbon(&snake);
          break;
        case (STATUS_WIN):
          win       = TRUE;
          game_over = TRUE;
          paused    = TRUE;
          for (size_t i = 0; i < SIZE_GRADIENT; i++) {
            snake.gradient_indices[i] = (snake.gradient_indices[i] + 1) % snake.ghost_pointer;
          }
          draw_snake_victory(&snake);
          draw_win_ribbon();
          break;
      }
    } else if (win && !hidden) {
      for (size_t i = 0; i < SIZE_GRADIENT; i++) {
        snake.gradient_indices[i] = (snake.gradient_indices[i] + 1) % snake.ghost_pointer;
      }
      draw_snake_victory(&snake);
    }
  }
  end:
    clean();
    free_all(&snake, bounds, collision);
}


