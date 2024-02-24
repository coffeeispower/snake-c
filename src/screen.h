#ifndef SCREEN_H
#define SCREEN_H
#include "vectors.h"
/// Moves the cursor to the specified coordinates
/// `x` and `y` start from 0
void move_cursor(int x, int y);
/// Hide terminal cursor
void hide_cursor(void);
/// Show terminal cursor
void show_cursor(void);
/// Clears the entire screen, including the scroll back
void clear_screen(void);
/// Clears the entire screen and moves the cursor to the
/// top-left corner of the screen
void reset_screen(void);

/// Resets any styling, including colors, underline, bold, etc...
void reset_styles(void);

/// Uses `ioctl()` to get the screen size
struct Vector2 get_terminal_size(void);

/// Sets the foreground color to the specified hex color
void set_color_rgb(unsigned int color);

/// Sets the background color to the specified hex color
void set_bg_color_rgb(unsigned int color);

/// Sets the foreground color to the specified color
///
/// See https://user-images.githubusercontent.com/995050/47952855-ecb12480-df75-11e8-89d4-ac26c50e80b9.png
/// for the available colors
void set_color_256(unsigned char color);

/// Sets the background color to the specified color
///
/// See https://user-images.githubusercontent.com/995050/47952855-ecb12480-df75-11e8-89d4-ac26c50e80b9.png
/// for the available colors
void set_bg_color_256(unsigned char color);

void draw_rectangle_border(struct Vector2 position, struct Vector2 size);


void switch_to_alternative_screen(void);
void exit_alternative_screen(void);
void delete_character_at(struct Vector2 position);
#endif
