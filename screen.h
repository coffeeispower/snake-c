#ifndef SCREEN_H
#define SCREEN_H
#include "vectors.h"
#define T_ESCAPE "\033"

/// Moves the cursor to the specified coordinates
/// `x` and `y` start from 0
void move_cursor(unsigned int x, unsigned int y);
/// Hide terminal cursor
void hide_cursor();
/// Show terminal cursor
void show_cursor();
/// Clears the entire screen, including the scroll back
void clear_screen();
/// Clears the entire screen and moves the cursor to the
/// top-left corner of the screen
void reset_screen();
/// Uses `ioctl()` to get the screen size
struct Vector2 get_terminal_size();
#endif
