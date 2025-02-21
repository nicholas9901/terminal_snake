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
#define        BOUNDARY_WIDTH 32 /* Must be even */
#define       BOUNDARY_HEIGHT 16
#define  BOUNDARY_WIDTH_SNAKE (BOUNDARY_WIDTH + 1)
#define BOUNDARY_WIDTH_ACTUAL (BOUNDARY_WIDTH / 2)

/*  */
#define      TOP_BOUND 1
#define   BOTTOM_BOUND BOUNDARY_HEIGHT
#define     LEFT_BOUND 1
#define    RIGHT_BOUND BOUNDARY_WIDTH
#define PERIMETER_SIZE (2 * BOUNDARY_WIDTH + 2 * BOUNDARY_HEIGHT - 4)

/*  */
#define  ACTIVE_WIDTH ((BOUNDARY_WIDTH - 2) / 2)
#define ACTIVE_HEIGHT (BOUNDARY_HEIGHT - 2)
#define   ACTIVE_AREA (ACTIVE_WIDTH * ACTIVE_HEIGHT)

/* Buffer lengths */
#define      POINT_U_LENGTH 4 /* Fits 1 unicode character */
#define POINT_U_WIDE_LENGTH 8 /* Fits 2 unicode characters */

// ANSI ESCape codes
#define        ESC "\e["    
#define         YX "%d;%dH"    /* Terminal coordinates */
#define TERM_CLEAR ESC "2J"     
#define    ALT_BUF ESC "?1049"
#define     CURSOR ESC "?25"
#define         ON "h"
#define        OFF "l"

#define FMT_INFO  ESC "38;5;0;48;5;251m"
#define FMT_CLEAR ESC "0m"

/* Sprites */ 
#define SPRITE_SNAKE_HEAD     "██"
#define SPRITE_SNAKE_BODY     ESC "38;5;244m" SPRITE_SNAKE_HEAD FMT_CLEAR
#define SPRITE_CRASH          "░░"
#define SPRITE_APPLE          "()"
#define SPRITE_CLEAR          "  "
#define SPRITE_VERTICAL_BAR   "┃"
#define SPRITE_HORIZONTAL_BAR "━"
#define SPRITE_TOP_LEFT       "┏"
#define SPRITE_TOP_RIGHT      "┓"
#define SPRITE_BOTTOM_LEFT    "┗"
#define SPRITE_BOTTOM_RIGHT   "┛"

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

typedef struct point_u {
    int x;
    int y;
    char icon[POINT_U_LENGTH];
} point_u;

typedef struct point_u_wide {
    int x;
    int y;
    char icon[POINT_U_WIDE_LENGTH];
} point_u_wide;

typedef struct snake {
    int ghost_pointer;
    int score;
    point segments[ACTIVE_AREA];
} snake;
