#include "prototypes.h"

struct termios initial;

byte 
	paused    = FALSE,
	hidden    = FALSE,
	redraw    = FALSE,
	retry     = FALSE,
	game_over = FALSE;

int 
  width, 
  height, 
  mid_x,
  mid_y,
  perimeter,
  left_bound_adj,
  right_bound_adj,
  top_bound_adj,
  bottom_bound_adj;

gradient_pair gradient_table[NUM_GRADIENTS] = {
  {"warm",  {"196", "202", "208", "214", "220", "226"}},
  {"cool",  {"21",  "27",  "33",  "39",  "45",  "51"}},
  {"barf",  {"34",  "40",  "46",  "82",  "118", "154"}},
  {"16bit", {"9",   "10",  "11",  "12",  "13",  "14"}},
  {"none",  {"239", "239", "239", "239", "239", "239"}}
};

void clean() 
{
	printf(
	  ALT_BUF ON
	  TERM_CLEAR
		CURSOR ON
	  ALT_BUF OFF
  );
	tcsetattr(1, TCSANOW, &initial);
}

void init_canvas() 
{
	printf(TERM_CLEAR);
  struct winsize ws;
	ioctl(1, TIOCGWINSZ, &ws);
  width            = ws.ws_col;
  height           = ws.ws_row;
  mid_x            = width / 2;
  mid_y            = height / 2;
  perimeter        = PERIMETER_SIZE;
  left_bound_adj   = LEFT_BOUND   + mid_x - (BOUNDARY_WIDTH  / 2) - 1;
  right_bound_adj  = RIGHT_BOUND  + mid_x - (BOUNDARY_WIDTH  / 2);
  top_bound_adj    = TOP_BOUND    + mid_y - (BOUNDARY_HEIGHT / 2);
  bottom_bound_adj = BOTTOM_BOUND + mid_y - (BOUNDARY_HEIGHT / 2);
  redraw           = TRUE;
}

void init_term() 
{
	struct termios current;
  fcntl(0, F_SETFL, O_NONBLOCK);
	setvbuf(stdout, NULL, _IONBF, 0);
	tcgetattr(1, &current);
	atexit(clean);
	initial = current;
	printf(
	  ALT_BUF ON 
	  CURSOR OFF);
	current.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(1, TCSANOW, &current);
}

void init_signal()
{
	signal(SIGTERM, exit);
	signal(SIGINT, exit);
	signal(SIGWINCH, init_canvas);
}
