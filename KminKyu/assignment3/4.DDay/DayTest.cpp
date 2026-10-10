#include <gtest/gtest.h>
#include <sstream>
#include "Day.h"

namespace {
    void expectDay(const Day &actual, int year, int month, int day) {
        EXPECT_EQ(actual.getYear(), year);
        EXPECT_EQ(actual.getMonth(), month);
        EXPECT_EQ(actual.getDay(), day);
    }

    void expectSame(const Day &actual, const Day &expected) {
        expectDay(actual, expected.getYear(), expected.getMonth(), expected.getDay());
    }

    std::string str(const Day &d) {
        std::ostringstream oss;
        oss << d;
        return oss.str();
    }
}

// ---------- 생성자 ----------

TEST(DayConstructorTest, DefaultIs20261001) {
    expectDay(Day(), 2026, 10, 1);
}

TEST(DayConstructorTest, ValidDateIsStored) {
    expectDay(Day(2024, 2, 29), 2024, 2, 29);
}

TEST(DayConstructorTest, InvalidDateFallsBackToDefault) {
    expectDay(Day(2026, 2, 29), 2026, 10, 1);
    expectDay(Day(2026, 13, 1), 2026, 10, 1);
    expectDay(Day(2026, 0, 1), 2026, 10, 1);
    expectDay(Day(2026, 1, 0), 2026, 10, 1);
    expectDay(Day(2026, 4, 31), 2026, 10, 1);
}

// ---------- isCorrectDate ----------

TEST(DayIsCorrectDateTest, MonthRange) {
    EXPECT_FALSE(Day::isCorrectDate(2026, 0, 1));
    EXPECT_TRUE(Day::isCorrectDate(2026, 1, 1));
    EXPECT_TRUE(Day::isCorrectDate(2026, 12, 31));
    EXPECT_FALSE(Day::isCorrectDate(2026, 13, 1));
    EXPECT_FALSE(Day::isCorrectDate(2026, -1, 1));
}

TEST(DayIsCorrectDateTest, DayRangePerMonth) {
    const int maxDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int m = 1; m <= 12; ++m) {
        SCOPED_TRACE(m);
        EXPECT_FALSE(Day::isCorrectDate(2026, m, 0));
        EXPECT_TRUE(Day::isCorrectDate(2026, m, 1));
        EXPECT_TRUE(Day::isCorrectDate(2026, m, maxDays[m]));
        EXPECT_FALSE(Day::isCorrectDate(2026, m, maxDays[m] + 1));
    }
}

TEST(DayIsCorrectDateTest, LeapYearRules) {
    EXPECT_TRUE(Day::isCorrectDate(2024, 2, 29));   // 4의 배수
    EXPECT_FALSE(Day::isCorrectDate(2026, 2, 29));  // 평년
    EXPECT_FALSE(Day::isCorrectDate(1900, 2, 29));  // 100의 배수
    EXPECT_FALSE(Day::isCorrectDate(2100, 2, 29));
    EXPECT_TRUE(Day::isCorrectDate(2000, 2, 29));   // 400의 배수
    EXPECT_TRUE(Day::isCorrectDate(0, 2, 29));      // 0년
}

// ---------- set / getter ----------

TEST(DaySetTest, SetChangesAllFields) {
    Day d;
    d.set(2030, 5, 17);
    expectDay(d, 2030, 5, 17);
}

// ---------- operator+ ----------

TEST(DayPlusTest, ZeroReturnsSameDate) {
    expectDay(Day(2026, 10, 8) + 0, 2026, 10, 8);
}

TEST(DayPlusTest, WithinSameMonth) {
    expectDay(Day(2026, 10, 8) + 10, 2026, 10, 18);
}

TEST(DayPlusTest, CrossesMonth) {
    expectDay(Day(2026, 10, 8) + 30, 2026, 11, 7);
}

TEST(DayPlusTest, CrossesYear) {
    expectDay(Day(2026, 12, 25) + 10, 2027, 1, 4);
}

TEST(DayPlusTest, SpansSeveralMonths) {
    expectDay(Day(2026, 1, 1) + 100, 2026, 4, 11);
}

TEST(DayPlusTest, OneFullCommonYear) {
    expectDay(Day(2026, 1, 1) + 365, 2027, 1, 1);
}

TEST(DayPlusTest, OneFullLeapYear) {
    expectDay(Day(2024, 1, 1) + 366, 2025, 1, 1);
}

TEST(DayPlusTest, MultiYear) {
    expectDay(Day(2026, 1, 1) + 1000, 2028, 9, 27);
    expectDay(Day(2024, 1, 12) + 1000, 2026, 10, 8);
}

TEST(DayPlusTest, FromLeapDayPlus365) {
    expectDay(Day(2000, 2, 29) + 365, 2001, 2, 28);
}

TEST(DayPlusTest, LandsExactlyOnDec31) {
    // dday가 정확히 0으로 끝나는 경계 (resultYear - 1 보정 확인)
    expectDay(Day(2026, 1, 1) + 364, 2026, 12, 31);
    expectDay(Day(2027, 12, 31) + 366, 2028, 12, 31);
}

TEST(DayPlusTest, FromDec31) {
    expectDay(Day(2026, 12, 31) + 1, 2027, 1, 1);
    expectDay(Day(2028, 12, 31) + 1, 2029, 1, 1);
}

TEST(DayPlusTest, LeapDayHandling) {
    expectDay(Day(2024, 2, 28) + 1, 2024, 2, 29);
    expectDay(Day(2024, 2, 29) + 1, 2024, 3, 1);
    expectDay(Day(2026, 2, 28) + 1, 2026, 3, 1);
}

TEST(DayPlusTest, FourHundredYearCycle) {
    expectDay(Day(2000, 1, 1) + 146097, 2400, 1, 1);
}

TEST(DayPlusTest, DoesNotModifyOriginal) {
    Day d(2026, 10, 8);
    Day r = d + 5;
    expectDay(d, 2026, 10, 8);
    expectDay(r, 2026, 10, 13);
}

// ---------- operator- ----------

TEST(DayMinusTest, ZeroReturnsSameDate) {
    expectDay(Day(2026, 10, 8) - 0, 2026, 10, 8);
}

TEST(DayMinusTest, CrossesMonth) {
    expectDay(Day(2026, 10, 8) - 8, 2026, 9, 30);
}

TEST(DayMinusTest, CrossesYear) {
    expectDay(Day(2026, 1, 1) - 1, 2025, 12, 31);
}

TEST(DayMinusTest, FullCommonYear) {
    expectDay(Day(2026, 1, 1) - 365, 2025, 1, 1);
}

TEST(DayMinusTest, IntoLeapYear) {
    expectDay(Day(2026, 1, 1) - 366, 2024, 12, 31);
}

TEST(DayMinusTest, MarchFirstGoesToFeb) {
    expectDay(Day(2026, 3, 1) - 1, 2026, 2, 28);
    expectDay(Day(2024, 3, 1) - 1, 2024, 2, 29);
}

TEST(DayMinusTest, FromDec31) {
    expectDay(Day(2026, 12, 31) - 365, 2025, 12, 31);
    expectDay(Day(2023, 12, 31) - 1, 2023, 12, 30);   // 기존 ShortDayCalculate 대체
}

TEST(DayMinusTest, MultiYear) {
    expectDay(Day(2026, 10, 8) - 1000, 2024, 1, 12);
}

TEST(DayMinusTest, NegativeArgumentMovesForward) {
    expectDay(Day(2026, 10, 8) - (-5), 2026, 10, 13);
}

TEST(DayPlusTest, NegativeArgumentMovesBackward) {
    expectDay(Day(2026, 10, 8) + (-8), 2026, 9, 30);
}

// ---------- 왕복 / 일관성 ----------

TEST(DayRoundTripTest, PlusThenMinusReturnsOriginal) {
    const Day starts[] = {Day(2026, 1, 1), Day(2026, 12, 31), Day(2024, 2, 29),
                          Day(2000, 3, 1), Day(1, 1, 1), Day(0, 12, 31)};
    const int offsets[] = {1, 30, 364, 365, 366, 1000, 5000, 146097};
    for (const Day &s : starts) {
        for (int n : offsets) {
            SCOPED_TRACE(str(s) + " n=" + std::to_string(n));
            expectSame((s + n) - n, s);
            expectSame((s - n) + n, s);
        }
    }
}

TEST(DayConsistencyTest, PlusMatchesRepeatedIncrement) {
    Day start(2023, 1, 1);
    Day cur = start;
    for (int i = 0; i <= 1500; ++i) {
        SCOPED_TRACE(i);
        expectSame(start + i, cur);
        ++cur;
    }
}

TEST(DayConsistencyTest, MinusMatchesRepeatedDecrement) {
    Day start(2026, 12, 31);
    Day cur = start;
    for (int i = 0; i <= 1500; ++i) {
        SCOPED_TRACE(i);
        expectSame(start - i, cur);
        --cur;
    }
}

TEST(DayConsistencyTest, BcRangeAlsoConsistent) {
    Day start(-2, 1, 1);
    Day cur = start;
    for (int i = 0; i <= 1200; ++i) {
        SCOPED_TRACE(i);
        expectSame(start + i, cur);
        ++cur;
    }
}

// ---------- operator++ ----------

TEST(DayIncrementTest, NormalDay) {
    Day d(2026, 10, 8);
    ++d;
    expectDay(d, 2026, 10, 9);
}

TEST(DayIncrementTest, CrossesMonth) {
    Day d(2026, 10, 31);
    ++d;
    expectDay(d, 2026, 11, 1);
}

TEST(DayIncrementTest, CrossesYear) {
    Day d(2026, 12, 31);
    ++d;
    expectDay(d, 2027, 1, 1);
}

TEST(DayIncrementTest, LeapDay) {
    Day d(2024, 2, 28);
    ++d;
    expectDay(d, 2024, 2, 29);
    ++d;
    expectDay(d, 2024, 3, 1);
}

TEST(DayIncrementTest, CommonYearFeb28) {
    Day d(2026, 2, 28);
    ++d;
    expectDay(d, 2026, 3, 1);
}

TEST(DayIncrementTest, ReturnsSelfReference) {
    Day d(2026, 10, 8);
    Day &r = ++d;
    EXPECT_EQ(&r, &d);
}

TEST(DayIncrementTest, FullYearOfCalls) {
    Day d(2026, 1, 1);
    for (int i = 0; i < 365; ++i) ++d;
    expectDay(d, 2027, 1, 1);
}

// ---------- operator-- ----------

TEST(DayDecrementTest, NormalDay) {
    Day d(2026, 10, 8);
    --d;
    expectDay(d, 2026, 10, 7);
}

TEST(DayDecrementTest, CrossesMonth) {
    Day d(2026, 11, 1);
    --d;
    expectDay(d, 2026, 10, 31);
}

TEST(DayDecrementTest, CrossesYear) {
    Day d(2026, 1, 1);
    --d;
    expectDay(d, 2025, 12, 31);
}

TEST(DayDecrementTest, MarchFirst) {
    Day common(2026, 3, 1);
    --common;
    expectDay(common, 2026, 2, 28);

    Day leap(2024, 3, 1);
    --leap;
    expectDay(leap, 2024, 2, 29);
}

TEST(DayDecrementTest, ReturnsSelfReference) {
    Day d(2026, 10, 8);
    Day &r = --d;
    EXPECT_EQ(&r, &d);
}

// ---------- operator<< ----------

TEST(DayOutputTest, PadsMonthAndDay) {
    EXPECT_EQ(str(Day(2026, 10, 8)), "2026/10/08");
    EXPECT_EQ(str(Day(2026, 1, 1)), "2026/01/01");
}

TEST(DayOutputTest, PadsYearToFourDigits) {
    EXPECT_EQ(str(Day(5, 1, 1)), "0005/01/01");
}

TEST(DayOutputTest, BcYears) {
    // 0년 = BC 1년, -1년 = BC 2년
    EXPECT_EQ(str(Day(0, 12, 31)), "[BC]0001/12/31");
    EXPECT_EQ(str(Day(-1, 1, 1)), "[BC]0002/01/01");
}

TEST(DayOutputTest, YearOneMinusOneDayIsBc) {
    EXPECT_EQ(str(Day(1, 1, 1) - 1), "[BC]0001/12/31");
}