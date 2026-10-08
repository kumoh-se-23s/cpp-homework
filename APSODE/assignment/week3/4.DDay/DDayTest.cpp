//
// Created by leegu on 26. 10. 8..
//
#include <gtest/gtest.h>
#include "DDay.h"

namespace {
    void expect_day(const Day &actual, const int year, const int month, const int day) {
        EXPECT_EQ(actual.get_year(), year);
        EXPECT_EQ(actual.get_month(), month);
        EXPECT_EQ(actual.get_day(), day);
    }
}

// ---------- 생성 ----------

TEST(DDayBasicTest, DefaultConstructorUsesDefaultDay) {
    DDay d;
    expect_day(d.calc_dday(), 2026, 10, 1);
}

TEST(DDayBasicTest, ConstructWithManualDay) {
    DDay d(Day(2026, 10, 8));
    expect_day(d.calc_dday(), 2026, 10, 8);
}

TEST(DDayBasicTest, ConstructWithLeapDay) {
    DDay d(Day(2024, 2, 29));
    expect_day(d.calc_dday(), 2024, 2, 29);
}

TEST(DDayBasicTest, ConstructDoesNotChangeSourceDay) {
    Day source(2026, 10, 8);
    DDay d(source);
    d.tomorrow();

    expect_day(source, 2026, 10, 8);
}

// ---------- set_dday ----------

TEST(DDaySetTest, ZeroKeepsDate) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(0);
    expect_day(d.calc_dday(), 2026, 10, 8);
}

TEST(DDaySetTest, PositiveWithinSameMonth) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(10);
    expect_day(d.calc_dday(), 2026, 10, 18);
}

TEST(DDaySetTest, PositiveCrossesMonth) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(30);
    expect_day(d.calc_dday(), 2026, 11, 7);
}

TEST(DDaySetTest, PositiveCrossesYear) {
    DDay d(Day(2026, 12, 25));
    d.set_dday(10);
    expect_day(d.calc_dday(), 2027, 1, 4);
}

TEST(DDaySetTest, PositiveSpansSeveralMonths) {
    DDay d(Day(2026, 1, 1));
    d.set_dday(100);
    expect_day(d.calc_dday(), 2026, 4, 11);
}

TEST(DDaySetTest, OneFullYear) {
    DDay d(Day(2026, 1, 1));
    d.set_dday(365);
    expect_day(d.calc_dday(), 2027, 1, 1);
}

TEST(DDaySetTest, NegativeMovesBackward) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(-8);
    expect_day(d.calc_dday(), 2026, 9, 30);
}

TEST(DDaySetTest, NegativeCrossesYear) {
    DDay d(Day(2026, 1, 1));
    d.set_dday(-1);
    expect_day(d.calc_dday(), 2025, 12, 31);
}

TEST(DDaySetTest, DefaultDayWithSet) {
    DDay d;
    d.set_dday(31);
    expect_day(d.calc_dday(), 2026, 11, 1);
}

TEST(DDaySetTest, SetOverwritesPreviousValue) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(10);
    d.set_dday(3);
    expect_day(d.calc_dday(), 2026, 10, 11);
}

TEST(DDaySetTest, SetBackToZeroRestoresBaseDate) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(50);
    d.set_dday(0);
    expect_day(d.calc_dday(), 2026, 10, 8);
}

TEST(DDaySetTest, LeapDayHandling) {
    DDay d(Day(2024, 2, 28));
    d.set_dday(1);
    expect_day(d.calc_dday(), 2024, 2, 29);
}

TEST(DDaySetTest, CommonYearSkipsFeb29) {
    DDay d(Day(2026, 2, 28));
    d.set_dday(1);
    expect_day(d.calc_dday(), 2026, 3, 1);
}

// ---------- tomorrow ----------

TEST(DDayTomorrowTest, MovesOneDayForward) {
    DDay d(Day(2026, 10, 8));
    d.tomorrow();
    expect_day(d.calc_dday(), 2026, 10, 9);
}

TEST(DDayTomorrowTest, CrossesMonth) {
    DDay d(Day(2026, 10, 31));
    d.tomorrow();
    expect_day(d.calc_dday(), 2026, 11, 1);
}

TEST(DDayTomorrowTest, CrossesYear) {
    DDay d(Day(2026, 12, 31));
    d.tomorrow();
    expect_day(d.calc_dday(), 2027, 1, 1);
}

TEST(DDayTomorrowTest, ThroughLeapDay) {
    DDay d(Day(2024, 2, 28));
    d.tomorrow();
    expect_day(d.calc_dday(), 2024, 2, 29);
    d.tomorrow();
    expect_day(d.calc_dday(), 2024, 3, 1);
}

TEST(DDayTomorrowTest, RepeatedCallsAccumulate) {
    DDay d(Day(2026, 12, 25));
    for (int i = 0; i < 10; ++i) {
        d.tomorrow();
    }
    expect_day(d.calc_dday(), 2027, 1, 4);
}

TEST(DDayTomorrowTest, FullYearOfCalls) {
    DDay d(Day(2026, 1, 1));
    for (int i = 0; i < 365; ++i) {
        d.tomorrow();
    }
    expect_day(d.calc_dday(), 2027, 1, 1);
}

// ---------- yesterday ----------

TEST(DDayYesterdayTest, MovesOneDayBackward) {
    DDay d(Day(2026, 10, 8));
    d.yesterday();
    expect_day(d.calc_dday(), 2026, 10, 7);
}

TEST(DDayYesterdayTest, CrossesMonth) {
    DDay d(Day(2026, 11, 1));
    d.yesterday();
    expect_day(d.calc_dday(), 2026, 10, 31);
}

TEST(DDayYesterdayTest, CrossesYear) {
    DDay d(Day(2026, 1, 1));
    d.yesterday();
    expect_day(d.calc_dday(), 2025, 12, 31);
}

TEST(DDayYesterdayTest, MarchFirstInCommonYear) {
    DDay d(Day(2026, 3, 1));
    d.yesterday();
    expect_day(d.calc_dday(), 2026, 2, 28);
}

TEST(DDayYesterdayTest, MarchFirstInLeapYear) {
    DDay d(Day(2024, 3, 1));
    d.yesterday();
    expect_day(d.calc_dday(), 2024, 2, 29);
}

TEST(DDayYesterdayTest, RepeatedCallsAccumulate) {
    DDay d(Day(2027, 1, 4));
    for (int i = 0; i < 10; ++i) {
        d.yesterday();
    }
    expect_day(d.calc_dday(), 2026, 12, 25);
}

// ---------- tomorrow / yesterday / set_dday 조합 ----------

TEST(DDayComboTest, TomorrowThenYesterdayReturnsOriginal) {
    DDay d(Day(2026, 10, 8));
    d.tomorrow();
    d.yesterday();
    expect_day(d.calc_dday(), 2026, 10, 8);
}

TEST(DDayComboTest, YesterdayThenTomorrowReturnsOriginal) {
    DDay d(Day(2026, 3, 1));
    d.yesterday();
    d.tomorrow();
    expect_day(d.calc_dday(), 2026, 3, 1);
}

TEST(DDayComboTest, TomorrowAfterSetAddsOne) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(10);
    d.tomorrow();
    expect_day(d.calc_dday(), 2026, 10, 19);
}

TEST(DDayComboTest, YesterdayAfterSetSubtractsOne) {
    DDay d(Day(2026, 10, 8));
    d.set_dday(10);
    d.yesterday();
    expect_day(d.calc_dday(), 2026, 10, 17);
}

TEST(DDayComboTest, RoundTripAcrossLeapDay) {
    DDay d(Day(2024, 2, 28));
    for (int i = 0; i < 3; ++i) d.tomorrow();
    for (int i = 0; i < 3; ++i) d.yesterday();
    expect_day(d.calc_dday(), 2024, 2, 28);
}

// ---------- calc_dday 자체의 성질 ----------
TEST(DDayCalcTest, RepeatedCallsReturnSameResult) {
    // Day::operator+가 원본을 직접 변경하므로, 구현에 따라 호출할 때마다 값이 누적될 수 있음
    DDay d(Day(2026, 10, 8));
    d.set_dday(5);

    expect_day(d.calc_dday(), 2026, 10, 13);
    expect_day(d.calc_dday(), 2026, 10, 13);
    expect_day(d.calc_dday(), 2026, 10, 13);
}

TEST(DDayCalcTest, RepeatedCallsAfterTomorrowReturnSameResult) {
    DDay d(Day(2026, 10, 8));
    d.tomorrow();

    expect_day(d.calc_dday(), 2026, 10, 9);
    expect_day(d.calc_dday(), 2026, 10, 9);
}

// ---------- 복사 ----------

TEST(DDayCopyTest, CopyHasSameResult) {
    DDay original(Day(2026, 10, 8));
    original.set_dday(7);
    DDay copy = original;

    expect_day(copy.calc_dday(), 2026, 10, 15);
}

TEST(DDayCopyTest, CopyIsIndependent) {
    DDay original(Day(2026, 10, 8));
    DDay copy = original;
    copy.set_dday(30);
    copy.tomorrow();

    expect_day(original.calc_dday(), 2026, 10, 8);
    expect_day(copy.calc_dday(), 2026, 11, 8);
}