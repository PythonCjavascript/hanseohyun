# 한서현 | Data Analysis Portfolio

Python과 SQL로 데이터를 정제하고, 통계 모형의 가정과 검증 과정을 확인하며, 분석 결과를 재현 가능한 코드·표·그래프로 정리했습니다.

**Focus** · Data Analysis · Statistical Modeling · Data Quality · Reproducible Workflow

## Featured Data Projects

### 01. [KBO 월별 타격 데이터 표본 크기 보정 및 SQLite 파이프라인](./전처리/)

월별 타율을 그대로 비교할 때 소표본 기록이 과도하게 부각되는 문제를 다뤘습니다.

- 2015–2025년 구자욱 선수 월별 기록 **77건** 정제 및 품질 검증
- 전체 기간 가중 타율을 사전 평균으로 사용한 **베타-이항 축소 추정**
- 사전 강도 `25·50·100` 민감도 점검과 95% 신용구간 계산
- 전처리 결과를 **CSV·SQLite**로 저장하고 SQL로 월별·연도별 지표 재조회

**Tech** `Python` `Pandas` `NumPy` `SciPy` `SQLite` `SQL`

---

### 02. [KBO 팀 득점 추정: 공격 이벤트 분해와 연도별 교차검증](./회귀분석/)

홈런이 안타에 포함되는 기존 변수 구조를 정리하고, 보지 못한 연도에서도 성능이 유지되는지 검증했습니다.

- 2020–2024년 KBO **50개 팀-시즌** 분석
- 안타를 `1루타·2루타·3루타·홈런`으로 분해해 계수 해석 개선
- 평균 기준선·기존 5변수 OLS·이벤트 분해 OLS 비교
- **Leave-One-Year-Out 교차검증:** MAE `15.65`, RMSE `18.85`, Out-of-fold R² `0.941`
- VIF, 강건 표준오차, 잔차, 이분산성, 영향점 민감도 진단

**Tech** `Python` `Pandas` `Scikit-learn` `Statsmodels` `SciPy`

---

### 03. [MLB 피치클락 도입 전후 부상률 분석 및 인과 식별 진단](./인과추론/)

관찰자료에서 제도 도입 효과를 주장하기 전에 비교 가능성·표본 선택·공변량 측정 시점을 점검했습니다.

- 2022·2023년 전체 선수, 완전 사례, 동일 투수, Statcast ID 패널 결과 비교
- 위험도 차이·위험비·신뢰구간과 Fisher·McNemar 검정
- 기존 성향점수 모형의 공통지지와 공변량 균형 재검토
- 연도 분류 AUC `0.989`, 트리밍 후에도 모든 공변량이 `|SMD| < 0.1` 기준 미충족 확인
- 데이터가 전후 차이는 기술할 수 있지만 피치클락의 인과효과를 분리하기 어렵다는 결론 제시

**Tech** `Python` `NumPy` `SciPy` `Statsmodels` `Scikit-learn`

## Application & Programming Projects

### 04. [Winter Fit Finder — 조건 기반 겨울 코디 추천 웹서비스](./웹디자인/)

기온·일정·스타일·보온 우선 여부를 최대 12점으로 평가해 코디를 정렬합니다. 즐겨찾기·개인 메모·최근 입력·화면 설정을 `LocalStorage`에 저장하며, 반응형 레이아웃과 키보드 접근성을 적용했습니다.

**Tech** `HTML5` `CSS3` `Vanilla JavaScript` `LocalStorage`

### 05. [Text Canvas CLI — C11 텍스트 그림판](./C언어/)

20×70 문자 캔버스에 좌표 기반 드로잉과 사각형 생성을 구현했습니다. 표준 C11로 리팩터링하고, 안전한 입력 처리·버전이 있는 저장 형식·손상 파일 검증·자동 테스트를 추가했습니다.

**Tech** `C11` `Standard C Library` `Make` `GCC/Clang`

## Core Skills

| 영역 | 기술 |
|---|---|
| Data Processing | Python, Pandas, NumPy, 데이터 품질 검증, 파생변수 설계 |
| Statistics | 회귀분석, 교차검증, 강건 표준오차, 잔차·영향점 진단, 신뢰구간 |
| Causal Reasoning | 비교군 적절성, 공통지지, 공변량 균형, 표본 민감도, 식별 한계 점검 |
| Database | SQL, SQLite, 데이터 적재와 집계 쿼리 |
| Development | HTML, CSS, JavaScript, C11, Git/GitHub |

## Repository Guide

```text
hanseohyun/
├── 전처리/      # KBO 월별 기록 보정·SQLite 파이프라인
├── 회귀분석/    # KBO 팀 득점 추정·연도별 교차검증
├── 인과추론/    # MLB 피치클락 전후 분석·식별 진단
├── 웹디자인/    # Winter Fit Finder
└── C언어/       # Text Canvas CLI
```

각 프로젝트 폴더에는 분석 또는 실행 코드, 원본·가공 데이터, 결과 파일, 시각화, 실행 방법과 한계를 정리한 README가 포함되어 있습니다.
