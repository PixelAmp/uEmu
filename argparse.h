#pragma once

#include <stddef.h>

#define ARG_MAX 32

#ifndef ARGPARSE_C
extern char *const args[ARG_MAX];
#endif

void parse_args(const char *line);
