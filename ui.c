#include "ui.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ui_init(struct ui *ui, const char *prompt) {
  assert(__builtin_popcount(HISTORY_SIZE) == 1);

  int prompt_len = strlen(prompt);
  ui->prompt = malloc(prompt_len + 1);
  memcpy(ui->prompt, prompt, prompt_len + 1);

  ui->history_index = 0;
  for (int i = HISTORY_SIZE; i--;)
    ui->history[i] = 0;
}

void ui_free(struct ui *ui) {
  free(ui->prompt);
  for (int i = HISTORY_SIZE; i--;)
    free(ui->history[i]);
}

const char *ui_get_line(struct ui *ui, size_t *size) {
  const int i = ui->history_index;
  do {
    printf("%s ", ui->prompt);
    *size = getline(&ui->history[i], &ui->history_size[i], stdin);
  } while (ui->history[i][0] == 0);

  ui->history_index++;
  ui->history_index &= HISTORY_SIZE - 1;

  return ui->history[i];
}
