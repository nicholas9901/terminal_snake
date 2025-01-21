#include "prototypes.h"

void init_point(point* p, int x, int y, char* icon)
{
    p->x = x;
    p->y = y;
    strcpy(p->icon, icon);
}

void init_point_wide(point_wide* p, int x, int y, char* icon)
{
    p->x = x;
    p->y = y;
    strcpy(p->icon, icon);
}

void init_snake(snake* s, uint8_t c[BOUND_WIDTH][BOUND_HEIGHT])
{
    s->num_segments = PARTS_START;
    s->score        = 0;

    init_point_wide(&s->segments[0], left_bound_adj + 1, mid_y, SPRITE_SNAKE);
    for (int i = 1; i < s->num_segments; i++) {
        s->segments[i].x = s->segments[i - 1].x;
        s->segments[i].y = s->segments[i - 1].y;
        strcpy(s->segments[i].icon, SPRITE_SNAKE);
    }
    for (int i = 0; i < s->num_segments - 1; i++) {
        c[s->segments[i].x - left_bound_adj_snk]
         [s->segments[i].y - top_bound_adj] = COLLISION_BAD;
    }
}

void init_bounds(point* p, uint8_t c[BOUND_WIDTH][BOUND_HEIGHT])
{
    int points_index = 3;

    /* Top left */
    init_point(&p[0], left_bound_adj, top_bound_adj, SPRITE_TOP_LEFT);

    /* Top right */
    init_point(&p[1], right_bound_adj, top_bound_adj, SPRITE_TOP_RIGHT);
    
    /* Bottom left */
    init_point(&p[2], left_bound_adj, bottom_bound_adj, SPRITE_BOTTOM_LEFT);

    /* Bottom right */
    init_point(&p[3], right_bound_adj, bottom_bound_adj, SPRITE_BOTTOM_RIGHT);
    
    for (int i = left_bound_adj + 1; i < right_bound_adj; i++) {
        /* Top Border */
        init_point(&p[++points_index], i, top_bound_adj, SPRITE_HORIZONTAL_BAR);

        /* Bottom Border */
        init_point(&p[++points_index], i, bottom_bound_adj, SPRITE_HORIZONTAL_BAR);

    } 

    for (int i = top_bound_adj + 1; i < bottom_bound_adj; i++) {
        /* Left border */
        init_point(&p[++points_index], left_bound_adj, i, SPRITE_VERTICAL_BAR);

        /* Right border */
        init_point(&p[++points_index], right_bound_adj, i, SPRITE_VERTICAL_BAR);
    }

    /*
    Shift collision fields aside from the left boundary by one to account for
    the snake which is 2 characters wide    
    */
     
    for (int i = 2; i < BOUND_WIDTH; i += 2) {
        /* Top Border */
        c[i][0] = COLLISION_BAD;

        /* Bottom Border */
        c[i][BOUND_HEIGHT - 1] = COLLISION_BAD;
    }

    for (int i = 1; i < BOUND_HEIGHT - 1; i++) {
        /* Left border */
        c[0][i] = COLLISION_BAD;

        /* Right border */
        c[BOUND_WIDTH][i] = COLLISION_BAD;
    }
    
    /* Zeroing out the middle */
    for (int i = 1; i < BOUND_WIDTH - 1; i++) {
        for (int j = 1; j < BOUND_HEIGHT - 1; j++) {
            c[i][j] = COLLISION_NONE;
        }
    }
}

void random_pos(point_wide* p, uint8_t c[BOUND_WIDTH][BOUND_HEIGHT], snake* s)
{
    int rand_bit = rand() % ACTUAL_AREA;
    int x        = rand_bit % ACTIVE_WIDTH;
    int y        = rand_bit / ACTIVE_WIDTH;

    // while (get_bit_linear(b, rand_bit)) {
    //     rand_bit %= BITMAP_LENGTH;
    //     rand_bit++; 
    // }

    p->x = x + left_bound_adj_snk;
    p->y = y + top_bound_adj;
    c[x][y] = COLLISION_APPLE;

    #if DEBUG
        // for (int i = top_bound_adj+1; i <= bottom_bound_adj-1; i++) {
        //     for (int j = left_bound_adj+1; j <= right_bound_adj-1; j++) {
        //         printf(esc "%d;%dHa", p->y+10,p->x);
        //     }
        // }
        printf(esc "4;0Haw:%d", ACTIVE_WIDTH);
        printf(esc "5;0Hcx:%d cy:%d", (rand_bit % ACTIVE_WIDTH) + left_bound_adj, (rand_bit / ACTIVE_WIDTH) + top_bound_adj);
        printf(esc "6;0Hrb:%d", rand_bit);
        printf(esc "7;0Hpx:%d", p->x);
        printf(esc "8;0Hpy:%d", p->y);
        printf(esc "9;0Hr%%aw:%d", rand_bit % ACTIVE_WIDTH);
        printf(esc "10;0Hr+aw:%d", rand_bit + ACTIVE_WIDTH);
        printf(esc "11;0H%d", rand_bit % ACTIVE_WIDTH);
        printf(esc "13;0Hrb:%d", rand_bit);
        printf(esc "12;0Haw:%d", ACTIVE_WIDTH);
    # endif
}

void add_segment(snake* s)
{
    point_wide* curr = &s->segments[s->num_segments];
    point_wide  prev = s->segments[s->num_segments - 1];
    curr->x = prev.x;
    curr->y = prev.y;
    strcpy(curr->icon, "█");
    s->num_segments++;
}

char get_movement_key(char* key_curr)
{
    switch(*key_curr) {
        case '\x1b': // Arrow Keys
            read(0, key_curr, 1);
            read(0, key_curr, 1);
            switch(*key_curr) {
                case 'A': // Up
                    return 'w';
                case 'B': // Down
                    return 's';
                case 'C': // Right
                    return 'd';
                case 'D': // Left
                    return 'a';
            }
        break;

        case 'w':
            return 'w';
        case 's':
            return 's';
        case 'd':
            return 'd';
        case 'a':
            return 'a';
    }
    return '0';
}

int move(
    snake* s, 
    point_wide* a, 
    uint8_t c[BOUND_WIDTH][BOUND_HEIGHT], 
    char key_curr, 
    char* key_prev
    ) 
{
    /*
    The tail of the snake is cleared in each call of `draw_sprites()` so it 
    does not have collision
    */
    uint8_t* collision_pos;
    uint8_t  done = 0;

    c[s->segments[s->num_segments - 1].x - left_bound_adj_snk]
     [s->segments[s->num_segments - 1].y - top_bound_adj] = COLLISION_NONE;

    for (int i = s->num_segments - 1; i > 0; i--) {
        s->segments[i].x = s->segments[i - 1].x;
        s->segments[i].y = s->segments[i - 1].y;
    }
    
    while (!done) {
        switch(key_curr) {
            case 'w':
                s->segments[0].y--;
                done = 1;
                break;
            case 's':
                s->segments[0].y++;
                done = 1;
                break;
            case 'a':
                s->segments[0].x -= 2;
                done = 1;
                break;
            case 'd':
                s->segments[0].x += 2;
                done = 1;
                break;
            default:
                key_curr = *key_prev;
                continue;
        }
    }

    collision_pos = &c[s->segments[0].x - left_bound_adj_snk]
                      [s->segments[0].y - top_bound_adj];
    *key_prev = key_curr;

    switch (*collision_pos) {
        case COLLISION_NONE:
            break;
        case COLLISION_APPLE:   
            add_segment(s);
            random_pos(a, c, s);
            *collision_pos = COLLISION_NONE;
            s->score++;
            break; 
        case COLLISION_BAD:
            return 0;
        default:
            exit(1);
    }

    *collision_pos = COLLISION_BAD;

    return 1;
}