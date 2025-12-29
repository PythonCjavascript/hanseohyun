#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>  // _access 함수 사용
#define rows 20
#define cols 70

char board[rows][cols];

// 화면 초기화 함수
void resetboard() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '_';  // 초기 화면에 '_' 문자를 사용하여 비워두기
        }
    }
}

// 화면 출력 함수
void displayboard() {
    printf("**1234567890123456789012345678901234567890123456789012345678901234567890\n");
    printf("*************************************************************************************\n");
    for (int i = 0; i < rows; i++) {
        printf("%02d", i + 1);
        for (int j = 0; j < cols; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

// 좌표에 문자 그리기 함수
void drawcharacter(int row, int col, char ch) {
    if (row >= 1 && row <= rows && col >= 1 && col <= cols) {
        board[row - 1][col - 1] = ch;
    }
    else {
        printf("틀린 좌표입니다.\n");
    }
}

// 좌표 지우기 함수
void erasecharacter(int row, int col) {
    if (row >= 1 && row <= rows && col >= 1 && col <= cols) {
        board[row - 1][col - 1] = '_';
    }
    else {
        printf("틀린 좌표입니다.\n");
    }
}

// 화면 초기화
void clearscreen() {
    resetboard();
    system("cls");
    displayboard();
}

// 화면 저장 함수
void saveboard() {
    char filename[100];
    printf("저장할 파일 이름 입력: ");
    scanf("%s", filename);

    if (_access(filename, 0) == 0) {
        printf("파일이 이미 존재합니다. 덮어쓸까요? (y/n): ");
        char overwrite;
        scanf(" %c", &overwrite);
        if (overwrite != 'y' && overwrite != 'Y') {
            printf("저장 취소\n");
            return;
        }
        else {
            printf("새 파일을 생성하여 저장합니다.\n");
        }
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("파일 저장 실패\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        fwrite(board[i], sizeof(char), cols, file);
        fputc('\n', file);
    }
    fclose(file);
    printf("저장 완료\n");
}

// 화면 불러오기 함수
void loadboard() {
    char filename[100];
    printf("불러올 파일 이름 입력: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("파일 불러오기 실패\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        if (fread(board[i], sizeof(char), cols, file) < cols) {
            printf("파일 데이터가 올바르지 않습니다.\n");
            fclose(file);
            return;
        }
        fgetc(file); // 개행 문자 읽기
    }
    fclose(file);
    printf("불러오기 완료\n");
}

// 메뉴 출력 및 사용자 선택 처리
void displaymenu() {
    int choice;
    while (1) {
        printf("\n1. 그리기\n2. 지우기\n3. 저장하기\n4. 불러오기\n5. 새 화면\n6. 종료\n");
        printf("선택: ");
        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다. 프로그램 종료\n");
            exit(1);
        }

        if (choice == 1) {  // 그리기
            int row, col;
            char ch;
            printf("행, 열 번호 및 문자를 입력 (예: 5 10 *): ");
            if (scanf("%d %d %c", &row, &col, &ch) != 3) {
                printf("잘못된 입력입니다. 프로그램 종료\n");
                exit(1);
            }
            drawcharacter(row, col, ch);
            displayboard();
        }
        else if (choice == 2) {  // 지우기
            int row, col;
            printf("지울 행과 열 번호 입력 (예: 5 10): ");
            if (scanf("%d %d", &row, &col) != 2) {
                printf("잘못된 입력입니다. 프로그램 종료\n");
                exit(1);
            }
            erasecharacter(row, col);
            displayboard();
        }
        else if (choice == 3) {  // 저장하기
            saveboard();
        }
        else if (choice == 4) {  // 불러오기
            loadboard();
            displayboard();
        }
        else if (choice == 5) {  // 새 화면
            clearscreen();
        }
        else if (choice == 6) {  // 종료
            printf("프로그램 종료\n");
            break;
        }
        else {
            printf("유효하지 않은 선택입니다.\n");
        }
    }
}

int main() {
    resetboard();
    displayboard();
    displaymenu();
    return 0;
}
