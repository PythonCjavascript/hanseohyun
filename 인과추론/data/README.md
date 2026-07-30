# Data files

| Portfolio filename | Original uploaded filename | Main fields |
|---|---|---|
| `2022_pitch_tempo.csv` | `2022 투구 템포-1.csv` | player name, pitch tempo, pitch count, injury label |
| `2023_pitch_tempo.csv` | `2023 투구 템포-1.csv` | player name, pitch tempo, pitch count, injury label |
| `2022_pitch_velocity.csv` | `2022 투구 평속.csv` | Statcast player ID and average velocity by pitch type |
| `2023_pitch_velocity.csv` | `2023 투구 평속.csv` | Statcast player ID and average velocity by pitch type |
| `2022_merged_data.csv` | `2022_merged_data (1).csv` | tempo and velocity merged by player |
| `2023_merged_data.csv` | `2023_merged_data.csv` | tempo and velocity merged by player |

## Important provenance note

The pitch-tempo and velocity columns are consistent with MLB Baseball Savant / Statcast exports. The notebook cross-checks every supplied merged row against the raw velocity export by Statcast pitcher ID; all merged rows matched and the maximum difference in the selected primary-velocity value was 0.0 mph.

The binary injury fields (`arm_injury_2022`, `Inj_23`) are already present in the supplied files, but the original injury list, inclusion rule, injury date, and source URL were not included. The export settings and minimum-pitch thresholds used to create the two annual tempo samples were also not documented. The revised project therefore treats the injury labels and annual samples as supplied data and does not claim they have been independently validated or sampled under identical eligibility rules.

Official references:

- MLB pitch timer rule: https://www.mlb.com/glossary/rules/pitch-timer
- Baseball Savant Statcast search: https://baseballsavant.mlb.com/statcast_search
- Baseball Savant pitch-tempo definition: https://baseballsavant.mlb.com/leaderboard/pitch-tempo
