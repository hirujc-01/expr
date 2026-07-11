CC := gcc

TARGET := build/mathparser
TEST_TARGET := build/tests

#
# Sources
#

APP_SRC := $(shell find src -name "*.c")
APP_OBJ := $(patsubst src/%.c,build/obj/%.o,$(APP_SRC))

#
# For tests we don't want src/main.c
#

CORE_SRC := $(filter-out src/main.c,$(APP_SRC))
CORE_OBJ := $(patsubst src/%.c,build/obj/%.o,$(CORE_SRC))

TEST_SRC := $(shell find tests -name "*.c")
TEST_OBJ := $(patsubst tests/%.c,build/obj/tests/%.o,$(TEST_SRC))

#
# Flags
#

CFLAGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-std=c17 \
	-g \
	-fsanitize=address,undefined \
	-Iinclude \
	-Itests/include

LDFLAGS := \
	-fsanitize=address,undefined \
	-lm

.PHONY: all clean run debug release test build

all: $(TARGET)

#
# Create build directory
#

build:
	@mkdir -p build

#
# Compile application sources
#

build/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#
# Compile test sources
#

build/obj/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

#
# Main executable
#

$(TARGET): build $(APP_OBJ)
	$(CC) $(APP_OBJ) -o $@ $(LDFLAGS)

#
# Test executable
#

$(TEST_TARGET): build $(CORE_OBJ) $(TEST_OBJ)
	$(CC) $(CORE_OBJ) $(TEST_OBJ) -o $@ $(LDFLAGS)

#
# Convenience targets
#

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

debug: $(TARGET)
	gdb $(TARGET)

release: CFLAGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-std=c17 \
	-O2 \
	-Iinclude \
	-Itests/include

release: LDFLAGS := -lm

release: clean all

clean:
	rm -rf build