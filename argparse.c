#define ARGPARSE_C
#include "argparse.h"

#include <stdio.h>
#include <string.h>

#define MAX_LINE 4096

char *args[ARG_MAX];
static char command[MAX_LINE + 1];

void parse_args(const char *line) {
  strncpy(command, line, MAX_LINE);
  char **p = args;

  for (*p++ = strtok(command, " \n");
       p != args + ARG_MAX && (*p++ = strtok(NULL, " \n"));)
    ;

  if (p == args + ARG_MAX)
    args[ARG_MAX - 1] = 0;
}
