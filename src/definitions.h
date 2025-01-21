#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


/* Main game-related definitions */
#define        DEBUG 0
#define  PARTS_START 2
#define   GAME_SPEED 100000
#define  BOUND_WIDTH 32
#define BOUND_HEIGHT 16

/*  */
#define      TOP_BOUND 1
#define   BOTTOM_BOUND BOUND_HEIGHT
#define     LEFT_BOUND 1
#define    RIGHT_BOUND BOUND_WIDTH
#define PERIMETER_SIZE (2 * BOUND_WIDTH + 2 * BOUND_HEIGHT - 4)

/*  */
#define        ACTIVE_WIDTH (BOUND_WIDTH - 2)
#define       ACTIVE_HEIGHT (BOUND_HEIGHT - 2)
#define         ACTIVE_AREA (ACTIVE_WIDTH * ACTIVE_HEIGHT)
#define ACTUAL_ACTIVE_WIDTH (ACTIVE_WIDTH / 2)

/* Buffer lengths */
#define      POINT_LENGTH 4 /* Fits 1 unicode character */
#define POINT_WIDE_LENGTH 8 /* Fits 2 unicode characters */

// ANSI escape codes
#define        esc "\x1b["    
#define         yx "%d;%dH"    /* Terminal coordinates */
#define term_clear esc "2J"     
#define    alt_buf esc "?1049"
#define     cursor esc "?25"
#define         on "h"
#define        off "l"

#define fmt_clear esc "0m"

#define say(str) write(1, str, sizeof(str))

/* Sprites */ 
#define SPRITE_SNAKE          "██"
#define SPRITE_CRASH          "░░"
#define SPRITE_APPLE          "()"
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
    char icon[POINT_LENGTH];
} point;

typedef struct point_wide {
    int x;
    int y;
    char icon[POINT_WIDE_LENGTH];
} point_wide;

typedef struct snake {
    int num_segments;
    int score;
    point_wide segments[ACTIVE_AREA];
} snake;

#endif
