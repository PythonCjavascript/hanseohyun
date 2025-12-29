# [Archived] Interactive Fashion Recommendation Service
> **Note**: This project is archived for portfolio purposes. It demonstrates early frontend development skills and understanding of data persistence logic.

## 1. Project Overview (프로젝트 개요)
사용자의 환경(계절)과 니즈에 맞춰 패션 아이템을 제안하고, 개인 메모를 기록할 수 있는 인터랙티브 웹 서비스입니다.
단순한 정보 전달을 넘어, 사용자가 직접 웹페이지의 스타일(폰트, 색상, 레이아웃)을 제어하고 데이터를 브라우저에 저장하는 기능을 구현했습니다.

* **Type:** Individual Project (웹디자인 기말 과제)
* **Role:** Full Implementation (Planning, Design, Coding, Documentation)
* **Tech Stack:** HTML5, CSS3, JavaScript (Vanilla)

## 2. Key Features (핵심 기능)
### A. Dynamic UI Control (동적 UI 제어)
사용자가 직접 대시보드 형태의 컨트롤 패널을 통해 웹페이지의 시각적 요소를 실시간으로 변경할 수 있도록 구현했습니다. DOM 조작과 이벤트 핸들링에 대한 이해를 바탕으로 제작되었습니다.
* **텍스트 스타일링:** 제목 및 본문의 색상(Color), 크기(Size), 정렬(Align), 폰트(Font-family) 변경
* **스타일 토글:** Bold, Italic 등의 속성을 Checkbox 입력을 통해 즉각 반영

### B. Data Persistence with LocalStorage (데이터 저장)
사용자가 입력한 메모 데이터를 서버 없이 브라우저 내부에 영구 저장하고 불러오는 기능을 구현했습니다.
* **CRUD Logic:** `localStorage.setItem`과 `getItem` 메서드를 활용하여 데이터의 생성(Create) 및 조회(Read) 기능 구현
* **State Management:** 페이지를 새로고침(Reload)하더라도 작성한 메모가 유지되도록 상태 관리 로직 적용

### C. Rule-based Recommendation (규칙 기반 추천)
계절별(겨울) 기온과 트렌드에 적합한 아이템 조합을 제안합니다.
* **Items:** 롱코트&스웨터, 무스탕&조거팬츠, 니트&롱스커트 등 5가지 카테고리 분류
* **Layout:** 이미지와 텍스트가 조화된 카드(Card) 형태의 레이아웃 구성

## 3. Project Gallery

## 4. Retrospect (회고: From Web Design to Data Analysis)
이 프로젝트는 **'데이터가 사용자에게 시각화되어 전달되는 과정'**을 이해하는 데 큰 도움을 주었습니다.

* **Logic Implementation:** 당시에는 JavaScript의 조건문(if-else)을 활용해 규칙 기반(Rule-based)으로 추천을 구현했으나, 현재는 통계학 전공 지식을 살려 **사용자 행동 로그 기반의 협업 필터링(Collaborative Filtering) 모델**로 발전시키는 방법에 대해 고민하고 있습니다.
* **Data Handling:** `localStorage`를 직접 다뤄보며 데이터의 저장 구조(Key-Value)와 클라이언트 사이드 데이터 관리의 중요성을 경험했습니다.
* **Documentation:** 코드의 재사용성과 유지보수를 위해 각 함수별 기능 명세서(Function Specification)를 상세히 작성하는 습관을 길렀습니다. (상세 내용은 `WebDesign_report.pdf` 참조)
