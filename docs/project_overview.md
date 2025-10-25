# INFO-3140 Project "Calendar" Overview

## Course and Schedule
- **Course:** INFO-3140 Object-Oriented Analysis and Modeling (Fall 2025)
- **Project:** Calendar (Project #1)
- **Weight:** 15% of course grade
- **Assigned:** Wednesday, September 24, 2025
- **Due:** Thursday, October 23, 2025

## Core Objectives
1. Extend the in-class Gregorian calendar implementation to support time-of-day (hours, minutes, seconds) using Julian Day Numbers (JDN) where `.0` represents noon (12:00:00).
2. Pass all provided Boost unit tests as features are completed, progressively enabling their corresponding phases in `ut_Khronos.hpp`.
3. Implement parallel calendar systems—Julian, Islamic, Hebrew, and Vulcan—with equivalent functionality to the Gregorian implementation.

## Time Representation Requirements
- Internal time is stored as the fractional portion of a JDN.
- Hours range 0–23, minutes 0–59, seconds 0–59.
- JDN values ending in `.0` correspond to 12:00:00 (noon) on that day.

## Class Deliverables
### `Jd`
- Constructors: default (current time, accurate to one second) and literal (real-number JDN).
- Provide all relational operators (`<=>`, `==`, `!=`, `<`, `<=`, `>`, `>=`).
- Support addition/subtraction with tagged durations: seconds, minutes, hours, days, weeks, orbital years.
- Support difference between two `Jd` values returning elapsed time (unit validated by tests).

### `Gregorian`
- Constructors: default (current time), date-only (Y/M/D), date-time (Y/M/D + time).
- Arithmetic: add/subtract seconds, minutes, hours, days, weeks; add whole months and years with leap-year and end-of-month clamping rules.
- Formatting: `to_string()` returns weekday, month name, day, year with "CE", and 12-hour time (`am`/`pm` lowercase).
- Suggested helpers: `is_gregorian_leap_year()`, `days_in_month()`, `jd_to_hms()`, `hms_to_days()`, `month_name()`.

### `Julian`
- Mirror the Gregorian surface (constructors, arithmetic, formatting) adjusted for Julian calendar rules.

### `Islamic`, `Hebrew`, `Vulcan`
- Implement two-way conversions between each calendar and `Jd`.
- Provide calendar classes with the same API surface and behaviors as Gregorian.
- Ensure tests `ut_Khronos_13`–`ut_Khronos_21` pass for their respective phases.

## Testing Strategy
- Only modify `ut_Khronos.hpp` among test files to toggle phase booleans.
- Enable phases progressively after implementing corresponding features.
- Passing tests is required but must reflect correct behavior beyond mere test conformance.

## Submission Requirements
- Submit the Visual Studio project directory (without `Debug/` or `Release/`) as a `.zip` or `.7z`.
- Use the provided Khronos 2022 solution with the Visual C++ 2022 toolset.
- Ensure clean rebuilds without warnings and unmodified test cases.

## Grading Notes
- Environment deductions apply for altering tests, using incorrect toolsets, or generating build warnings.
- Project phases (1–119) collectively contribute 100% of the project grade, with weights defined in the provided rubric CSV.

## Implementation Tips
- Start with early phases disabled except the first, enabling them as features become available.
- Follow the order of the test phases for steady progress.
