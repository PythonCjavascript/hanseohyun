# Text Canvas CLI — C11 텍스트 그림판

20×70 문자 배열을 캔버스로 사용해 좌표에 문자를 그리고, 사각형 테두리를 만들며, 작업 결과를 파일로 저장·복원하는 터미널 프로그램입니다.

초기 수업 프로젝트를 취업용 포트폴리오에 맞게 다시 구성했습니다. Windows 전용 함수와 `scanf` 중심 입력을 제거하고, 표준 C11 코드·입력 검증·버전이 있는 파일 형식·자동 테스트를 추가했습니다.

## 개선 내용

| 초기 버전 | 포트폴리오 버전 |
|---|---|
| 전역 2차원 배열과 단일 소스 파일 | `Canvas` 구조체와 헤더·구현·실행 파일 분리 |
| `_access`, `system("cls")` 등 Windows 의존 | 표준 C 라이브러리만 사용해 Windows·macOS·Linux에서 컴파일 가능 |
| `scanf` 오류 시 프로그램 종료 | `fgets` 기반 한 줄 입력과 형식·범위 재검증 |
| 행 데이터만 저장 | 매직 문자열·버전·캔버스 크기를 포함한 파일 형식 |
| 일부만 읽힌 파일이 현재 캔버스를 훼손할 수 있음 | 임시 캔버스에서 전체 검증 후 성공할 때만 교체 |
| 수동 확인 | 좌표·도형·저장/복원·잘못된 파일을 검증하는 테스트 추가 |

## 핵심 기능

- **문자 그리기·지우기:** 1부터 시작하는 행·열 좌표를 2차원 배열에 매핑
- **사각형 테두리:** 시작·끝 좌표와 문자를 받아 경계 셀을 일괄 갱신
- **저장·불러오기:** 20×70 캔버스를 버전이 있는 텍스트 포맷으로 영구 저장
- **파일 무결성 검사:** 헤더, 행·열 크기, 각 행 길이, 허용 문자를 검증
- **입력 방어:** 메뉴, 좌표, 문자, 경로 입력 오류가 발생해도 반복 실행 유지
- **데모·테스트:** `--demo` 실행 모드와 `make test` 제공

## 설계

```text
입력 처리(main.c)
    ↓ 검증된 명령
Canvas API(canvas.h)
    ↓
배열 조작·렌더링·파일 입출력(canvas.c)
    ↓
20 × 70 고정 크기 Canvas 구조체
```

주요 API는 다음과 같습니다.

| 함수 | 역할 |
|---|---|
| `canvas_init` | 모든 셀을 빈 문자로 초기화 |
| `canvas_put` / `canvas_erase` | 좌표 검증 후 한 셀 변경 |
| `canvas_draw_rectangle` | 좌표 범위 검증 후 사각형 경계 생성 |
| `canvas_render` | 행·열 번호와 함께 터미널 출력 |
| `canvas_save` | 버전·크기·셀 데이터를 파일에 기록 |
| `canvas_load` | 파일 전체를 검증한 뒤 캔버스 교체 |

## 저장 파일 형식

```text
TEXTCANVAS 1
20 70
<70 characters for row 1>
...
<70 characters for row 20>
```

`TEXTCANVAS 1`은 파일 형식 식별자와 버전입니다. 다른 크기, 손상된 행, 허용되지 않은 문자가 발견되면 불러오기를 중단하고 기존 캔버스를 유지합니다.

## 빌드 및 실행

### Make 사용

```bash
make
./build/text_canvas
```

```bash
make demo
make test
```

### GCC 직접 사용

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude \
  src/main.c src/canvas.c -o text_canvas
./text_canvas
```

Windows MinGW에서는 출력 파일명을 `text_canvas.exe`로 지정하면 됩니다.

## 실행 예시

```text
[메뉴]
1. 문자 그리기
2. 문자 지우기
3. 사각형 테두리 그리기
4. 파일 저장
5. 파일 불러오기
6. 새 캔버스
7. 종료

선택: 1
행 열 문자 (예: 5 10 *): 5 10 *
완료되었습니다.
```

전체 데모 출력은 [`outputs/demo_output.txt`](outputs/demo_output.txt)에서 확인할 수 있습니다.

## 테스트 범위

`make test`는 다음 항목을 자동 확인합니다.

- 20×70 전체 셀 초기화
- 정상·비정상 좌표 처리
- 허용되지 않은 문자 거부
- 사각형 경계와 내부 셀 구분
- 저장 후 불러오기 결과의 바이트 단위 일치
- 잘못된 헤더를 읽었을 때 기존 캔버스 유지

## 프로젝트 구조

```text
.
├── README.md
├── Makefile
├── .gitignore
├── include/
│   └── canvas.h
├── src/
│   ├── canvas.c
│   └── main.c
├── tests/
│   └── test_canvas.c
├── examples/
│   └── sample.tdb
└── outputs/
    └── demo_output.txt
```

## 기술 스택

- C11
- Standard C Library
- Make
- GCC/Clang 경고 옵션: `-Wall -Wextra -Wpedantic -Werror`

## 한계

- 캔버스 크기는 20×70으로 고정되어 있습니다.
- 한 셀에는 한 바이트의 가시 ASCII 문자만 저장하므로 한글·이모지 등 멀티바이트 문자는 지원하지 않습니다.
- 실행 취소, 자유선, 색상과 같은 고급 편집 기능은 포함하지 않았습니다.
- 저장 형식은 교육용으로 설계했으며 암호화나 동시 편집을 지원하지 않습니다.
