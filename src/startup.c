#include "prototypes.h"

struct termios initial;

uint8_t new_segment_added = 0;

int 
  width, 
  height, 
  mid_x,
  apple_x_start,
  mid_y,
  left_bound_adj,
  left_bound_adj_snk,
  right_bound_adj,
  top_bound_adj,
  bottom_bound_adj;

void clean() {
	puts(
        ALT_BUF ON
        TERM_CLEAR
	    CURSOR ON
        ALT_BUF OFF
        "\n"
    );
	tcsetattr(1, TCSANOW, &initial);
}

void init_canvas() {
  struct winsize ws;
	ioctl(1, TIOCGWINSZ, &ws);
  width              = ws.ws_col;
  height             = ws.ws_row;
  mid_x              = width / 2;
  apple_x_start      = mid_x % 2 == 0 ? mid_x +1 : mid_x;
  mid_y              = height / 2;
  left_bound_adj     = LEFT_BOUND   + mid_x - (BOUNDARY_WIDTH  / 2);
  left_bound_adj_snk = left_bound_adj - 1;
  right_bound_adj    = RIGHT_BOUND  + mid_x - (BOUNDARY_WIDTH  / 2);
  top_bound_adj      = TOP_BOUND    + mid_y - (BOUNDARY_HEIGHT / 2);
  bottom_bound_adj   = BOTTOM_BOUND + mid_y - (BOUNDARY_HEIGHT / 2);
}

void init_term() {
	setvbuf(stdout, NULL, _IONBF, 0);
	struct termios t;
	tcgetattr(1, &t);
	initial = t;
	atexit(clean);
	signal(SIGTERM, exit);
	signal(SIGINT, exit);
	puts(
        ALT_BUF ON 
        CURSOR OFF
    );
	t.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(1, TCSANOW, &t);
}
