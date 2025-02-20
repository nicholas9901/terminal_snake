#include "prototypes.h"

void draw_sprites(snake* s) 
{
#if DEBUG /* Print the location of each segment */
    printf(ESC "0;0H");
    for (int i = 0; i <= s->ghost_pointer; i++) {
        printf("(%d,%d)", s->segments[i].x, s->segments[i].y);
    }
#endif
    printf(
        ESC YX SPRITE_CLEAR, 
        s->segments[s->ghost_pointer].y + top_bound_adj, 
        s->segments[s->ghost_pointer].x + left_bound_adj_snk
    );
    printf( /* Extra print for add_segment case */
        ESC YX SPRITE_SNAKE_BODY, 
        s->segments[s->ghost_pointer - 1].y + top_bound_adj, 
        s->segments[s->ghost_pointer - 1].x + left_bound_adj_snk
    );
    printf(
        ESC YX SPRITE_SNAKE_BODY, 
        s->segments[1].y + top_bound_adj, 
        s->segments[1].x + left_bound_adj_snk
    );
    printf(
        ESC YX SPRITE_SNAKE_HEAD, 
        s->segments[0].y + top_bound_adj, 
        s->segments[0].x + left_bound_adj_snk
    );
    printf(ESC YX "Score: %d", top_bound_adj - 2, left_bound_adj, s->score);
}

void draw_apple(point* a)
{
    printf(ESC YX SPRITE_APPLE, a->y + top_bound_adj, a->x + left_bound_adj_snk);
}

void draw_bounds(point_u* bounds, int perimeter) 
{
    for (int i = 0; i < perimeter; i++) {
        printf(ESC YX "%s", bounds[i].y, bounds[i].x, bounds[i].icon);
    }
}

void draw_controls() 
{
    printf(ESC "%d;1H" FMT_INFO "Quit: [q] Pause: [e]" FMT_CLEAR, height);
}