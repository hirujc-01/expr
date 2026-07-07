CC := gcc

TARGET := build/mathparser

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/obj/%.o,$(SRC))

CFLAGS := -Wall -Wextra -Wpedantic -std=c17 -g \
           -fsanitize=address,undefined \
           -Iinclude

LDFLAGS := -fsanitize=address,undefined

.PHONY: all clean run debug release

all: $(TARGET)

build:
	mkdir -p build build/obj

$(TARGET): build $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/obj/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: $(TARGET)
	gdb $(TARGET)

release: CFLAGS := -Wall -Wextra -Wpedantic -std=c17 -O2 -Iinclude
release: LDFLAGS :=
release: clean all

clean:
	rm -rf build