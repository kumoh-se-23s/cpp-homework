#include <gtest/gtest.h>
#include <sstream>
#include "DDay.h"

namespace {
    void expectDay(const Day &actual, int year, int month, int day) {
        EXPECT_EQ(actual.getYear(), year);
        EXPECT_EQ(actual.getMonth(), month);
        EXPECT_EQ(actual.getDay(), day);
    }

    DDay make(int startYmd, int dday) {
        DDay d;
        EXPECT_TRUE(d.setStartDay(startYmd));
        d.setDDay(dday);
        return d;
    }
}

// ---------- 생성 ----------

TEST(DDayBasicTest, DefaultConstructor) {
    DDay d;
    EXPECT_EQ(d.getDDay(), 0);
    expectDay(d.getEndDay(), d.getStartDay().getYear(),
              d.getStartDay().getMonth(), d.getStartDay().getDay());
}

// ---------- setStartDay ----------

TEST(DDaySetStartDayTest, ValidDateReturnsTrue) {
    DDay d;
    EXPECT_TRUE(d.setStartDay(20261008));
    expectDay(d.getStartDay(), 2026, 10, 8);
    expectDay(d.getEndDay(), 2026, 10, 8);
}

TEST(DDaySetStartDayTest, LeapDayAccepted) {
    DDay d;
    EXPECT_TRUE(d.setStartDay(20240229));
    expectDay(d.getStartDay(), 2024, 2, 29);
}

TEST(DDaySetStartDayTest, Feb29InCommonYearRejected) {
    DDay d;
    EXPECT_FALSE(d.setStartDay(20260229));
}

TEST(DDaySetStartDayTest, Month13Rejected) {
    DDay d;
    EXPECT_FALSE(d.setStartDay(20261301));
}

TEST(DDaySetStartDayTest, Day32Rejected) {
    DDay d;
    EXPECT_FALSE(d.setStartDay(20261032));
}

TEST(DDaySetStartDayTest, April31Rejected) {
    DDay d;
    EXPECT_FALSE(d.setStartDay(20260431));
}

// isCorrectDate가 음수만 걸러서 0이 통과할 가능성 (버그 확인용)
TEST(DDaySetStartDayTest, Month0Rejected) {
    DDay d;
    EXPECT_FALSE(d.setStartDay(20260001));
}

TEST(DDaySetStartDayTest, Day0Rejected) {
    DDay d;
    EXPECT_FALSE(d.setStartDay(20261000));
}

TEST(DDaySetStartDayTest, InvalidInputKeepsPreviousState) {
    DDay d = make(20261008, 5);
    EXPECT_FALSE(d.setStartDay(20261332));
    expectDay(d.getStartDay(), 2026, 10, 8);
    expectDay(d.getEndDay(), 2026, 10, 13);
    EXPECT_EQ(d.getDDay(), 5);
}

TEST(DDaySetStartDayTest, ChangingStartRecalculatesEndWithSameDDay) {
    DDay d = make(20261008, 10);
    EXPECT_TRUE(d.setStartDay(20261220));
    expectDay(d.getStartDay(), 2026, 12, 20);
    expectDay(d.getEndDay(), 2026, 12, 30);
    EXPECT_EQ(d.getDDay(), 10);
}
TEST(DDaySetDDayTest, remainDayTest) {
    EXPECT_EQ (Day::getRemainDays(2026,01,01), 1);
}
// ---------- setDDay ----------

TEST(DDaySetDDayTest, ZeroKeepsEndEqualToStart) {
    DDay d = make(20261008, 0);
    expectDay(d.getEndDay(), 2026, 10, 8);
    EXPECT_EQ(d.getDDay(), 0);
}

TEST(DDaySetDDayTest, PositiveWithinSameMonth) {
    DDay d = make(20261008, 10);
    expectDay(d.getEndDay(), 2026, 10, 18);
}

TEST(DDaySetDDayTest, PositiveCrossesMonth) {
    DDay d = make(20261008, 30);
    expectDay(d.getEndDay(), 2026, 11, 7);
}

TEST(DDaySetDDayTest, PositiveCrossesYear) {
    DDay d = make(20261225, 10);
    expectDay(d.getEndDay(), 2027, 1, 4);
}

TEST(DDaySetDDayTest, PositiveSpansSeveralMonths) {
    DDay d = make(20260101, 100);
    expectDay(d.getEndDay(), 2026, 4, 11);
}

TEST(DDaySetDDayTest, OneFullCommonYear) {
    DDay d = make(20260101, 365);
    expectDay(d.getEndDay(), 2027, 1, 1);
}

TEST(DDaySetDDayTest, OneFullLeapYear) {
    DDay d = make(20240101, 366);
    expectDay(d.getEndDay(), 2025, 1, 1);
}

TEST(DDaySetDDayTest, MultiYear) {
    DDay d = make(20260101, 1000);
    expectDay(d.getEndDay(), 2028, 9, 27);
}

TEST(DDaySetDDayTest, FromLeapDayPlus365) {
    DDay d = make(20000229, 365);
    expectDay(d.getEndDay(), 2001, 2, 28);
}

TEST(DDaySetDDayTest, NegativeMovesBackward) {
    DDay d = make(20261008, -8);
    expectDay(d.getEndDay(), 2026, 9, 30);
    EXPECT_EQ(d.getDDay(), -8);
}

TEST(DDaySetDDayTest, NegativeCrossesYear) {
    DDay d = make(20260101, -1);
    expectDay(d.getEndDay(), 2025, 12, 31);
}

TEST(DDaySetDDayTest, ShortDayCalculate) {
    expectDay(Day::calculateShortDays(2023, -1), 2023, 12, 30);
}



TEST(DDaySetDDayTest, NegativeFullYear) {
    DDay d = make(20260101, -365);
    expectDay(d.getEndDay(), 2025, 1, 1);
}

TEST(DDaySetDDayTest, LeapDayHandling) {
    DDay d = make(20240228, 1);
    expectDay(d.getEndDay(), 2024, 2, 29);
}

TEST(DDaySetDDayTest, CommonYearSkipsFeb29) {
    DDay d = make(20260228, 1);
    expectDay(d.getEndDay(), 2026, 3, 1);
}

TEST(DDaySetDDayTest, SetDDayDoesNotChangeStart) {
    DDay d = make(20261008, 0);
    d.setDDay(50);
    expectDay(d.getStartDay(), 2026, 10, 8);
}

TEST(DDaySetDDayTest, SetOverwritesPreviousValue) {
    DDay d = make(20261008, 10);
    d.setDDay(3);
    expectDay(d.getEndDay(), 2026, 10, 11);
    EXPECT_EQ(d.getDDay(), 3);
}

TEST(DDaySetDDayTest, SetBackToZeroRestoresEnd) {
    DDay d = make(20261008, 50);
    d.setDDay(0);
    expectDay(d.getEndDay(), 2026, 10, 8);
}

TEST(DDaySetDDayTest, RepeatedCallsReturnSameResult) {
    // operator+가 원본을 변경하지 않는지 확인
    DDay d = make(20261008, 5);
    expectDay(d.getEndDay(), 2026, 10, 13);
    expectDay(d.getEndDay(), 2026, 10, 13);
    expectDay(d.getStartDay(), 2026, 10, 8);
    expectDay(d.getStartDay(), 2026, 10, 8);
}

// ---------- setTomorrow ----------

TEST(DDayTomorrowTest, MovesBothOneDayForward) {
    DDay d = make(20261008, 10);
    d.setTomarrow();
    expectDay(d.getStartDay(), 2026, 10, 9);
    expectDay(d.getEndDay(), 2026, 10, 19);
    EXPECT_EQ(d.getDDay(), 10);
}

TEST(DDayTomorrowTest, CrossesMonth) {
    DDay d = make(20261031, 0);
    d.setTomarrow();
    expectDay(d.getStartDay(), 2026, 11, 1);
    expectDay(d.getEndDay(), 2026, 11, 1);
}

TEST(DDayTomorrowTest, CrossesYear) {
    DDay d = make(20261231, 0);
    d.setTomarrow();
    expectDay(d.getStartDay(), 2027, 1, 1);
    expectDay(d.getEndDay(), 2027, 1, 1);
}

TEST(DDayTomorrowTest, ThroughLeapDay) {
    DDay d = make(20240228, 0);
    d.setTomarrow();
    expectDay(d.getStartDay(), 2024, 2, 29);
    d.setTomarrow();
    expectDay(d.getStartDay(), 2024, 3, 1);
}

TEST(DDayTomorrowTest, EndCrossesMonthSeparately) {
    // start는 월 안에서 이동, end만 월을 넘는 경우
    DDay d = make(20261008, 23);   // end = 10/31
    d.setTomarrow();
    expectDay(d.getStartDay(), 2026, 10, 9);
    expectDay(d.getEndDay(), 2026, 11, 1);
}

TEST(DDayTomorrowTest, RepeatedCallsAccumulate) {
    DDay d = make(20261225, 5);
    for (int i = 0; i < 10; ++i) d.setTomarrow();
    expectDay(d.getStartDay(), 2027, 1, 4);
    expectDay(d.getEndDay(), 2027, 1, 9);
}

TEST(DDayTomorrowTest, FullYearOfCalls) {
    DDay d = make(20260101, 0);
    for (int i = 0; i < 365; ++i) d.setTomarrow();
    expectDay(d.getStartDay(), 2027, 1, 1);
}

// ---------- setYesterday ----------

TEST(DDayYesterdayTest, MovesBothOneDayBackward) {
    DDay d = make(20261008, 10);
    d.setYesterDay();
    expectDay(d.getStartDay(), 2026, 10, 7);
    expectDay(d.getEndDay(), 2026, 10, 17);
    EXPECT_EQ(d.getDDay(), 10);
}

TEST(DDayYesterdayTest, CrossesMonth) {
    DDay d = make(20261101, 0);
    d.setYesterDay();
    expectDay(d.getStartDay(), 2026, 10, 31);
    expectDay(d.getEndDay(), 2026, 10, 31);
}

// Day::operator--가 month==0일 때 getMaxOfDays(year, 0)을 쓰므로 실패할 가능성 큼
TEST(DDayYesterdayTest, CrossesYear) {
    DDay d = make(20260101, 0);
    d.setYesterDay();
    expectDay(d.getStartDay(), 2025, 12, 31);
    expectDay(d.getEndDay(), 2025, 12, 31);
}

TEST(DDayYesterdayTest, MarchFirstInCommonYear) {
    DDay d = make(20260301, 0);
    d.setYesterDay();
    expectDay(d.getStartDay(), 2026, 2, 28);
}

TEST(DDayYesterdayTest, MarchFirstInLeapYear) {
    DDay d = make(20240301, 0);
    d.setYesterDay();
    expectDay(d.getStartDay(), 2024, 2, 29);
}

TEST(DDayYesterdayTest, RepeatedCallsAccumulate) {
    DDay d = make(20270104, 5);
    for (int i = 0; i < 10; ++i) d.setYesterDay();
    expectDay(d.getStartDay(), 2026, 12, 25);
    expectDay(d.getEndDay(), 2026, 12, 30);
}

// ---------- 조합 ----------

TEST(DDayComboTest, TomorrowThenYesterdayReturnsOriginal) {
    DDay d = make(20261008, 10);
    d.setTomarrow();
    d.setYesterDay();
    expectDay(d.getStartDay(), 2026, 10, 8);
    expectDay(d.getEndDay(), 2026, 10, 18);
}

TEST(DDayComboTest, YesterdayThenTomorrowReturnsOriginal) {
    DDay d = make(20260301, 10);
    d.setYesterDay();
    d.setTomarrow();
    expectDay(d.getStartDay(), 2026, 3, 1);
    expectDay(d.getEndDay(), 2026, 3, 11);
}

TEST(DDayComboTest, SetDDayAfterTomorrowUsesMovedStart) {
    DDay d = make(20261008, 0);
    d.setTomarrow();
    d.setDDay(10);
    expectDay(d.getStartDay(), 2026, 10, 9);
    expectDay(d.getEndDay(), 2026, 10, 19);
}

TEST(DDayComboTest, SetStartDayAfterTomorrowKeepsDDay) {
    DDay d = make(20261008, 10);
    d.setTomarrow();
    EXPECT_TRUE(d.setStartDay(20270101));
    expectDay(d.getStartDay(), 2027, 1, 1);
    expectDay(d.getEndDay(), 2027, 1, 11);
}

TEST(DDayComboTest, RoundTripAcrossLeapDay) {
    DDay d = make(20240228, 2);
    for (int i = 0; i < 3; ++i) d.setTomarrow();
    for (int i = 0; i < 3; ++i) d.setYesterDay();
    expectDay(d.getStartDay(), 2024, 2, 28);
    expectDay(d.getEndDay(), 2024, 3, 1);
}

// ---------- 복사 ----------

TEST(DDayCopyTest, CopyHasSameValues) {
    DDay original = make(20261008, 7);
    DDay copy = original;
    expectDay(copy.getStartDay(), 2026, 10, 8);
    expectDay(copy.getEndDay(), 2026, 10, 15);
    EXPECT_EQ(copy.getDDay(), 7);
}

TEST(DDayCopyTest, CopyIsIndependent) {
    DDay original = make(20261008, 0);
    DDay copy = original;
    copy.setDDay(30);
    copy.setTomarrow();

    expectDay(original.getStartDay(), 2026, 10, 8);
    expectDay(original.getEndDay(), 2026, 10, 8);
    expectDay(copy.getStartDay(), 2026, 10, 9);
    expectDay(copy.getEndDay(), 2026, 11, 8);
}

// ---------- operator<< ----------

TEST(DDayOutputTest, PositiveDDayHasPlusSign) {
    DDay d = make(20261008, 5);
    std::ostringstream oss;
    oss << d;
    EXPECT_EQ(oss.str(), "<< 2026/10/08 [D-day:+5] 2026/10/13\n");
}

TEST(DDayOutputTest, ZeroDDayHasPlusSign) {
    DDay d = make(20261008, 0);
    std::ostringstream oss;
    oss << d;
    EXPECT_EQ(oss.str(), "<< 2026/10/08 [D-day:+0] 2026/10/08\n");
}

TEST(DDayOutputTest, NegativeDDayHasMinusSignOnly) {
    DDay d = make(20261008, -8);
    std::ostringstream oss;
    oss << d;
    EXPECT_EQ(oss.str(), "<< 2026/10/08 [D-day:-8] 2026/09/30\n");
}