#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define  TRUE 1
#define FALSE 0

/* Main game-related definitions */
#define             PARTS_START 3
#define              GAME_SPEED 100000
#define GAME_SPEED_MILLISECONDS (GAME_SPEED / 1000)
#define            POLLING_RATE 10000
#define          BOUNDARY_WIDTH 32 /* Must be even */
#define         BOUNDARY_HEIGHT 16
#define    BOUNDARY_WIDTH_SNAKE (BOUNDARY_WIDTH + 1)
#define       SIZE_INPUT_BUFFER 3
#define          NUM_DIRECTIONS 4
#define            NUM_GRADIENT 6
#define                NUM_KEYS 6
#define             NUM_KEYMAPS 3

/* Boundaries */
#define      TOP_BOUND 1
#define   BOTTOM_BOUND BOUNDARY_HEIGHT
#define     LEFT_BOUND 1
#define    RIGHT_BOUND BOUNDARY_WIDTH
#define PERIMETER_SIZE (BOUNDARY_WIDTH + (2 * BOUNDARY_HEIGHT) - 2)
#define   ACTIVE_WIDTH ((BOUNDARY_WIDTH - 2) / 2)
#define  ACTIVE_HEIGHT (BOUNDARY_HEIGHT - 2)
#define    ACTIVE_AREA (ACTIVE_WIDTH * ACTIVE_HEIGHT)

/* Key Codes */
#define KEY_NONE 0xFF
#define KEY_HIDE ' '
#define ACTION_NONE KEY_NONE

/* ANSI escape codes */
#define        ESC "\e["
#define          Y "%d;1H"
#define         YX "%d;%dH"
#define TERM_CLEAR ESC "2J"     
#define    ALT_BUF ESC "?1049"
#define     CURSOR ESC "?25"
#define         ON "h"
#define        OFF "l"

#define       FG "38;5;"
#define       BG "48;5;"
#define FG_BLACK FG "0"
#define BG_WHITE BG "7"
#define  BG_GREY BG "8"
#define   BG_RED BG "1"
#define  BG_BLUE BG "4"
#define     WITH ";"

#define   FMT_END "m"
#define  FMT_INFO ESC FG_BLACK WITH BG_WHITE FMT_END
#define FMT_CLEAR ESC "0m"

/* Gradient-related */
#define NUM_GRADIENTS 5
#define SIZE_GRADIENT 6
#define    SIZE_COLOR 4
#define     SIZE_NAME 6

/* Sprites */ 
#define      SPRITE_BLOCK "  "
#define SPRITE_SNAKE_HEAD ESC BG_WHITE FMT_END SPRITE_BLOCK FMT_CLEAR
#define SPRITE_SNAKE_BODY ESC BG_WHITE FMT_END SPRITE_BLOCK FMT_CLEAR
#define      SPRITE_CRASH ESC BG_BLUE FMT_END SPRITE_BLOCK FMT_CLEAR
#define      SPRITE_APPLE ESC BG_RED FMT_END SPRITE_BLOCK FMT_CLEAR
#define   SPRITE_BOUNDARY ESC BG_GREY FMT_END SPRITE_BLOCK FMT_CLEAR
#define      SPRITE_CLEAR SPRITE_BLOCK FMT_CLEAR

/* Text-related */
#define       MSG_SCORE " Score: "
#define       MSG_PAUSE "Paused"
#define MSG_PAUSE_CLEAR "      "
#define    MSG_CONTROLS " Quit: [Ctrl-c] Move: [%c%c%c%c] Pause: [%c] Work: [Space]"
#define   MSG_GAME_OVER "Game Over! Restart: [%c] "

/* Types */
typedef unsigned char byte;

/* Enumerated types */
typedef enum { 
  COLLISION_NONE, 
  COLLISION_APPLE, 
  COLLISION_BAD
} collision_enum;

typedef enum { 
  KEY_UP, 
  KEY_RIGHT, 
  KEY_DOWN, 
  KEY_LEFT,
  KEY_PAUSE,
  KEY_RESTART
} key_enum;

typedef enum {
  QWERTY,
  AZERTY,
  DVORAK
} keymap_enum;

/* Data types */
typedef struct input_buffer {
  byte inputs[SIZE_INPUT_BUFFER];
  byte current;
} input_buffer;

typedef struct gradient_pair {
  char name[SIZE_NAME];
  char colors[SIZE_GRADIENT][SIZE_COLOR];
} gradient_pair;

typedef struct point {
  int x;
  int y;
} point;

typedef struct snake {
  int ghost_pointer;
  int score;
  byte direction;
  byte new_segment_added;
  char (*gradient_chosen)[SIZE_GRADIENT][SIZE_COLOR];
  byte gradient_pointer;
  byte gradient_indices[NUM_GRADIENT];
  point segments[ACTIVE_AREA];
} snake;
