-- 1. 월별 통산 가중 타율
SELECT
    month,
    COUNT(*) AS monthly_records,
    SUM(hits) AS total_hits,
    SUM(at_bats) AS total_at_bats,
    ROUND(1.0 * SUM(hits) / SUM(at_bats), 3) AS weighted_observed_avg,
    ROUND(AVG(adjusted_avg), 3) AS mean_adjusted_avg
FROM monthly_batting
GROUP BY month
ORDER BY month;

-- 2. 표본 크기 보정 폭이 큰 기록
SELECT
    year,
    month,
    at_bats,
    hits,
    ROUND(avg_calculated, 3) AS observed_avg,
    ROUND(adjusted_avg, 3) AS adjusted_avg,
    ROUND(adjustment, 3) AS adjustment,
    ROUND(reliability_weight, 3) AS reliability_weight
FROM monthly_batting
ORDER BY ABS(adjustment) DESC
LIMIT 10;

-- 3. 연도별 가중 타율과 평균 월별 OPS
SELECT
    year,
    SUM(hits) AS total_hits,
    SUM(at_bats) AS total_at_bats,
    ROUND(1.0 * SUM(hits) / SUM(at_bats), 3) AS weighted_avg,
    ROUND(AVG(on_base_plus_slugging), 3) AS mean_monthly_ops
FROM monthly_batting
GROUP BY year
ORDER BY year;

-- 4. 관측값 신뢰도 가중치가 0.5 미만인 기록
SELECT
    year, month, at_bats, hits,
    ROUND(avg_calculated, 3) AS observed_avg,
    ROUND(adjusted_avg, 3) AS adjusted_avg,
    ROUND(reliability_weight, 3) AS reliability_weight
FROM monthly_batting
WHERE reliability_weight < 0.5
ORDER BY reliability_weight, year, month;
