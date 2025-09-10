#include "prototypes.h"

unsigned char keymap_qwerty[NUM_KEYS] = {'w', 'd', 's', 'a', 'e'};
unsigned char keymap_dvorak[NUM_KEYS] = {',', 'e', 'o', 'a', '.'};
unsigned char keymap_azerty[NUM_KEYS] = {'z', 'd', 's', 'q', 'e'};

static unsigned char* keymap;

void set_keymap(unsigned char* keymap_chosen) { keymap = keymap_chosen; }

byte get_input(unsigned char* key)
{
  read(0, key, 1);

  if (paused) {
    if (hidden) {
      if (*key != KEY_NONE) {
        puts(ALT_BUF ON);
        *key   = KEY_NONE;
        hidden = FALSE;
        redraw = TRUE;
        return DIRECTION_NONE;
      }
    }
    if (*key == keymap[PAUSE]) {
      printf(ESC YX FMT_INFO "      " FMT_CLEAR, TOP_BOUND, width - 6); 
      *key   = KEY_NONE;
      paused = FALSE;
      return DIRECTION_NONE;
    }
  }
  if (*key == '\x1b') { // Arrow Keys
    read(0, key, 2);    // Skip to the important part of the key code
    switch(*key) {
      case 'A':             
        return UP;
      case 'B':             
        return DOWN;
      case 'C':             
        return RIGHT;
      case 'D':             
        return LEFT;
      default:
        return DIRECTION_NONE;
    }
  }

  if (*key == keymap[UP])    return UP;
  if (*key == keymap[DOWN])  return DOWN;
  if (*key == keymap[RIGHT]) return RIGHT;
  if (*key == keymap[LEFT])  return LEFT;
  if (*key == keymap[PAUSE]) {
    *key   = KEY_NONE;
    paused = TRUE;
    printf(ESC YX FMT_INFO "Paused" FMT_CLEAR,  TOP_BOUND, width - 6);
    return DIRECTION_NONE;
  }
  if (*key == KEY_HIDE) {
    puts(ALT_BUF OFF);
    *key   = KEY_NONE;
    hidden = TRUE;
    paused = TRUE;
    return DIRECTION_NONE;
  }
  return DIRECTION_NONE;
}

void queue_input(input_buffer* input_buffer, byte direction)
{
  if (direction != DIRECTION_NONE && 
      direction != input_buffer->inputs[input_buffer->current] &&       
      input_buffer->current < SIZE_INPUT_BUFFER - 1) { 
    input_buffer->inputs[input_buffer->current++] = direction;
    input_buffer->inputs[input_buffer->current]   = direction; 
    /* Is writing twice really faster than doing a comparison...? */
  }
}

byte dequeue_input(input_buffer* input_buffer)
{
  byte direction = input_buffer->inputs[0];
  if (input_buffer->current > 0) {
    for (int i = 0; i < input_buffer->current; i++) {
      input_buffer->inputs[i] = input_buffer->inputs[i + 1];
    }
    input_buffer->current--;
  }
  return direction;
}
