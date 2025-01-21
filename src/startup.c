#include "prototypes.h"

struct termios initial;

int 
    width, 
    height, 
    mid_x, 
    mid_y,
    left_bound_adj,
    right_bound_adj,
    top_bound_adj,
    bottom_bound_adj;

void clean() {
	say(
        alt_buf on
        term_clear
	    cursor on
        alt_buf off
        "\n"
    );
	tcsetattr(1, TCSANOW, &initial);
}

void init_canvas() {
    struct winsize ws;
	ioctl(1, TIOCGWINSZ, &ws);
    width            = ws.ws_col;
    height           = ws.ws_row;
    mid_x            = width / 2;
    mid_y            = height / 2;
    left_bound_adj   = LEFT_BOUND   + mid_x - (BOUND_WIDTH  / 2);
    right_bound_adj  = RIGHT_BOUND  + mid_x - (BOUND_WIDTH  / 2);
    top_bound_adj    = TOP_BOUND    + mid_y - (BOUND_HEIGHT / 2);
    bottom_bound_adj = BOTTOM_BOUND + mid_y - (BOUND_HEIGHT / 2);
}

void init_term() {
	setvbuf(stdout, NULL, _IONBF, 0);
	struct termios t;
	tcgetattr(1, &t);
	initial = t;
	atexit(clean);
	signal(SIGTERM, exit);
	signal(SIGINT, exit);
	say(
        alt_buf on
        cursor off
    );
	t.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(1, TCSANOW, &t);
}