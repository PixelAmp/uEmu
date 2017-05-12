CFLAGS += -march=native -pipe -MMD -Wall -Wextra -std=gnu99 -D_GNU_SOURCE
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

release: CFLAGS += -O3 -DNDEBUG
release: uEmu

debug: CFLAGS += -O0 -ggdb3
debug: uEmu

uEmu: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f *.[od] uEmu

DEPS := $(OBJ:.o=.d)
-include $(DEPS)
