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
  apple_x_start,
  perimeter,
  left_bound_adj,
  right_bound_adj,
  top_bound_adj,
  bottom_bound_adj;

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
  apple_x_start    = mid_x % 2 == 0 ? mid_x + 1 : mid_x;
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
