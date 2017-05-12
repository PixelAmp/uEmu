#pragma once

#include <stddef.h>
#define HISTORY_SIZE 16

// A manager to handle the user interface.
struct ui {
  // The prompt to display to the user.
  char *prompt;

  // An array of strings of the commands inputted.
  char *history[HISTORY_SIZE];

  // The sizes of the buffers (not strings) in history.
  size_t history_size[HISTORY_SIZE];

  // The currently active index in the history.
  int history_index;
};

/*##################################################################################
     PURPOSE: Initializes a user interface.
  PARAMETERS: A pointer to the ui to initialize.
  PARAMETERS: prompt: The prompt which should be displayed to the user.
###################################################################################*/
void ui_init(struct ui *, const char *prompt);

/*##################################################################################
     PURPOSE: Frees resources associated with a UI.
  PARAMETERS: The user interface struct to free.
###################################################################################*/
void ui_free(struct ui *);

/*##################################################################################
     PURPOSE: Displays prompt, reads a line of input, and returns the associated
              string. Will perform history lookups if the user does '!n'
  PARAMETERS: The user interface.
      RETURN: A string of input.
###################################################################################*/
const char *ui_get_line(struct ui *ui);

/*##################################################################################
     PURPOSE: Print the history to the screen.
  PARAMETERS: The user interface struct.
###################################################################################*/
void ui_history(const struct ui *ui);
