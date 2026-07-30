CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
CPPFLAGS ?= -Iinclude

BUILD_DIR := build
APP := $(BUILD_DIR)/text_canvas
TEST_APP := $(BUILD_DIR)/test_canvas

.PHONY: all run demo test clean

all: $(APP)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(APP): src/main.c src/canvas.c include/canvas.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c src/canvas.c -o $(APP)

$(TEST_APP): tests/test_canvas.c src/canvas.c include/canvas.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_canvas.c src/canvas.c -o $(TEST_APP)

run: $(APP)
	./$(APP)

demo: $(APP)
	./$(APP) --demo

test: $(TEST_APP)
	./$(TEST_APP)

clean:
	rm -rf $(BUILD_DIR)
