//
// Created by leegu on 26. 10. 7..
//
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include "Matrix.h"

namespace {
    constexpr int ROWS = Matrix::DEFAULT_ROW_SIZE;
    constexpr int COLS = Matrix::DEFAULT_COL_SIZE;

    // 1 2 3 / 4 5 6 / 7 8 9
    Matrix make_sequential_matrix() {
        Matrix m;
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                m.set(r, c, r * COLS + c + 1);
            }
        }
        return m;
    }

    // 9 8 7 / 6 5 4 / 3 2 1
    Matrix make_reversed_matrix() {
        Matrix m;
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                m.set(r, c, ROWS * COLS - (r * COLS + c));
            }
        }
        return m;
    }

    Matrix make_identity_matrix() {
        Matrix m;
        for (int i = 0; i < ROWS; ++i) {
            m.set(i, i, 1);
        }
        return m;
    }

    void expect_matrix_equal(const Matrix &actual, const Matrix &expected) {
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                EXPECT_EQ(actual.get(r, c), expected.get(r, c))
                    << "불일치 위치: (" << r << ", " << c << ")";
            }
        }
    }

    std::string capture_output(const Matrix &m) {
        testing::internal::CaptureStdout();
        std::cout << m;
        std::cout.flush();
        return testing::internal::GetCapturedStdout();
    }
}

// ---------- 생성 / get / set ----------

TEST(MatrixBasicTest, DefaultConstructorFillsZero) {
    Matrix m;
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            EXPECT_EQ(m.get(r, c), 0);
        }
    }
}

TEST(MatrixBasicTest, SetAndGet) {
    Matrix m;
    m.set(0, 0, 7);
    m.set(ROWS - 1, COLS - 1, -3);

    EXPECT_EQ(m.get(0, 0), 7);
    EXPECT_EQ(m.get(ROWS - 1, COLS - 1), -3);
}

TEST(MatrixBasicTest, SetDoesNotAffectOtherElements) {
    Matrix m;
    m.set(1, 1, 5);

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            if (r == 1 && c == 1) continue;
            EXPECT_EQ(m.get(r, c), 0);
        }
    }
}

TEST(MatrixBasicTest, CopyIsIndependent) {
    Matrix original = make_sequential_matrix();
    Matrix copy = original;
    copy.set(0, 0, 100);

    EXPECT_EQ(original.get(0, 0), 1);
    EXPECT_EQ(copy.get(0, 0), 100);
}

// ---------- 전치 ----------

TEST(MatrixTransposeTest, SwapsRowAndColumn) {
    Matrix m = make_sequential_matrix();
    Matrix t = m.transpose();

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            EXPECT_EQ(t.get(c, r), m.get(r, c));
        }
    }
}

TEST(MatrixTransposeTest, KnownValues) {
    Matrix t = make_sequential_matrix().transpose();

    EXPECT_EQ(t.get(0, 1), 4);
    EXPECT_EQ(t.get(0, 2), 7);
    EXPECT_EQ(t.get(1, 0), 2);
    EXPECT_EQ(t.get(2, 0), 3);
}

TEST(MatrixTransposeTest, DoubleTransposeReturnsOriginal) {
    Matrix m = make_sequential_matrix();
    expect_matrix_equal(m.transpose().transpose(), m);
}

TEST(MatrixTransposeTest, NotOperatorEqualsTranspose) {
    Matrix m = make_sequential_matrix();
    expect_matrix_equal(!m, m.transpose());
}

TEST(MatrixTransposeTest, DoesNotModifyOriginal) {
    Matrix m = make_sequential_matrix();
    Matrix t = m.transpose();
    (void) t;
    expect_matrix_equal(m, make_sequential_matrix());
}

TEST(MatrixTransposeTest, IdentityStaysIdentity) {
    Matrix identity = make_identity_matrix();
    expect_matrix_equal(identity.transpose(), identity);
}

// ---------- 덧셈 ----------

TEST(MatrixAddTest, AddsElementWise) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_reversed_matrix();
    Matrix result = a.add(b);

    // 1 + 9 = 2 + 8 = ... = 10
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            EXPECT_EQ(result.get(r, c), 10);
        }
    }
}

TEST(MatrixAddTest, OperatorPlusEqualsAdd) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_identity_matrix();
    expect_matrix_equal(a + b, a.add(b));
}

TEST(MatrixAddTest, AddingZeroMatrixKeepsValues) {
    Matrix a = make_sequential_matrix();
    Matrix zero;
    expect_matrix_equal(a + zero, a);
}

TEST(MatrixAddTest, IsCommutative) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_identity_matrix();
    expect_matrix_equal(a + b, b + a);
}

TEST(MatrixAddTest, HandlesNegativeValues) {
    Matrix a;
    Matrix b;
    a.set(0, 0, 5);
    b.set(0, 0, -8);

    EXPECT_EQ((a + b).get(0, 0), -3);
}

TEST(MatrixAddTest, DoesNotModifyOperands) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_reversed_matrix();
    Matrix result = a + b;
    (void) result;

    expect_matrix_equal(a, make_sequential_matrix());
    expect_matrix_equal(b, make_reversed_matrix());
}

// ---------- 곱셈 ----------

TEST(MatrixMultiplyTest, KnownResult) {
    // [1 2 3; 4 5 6; 7 8 9] * [9 8 7; 6 5 4; 3 2 1]
    Matrix result = make_sequential_matrix() * make_reversed_matrix();

    EXPECT_EQ(result.get(0, 0), 30);
    EXPECT_EQ(result.get(0, 1), 24);
    EXPECT_EQ(result.get(0, 2), 18);
    EXPECT_EQ(result.get(1, 0), 84);
    EXPECT_EQ(result.get(1, 1), 69);
    EXPECT_EQ(result.get(1, 2), 54);
    EXPECT_EQ(result.get(2, 0), 138);
    EXPECT_EQ(result.get(2, 1), 114);
    EXPECT_EQ(result.get(2, 2), 90);
}

TEST(MatrixMultiplyTest, SquareOfSequentialMatrix) {
    Matrix m = make_sequential_matrix();
    Matrix result = m * m;

    EXPECT_EQ(result.get(0, 0), 30);
    EXPECT_EQ(result.get(0, 1), 36);
    EXPECT_EQ(result.get(0, 2), 42);
    EXPECT_EQ(result.get(1, 0), 66);
    EXPECT_EQ(result.get(1, 1), 81);
    EXPECT_EQ(result.get(1, 2), 96);
    EXPECT_EQ(result.get(2, 0), 102);
    EXPECT_EQ(result.get(2, 1), 126);
    EXPECT_EQ(result.get(2, 2), 150);
}

TEST(MatrixMultiplyTest, OperatorStarEqualsMulti) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_reversed_matrix();
    expect_matrix_equal(a * b, a.multi(b));
}

TEST(MatrixMultiplyTest, IdentityKeepsMatrix) {
    Matrix m = make_sequential_matrix();
    Matrix identity = make_identity_matrix();

    expect_matrix_equal(m * identity, m);
    expect_matrix_equal(identity * m, m);
}

TEST(MatrixMultiplyTest, ZeroMatrixGivesZero) {
    Matrix m = make_sequential_matrix();
    Matrix zero;
    expect_matrix_equal(m * zero, zero);
}

TEST(MatrixMultiplyTest, IsNotCommutative) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_reversed_matrix();

    // (0,0) 위치: a*b = 30, b*a = 90
    EXPECT_NE((a * b).get(0, 0), (b * a).get(0, 0));
}

TEST(MatrixMultiplyTest, HandlesNegativeValues) {
    Matrix a;
    Matrix b;
    a.set(0, 0, -2);
    b.set(0, 0, 3);

    EXPECT_EQ((a * b).get(0, 0), -6);
}

TEST(MatrixMultiplyTest, DoesNotModifyOperands) {
    Matrix a = make_sequential_matrix();
    Matrix b = make_reversed_matrix();
    Matrix result = a * b;
    (void) result;

    expect_matrix_equal(a, make_sequential_matrix());
    expect_matrix_equal(b, make_reversed_matrix());
}

// ---------- get_max_element_length ----------

TEST(MatrixMaxLengthTest, SingleDigitValues) {
    EXPECT_EQ(make_sequential_matrix().get_max_element_length(), 1);
}

TEST(MatrixMaxLengthTest, TwoDigitValue) {
    Matrix m;
    m.set(0, 0, 10);
    EXPECT_EQ(m.get_max_element_length(), 2);
}

TEST(MatrixMaxLengthTest, ThreeDigitValue) {
    Matrix m;
    m.set(2, 2, 100);
    EXPECT_EQ(m.get_max_element_length(), 3);
}

TEST(MatrixMaxLengthTest, NegativeValueCountsSignCharacter) {
    // -5는 부호 포함 2글자
    Matrix m;
    m.set(0, 0, -5);
    EXPECT_EQ(m.get_max_element_length(), 2);
}

TEST(MatrixMaxLengthTest, NegativeValueLongerThanPositive) {
    // 부호 포함 -50은 3글자, 양수 9는 1글자
    Matrix m;
    m.set(0, 0, 9);
    m.set(1, 1, -50);
    EXPECT_EQ(m.get_max_element_length(), 3);
}

// ---------- 출력 (operator<<) ----------

TEST(MatrixOutputTest, PrintsSequentialMatrix) {
    const std::string expected =
            "| 1 2 3 |\n"
            "| 4 5 6 |\n"
            "| 7 8 9 |\n";

    EXPECT_EQ(capture_output(make_sequential_matrix()), expected);
}

TEST(MatrixOutputTest, AlignsToWidestElement) {
    Matrix m;
    m.set(0, 0, 10);

    const std::string expected =
            "| 10  0  0 |\n"
            "|  0  0  0 |\n"
            "|  0  0  0 |\n";

    EXPECT_EQ(capture_output(m), expected);
}

TEST(MatrixOutputTest, AlignsWithNegativeValue) {
    Matrix m;
    m.set(0, 0, -5);

    const std::string expected =
            "| -5  0  0 |\n"
            "|  0  0  0 |\n"
            "|  0  0  0 |\n";

    EXPECT_EQ(capture_output(m), expected);
}

TEST(MatrixOutputTest, PrintsOneLinePerRow) {
    const std::string output = capture_output(make_identity_matrix());

    int newline_count = 0;
    for (const char ch : output) {
        if (ch == '\n') ++newline_count;
    }
    EXPECT_EQ(newline_count, ROWS);
}

// ---------- 입력 (operator>>) ----------

TEST(MatrixInputTest, ReadsValuesInRowMajorOrder) {
    std::istringstream input("1 2 3 4 5 6 7 8 9");
    Matrix m;
    input >> m;

    expect_matrix_equal(m, make_sequential_matrix());
}

TEST(MatrixInputTest, ReadsNegativeValues) {
    std::istringstream input("-1 -2 -3 -4 -5 -6 -7 -8 -9");
    Matrix m;
    input >> m;

    EXPECT_EQ(m.get(0, 0), -1);
    EXPECT_EQ(m.get(2, 2), -9);
}

TEST(MatrixInputTest, AcceptsNewlineSeparatedInput) {
    std::istringstream input("1 2 3\n4 5 6\n7 8 9\n");
    Matrix m;
    input >> m;

    expect_matrix_equal(m, make_sequential_matrix());
}

TEST(MatrixInputTest, ReadsTwoMatricesFromOneStream) {
    std::istringstream input("1 2 3 4 5 6 7 8 9  9 8 7 6 5 4 3 2 1");
    Matrix a;
    Matrix b;
    input >> a >> b;

    expect_matrix_equal(a, make_sequential_matrix());
    expect_matrix_equal(b, make_reversed_matrix());
}