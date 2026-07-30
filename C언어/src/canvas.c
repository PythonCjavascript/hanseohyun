#include "canvas.h"

#include <string.h>

#define FILE_MAGIC "TEXTCANVAS 1"
#define HEADER_BUFFER_SIZE 64

static bool stored_symbol_is_valid(char symbol) {
    return symbol == CANVAS_EMPTY || canvas_symbol_is_valid(symbol);
}

void canvas_init(Canvas *canvas) {
    size_t row;
    size_t col;

    if (canvas == NULL) {
        return;
    }

    for (row = 0; row < CANVAS_ROWS; ++row) {
        for (col = 0; col < CANVAS_COLS; ++col) {
            canvas->cells[row][col] = CANVAS_EMPTY;
        }
    }
}

bool canvas_coordinate_is_valid(size_t row, size_t col) {
    return row >= 1 && row <= CANVAS_ROWS && col >= 1 && col <= CANVAS_COLS;
}

bool canvas_symbol_is_valid(char symbol) {
    const unsigned char value = (unsigned char) symbol;
    return value >= 33U && value <= 126U;
}

CanvasStatus canvas_put(Canvas *canvas, size_t row, size_t col, char symbol) {
    if (canvas == NULL) {
        return CANVAS_ERR_ARGUMENT;
    }
    if (!canvas_coordinate_is_valid(row, col)) {
        return CANVAS_ERR_COORDINATE;
    }
    if (!canvas_symbol_is_valid(symbol)) {
        return CANVAS_ERR_SYMBOL;
    }

    canvas->cells[row - 1][col - 1] = symbol;
    return CANVAS_OK;
}

CanvasStatus canvas_erase(Canvas *canvas, size_t row, size_t col) {
    if (canvas == NULL) {
        return CANVAS_ERR_ARGUMENT;
    }
    if (!canvas_coordinate_is_valid(row, col)) {
        return CANVAS_ERR_COORDINATE;
    }

    canvas->cells[row - 1][col - 1] = CANVAS_EMPTY;
    return CANVAS_OK;
}

CanvasStatus canvas_draw_rectangle(
    Canvas *canvas,
    size_t top,
    size_t left,
    size_t bottom,
    size_t right,
    char symbol
) {
    size_t row;
    size_t col;

    if (canvas == NULL) {
        return CANVAS_ERR_ARGUMENT;
    }
    if (!canvas_coordinate_is_valid(top, left)
        || !canvas_coordinate_is_valid(bottom, right)) {
        return CANVAS_ERR_COORDINATE;
    }
    if (top > bottom || left > right) {
        return CANVAS_ERR_RANGE;
    }
    if (!canvas_symbol_is_valid(symbol)) {
        return CANVAS_ERR_SYMBOL;
    }

    for (col = left; col <= right; ++col) {
        canvas->cells[top - 1][col - 1] = symbol;
        canvas->cells[bottom - 1][col - 1] = symbol;
    }
    for (row = top; row <= bottom; ++row) {
        canvas->cells[row - 1][left - 1] = symbol;
        canvas->cells[row - 1][right - 1] = symbol;
    }

    return CANVAS_OK;
}

void canvas_render(const Canvas *canvas, FILE *stream) {
    size_t row;
    size_t col;

    if (canvas == NULL || stream == NULL) {
        return;
    }

    fputs("    ", stream);
    for (col = 1; col <= CANVAS_COLS; ++col) {
        const size_t tens = col / 10;
        fputc(tens == 0 ? ' ' : (int) ('0' + (tens % 10)), stream);
    }
    fputc('\n', stream);

    fputs("    ", stream);
    for (col = 1; col <= CANVAS_COLS; ++col) {
        fputc((int) ('0' + (col % 10)), stream);
    }
    fputc('\n', stream);

    fputs("   +", stream);
    for (col = 0; col < CANVAS_COLS; ++col) {
        fputc('-', stream);
    }
    fputs("+\n", stream);

    for (row = 0; row < CANVAS_ROWS; ++row) {
        fprintf(stream, "%2zu |", row + 1);
        for (col = 0; col < CANVAS_COLS; ++col) {
            fputc(canvas->cells[row][col], stream);
        }
        fputs("|\n", stream);
    }

    fputs("   +", stream);
    for (col = 0; col < CANVAS_COLS; ++col) {
        fputc('-', stream);
    }
    fputs("+\n", stream);
}

CanvasStatus canvas_save(const Canvas *canvas, const char *path) {
    FILE *file;
    size_t row;

    if (canvas == NULL || path == NULL || path[0] == '\0') {
        return CANVAS_ERR_ARGUMENT;
    }

    file = fopen(path, "wb");
    if (file == NULL) {
        return CANVAS_ERR_OPEN;
    }

    if (fprintf(file, "%s\n%d %d\n", FILE_MAGIC, CANVAS_ROWS, CANVAS_COLS) < 0) {
        fclose(file);
        return CANVAS_ERR_WRITE;
    }

    for (row = 0; row < CANVAS_ROWS; ++row) {
        if (fwrite(canvas->cells[row], sizeof(char), CANVAS_COLS, file) != CANVAS_COLS
            || fputc('\n', file) == EOF) {
            fclose(file);
            return CANVAS_ERR_WRITE;
        }
    }

    if (fclose(file) != 0) {
        return CANVAS_ERR_WRITE;
    }
    return CANVAS_OK;
}

CanvasStatus canvas_load(Canvas *canvas, const char *path) {
    FILE *file;
    Canvas loaded;
    char header[HEADER_BUFFER_SIZE];
    char dimensions[HEADER_BUFFER_SIZE];
    int rows;
    int cols;
    size_t row;
    size_t col;
    int trailing;

    if (canvas == NULL || path == NULL || path[0] == '\0') {
        return CANVAS_ERR_ARGUMENT;
    }

    file = fopen(path, "rb");
    if (file == NULL) {
        return CANVAS_ERR_OPEN;
    }

    if (fgets(header, sizeof(header), file) == NULL
        || strcmp(header, FILE_MAGIC "\n") != 0) {
        fclose(file);
        return CANVAS_ERR_FORMAT;
    }

    if (fgets(dimensions, sizeof(dimensions), file) == NULL
        || sscanf(dimensions, "%d %d", &rows, &cols) != 2
        || rows != CANVAS_ROWS
        || cols != CANVAS_COLS) {
        fclose(file);
        return CANVAS_ERR_FORMAT;
    }

    canvas_init(&loaded);
    for (row = 0; row < CANVAS_ROWS; ++row) {
        if (fread(loaded.cells[row], sizeof(char), CANVAS_COLS, file) != CANVAS_COLS) {
            fclose(file);
            return CANVAS_ERR_READ;
        }
        for (col = 0; col < CANVAS_COLS; ++col) {
            if (!stored_symbol_is_valid(loaded.cells[row][col])) {
                fclose(file);
                return CANVAS_ERR_FORMAT;
            }
        }
        if (fgetc(file) != '\n') {
            fclose(file);
            return CANVAS_ERR_FORMAT;
        }
    }

    do {
        trailing = fgetc(file);
    } while (trailing == ' ' || trailing == '\t' || trailing == '\r' || trailing == '\n');

    if (trailing != EOF) {
        fclose(file);
        return CANVAS_ERR_FORMAT;
    }

    if (ferror(file)) {
        fclose(file);
        return CANVAS_ERR_READ;
    }

    if (fclose(file) != 0) {
        return CANVAS_ERR_READ;
    }

    *canvas = loaded;
    return CANVAS_OK;
}

const char *canvas_status_message(CanvasStatus status) {
    switch (status) {
        case CANVAS_OK:
            return "success";
        case CANVAS_ERR_ARGUMENT:
            return "invalid argument";
        case CANVAS_ERR_COORDINATE:
            return "coordinate is outside the 20 x 70 canvas";
        case CANVAS_ERR_SYMBOL:
            return "symbol must be one visible ASCII character";
        case CANVAS_ERR_RANGE:
            return "rectangle start must not exceed its end";
        case CANVAS_ERR_OPEN:
            return "file could not be opened";
        case CANVAS_ERR_WRITE:
            return "file could not be written completely";
        case CANVAS_ERR_READ:
            return "file could not be read completely";
        case CANVAS_ERR_FORMAT:
            return "file format or canvas dimensions are invalid";
        default:
            return "unknown error";
    }
}
