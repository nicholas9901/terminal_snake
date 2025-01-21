#include "prototypes.h"

#include <fcntl.h>
#include <time.h>
#include <unistd.h>

int main() 
{
    snake      snake;
    point_wide apple;
    point      bounds[PERIMETER_SIZE];
    uint8_t    collision[BOUND_WIDTH_SNK][BOUND_HEIGHT];

    srand(time(NULL));

    init_canvas();
    init_term();
    init_bounds(bounds, collision);
    init_snake(&snake, collision);
    init_point_wide(&apple, mid_x, mid_y, SPRITE_APPLE);
    collision[mid_x - left_bound_adj_snk][mid_y - top_bound_adj] = COLLISION_APPLE;

    draw_sprites(&snake, &apple);
    draw_bounds(bounds, PERIMETER_SIZE);
    draw_controls();

    fcntl(0, F_SETFL, O_NONBLOCK);

    #if DEBUG
        printf(esc "2;0Hwidth:%d height:%d %d %d", width, height, mid_x, mid_y);
        printf(esc "3;0Htba:%d lba:%d", top_bound_adj, left_bound_adj);
    #endif

    char 
        key_main   = '0',
        key_m_curr = 'd',
        key_m_prev = 'd';
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
                printf(esc yx "Paused",  top_bound_adj - 2, pos_x);
                while(key_main != 'e') {
                    read(0, &key_main, 1);
                    if (key_main == 'q') {
                        exit(0);
                    }
                }
                printf(esc yx "H      ",  top_bound_adj - 2, pos_x);
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

        if (!move(&snake, &apple, collision, key_m_curr, &key_m_prev)) {
            draw_sprites(&snake, &apple);
            printf(esc yx "%s", snake.segments[0].y, snake.segments[0].x, SPRITE_CRASH);
            break;
        }
        draw_sprites(&snake, &apple);

        #if DEBUG
            for (int i = 0; i < BOUND_WIDTH_SNK; i++) {
                for (int j = 0; j < BOUND_HEIGHT; j++) {
                    printf(esc "%d;%dH%d", j+15, i+8,collision[i][j]);
                    printf(esc yx "X", snake.segments[0].y+15-top_bound_adj, snake.segments[0].x+8-left_bound_adj_snk);
                }
            }
        #endif

        usleep(GAME_SPEED);
    }
    printf(esc "%d;%dHGame Over!", bottom_bound_adj + 2, left_bound_adj);
    while (key_main != 'q' && key_main != '\n') {
        read(0, &key_main, 1);
    }
    clean();
}
