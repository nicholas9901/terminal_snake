#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


/* Main game-related definitions */
#define                 DEBUG 0
#define           PARTS_START 3
#define             GAME_WAIT 100000
#define          POLLING_RATE 10000
#define        BOUNDARY_WIDTH 32 /* Must be even */
#define       BOUNDARY_HEIGHT 16
#define  BOUNDARY_WIDTH_SNAKE (BOUNDARY_WIDTH + 1)
#define BOUNDARY_WIDTH_ACTUAL (BOUNDARY_WIDTH / 2)
#define          NUM_GRADIENT 6

/* Boundaries */
#define      TOP_BOUND 1
#define   BOTTOM_BOUND BOUNDARY_HEIGHT
#define     LEFT_BOUND 1
#define    RIGHT_BOUND BOUNDARY_WIDTH
#define PERIMETER_SIZE ((2 * BOUNDARY_WIDTH_ACTUAL) + (2 * BOUNDARY_HEIGHT) - 2)
#define   ACTIVE_WIDTH ((BOUNDARY_WIDTH - 2) / 2)
#define  ACTIVE_HEIGHT (BOUNDARY_HEIGHT - 2)
#define    ACTIVE_AREA (ACTIVE_WIDTH * ACTIVE_HEIGHT)

/* Key Codes */
#define KEY_NONE -1

/* ANSI escape codes */
#define        ESC "\e["
#define         YX "%d;%dH"    /* Terminal coordinates */
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

/* Sprites */ 
#define      SPRITE_BLOCK "  "
#define SPRITE_SNAKE_HEAD ESC BG_WHITE FMT_END SPRITE_BLOCK FMT_CLEAR
#define SPRITE_SNAKE_BODY ESC BG_WHITE FMT_END SPRITE_BLOCK FMT_CLEAR
#define      SPRITE_CRASH ESC BG_BLUE FMT_END SPRITE_BLOCK FMT_CLEAR
#define      SPRITE_APPLE ESC BG_RED FMT_END SPRITE_BLOCK FMT_CLEAR
#define   SPRITE_BOUNDARY ESC BG_GREY FMT_END SPRITE_BLOCK FMT_CLEAR
#define      SPRITE_CLEAR SPRITE_BLOCK FMT_CLEAR

/* Types */
typedef unsigned char byte;

/* Enumerated types */
typedef enum { 
  COLLISION_NONE, 
  COLLISION_APPLE, 
  COLLISION_BAD
} collision_enum;

typedef enum { 
  UP, 
  DOWN, 
  LEFT, 
  RIGHT 
} direction_enum;

/* Data types */
typedef struct point {
  int x;
  int y;
} point;

typedef struct snake {
  int ghost_pointer;
  int score;
  byte gradient_pointer;
  byte gradient_indices[NUM_GRADIENT];
  point segments[ACTIVE_AREA];
} snake;
