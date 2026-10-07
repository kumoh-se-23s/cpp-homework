//
// Created by leegu on 26. 10. 7..
//
#include <gtest/gtest.h>
#include "Fraction.h"

// 생성자
TEST(FractionTest, ConstructorStoresValues) {
    Fraction f(1, 2);
    EXPECT_EQ(f.get_numerator(), 1);
    EXPECT_EQ(f.get_denominator(), 2);
}

// 기약분수 변환
TEST(FractionTest, ReducesToLowestTerms) {
    Fraction f(2, 4);
    EXPECT_EQ(f.get_numerator(), 1);
    EXPECT_EQ(f.get_denominator(), 2);
}

// 덧셈
TEST(FractionTest, Add) {
    Fraction a(1, 2);
    Fraction b(1, 3);
    Fraction result = a + b;
    EXPECT_EQ(result.get_numerator(), 5);
    EXPECT_EQ(result.get_denominator(), 6);
}

// 분모가 0이면 분모가 1로 보정되는지 확인
TEST(FractionZeroDenominatorTest, DenominatorBecomesOne) {
    Fraction f(3, 0);
    EXPECT_EQ(f.get_denominator(), 1);
}

// 분모가 0이어도 분자는 그대로 유지되는지 확인
TEST(FractionZeroDenominatorTest, NumeratorIsPreserved) {
    Fraction f(3, 0);
    EXPECT_EQ(f.get_numerator(), 3);
}

// 분모가 0이면 "ERR"가 출력되는지 확인
TEST(FractionZeroDenominatorTest, PrintsErrMessage) {
    testing::internal::CaptureStdout();
    Fraction f(3, 0);
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "ERR\n");
}

// 정상 분모일 때는 "ERR"가 출력되지 않는지 확인
TEST(FractionZeroDenominatorTest, NoErrMessageForValidDenominator) {
    testing::internal::CaptureStdout();
    Fraction f(3, 4);
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "");
}

// set()으로 분모를 0으로 바꿀 때도 동일하게 동작하는지 확인
TEST(FractionZeroDenominatorTest, SetWithZeroDenominator) {
    Fraction f(1, 2);

    testing::internal::CaptureStdout();
    f.set(5, 0);
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "ERR\n");
    EXPECT_EQ(f.get_numerator(), 5);
    EXPECT_EQ(f.get_denominator(), 1);
}

// 분자도 0, 분모도 0인 경우
TEST(FractionZeroDenominatorTest, ZeroOverZero) {
    Fraction f(0, 0);
    EXPECT_EQ(f.get_numerator(), 0);
    EXPECT_EQ(f.get_denominator(), 1);
}