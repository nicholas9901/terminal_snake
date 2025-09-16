#include "prototypes.h"

struct termios initial;

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
  width_terminal        = ws.ws_col;
  height_terminal       = ws.ws_row;
  width_terminal_min    = MAX(sizeof(MSG_CONTROLS) - 5, boundary_context.width);
  height_terminal_min   = boundary_context.height + 2;
  mid_width_terminal    = width_terminal / 2;
  mid_height_terminal   = height_terminal / 2;
  left_bound_terminal   = LEFT_BOUND + mid_width_terminal - boundary_context.width;
  right_bound_terminal  = boundary_context.width + mid_width_terminal - boundary_context.width;
  top_bound_terminal    = TOP_BOUND + mid_height_terminal - (boundary_context.height / 2);
  bottom_bound_terminal = boundary_context.height + mid_height_terminal - (boundary_context.height / 2);
  redraw                = TRUE;
}

void init_term() 
{
	struct termios current;
  fcntl(0, F_SETFL, O_NONBLOCK);
	setvbuf(stdout, NULL, _IONBF, 0);
	tcgetattr(1, &current);
	initial = current;
	printf(
	  ALT_BUF ON 
	  CURSOR OFF);
	current.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(1, TCSANOW, &current);
}

void init_signal()
{
	signal(SIGTERM, handle_exit);
	signal(SIGINT, handle_exit);
	signal(SIGWINCH, init_canvas);
}

void handle_exit() { quit = TRUE; }
