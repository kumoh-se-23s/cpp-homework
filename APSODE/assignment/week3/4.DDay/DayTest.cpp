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
    expect_day(Day(2026, 10, 8), 2026, 10, 8);
}

TEST(DayBasicTest, DefaultConstructorIsFixedDate) {
    expect_day(Day(), 2026, 10, 1);
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

TEST(DayBasicTest, ConstructorDoesNotNormalize) {
    // 생성자는 값을 그대로 저장하고 보정하지 않음
    expect_day(Day(2026, 1, 32), 2026, 1, 32);
}

// ---------- set()의 보정 ----------

TEST(DaySetNormalizeTest, DayOverflowMovesToNextMonth) {
    Day d;
    d.set(2026, 1, 32);
    expect_day(d, 2026, 2, 1);
}

TEST(DaySetNormalizeTest, Feb29InCommonYearMovesToMarch1) {
    Day d;
    d.set(2026, 2, 29);
    expect_day(d, 2026, 3, 1);
}

TEST(DaySetNormalizeTest, Feb29InLeapYearIsKept) {
    Day d;
    d.set(2024, 2, 29);
    expect_day(d, 2024, 2, 29);
}

TEST(DaySetNormalizeTest, DayOverflowCrossesYear) {
    Day d;
    d.set(2026, 12, 32);
    expect_day(d, 2027, 1, 1);
}

TEST(DaySetNormalizeTest, ZeroDayMovesToPreviousMonthEnd) {
    Day d;
    d.set(2026, 3, 0);
    expect_day(d, 2026, 2, 28);
}

TEST(DaySetNormalizeTest, ZeroDayInJanuaryMovesToPreviousYear) {
    Day d;
    d.set(2026, 1, 0);
    expect_day(d, 2025, 12, 31);
}

TEST(DaySetNormalizeTest, NegativeDayMovesBackSeveralMonths) {
    Day d;
    d.set(2026, 3, -30);
    // 3월 기준 -30일: 2월 분량(28) 더하면 -2, 1월 분량(31) 더하면 29 -> 2026-01-29
    expect_day(d, 2026, 1, 29);
}

TEST(DaySetNormalizeTest, ValidDateIsUnchanged) {
    Day d;
    d.set(2026, 10, 31);
    expect_day(d, 2026, 10, 31);
}

TEST(DaySetNormalizeTest, LargeOverflowSpansSeveralMonths) {
    Day d;
    d.set(2026, 1, 100);
    // 1월 31 + 2월 28 + 3월 31 = 90 -> 4월 10일
    expect_day(d, 2026, 4, 10);
}

// ---------- 덧셈 ----------

TEST(DayAddTest, AddZeroKeepsDate) {
    expect_day(Day(2026, 10, 8) + 0, 2026, 10, 8);
}

TEST(DayAddTest, AddWithinSameMonth) {
    expect_day(Day(2026, 10, 8) + 5, 2026, 10, 13);
}

TEST(DayAddTest, ReachesLastDayOfMonth) {
    expect_day(Day(2026, 10, 8) + 23, 2026, 10, 31);
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
    expect_day(Day(2026, 1, 31) + 60, 2026, 4, 1);
}

TEST(DayAddTest, AddSpansSeveralYears) {
    // 2026-01-01 + 1000일 = 2028-09-27 (2028년은 윤년)
    expect_day(Day(2026, 1, 1) + 1000, 2028, 9, 27);
}

TEST(DayAddTest, AddNegativeMovesBackward) {
    expect_day(Day(2026, 10, 8) + (-8), 2026, 9, 30);
}

TEST(DayAddTest, ReturnsSameValueAsMutatedObject) {
    // 현재 구현은 operator+가 원본을 기준으로 새로운 객체를 만든뒤 해당 객체를 수정하는 방식으로 구현됨
    Day d(2026, 10, 8);
    Day result = d + 10;
    expect_day(result, 2026, 10, 18);
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
    expect_day(Day(2100, 2, 28) + 1, 2100, 3, 1);
}

TEST(DayLeapYearTest, Year2000IsLeap) {
    expect_day(Day(2000, 2, 28) + 1, 2000, 2, 29);
}

TEST(DayLeapYearTest, Year1900IsNotLeap) {
    expect_day(Day(1900, 2, 28) + 1, 1900, 3, 1);
}

TEST(DayLeapYearTest, FullYearFromLeapDayLandsOnFeb28) {
    expect_day(Day(2024, 2, 29) + 365, 2025, 2, 28);
}

TEST(DayLeapYearTest, FullYearPlusOneFromLeapDayLandsOnMarch1) {
    expect_day(Day(2024, 2, 29) + 366, 2025, 3, 1);
}

// ---------- 뺄셈 ----------

TEST(DaySubTest, SubZeroKeepsDate) {
    expect_day(Day(2026, 10, 8) - 0, 2026, 10, 8);
}

TEST(DaySubTest, SubWithinSameMonth) {
    expect_day(Day(2026, 10, 8) - 5, 2026, 10, 3);
}

TEST(DaySubTest, ReachesFirstDayOfMonth) {
    expect_day(Day(2026, 10, 8) - 7, 2026, 10, 1);
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

TEST(DaySubTest, SubSpansSeveralYears) {
    expect_day(Day(2028, 9, 27) - 1000, 2026, 1, 1);
}

TEST(DaySubTest, SubNegativeMovesForward) {
    expect_day(Day(2026, 9, 30) - (-8), 2026, 10, 8);
}

TEST(DaySubTest, ReturnsSameValueAsMutatedObject) {
    // 현재 구현은 operator-가 원본을 기준으로 새로운 객체를 만든뒤 해당 객체를 수정하는 방식으로 구현됨
    Day d(2026, 10, 8);
    Day result = d - 3;
    expect_day(result, 2026, 10, 5);
    expect_day(d, 2026, 10, 8);
}

// ---------- 덧셈과 뺄셈의 관계 ----------

TEST(DayRoundTripTest, AddThenSubReturnsOriginal) {
    expect_day((Day(2026, 10, 8) + 100) - 100, 2026, 10, 8);
}

TEST(DayRoundTripTest, SubThenAddReturnsOriginal) {
    expect_day((Day(2026, 3, 15) - 400) + 400, 2026, 3, 15);
}

TEST(DayRoundTripTest, RoundTripAcrossLeapDay) {
    expect_day((Day(2024, 2, 28) + 3) - 3, 2024, 2, 28);
}

TEST(DayRoundTripTest, LargeRoundTrip) {
    expect_day((Day(2026, 10, 8) + 5000) - 5000, 2026, 10, 8);
}

TEST(DayRoundTripTest, EveryDayOfLeapYearRoundTrips) {
    for (int offset = 0; offset < 366; ++offset) {
        Day forward = Day(2024, 1, 1) + offset;
        Day back = forward - offset;
        EXPECT_EQ(back.get_year(), 2024) << "offset=" << offset;
        EXPECT_EQ(back.get_month(), 1) << "offset=" << offset;
        EXPECT_EQ(back.get_day(), 1) << "offset=" << offset;
    }
}

// ---------- ++ / -- (객체 자신을 변경) ----------

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

TEST(DayIncDecTest, IncrementInCommonFebruary) {
    Day d(2026, 2, 28);
    ++d;
    expect_day(d, 2026, 3, 1);
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

TEST(DayIncDecTest, DecrementToThirtyDayMonthEnd) {
    Day d(2026, 5, 1);
    --d;
    expect_day(d, 2026, 4, 30);
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

TEST(DayIncDecTest, RepeatedDecrementMatchesSub) {
    Day stepped(2027, 1, 4);
    for (int i = 0; i < 10; ++i) {
        --stepped;
    }
    expect_day(stepped, 2026, 12, 25);
}

TEST(DayIncDecTest, FullYearOfIncrementsMatchesAdd365) {
    Day stepped(2026, 1, 1);
    for (int i = 0; i < 365; ++i) {
        ++stepped;
    }
    expect_day(stepped, 2027, 1, 1);
}

TEST(DayIncDecTest, IncrementDoesNotAffectCopy) {
    Day original(2026, 10, 8);
    Day copy = original;
    ++copy;

    expect_day(original, 2026, 10, 8);
    expect_day(copy, 2026, 10, 9);
}

// ---------- 출력 (operator<<) ----------

TEST(DayOutputTest, PrintsWithSlashSeparator) {
    EXPECT_EQ(capture_output(Day(2026, 10, 8)), "2026/10/8");
}

TEST(DayOutputTest, DoesNotPadSingleDigits) {
    EXPECT_EQ(capture_output(Day(2026, 1, 5)), "2026/1/5");
}

TEST(DayOutputTest, PrintsDefaultDate) {
    EXPECT_EQ(capture_output(Day()), "2026/10/1");
}

TEST(DayOutputTest, PrintsAfterAddition) {
    EXPECT_EQ(capture_output(Day(2026, 12, 31) + 1), "2027/1/1");
}

TEST(DayOutputTest, PrintsAfterIncrement) {
    Day d(2024, 2, 28);
    ++d;
    EXPECT_EQ(capture_output(d), "2024/2/29");
}

TEST(DayOutputTest, DoesNotAppendNewline) {
    const std::string output = capture_output(Day(2026, 10, 8));
    EXPECT_EQ(output.find('\n'), std::string::npos);
}