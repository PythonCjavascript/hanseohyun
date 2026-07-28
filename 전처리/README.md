# KBO 월별 타격 데이터 신뢰도 보정 및 SQLite 분석 파이프라인

## 프로젝트 개요

구자욱 선수의 2015–2025년 월별 타격 기록 77건을 정제하고, 타수가 적은 달의 타율이 과도하게 흔들리는 문제를 베타-이항 축소 추정으로 보정한 프로젝트입니다. 분석 결과는 CSV와 SQLite로 저장하고 SQL로 재조회했습니다.

## 문제 정의

월별 타율은 표본 크기가 서로 다릅니다. 예를 들어 `4타수 2안타(0.500)`와 `100타수 30안타(0.300)`는 관측 타율만 보면 전자가 높지만, 두 값의 신뢰도를 같다고 보기 어렵습니다. 이 프로젝트에서는 타수가 적을수록 커리어 가중 평균 쪽으로 더 크게 조정하는 방식으로 월별 타율을 해석했습니다.

## 수행 내용

1. 연도 결측값 보완 및 연도·월 기준 정렬
2. 자료형, 중복, 결측, `안타 ≤ 타수`, 타율 재계산 검증
3. 커리어 가중 타율을 사전 평균으로 사용한 베타-이항 축소 추정
4. 95% 신용구간과 표본 신뢰도 가중치 산출
5. 사전 강도 25·50·100에 대한 민감도 점검
6. 최종 데이터를 CSV와 SQLite로 적재
7. SQL을 이용한 월별 가중 타율, 연도별 성적, 보정 폭 상위 기록 조회

## 핵심 결과

- 77개 월별 기록의 합계는 1,656안타, 5,210타수이며 가중 커리어 타율은 약 `0.318`입니다.
- 2017년 3월 `4타수 2안타(0.500)`는 표본이 매우 작아 조정 타율이 약 `0.331`로 이동했습니다.
- 2025년 7월 `71타수 33안타(0.465)`는 더 많은 타수가 확보되어 조정 후에도 약 `0.404`를 유지했습니다.
- SQL 월별 요약에서는 `AVG(월별 타율)` 대신 `SUM(안타) / SUM(타수)`를 사용해 표본 크기를 반영했습니다.

![Observed vs adjusted batting average](outputs/figures/observed_vs_adjusted.png)

![Adjustment magnitude by at-bats](outputs/figures/adjustment_by_at_bats.png)

## 데이터

- 기간: 2015–2025년
- 단위: 선수·연도·월
- 원본 행 수: 77
- 원본 열 수: 29
- 원본 파일: `kjw.xlsx`
- 수집 출처: STATIZ 구자욱 선수 상황별 기록
- 제출 전 권장 사항: 실제 수집 페이지 주소와 수집일을 이 항목에 함께 기록

## 분석 방법

관측 타율을 `H / AB`, 커리어 가중 타율을 `p0`, 사전 강도를 `k=50`으로 두었습니다.

```text
adjusted_avg = (H + k × p0) / (AB + k)
```

타수가 많을수록 관측 타율의 비중이 커지고, 타수가 적을수록 커리어 평균의 비중이 커집니다. `k=50`은 결과를 설명하기 쉬운 기준값이며, 최적값이라고 주장하지 않습니다.

## 프로젝트 구조

```text
KBO_portfolio_revised/
├── README.md
├── KBO_Monthly_Analysis_clean.ipynb
├── kjw.xlsx
├── analysis_queries.sql
├── requirements.txt
├── .gitignore
└── outputs/
    ├── kbo_monthly_processed.csv
    ├── kbo_monthly.db
    └── figures/
        ├── observed_vs_adjusted.png
        └── adjustment_by_at_bats.png
```

## 실행 방법

```bash
pip install -r requirements.txt
jupyter notebook KBO_Monthly_Analysis_clean.ipynb
```

노트북에서 `Restart Kernel and Run All`을 실행하면 CSV, SQLite DB, 그래프가 `outputs/`에 생성됩니다.

## 기술 스택

- Python
- Pandas, NumPy, SciPy
- Matplotlib
- SQLite, SQL

## 한계

- 상대 투수, 구장, 부상, 타순 등 상황 변수를 반영하지 않았습니다.
- 월별 기록은 시간적으로 연관될 수 있습니다.
- 조정 타율은 표본 크기를 반영한 기술적 추정치이며, 미래 성적을 보장하는 예측값이 아닙니다.
