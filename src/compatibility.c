#include "prototypes.h"

void get_key(unsigned char* key) 
{
#ifdef _WIN32
  if (_kbhit())
    *key = _getch();
#else
  read(0, key, 1);
#endif
}

void get_terminal_size(unsigned int* width, unsigned int* height)
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  *width  = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
  struct winsize ws;
	ioctl(1, TIOCGWINSZ, &ws);
  width_terminal  = ws.ws_col;
  height_terminal = ws.ws_row;
#endif
}

void terminal_switch_alternate()
{
#ifdef _WIN32
  HANDLE hOut  = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  if (GetConsoleMode(hOut, &dwMode)) {
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  }
  printf(
    ALT_BUF ON 
    CURSOR OFF);
#else
	struct termios current;
  fcntl(0, F_SETFL, O_NONBLOCK);
	tcgetattr(1, &current);
	initial = current;
	printf(
	  ALT_BUF ON 
	  CURSOR OFF);
	current.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(1, TCSANOW, &current);
#endif
}

void terminal_switch_main()
{
#ifdef _WIN32
#else
  tcsetattr(1, TCSANOW, &initial);
#endif
}
