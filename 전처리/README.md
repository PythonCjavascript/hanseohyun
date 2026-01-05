
# ⚾ KBO 타자 월별 성적 보정 및 시뮬레이션 (with SQL Pipeline)

### (KBO Hitter Monthly Performance Analysis: From Preprocessing to DB Pipeline)

## 📌 프로젝트 소개 (Overview)

이 프로젝트는 **'데이터 전처리(Data Preprocessing)'**의 중요성을 증명하고, **Python 분석 데이터를 DB로 연결하는 엔지니어링 역량**을 보여주기 위해 기획되었습니다.

삼성 라이온즈의 대표 타자 **구자욱 선수**의 기록을 분석 대상으로 삼았으며, 타수(AB)가 적은 달에 발생하는 **'평균의 함정(Small Sample Size Problem)'**을 통계적으로 보정하는 시뮬레이션을 수행했습니다. 또한, 전처리된 데이터를 **SQL Database에 적재하고 쿼리로 추출하는 파이프라인**을 직접 구축했습니다.

> *"전처리는 기계가 데이터를 빠르게 해석할 수 있도록 하는 첫 단계이며, 전체 분석 과정의 80%를 차지할 정도로 중요하다."*

## 🎯 문제 의식 (Problem Statement)

### "10타수 4안타(0.400)가 100타수 30안타(0.300)보다 정말 더 잘한 것일까?"

야구 데이터, 특히 월별 성적을 볼 때 우리는 종종 **착시 현상**을 겪습니다.

* **현상:** 특정 달에 타율이 4할이 넘는 맹타를 휘두르다가, 어떤 달은 1할대로 추락하는 등 기복이 심해 보임.
* **원인:** 부상 복귀, 시즌 초반 등 **타석 수가 비정상적으로 적은 달**의 기록이 전체 평균을 왜곡함.
* **해결책:** 표본(타수)이 부족한 달의 데이터를 단순히 제외하는 것이 아니라, **'만약 정상적으로 타석에 들어섰다면?'**이라는 통계적 가정하에 데이터를 보정하고 시뮬레이션합니다.

## 🛠 분석 방법론 (Methodology)

### 1. 데이터 수집 및 정제 (Data Cleaning)

* **Data Source:** KBO 공식 기록 (`구자욱 기록.xlsx`)
* **Preprocessing:**
* 누락된 연도(`Year`) 데이터를 `ffill()` (Forward Fill) 방식으로 채워 시계열 연속성 확보.
* 분석에 불필요한 인덱스 컬럼 제거 및 재정렬.



### 2. 타수 보정 알고리즘 (Adjustment Logic)

통계적 왜곡을 줄이기 위해 다음과 같은 보정 로직을 설계했습니다.

1. **월별 평균 타수 산출:** 커리어 동안 각 월마다 평균적으로 소화한 타수를 계산.
2. **조건부 보정 (`modified_AB`):** 실제 타수가 평균보다 적을 경우, 평균 타수를 기준으로 데이터를 재조정.

### 3. 성적 시뮬레이션 (Simulation)

* 보정된 타수를 기반으로 **기대 타율(Simulated AVG)**과 **기대 안타 수**를 재산출.
* 이를 통해 "운 좋게 타율이 높았던 달"의 거품을 걷어내고 객관적인 지표 생성.

### 4. Data Engineering Pipeline (SQL Integration) ✨ *Updated*

* **DB 적재:** Python(Pandas)으로 전처리 완료된 데이터를 `sqlite3`를 활용해 **Database Table(`batter_stats`)로 적재**.
* **SQL Analysis:** 실제 현업 환경을 고려하여, Python 코드가 아닌 **SQL 쿼리(SELECT, GROUP BY, AVG 등)**를 직접 작성해 월별 주요 지표(AVG, OPS)를 추출 및 검증.

## 📂 파일 구조 (File Structure)

| 파일명 | 설명 |
| --- | --- |
| `KBO_Monthly_Analysis.ipynb` | **메인 분석 코드.** 전처리, 시뮬레이션 로직 및 **SQL 파이프라인(DB 적재/쿼리)** 구현 |
| `Project_Report.pdf` | **프로젝트 보고서.** 분석 배경, 문제 정의(평균의 함정), 해결 방안 상세 기술 |
| `Theory_Preprocessing.pdf` | **이론 배경.** 데이터 전처리의 중요성, 주요 데이터 문제 유형 정리 |
| `구자욱 기록.xlsx` | 분석에 사용된 Raw Data 파일 |

## 💻 사용 기술 (Tech Stack)

* **Language:** Python 3.x, **SQL**
* **Libraries:**
* **Data Analysis:** `Pandas`, `NumPy`
* **Database:** `sqlite3` (Embedded DB)
* **Visualization:** `Seaborn`, `Matplotlib`



## 📊 결과 요약 (Insights)

1. **통계적 보정:** 타수가 극단적으로 적었던 달의 '뻥튀기된 타율'을 보정하여 선수의 실질적인 컨디션 사이클을 확인했습니다.
2. **지속 가능성 예측:** 단순 기록 너머의 '지속 가능한 성적'을 예측하는 새로운 지표를 제시했습니다.
3. **엔지니어링 역량 확보:** 단순 분석을 넘어, **[Raw Data → Python 전처리 → DB 적재 → SQL 추출]**로 이어지는 데이터 처리의 전체 흐름(End-to-End)을 구현했습니다.
