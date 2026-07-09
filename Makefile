CC := gcc

TARGET := build/mathparser
TEST_TARGET := build/tests

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/obj/%.o,$(SRC))

TEST_SRC := $(wildcard tests/*.c)
TEST_OBJ := $(patsubst tests/%.c,build/obj/tests/%.o,$(TEST_SRC))

CFLAGS := -Wall -Wextra -Wpedantic -std=c17 -g \
          -fsanitize=address,undefined \
          -Iinclude

LDFLAGS := -fsanitize=address,undefined -lm

.PHONY: all clean run debug release test

all: $(TARGET)

build:
	mkdir -p build build/obj build/obj/tests

#
# Main executable
#

$(TARGET): build $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/obj/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

#
# Unit tests
#

$(TEST_TARGET): build $(OBJ) $(TEST_OBJ)
	$(CC) $(OBJ) $(TEST_OBJ) -o $@ $(LDFLAGS)

build/obj/tests/%.o: tests/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

#
# Convenience targets
#

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb $(TARGET)

release: CFLAGS := -Wall -Wextra -Wpedantic -std=c17 -O2 -Iinclude
release: LDFLAGS := -lm
release: clean all

clean:
	rm -rf build