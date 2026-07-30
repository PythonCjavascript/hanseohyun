#include "canvas.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define ROUNDTRIP_FILE "build/test_roundtrip.tdb"
#define INVALID_FILE "build/test_invalid.tdb"

static void test_initialization(void) {
    Canvas canvas;
    size_t row;
    size_t col;

    canvas_init(&canvas);
    for (row = 0; row < CANVAS_ROWS; ++row) {
        for (col = 0; col < CANVAS_COLS; ++col) {
            assert(canvas.cells[row][col] == CANVAS_EMPTY);
        }
    }
}

static void test_put_and_erase(void) {
    Canvas canvas;

    canvas_init(&canvas);
    assert(canvas_put(&canvas, 1, 1, 'X') == CANVAS_OK);
    assert(canvas.cells[0][0] == 'X');
    assert(canvas_erase(&canvas, 1, 1) == CANVAS_OK);
    assert(canvas.cells[0][0] == CANVAS_EMPTY);
    assert(canvas_put(&canvas, 0, 1, 'X') == CANVAS_ERR_COORDINATE);
    assert(canvas_put(&canvas, 1, 71, 'X') == CANVAS_ERR_COORDINATE);
    assert(canvas_put(&canvas, 1, 1, ' ') == CANVAS_ERR_SYMBOL);
}

static void test_rectangle(void) {
    Canvas canvas;

    canvas_init(&canvas);
    assert(canvas_draw_rectangle(&canvas, 2, 3, 4, 6, '#') == CANVAS_OK);
    assert(canvas.cells[1][2] == '#');
    assert(canvas.cells[1][5] == '#');
    assert(canvas.cells[3][2] == '#');
    assert(canvas.cells[3][5] == '#');
    assert(canvas.cells[2][3] == CANVAS_EMPTY);
    assert(canvas_draw_rectangle(&canvas, 4, 6, 2, 3, '#') == CANVAS_ERR_RANGE);
}

static void test_save_load_roundtrip(void) {
    Canvas source;
    Canvas loaded;

    canvas_init(&source);
    assert(canvas_put(&source, 5, 10, '*') == CANVAS_OK);
    assert(canvas_draw_rectangle(&source, 2, 2, 6, 12, '+') == CANVAS_OK);
    assert(canvas_save(&source, ROUNDTRIP_FILE) == CANVAS_OK);

    canvas_init(&loaded);
    assert(canvas_load(&loaded, ROUNDTRIP_FILE) == CANVAS_OK);
    assert(memcmp(&source, &loaded, sizeof(Canvas)) == 0);
    assert(remove(ROUNDTRIP_FILE) == 0);
}

static void test_invalid_file_does_not_replace_canvas(void) {
    Canvas canvas;
    Canvas before;
    FILE *file;

    canvas_init(&canvas);
    assert(canvas_put(&canvas, 1, 1, 'K') == CANVAS_OK);
    before = canvas;

    file = fopen(INVALID_FILE, "wb");
    assert(file != NULL);
    fputs("WRONG HEADER\n20 70\n", file);
    assert(fclose(file) == 0);

    assert(canvas_load(&canvas, INVALID_FILE) == CANVAS_ERR_FORMAT);
    assert(memcmp(&canvas, &before, sizeof(Canvas)) == 0);
    assert(remove(INVALID_FILE) == 0);
}

int main(void) {
    test_initialization();
    test_put_and_erase();
    test_rectangle();
    test_save_load_roundtrip();
    test_invalid_file_does_not_replace_canvas();

    puts("All canvas tests passed.");
    return 0;
}
