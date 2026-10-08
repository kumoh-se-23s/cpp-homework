//
// Created by leegu on 26. 10. 7..
//
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "Day.h"

namespace {
    void expect_day(const Day &actual, const int year, const int month, const int day) {
        EXPECT_EQ(actual.get_year(), year);
        EXPECT_EQ(actual.get_month(), month);
        EXPECT_EQ(actual.get_day(), day);
    }

    std::string capture_output(const Day &day) {
        testing::internal::CaptureStdout();
        std::cout << day;
        std::cout.flush();
        return testing::internal::GetCapturedStdout();
    }
}

// ---------- 생성 / get / set ----------

TEST(DayBasicTest, ConstructWithYearMonthDay) {
    Day d(2026, 10, 8);
    expect_day(d, 2026, 10, 8);
}

TEST(DayBasicTest, DefaultConstructorGivesValidDate) {
    Day d;
    EXPECT_GE(d.get_month(), 1);
    EXPECT_LE(d.get_month(), 12);
    EXPECT_GE(d.get_day(), 1);
    EXPECT_LE(d.get_day(), 31);
}

TEST(DayBasicTest, SetChangesAllFields) {
    Day d(2026, 10, 8);
    d.set(2024, 2, 29);
    expect_day(d, 2024, 2, 29);
}

TEST(DayBasicTest, CopyIsIndependent) {
    Day original(2026, 10, 8);
    Day copy = original;
    copy.set(2000, 1, 1);

    expect_day(original, 2026, 10, 8);
    expect_day(copy, 2000, 1, 1);
}

// ---------- 덧셈 ----------

TEST(DayAddTest, AddZeroKeepsDate) {
    expect_day(Day(2026, 10, 8) + 0, 2026, 10, 8);
}

TEST(DayAddTest, AddWithinSameMonth) {
    expect_day(Day(2026, 10, 8) + 5, 2026, 10, 13);
}

TEST(DayAddTest, CrossesMonthBoundary) {
    expect_day(Day(2026, 10, 31) + 1, 2026, 11, 1);
}

TEST(DayAddTest, CrossesMonthBoundaryInThirtyDayMonth) {
    expect_day(Day(2026, 4, 30) + 1, 2026, 5, 1);
}

TEST(DayAddTest, CrossesYearBoundary) {
    expect_day(Day(2026, 12, 31) + 1, 2027, 1, 1);
}

TEST(DayAddTest, AddOneFullYear) {
    expect_day(Day(2026, 1, 1) + 365, 2027, 1, 1);
}

TEST(DayAddTest, AddOneFullLeapYear) {
    expect_day(Day(2024, 1, 1) + 366, 2025, 1, 1);
}

TEST(DayAddTest, AddSpansSeveralMonths) {
    // 2026-01-31 + 60일 = 2026-04-01
    expect_day(Day(2026, 1, 31) + 60, 2026, 4, 1);
}

TEST(DayAddTest, DoesNotModifyOriginal) {
    Day d(2026, 10, 8);
    Day result = d + 10;
    (void) result;
    expect_day(d, 2026, 10, 8);
}

// ---------- 윤년 ----------

TEST(DayLeapYearTest, LeapYearHasFeb29) {
    expect_day(Day(2024, 2, 28) + 1, 2024, 2, 29);
}

TEST(DayLeapYearTest, LeapYearFeb29ToMarch1) {
    expect_day(Day(2024, 2, 29) + 1, 2024, 3, 1);
}

TEST(DayLeapYearTest, CommonYearSkipsFeb29) {
    expect_day(Day(2023, 2, 28) + 1, 2023, 3, 1);
}

TEST(DayLeapYearTest, CenturyYearIsNotLeap) {
    // 2100년은 400으로 나누어떨어지지 않으므로 평년
    expect_day(Day(2100, 2, 28) + 1, 2100, 3, 1);
}

TEST(DayLeapYearTest, Year2000IsLeap) {
    // 2000년은 400으로 나누어떨어지므로 윤년
    expect_day(Day(2000, 2, 28) + 1, 2000, 2, 29);
}

// ---------- 뺄셈 ----------

TEST(DaySubTest, SubZeroKeepsDate) {
    expect_day(Day(2026, 10, 8) - 0, 2026, 10, 8);
}

TEST(DaySubTest, SubWithinSameMonth) {
    expect_day(Day(2026, 10, 8) - 5, 2026, 10, 3);
}

TEST(DaySubTest, CrossesMonthBoundary) {
    expect_day(Day(2026, 11, 1) - 1, 2026, 10, 31);
}

TEST(DaySubTest, CrossesYearBoundary) {
    expect_day(Day(2026, 1, 1) - 1, 2025, 12, 31);
}

TEST(DaySubTest, MarchFirstToFebInCommonYear) {
    expect_day(Day(2026, 3, 1) - 1, 2026, 2, 28);
}

TEST(DaySubTest, MarchFirstToFebInLeapYear) {
    expect_day(Day(2024, 3, 1) - 1, 2024, 2, 29);
}

TEST(DaySubTest, SubOneFullYear) {
    expect_day(Day(2027, 1, 1) - 365, 2026, 1, 1);
}

TEST(DaySubTest, DoesNotModifyOriginal) {
    Day d(2026, 10, 8);
    Day result = d - 10;
    (void) result;
    expect_day(d, 2026, 10, 8);
}

// ---------- 덧셈과 뺄셈의 관계 ----------

TEST(DayRoundTripTest, AddThenSubReturnsOriginal) {
    Day d(2026, 10, 8);
    expect_day((d + 100) - 100, 2026, 10, 8);
}

TEST(DayRoundTripTest, SubThenAddReturnsOriginal) {
    Day d(2026, 3, 15);
    expect_day((d - 400) + 400, 2026, 3, 15);
}

TEST(DayRoundTripTest, RoundTripAcrossLeapDay) {
    Day d(2024, 2, 28);
    expect_day((d + 3) - 3, 2024, 2, 28);
}

// ---------- 전위 ++ / -- (객체 자신을 변경) ----------

TEST(DayIncDecTest, IncrementMovesToNextDay) {
    Day d(2026, 10, 8);
    ++d;
    expect_day(d, 2026, 10, 9);
}

TEST(DayIncDecTest, IncrementCrossesMonth) {
    Day d(2026, 10, 31);
    ++d;
    expect_day(d, 2026, 11, 1);
}

TEST(DayIncDecTest, IncrementCrossesYear) {
    Day d(2026, 12, 31);
    ++d;
    expect_day(d, 2027, 1, 1);
}

TEST(DayIncDecTest, IncrementOnLeapDay) {
    Day d(2024, 2, 28);
    ++d;
    expect_day(d, 2024, 2, 29);
    ++d;
    expect_day(d, 2024, 3, 1);
}

TEST(DayIncDecTest, DecrementMovesToPreviousDay) {
    Day d(2026, 10, 8);
    --d;
    expect_day(d, 2026, 10, 7);
}

TEST(DayIncDecTest, DecrementCrossesMonth) {
    Day d(2026, 11, 1);
    --d;
    expect_day(d, 2026, 10, 31);
}

TEST(DayIncDecTest, DecrementCrossesYear) {
    Day d(2026, 1, 1);
    --d;
    expect_day(d, 2025, 12, 31);
}

TEST(DayIncDecTest, DecrementOnLeapDay) {
    Day d(2024, 3, 1);
    --d;
    expect_day(d, 2024, 2, 29);
    --d;
    expect_day(d, 2024, 2, 28);
}

TEST(DayIncDecTest, IncrementThenDecrementReturnsOriginal) {
    Day d(2026, 10, 8);
    ++d;
    --d;
    expect_day(d, 2026, 10, 8);
}

TEST(DayIncDecTest, RepeatedIncrementMatchesAdd) {
    Day stepped(2026, 12, 25);
    for (int i = 0; i < 10; ++i) {
        ++stepped;
    }
    expect_day(stepped, 2027, 1, 4);
    expect_day(Day(2026, 12, 25) + 10, 2027, 1, 4);
}

TEST(DayIncDecTest, IncrementDoesNotAffectCopy) {
    Day original(2026, 10, 8);
    Day copy = original;
    ++copy;

    expect_day(original, 2026, 10, 8);
    expect_day(copy, 2026, 10, 9);
}

// ---------- 출력 (operator<<) ----------

TEST(DayOutputTest, PrintsInIsoFormat) {
    EXPECT_EQ(capture_output(Day(2026, 10, 8)), "2026-10-08");
}

TEST(DayOutputTest, PadsSingleDigitMonthAndDay) {
    EXPECT_EQ(capture_output(Day(2026, 1, 5)), "2026-01-05");
}

TEST(DayOutputTest, PrintsAfterAddition) {
    EXPECT_EQ(capture_output(Day(2026, 12, 31) + 1), "2027-01-01");
}