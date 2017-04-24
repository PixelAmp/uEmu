#include "ui.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void history_update(struct ui *ui, const int current, const int prev,
                           size_t *size) {
  if (ui->history[current] == NULL) {
    ui->history[current] = malloc(ui->history_size[prev]);
    ui->history_size[current] = ui->history_size[prev];
  }

  *size = strlen(ui->history[prev]);
  if (*size >= ui->history_size[current])
    ui->history[current] = realloc(ui->history[current], *size + 1);
  memcpy(ui->history[current], ui->history[prev], *size + 1);
}

static void ui_history_last(struct ui *ui, size_t *size) {
  if (ui->history_index == 0)
    return;

  const int prev = (ui->history_index - 1) & (HISTORY_SIZE - 1);
  const int current = ui->history_index & (HISTORY_SIZE - 1);

  history_update(ui, current, prev, size);
}

static void hist_prev(struct ui *ui, size_t *size) {
  const int current = ui->history_index & (HISTORY_SIZE - 1);
  int prev = 0;

  if (*ui->history[current] != '!')
    return;

  if (ui->history[current][1] == '!' && ui->history[current][2] == '\n') {
    ui_history_last(ui, size);
    return;
  }

  for (const char *p = ui->history[current] + 1; *p != '\n'; ++p) {
    const int digit = *p ^ 0x30;
    if (digit > 9)
      return;

    prev *= 10;
    prev += digit;
  }

  if (prev >= ui->history_index || prev + HISTORY_SIZE <= ui->history_index)
    return;

  prev &= HISTORY_SIZE - 1;
  history_update(ui, current, prev, size);
}

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
  const int i = ui->history_index & (HISTORY_SIZE - 1);
  do {
    printf("%s ", ui->prompt);
    *size = getline(&ui->history[i], &ui->history_size[i], stdin);
  } while (ui->history[i][0] == 0);

  hist_prev(ui, size);
  ui->history_index++;

  return ui->history[i];
}

void ui_history(const struct ui *ui) {
  for (int i = ui->history_index < HISTORY_SIZE
                   ? 0
                   : ui->history_index - HISTORY_SIZE + 1;
       i != ui->history_index; i++) {
    printf("%d\t%s", i, ui->history[i & (HISTORY_SIZE - 1)]);
  }
}
