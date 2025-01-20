#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>

#define          DEBUG 1

// Game Constants
#define    PARTS_START 2
#define     GAME_SPEED 100000
#define    BOUND_WIDTH 32
#define   BOUND_HEIGHT 16

#define   ACTIVE_WIDTH (BOUND_WIDTH - 2)
#define  ACTIVE_HEIGHT (BOUND_HEIGHT - 2)

#define      PARTS_MAX (BOUND_WIDTH - 2) * (BOUND_HEIGHT - 2)

#define      TOP_BOUND 1
#define   BOTTOM_BOUND BOUND_HEIGHT
#define     LEFT_BOUND 1
#define    RIGHT_BOUND BOUND_WIDTH
#define PERIMETER_SIZE 2 * BOUND_WIDTH + 2 * BOUND_HEIGHT - 4

#define POINT_WIDE_LENGTH 8 /* To fit 2 unicode characters */
#define  BYTES_PER_ROW BOUND_WIDTH / BYTE_SIZE
#define      BYTE_SIZE 8
#define  BITMAP_LENGTH PARTS_MAX

// ANSI Escape Codes
#define        esc "\x1b["
#define         yx "%d;%dH"
#define term_clear esc "2J"
#define    alt_buf esc "?1049"
#define     cursor esc "?25"
#define         on "h"
#define        off "l"

#define fmt_clear esc "0m"

#define say(str) write(1, str, sizeof(str))

#endif
