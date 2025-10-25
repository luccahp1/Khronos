# Implementation Notes

- Julian Day helpers convert between HMS and fractional day with noon-based semantics to respect the project definition that JDN `.0` equals 12:00:00.
- Gregorian and Julian conversions follow the standard astronomical algorithms; day rollovers use the shared `jd_to_hms` helper to avoid rounding drift.
- Islamic conversions implement the tabular civil calendar: months alternate 30/29 days with leap days applied to Dhu I-Hijja; month arithmetic clamps to each month's length.
