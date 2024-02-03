#ifndef INPUT_H
#define INPUT_H
enum SnakeInput {
  /// No input was detected
  NONE,
  /// Turn snake up
  UP,
  /// Turn snake down
  DOWN,
  /// Turn snake left
  LEFT,
  /// Turn snake right
  RIGHT,
  /// Quit game
  QUIT,
  /// Restart the game
  RESTART
};

enum SnakeInput read_input();
// Enables raw mode to be able to get inputs immediately without pressing enter
void enable_raw_mode();
// Disables raw mode, switching back to the state of the terminal when the program started
void disable_raw_mode();
#endif
