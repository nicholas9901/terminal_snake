#include "prototypes.h"

#ifndef _WIN32
struct termios initial;
#endif

void clean() 
{
	printf(
	  ALT_BUF ON
	  TERM_CLEAR
		CURSOR ON
	  ALT_BUF OFF
  );
  terminal_switch_main();
}

void init_canvas() 
{
	printf(TERM_CLEAR);
  get_terminal_size(&width_terminal, &height_terminal);
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
	setvbuf(stdout, NULL, _IONBF, 0);
  terminal_switch_alternate();
}

void init_signal()
{
  signal(SIGTERM, handle_exit);
  signal(SIGINT, handle_exit);
#ifndef _WIN32
  signal(SIGWINCH, init_canvas);
#endif
}

void  handle_exit(int) { quit = TRUE; }
