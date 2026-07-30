#include "canvas.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER_SIZE 512
#define PATH_BUFFER_SIZE 256

static bool read_line(const char *prompt, char *buffer, size_t size) {
    size_t length;
    int ch;

    if (prompt != NULL) {
        fputs(prompt, stdout);
    }
    if (fgets(buffer, (int) size, stdin) == NULL) {
        return false;
    }

    length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
        return true;
    }

    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard the rest of an overlong line */
    }
    return false;
}

static bool parse_menu_choice(const char *text, int *value) {
    char *end;
    long parsed;

    if (text == NULL || value == NULL) {
        return false;
    }

    errno = 0;
    parsed = strtol(text, &end, 10);
    while (*end == ' ' || *end == '\t') {
        ++end;
    }

    if (errno != 0 || end == text || *end != '\0' || parsed < INT_MIN || parsed > INT_MAX) {
        return false;
    }

    *value = (int) parsed;
    return true;
}

static bool parse_point_and_symbol(
    const char *text,
    size_t *row,
    size_t *col,
    char *symbol
) {
    unsigned long parsed_row;
    unsigned long parsed_col;
    char parsed_symbol;
    char extra;

    if (sscanf(text, " %lu %lu %c %c", &parsed_row, &parsed_col, &parsed_symbol, &extra) != 3) {
        return false;
    }

    *row = (size_t) parsed_row;
    *col = (size_t) parsed_col;
    *symbol = parsed_symbol;
    return true;
}

static bool parse_point(const char *text, size_t *row, size_t *col) {
    unsigned long parsed_row;
    unsigned long parsed_col;
    char extra;

    if (sscanf(text, " %lu %lu %c", &parsed_row, &parsed_col, &extra) != 2) {
        return false;
    }

    *row = (size_t) parsed_row;
    *col = (size_t) parsed_col;
    return true;
}

static bool parse_rectangle(
    const char *text,
    size_t *top,
    size_t *left,
    size_t *bottom,
    size_t *right,
    char *symbol
) {
    unsigned long parsed_top;
    unsigned long parsed_left;
    unsigned long parsed_bottom;
    unsigned long parsed_right;
    char parsed_symbol;
    char extra;

    if (sscanf(
            text,
            " %lu %lu %lu %lu %c %c",
            &parsed_top,
            &parsed_left,
            &parsed_bottom,
            &parsed_right,
            &parsed_symbol,
            &extra
        ) != 5) {
        return false;
    }

    *top = (size_t) parsed_top;
    *left = (size_t) parsed_left;
    *bottom = (size_t) parsed_bottom;
    *right = (size_t) parsed_right;
    *symbol = parsed_symbol;
    return true;
}

static bool file_exists(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return false;
    }
    fclose(file);
    return true;
}

static bool confirm_overwrite(void) {
    char input[INPUT_BUFFER_SIZE];

    if (!read_line("파일이 이미 있습니다. 덮어쓸까요? (y/n): ", input, sizeof(input))) {
        return false;
    }
    return strcmp(input, "y") == 0 || strcmp(input, "Y") == 0;
}

static void print_result(CanvasStatus status) {
    if (status == CANVAS_OK) {
        puts("완료되었습니다.");
    } else {
        fprintf(stderr, "오류: %s\n", canvas_status_message(status));
    }
}

static void print_menu(void) {
    puts("\n[메뉴]");
    puts("1. 문자 그리기");
    puts("2. 문자 지우기");
    puts("3. 사각형 테두리 그리기");
    puts("4. 파일 저장");
    puts("5. 파일 불러오기");
    puts("6. 새 캔버스");
    puts("7. 종료");
}

static void place_text(Canvas *canvas, size_t row, size_t col, const char *text) {
    size_t offset;
    for (offset = 0; text[offset] != '\0' && col + offset <= CANVAS_COLS; ++offset) {
        (void) canvas_put(canvas, row, col + offset, text[offset]);
    }
}

static void run_demo(void) {
    Canvas canvas;

    canvas_init(&canvas);
    (void) canvas_draw_rectangle(&canvas, 3, 8, 18, 63, '#');
    place_text(&canvas, 7, 26, "TEXT-CANVAS");
    place_text(&canvas, 10, 20, "portable-C11-safe-input");
    place_text(&canvas, 13, 24, "save-load-validate");
    canvas_render(&canvas, stdout);
}

static int run_interactive(void) {
    Canvas canvas;
    char input[INPUT_BUFFER_SIZE];
    char path[PATH_BUFFER_SIZE];
    bool running = true;

    canvas_init(&canvas);
    canvas_render(&canvas, stdout);

    while (running) {
        int choice;
        CanvasStatus status;

        print_menu();
        if (!read_line("선택: ", input, sizeof(input))) {
            puts("입력을 읽을 수 없어 종료합니다.");
            break;
        }
        if (!parse_menu_choice(input, &choice)) {
            puts("1부터 7까지의 정수를 입력해 주세요.");
            continue;
        }

        switch (choice) {
            case 1: {
                size_t row;
                size_t col;
                char symbol;
                if (!read_line("행 열 문자 (예: 5 10 *): ", input, sizeof(input))
                    || !parse_point_and_symbol(input, &row, &col, &symbol)) {
                    puts("입력 형식이 올바르지 않습니다.");
                    break;
                }
                status = canvas_put(&canvas, row, col, symbol);
                print_result(status);
                if (status == CANVAS_OK) {
                    canvas_render(&canvas, stdout);
                }
                break;
            }
            case 2: {
                size_t row;
                size_t col;
                if (!read_line("행 열 (예: 5 10): ", input, sizeof(input))
                    || !parse_point(input, &row, &col)) {
                    puts("입력 형식이 올바르지 않습니다.");
                    break;
                }
                status = canvas_erase(&canvas, row, col);
                print_result(status);
                if (status == CANVAS_OK) {
                    canvas_render(&canvas, stdout);
                }
                break;
            }
            case 3: {
                size_t top;
                size_t left;
                size_t bottom;
                size_t right;
                char symbol;
                if (!read_line(
                        "위 왼쪽 아래 오른쪽 문자 (예: 3 5 10 20 #): ",
                        input,
                        sizeof(input)
                    )
                    || !parse_rectangle(input, &top, &left, &bottom, &right, &symbol)) {
                    puts("입력 형식이 올바르지 않습니다.");
                    break;
                }
                status = canvas_draw_rectangle(
                    &canvas,
                    top,
                    left,
                    bottom,
                    right,
                    symbol
                );
                print_result(status);
                if (status == CANVAS_OK) {
                    canvas_render(&canvas, stdout);
                }
                break;
            }
            case 4:
                if (!read_line("저장할 파일 경로: ", path, sizeof(path)) || path[0] == '\0') {
                    puts("파일 경로를 다시 입력해 주세요.");
                    break;
                }
                if (file_exists(path) && !confirm_overwrite()) {
                    puts("저장을 취소했습니다.");
                    break;
                }
                print_result(canvas_save(&canvas, path));
                break;
            case 5:
                if (!read_line("불러올 파일 경로: ", path, sizeof(path)) || path[0] == '\0') {
                    puts("파일 경로를 다시 입력해 주세요.");
                    break;
                }
                status = canvas_load(&canvas, path);
                print_result(status);
                if (status == CANVAS_OK) {
                    canvas_render(&canvas, stdout);
                }
                break;
            case 6:
                canvas_init(&canvas);
                puts("새 캔버스를 만들었습니다.");
                canvas_render(&canvas, stdout);
                break;
            case 7:
                running = false;
                puts("프로그램을 종료합니다.");
                break;
            default:
                puts("1부터 7까지의 번호를 선택해 주세요.");
                break;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "--demo") == 0) {
        run_demo();
        return EXIT_SUCCESS;
    }
    if (argc != 1) {
        fprintf(stderr, "사용법: %s [--demo]\n", argv[0]);
        return EXIT_FAILURE;
    }
    return run_interactive();
}
