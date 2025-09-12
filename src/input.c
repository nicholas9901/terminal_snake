#include "prototypes.h"

static unsigned char keymaps[NUM_KEYMAPS][NUM_KEYS] = {
  {'w', 'd', 's', 'a', 'e', 'r'},
  {'z', 'd', 's', 'q', 'e', 'r'},
  {',', 'e', 'o', 'a', '.', 'p'}
};

unsigned char* keymap;

void set_keymap(byte keymap_chosen) { keymap = keymaps[keymap_chosen]; }

byte get_action(unsigned char* key)
{
  read(0, key, 1);    
  byte action = parse_keypress(key);
  *key = KEY_NONE;
  return action;
}

byte parse_keypress(unsigned char* key)
{
  byte success = FALSE;
  byte current = ACTION_NONE;
  current = parse_state_game_over(key, &success);
  if (success) return current;
  current = parse_state_hidden(key, &success);
  if (success) return current;
  current = parse_state_paused(key, &success);
  if (success) return current;
  return parse_state_movement(key);
}

static inline byte parse_state_movement(unsigned char* key)
{
  if (*key == '\x1b') { 
    read(0, key, 1); /* Skip to the important part of the key code */    
    read(0, key, 1);    
    switch(*key) {
    case 'A': return KEY_UP;
    case 'B': return KEY_DOWN;
    case 'C': return KEY_RIGHT;
    case 'D': return KEY_LEFT;
    default: return ACTION_NONE;
    }
  }
  else if (*key == keymap[KEY_UP])    return KEY_UP;
  else if (*key == keymap[KEY_DOWN])  return KEY_DOWN;
  else if (*key == keymap[KEY_RIGHT]) return KEY_RIGHT;
  else if (*key == keymap[KEY_LEFT])  return KEY_LEFT;
  return ACTION_NONE;
}

static inline byte parse_state_paused(unsigned char* key, byte* success)
{
  if (paused) {
    parse_state_hidden(key, success);
    if (*key == keymap[KEY_PAUSE]) {
      draw_unpause();
      paused   = FALSE;
      *success = TRUE;
      return ACTION_NONE;
    } 
  } else if (*key == keymap[KEY_PAUSE]) {
    draw_pause();
    paused   = TRUE;
    *success = TRUE;
    return ACTION_NONE;
  }
  return ACTION_NONE;
}

static inline byte parse_state_hidden(unsigned char* key, byte* success)
{
  if (hidden) {
    if (*key != KEY_NONE) {
      printf(ALT_BUF ON);
      hidden   = FALSE;
      redraw   = TRUE;
      *success = TRUE;
      return ACTION_NONE;
    }
  } else {
    if (*key == KEY_HIDE) {
      printf(ALT_BUF OFF);
      hidden   = TRUE;
      paused   = TRUE;
      *success = TRUE;
      return ACTION_NONE;
    }
  }
  return ACTION_NONE;
}

static inline byte parse_state_game_over(unsigned char* key, byte* success)
{
  if (game_over) {
    parse_state_hidden(key, success);
    if (*key == keymap[KEY_RESTART]) {
      retry = TRUE;
    }
    *success = TRUE;
  }
  return ACTION_NONE;
}

void queue_input(input_buffer* input_buffer, byte direction)
{
  if (direction != ACTION_NONE && 
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
