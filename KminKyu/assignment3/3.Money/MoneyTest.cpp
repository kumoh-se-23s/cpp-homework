#include <gtest/gtest.h>
#include "Money.h"

// ---------- 생성자 / getter / set ----------

TEST(MoneyConstructor, DefaultIsZero) {
    Money m;
    EXPECT_EQ(m.getDollar(), 0);
    EXPECT_EQ(m.getCent(), 0);
}

TEST(MoneyConstructor, StoresGivenValues) {
    Money m(12, 34);
    EXPECT_EQ(m.getDollar(), 12);
    EXPECT_EQ(m.getCent(), 34);
}

TEST(MoneyConstructor, NormalizesCentOverflow) {
    Money m(1, 150);
    EXPECT_EQ(m.getDollar(), 2);
    EXPECT_EQ(m.getCent(), 50);
}

TEST(MoneyConstructor, NormalizesMixedSignPositiveDollar) {
    Money m(1, -50);  // 1달러 - 50센트 = 0.50달러
    EXPECT_EQ(m.getDollar(), 0);
    EXPECT_EQ(m.getCent(), 50);
}

TEST(MoneyConstructor, NormalizesMixedSignNegativeDollar) {
    Money m(-1, 50);  // -1달러 + 50센트 = -0.50달러
    EXPECT_EQ(m.getDollar(), 0);
    EXPECT_EQ(m.getCent(), -50);
}

TEST(MoneyConstructor, NormalizesNegativeCentOverflow) {
    Money m(0, -150);
    EXPECT_EQ(m.getDollar(), -1);
    EXPECT_EQ(m.getCent(), -50);
}

TEST(MoneySet, OverwritesValues) {
    Money m(1, 1);
    m.set(5, 25);
    EXPECT_EQ(m.getDollar(), 5);
    EXPECT_EQ(m.getCent(), 25);
}

TEST(MoneySet, NormalizesValues) {
    Money m;
    m.set(2, 130);
    EXPECT_EQ(m.getDollar(), 3);
    EXPECT_EQ(m.getCent(), 30);
}

// ---------- 덧셈 ----------

TEST(MoneyPlus, SimpleAddition) {
    Money result = Money(1, 20) + Money(2, 30);
    EXPECT_EQ(result.getDollar(), 3);
    EXPECT_EQ(result.getCent(), 50);
}

TEST(MoneyPlus, CentCarry) {
    Money result = Money(1, 70) + Money(0, 50);
    EXPECT_EQ(result.getDollar(), 2);
    EXPECT_EQ(result.getCent(), 20);
}

TEST(MoneyPlus, WithZero) {
    Money result = Money(3, 45) + Money();
    EXPECT_EQ(result.getDollar(), 3);
    EXPECT_EQ(result.getCent(), 45);
}

TEST(MoneyPlus, PositiveAndNegativeCancelOut) {
    Money result = Money(2, 50) + Money(-2, -50);
    EXPECT_EQ(result.getDollar(), 0);
    EXPECT_EQ(result.getCent(), 0);
}

TEST(MoneyPlus, ResultBecomesNegative) {
    Money result = Money(1, 0) + Money(-1, -75);
    EXPECT_EQ(result.getDollar(), 0);
    EXPECT_EQ(result.getCent(), -75);
}

TEST(MoneyPlus, DoesNotModifyOperands) {
    Money a(1, 10);
    Money b(2, 20);
    Money result = a + b;
    (void)result;
    EXPECT_EQ(a.getDollar(), 1);
    EXPECT_EQ(a.getCent(), 10);
    EXPECT_EQ(b.getDollar(), 2);
    EXPECT_EQ(b.getCent(), 20);
}

// ---------- 뺄셈 ----------

TEST(MoneyMinus, SimpleSubtraction) {
    Money result = Money(5, 75) - Money(2, 25);
    EXPECT_EQ(result.getDollar(), 3);
    EXPECT_EQ(result.getCent(), 50);
}

TEST(MoneyMinus, CentBorrow) {
    Money result = Money(5, 10) - Money(2, 25);
    EXPECT_EQ(result.getDollar(), 2);
    EXPECT_EQ(result.getCent(), 85);
}

TEST(MoneyMinus, SameValueIsZero) {
    Money result = Money(3, 33) - Money(3, 33);
    EXPECT_EQ(result.getDollar(), 0);
    EXPECT_EQ(result.getCent(), 0);
}

TEST(MoneyMinus, ResultNegativeDollarOnly) {
    Money result = Money(1, 0) - Money(3, 0);
    EXPECT_EQ(result.getDollar(), -2);
    EXPECT_EQ(result.getCent(), 0);
}

TEST(MoneyMinus, ResultNegativeCentOnly) {
    Money result = Money(1, 0) - Money(1, 30);
    EXPECT_EQ(result.getDollar(), 0);
    EXPECT_EQ(result.getCent(), -30);
}

TEST(MoneyMinus, ResultNegativeBoth) {
    Money result = Money(1, 20) - Money(3, 50);
    EXPECT_EQ(result.getDollar(), -2);
    EXPECT_EQ(result.getCent(), -30);
}

TEST(MoneyMinus, SubtractNegativeValue) {
    Money result = Money(1, 50) - Money(-1, -50);
    EXPECT_EQ(result.getDollar(), 3);
    EXPECT_EQ(result.getCent(), 0);
}

// ---------- 비교 연산자 ----------

TEST(MoneyCompare, Equal) {
    EXPECT_TRUE(Money(2, 50) == Money(2, 50));
    EXPECT_FALSE(Money(2, 50) == Money(2, 51));
    EXPECT_FALSE(Money(2, 50) == Money(3, 50));
}

TEST(MoneyCompare, EqualAfterNormalization) {
    EXPECT_TRUE(Money(1, 100) == Money(2, 0));
    EXPECT_TRUE(Money(0, 0) == Money());
}

TEST(MoneyCompare, LessThan) {
    EXPECT_TRUE(Money(1, 99) < Money(2, 0));
    EXPECT_TRUE(Money(2, 10) < Money(2, 11));
    EXPECT_TRUE(Money(-1, -50) < Money(0, 0));
    EXPECT_TRUE(Money(-2, 0) < Money(-1, -50));
    EXPECT_FALSE(Money(2, 0) < Money(2, 0));
    EXPECT_FALSE(Money(3, 0) < Money(2, 99));
}

TEST(MoneyCompare, GreaterThan) {
    EXPECT_TRUE(Money(2, 0) > Money(1, 99));
    EXPECT_TRUE(Money(2, 11) > Money(2, 10));
    EXPECT_TRUE(Money(0, 0) > Money(0, -1));
    EXPECT_TRUE(Money(-1, -50) > Money(-2, 0));
    EXPECT_FALSE(Money(2, 0) > Money(2, 0));
    EXPECT_FALSE(Money(1, 99) > Money(2, 0));
}

TEST(MoneyCompare, LessOrEqual) {
    EXPECT_TRUE(Money(1, 0) <= Money(2, 0));
    EXPECT_TRUE(Money(2, 0) <= Money(2, 0));
    EXPECT_TRUE(Money(-1, -50) <= Money(0, 0));
    EXPECT_FALSE(Money(2, 1) <= Money(2, 0));
}

TEST(MoneyCompare, GreaterOrEqual) {
    EXPECT_TRUE(Money(2, 0) >= Money(1, 0));
    EXPECT_TRUE(Money(2, 0) >= Money(2, 0));
    EXPECT_TRUE(Money(0, 0) >= Money(-1, -50));
    EXPECT_FALSE(Money(2, 0) >= Money(2, 1));
}

TEST(MoneyCompare, ZeroBoundary) {
    EXPECT_TRUE(Money(0, 1) > Money(0, 0));
    EXPECT_TRUE(Money(0, -1) < Money(0, 0));
    EXPECT_TRUE(Money(0, 0) == Money(0, 0));
}

// ---------- 대입 연산자 ----------

TEST(MoneyAssign, CopiesValues) {
    Money a(3, 40);
    Money b;
    b = a;
    EXPECT_EQ(b.getDollar(), 3);
    EXPECT_EQ(b.getCent(), 40);
}

TEST(MoneyAssign, SelfAssignmentKeepsValue) {
    Money a(3, 40);
    Money &ref = a;
    a = ref;
    EXPECT_EQ(a.getDollar(), 3);
    EXPECT_EQ(a.getCent(), 40);
}

TEST(MoneyAssign, ChainedAssignment) {
    Money a, b, c(7, 7);
    a = b = c;
    EXPECT_EQ(a.getDollar(), 7);
    EXPECT_EQ(a.getCent(), 7);
    EXPECT_EQ(b.getDollar(), 7);
    EXPECT_EQ(b.getCent(), 7);
}

TEST(MoneyAssign, ReturnsReferenceToSelf) {
    Money a, b(1, 1);
    Money &result = (a = b);
    EXPECT_EQ(&result, &a);
}