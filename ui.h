#pragma once
#include <stddef.h>
#define HISTORY_SIZE 16

struct ui {
  char *prompt;
  char *history[HISTORY_SIZE];
  size_t history_size[HISTORY_SIZE];
  int history_index;
};

void ui_init(struct ui *, const char *prompt);
void ui_free(struct ui *);

const char *ui_get_line(struct ui *ui, size_t *size);
void ui_history(const struct ui *ui);

/*##################################################################################
     PURPOSE: 
  PARAMETERS:           
      RETURN:               
###################################################################################*/