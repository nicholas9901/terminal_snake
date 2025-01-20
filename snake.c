#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "point.h"
#include "collision.h"
#include "defs.h"
#include "draw.h"
#include "startup.h"

int main() {
    int state = 1;
    int perimeter = PERIMETER_SIZE;
    snake snake;
    point_wide apple;
    point_wide bounds[perimeter];
    bitmap collision_map;
    srand(time(NULL));
    init_canvas();
    init_term();
    init_bounds(bounds);
    init_snake(&snake);
    init_point_wide(&apple, 0, 0, "()");
    random_pos(&apple, &collision_map, &snake);
    init_collision(&collision_map, bounds, &snake, perimeter);
    draw_snake(&snake, &apple);
    draw_bounds(bounds, perimeter);
    draw_controls();
    fcntl(0, F_SETFL, O_NONBLOCK);

    #if DEBUG
        printf(esc "2;0Hwidth:%d height:%d %d %d", width, height, mid_x, mid_y);
        printf(esc "3;0Htba:%d lba:%d", top_bound_adj, left_bound_adj);
    #endif

    char 
        key_main,
        key_m_curr = 'a',
        key_m_prev = 'a';
	while(1) {
        read(0, &key_main, 1);
        key_m_curr = get_movement_key(&key_main);

        switch(key_main) {
            case 'q':
                exit(0);
            case 'e':
                key_main = '0';
                char str[] = "Paused";
                int pos_x = right_bound_adj - strlen(str) + 1;
                printf(esc "%d;%dHPaused",  top_bound_adj - 2, pos_x);
                while(key_main != 'e') {
                    read(0, &key_main, 1);
                    if (key_main == 'q') {
                        exit(0);
                    }
                }
                printf(esc "%d;%dH      ",  top_bound_adj - 2, pos_x);
                key_main = '0';
                break;
        }

        #if DEBUG
        switch (key_main) {
            case 'n':
                add_segment(&snake);
                break;
            case 'z':
                while(key_main == 'z') {
                    read(0, &key_main, 1);
                };
                break;
        }
        #endif

        if (!move(&snake, &apple, &collision_map, key_m_curr, &key_m_prev)) {
            draw_snake(&snake, &apple);
            printf(esc yx "%s", snake.segments[0].y, snake.segments[0].x, "░░");
            break;
        }
        draw_snake(&snake, &apple);
        usleep(GAME_SPEED);
    }
    printf(esc "%d;%dHGame Over!", bottom_bound_adj + 2, left_bound_adj);
    while (key_main != 'q' && key_main != '\n') {
        read(0, &key_main, 1);
    }
    clean();
    #if DEBUG
        for (int i = top_bound_adj+1; i <= bottom_bound_adj-1; i++) {
            for (int j = left_bound_adj+1; j <= right_bound_adj-1; j++) {
                printf(esc "%d;%dH%d", i+10,j,get_bit(&collision_map, j, i));
            }
        }
    #endif
}
