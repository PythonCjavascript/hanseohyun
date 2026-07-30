#ifndef TEXT_CANVAS_H
#define TEXT_CANVAS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define CANVAS_ROWS 20
#define CANVAS_COLS 70
#define CANVAS_EMPTY '.'

typedef struct {
    char cells[CANVAS_ROWS][CANVAS_COLS];
} Canvas;

typedef enum {
    CANVAS_OK = 0,
    CANVAS_ERR_ARGUMENT,
    CANVAS_ERR_COORDINATE,
    CANVAS_ERR_SYMBOL,
    CANVAS_ERR_RANGE,
    CANVAS_ERR_OPEN,
    CANVAS_ERR_WRITE,
    CANVAS_ERR_READ,
    CANVAS_ERR_FORMAT
} CanvasStatus;

void canvas_init(Canvas *canvas);
bool canvas_coordinate_is_valid(size_t row, size_t col);
bool canvas_symbol_is_valid(char symbol);
CanvasStatus canvas_put(Canvas *canvas, size_t row, size_t col, char symbol);
CanvasStatus canvas_erase(Canvas *canvas, size_t row, size_t col);
CanvasStatus canvas_draw_rectangle(
    Canvas *canvas,
    size_t top,
    size_t left,
    size_t bottom,
    size_t right,
    char symbol
);
void canvas_render(const Canvas *canvas, FILE *stream);
CanvasStatus canvas_save(const Canvas *canvas, const char *path);
CanvasStatus canvas_load(Canvas *canvas, const char *path);
const char *canvas_status_message(CanvasStatus status);

#endif
