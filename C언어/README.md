# 🎨 C-based Interactive Text Drawing Board (텍스트 기반 그림판)
> **Note**: C언어의 포인터, 파일 입출력(File I/O), 메모리 구조에 대한 이해를 바탕으로 구현한 텍스트 드로잉 및 데이터 영구 저장 시스템입니다.

## 1. Project Overview (프로젝트 개요)
이 프로젝트는 20x70 크기의 2차원 배열을 캔버스로 활용하여, 사용자가 원하는 위치에 문자를 입력해 그림을 그리고 이를 파일로 저장하거나 불러올 수 있는 **CLI(Command-Line Interface) 기반의 그림판**입니다.

단순한 입출력을 넘어, 데이터를 **파일 시스템에 영구적으로 보존(Persistence)**하고 관리하는 로직을 로우 레벨(Low-level)에서 직접 구현하는 데 초점을 맞췄습니다.

* **Type:** Academic Project (C언어 기말 과제)
* **Role:** 1인 개발 (기획, 구현, 보고서 작성)
* **Tech Stack:** C Language (Standard Library)

## 2. Key Features (핵심 기능)
### A. Interactive Drawing (좌표 기반 드로잉)
사용자로부터 행(Row)과 열(Col), 그리고 출력할 문자(Char)를 입력받아 2차원 배열(`board[20][70]`)에 매핑합니다.
* **그리기(Draw):** 특정 좌표에 문자 입력 및 시각화
* **지우기(Erase):** 특정 좌표의 데이터를 초기화 문자(`_`)로 변경
* **초기화(Clear):** `system("cls")`와 배열 초기화 함수를 연동하여 캔버스 리셋

### B. File Persistence System (데이터 영구 저장)
메모리(RAM)에 존재하는 드로잉 데이터를 보조기억장치(File)로 옮겨 영구적으로 저장하고 복원합니다.
* **Save:** `fwrite`를 사용하여 2차원 배열의 데이터를 파일 스트림으로 출력
* **Load:** `fread`를 사용하여 저장된 파일의 바이너리/텍스트 데이터를 메모리로 적재
* **Duplicate Check:** `_access` 함수를 활용해 파일 중복 여부를 감지하고 덮어쓰기(Overwrite) 방지 로직 구현

### C. Exception Handling (예외 처리)
사용자의 잘못된 입력으로 인한 프로그램 비정상 종료를 방지하기 위해 다양한 방어 코드를 적용했습니다.
* 좌표 범위(20x70) 이탈 시 에러 메시지 출력 및 재입력 유도
* 파일 입출력 실패(NULL 포인터 반환) 시 예외 처리

## 3. Implementation Details (구현 내용)
코드의 재사용성과 모듈화를 위해 기능별로 함수를 분리하여 구현했습니다.

| 함수명 | 기능 설명 | 주요 기술 |
|---|---|---|
| `saveboard()` | 현재 캔버스 상태를 파일로 저장 | `fopen`, `fwrite`, `_access` |
| `loadboard()` | 저장된 파일을 읽어 캔버스에 복원 | `fopen`, `fread`, `fgetc` |
| `drawcharacter()` | 좌표 유효성 검사 후 문자 입력 | 2D Array Indexing |
| `displayboard()` | 이중 반복문을 통한 렌더링 | Nested Loop |

## 4. Retrospect (배운 점)
* **Data Structure:** 2차원 배열을 통해 화면 좌표계(Coordinate System)를 모델링하는 방법을 익혔습니다.
* **File I/O:** `fwrite`, `fread` 등 표준 입출력 함수를 사용하여 메모리와 파일 시스템 간의 데이터 흐름을 직접 제어해 보았습니다.
* **Buffer & Memory:** 파일 처리 과정에서 버퍼(Buffer)의 개념과 파일 포인터의 동작 원리를 이해하게 되었습니다.
