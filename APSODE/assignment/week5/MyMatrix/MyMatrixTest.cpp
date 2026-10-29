#include <gtest/gtest.h>
#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>

#include "MyMatrix.h"

namespace {
    constexpr int FAIL_VALUE = -999;

    MyMatrix make_matrix(int row, int column, std::initializer_list<int> elements) {
        std::vector<int> source(elements);
        MyMatrix matrix(row, column);
        matrix.init(source.data());
        return matrix;
    }

    // 행 우선 순서로 모든 원소를 꺼낸다.
    std::vector<int> elements_of(const MyMatrix &matrix) {
        std::vector<int> elements;
        for (int row = 0; row < matrix.row(); ++row) {
            for (int col = 0; col < matrix.column(); ++col) {
                elements.push_back(matrix.get(row, col));
            }
        }
        return elements;
    }

    void expect_matrix(const MyMatrix &matrix, int row, int column, const std::vector<int> &elements) {
        EXPECT_EQ(matrix.row(), row);
        EXPECT_EQ(matrix.column(), column);
        EXPECT_EQ(elements_of(matrix), elements);
    }

    void expect_error_matrix(const MyMatrix &matrix) {
        expect_matrix(matrix, 1, 1, {FAIL_VALUE});
    }

    std::string to_string(const MyMatrix &matrix) {
        std::ostringstream stream;
        stream << matrix;
        return stream.str();
    }

    // 원소 사이 공백 개수(정렬 폭)는 구현마다 다를 수 있으므로 연속 공백을 하나로 줄여 비교한다.
    std::string collapse_spaces(const std::string &text) {
        std::string result;
        for (char ch : text) {
            if (ch == ' ' && !result.empty() && result.back() == ' ') {
                continue;
            }
            result += ch;
        }
        return result;
    }
}

// =====================================================================
// 생성자 / 기본 동작
// =====================================================================

TEST(MyMatrixConstructor, DefaultCreatesValidMatrix) {
    MyMatrix matrix;

    EXPECT_GE(matrix.row(), 1);
    EXPECT_GE(matrix.column(), 1);
}

TEST(MyMatrixConstructor, SizedConstructorSetsSize) {
    MyMatrix matrix(2, 3);

    EXPECT_EQ(matrix.row(), 2);
    EXPECT_EQ(matrix.column(), 3);
}

TEST(MyMatrixConstructor, SizedConstructorFillsZero) {
    MyMatrix matrix(3, 2);

    expect_matrix(matrix, 3, 2, {0, 0, 0, 0, 0, 0});
}

TEST(MyMatrixInit, FillsInRowMajorOrder) {
    MyMatrix matrix = make_matrix(2, 3, {1, 2, 3, 3, 2, 1});

    EXPECT_EQ(matrix.get(0, 0), 1);
    EXPECT_EQ(matrix.get(0, 2), 3);
    EXPECT_EQ(matrix.get(1, 0), 3);
    EXPECT_EQ(matrix.get(1, 2), 1);
}

TEST(MyMatrixAccess, SetChangesOnlyTargetElement) {
    MyMatrix matrix(2, 2);
    matrix.set(1, 0, 7);

    expect_matrix(matrix, 2, 2, {0, 0, 7, 0});
}

TEST(MyMatrixAccess, OutOfRangeGetReturnsFailValue) {
    MyMatrix matrix(2, 2);

    EXPECT_EQ(matrix.get(2, 0), FAIL_VALUE);
    EXPECT_EQ(matrix.get(0, 2), FAIL_VALUE);
    EXPECT_EQ(matrix.get(-1, 0), FAIL_VALUE);
}

TEST(MyMatrixAccess, OutOfRangeSetIsIgnored) {
    MyMatrix matrix(2, 2);
    matrix.set(5, 5, 7);
    matrix.set(-1, 0, 7);

    expect_matrix(matrix, 2, 2, {0, 0, 0, 0});
}

// =====================================================================
// 복사 생성자 / 대입 연산자 (깊은 복사)
// =====================================================================

TEST(MyMatrixCopy, CopyConstructorCopiesElements) {
    MyMatrix original = make_matrix(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix copy(original);

    expect_matrix(copy, 2, 3, {1, 2, 3, 4, 5, 6});
}

TEST(MyMatrixCopy, CopyConstructorIsDeep) {
    MyMatrix original = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix copy(original);

    copy.set(0, 0, 99);

    EXPECT_EQ(original.get(0, 0), 1);
    EXPECT_EQ(copy.get(0, 0), 99);
}

TEST(MyMatrixCopy, CopyOutlivesOriginal) {
    MyMatrix *original = new MyMatrix(make_matrix(2, 2, {1, 2, 3, 4}));
    MyMatrix copy(*original);
    delete original;

    expect_matrix(copy, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAssign, AssignmentCopiesSizeAndElements) {
    MyMatrix source = make_matrix(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix target;

    target = source;

    expect_matrix(target, 2, 3, {1, 2, 3, 4, 5, 6});
}

TEST(MyMatrixAssign, AssignmentIsDeep) {
    MyMatrix source = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix target;

    target = source;
    target.set(0, 0, 99);

    EXPECT_EQ(source.get(0, 0), 1);
    EXPECT_EQ(target.get(0, 0), 99);
}

TEST(MyMatrixAssign, AssignmentToDifferentSize) {
    MyMatrix target = make_matrix(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    MyMatrix source = make_matrix(1, 2, {5, 6});

    target = source;

    expect_matrix(target, 1, 2, {5, 6});
}

TEST(MyMatrixAssign, SelfAssignmentKeepsElements) {
    MyMatrix matrix = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix &alias = matrix;

    matrix = alias;

    expect_matrix(matrix, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAssign, ChainedAssignment) {
    MyMatrix source = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix first, second;

    first = second = source;

    expect_matrix(first, 2, 2, {1, 2, 3, 4});
    expect_matrix(second, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAssign, AssignTemporary) {
    MyMatrix matrix;

    matrix = MyMatrix(2, 3);

    expect_matrix(matrix, 2, 3, {0, 0, 0, 0, 0, 0});
}

// =====================================================================
// 덧셈
// =====================================================================

TEST(MyMatrixAdd, AddsSameSizeMatrices) {
    MyMatrix left = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix right = make_matrix(2, 2, {1, -1, 0, 2});

    expect_matrix(left + right, 2, 2, {2, 1, 3, 6});
}

TEST(MyMatrixAdd, AddsNonSquareMatrices) {
    MyMatrix left = make_matrix(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix right = make_matrix(2, 3, {6, 5, 4, 3, 2, 1});

    expect_matrix(left + right, 2, 3, {7, 7, 7, 7, 7, 7});
}

TEST(MyMatrixAdd, DifferentSizeReturnsErrorMatrix) {
    MyMatrix left = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix right = make_matrix(2, 3, {1, 2, 3, 3, 2, 1});

    expect_error_matrix(left + right);
}

TEST(MyMatrixAdd, TransposedSizeReturnsErrorMatrix) {
    MyMatrix left(2, 3);
    MyMatrix right(3, 2);

    expect_error_matrix(left + right);
}

TEST(MyMatrixAdd, OperandsAreNotModified) {
    MyMatrix left = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix right = make_matrix(2, 2, {1, -1, 0, 2});

    MyMatrix result = left + right;

    expect_matrix(left, 2, 2, {1, 2, 3, 4});
    expect_matrix(right, 2, 2, {1, -1, 0, 2});
}

// =====================================================================
// 곱셈
// =====================================================================

TEST(MyMatrixMultiply, MultipliesSquareMatrices) {
    MyMatrix left = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix right = make_matrix(2, 2, {1, -1, 0, 2});

    expect_matrix(left * right, 2, 2, {1, 3, 3, 5});
}

TEST(MyMatrixMultiply, ResultHasLeftRowAndRightColumn) {
    MyMatrix left = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix right = make_matrix(2, 3, {1, 2, 3, 3, 2, 1});

    expect_matrix(left * right, 2, 3, {7, 6, 5, 15, 14, 13});
}

TEST(MyMatrixMultiply, InnerDimensionDiffersFromRows) {
    // (2x3) * (3x2) = (2x2): 내부 차원(3)이 행 수(2)와 다를 때도 끝까지 더해야 한다.
    MyMatrix left = make_matrix(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix right = make_matrix(3, 2, {7, 8, 9, 10, 11, 12});

    expect_matrix(left * right, 2, 2, {58, 64, 139, 154});
}

TEST(MyMatrixMultiply, ColumnVectorTimesRowVector) {
    // (3x1) * (1x3) = (3x3)
    MyMatrix left = make_matrix(3, 1, {1, 2, 3});
    MyMatrix right = make_matrix(1, 3, {4, 5, 6});

    expect_matrix(left * right, 3, 3, {4, 5, 6, 8, 10, 12, 12, 15, 18});
}

TEST(MyMatrixMultiply, RowVectorTimesColumnVector) {
    // (1x3) * (3x1) = (1x1)
    MyMatrix left = make_matrix(1, 3, {1, 2, 3});
    MyMatrix right = make_matrix(3, 1, {4, 5, 6});

    expect_matrix(left * right, 1, 1, {32});
}

TEST(MyMatrixMultiply, MismatchedSizeReturnsErrorMatrix) {
    MyMatrix left = make_matrix(2, 3, {1, 2, 3, 3, 2, 1});
    MyMatrix right = make_matrix(2, 2, {1, 2, 3, 4});

    expect_error_matrix(left * right);
}

TEST(MyMatrixMultiply, OperandsAreNotModified) {
    MyMatrix left = make_matrix(2, 2, {1, 2, 3, 4});
    MyMatrix right = make_matrix(2, 3, {1, 2, 3, 3, 2, 1});

    MyMatrix result = left * right;

    expect_matrix(left, 2, 2, {1, 2, 3, 4});
    expect_matrix(right, 2, 3, {1, 2, 3, 3, 2, 1});
}

// =====================================================================
// 입출력
// =====================================================================

TEST(MyMatrixOutput, FormatsEachRowWithBars) {
    // printf 등으로 표준 출력에 직접 쓰지 않고 전달받은 스트림에 써야 한다.
    MyMatrix matrix = make_matrix(2, 2, {1, 2, 3, 4});

    EXPECT_EQ(collapse_spaces(to_string(matrix)), "| 1 2 |\n| 3 4 |\n");
}

TEST(MyMatrixOutput, FormatsNegativeElements) {
    MyMatrix matrix = make_matrix(2, 2, {1, -1, 0, 2});

    EXPECT_EQ(collapse_spaces(to_string(matrix)), "| 1 -1 |\n| 0 2 |\n");
}

TEST(MyMatrixOutput, FormatsErrorMatrix) {
    MyMatrix left(2, 2);
    MyMatrix right(2, 3);

    EXPECT_EQ(collapse_spaces(to_string(left + right)), "| -999 |\n");
}

TEST(MyMatrixOutput, AlignsColumnsToSameWidth) {
    MyMatrix matrix = make_matrix(2, 3, {7, 6, 5, 15, 14, 13});

    std::istringstream lines(to_string(matrix));
    std::string first_line, second_line;
    std::getline(lines, first_line);
    std::getline(lines, second_line);

    EXPECT_EQ(first_line.size(), second_line.size());
    EXPECT_EQ(collapse_spaces(first_line), "| 7 6 5 |");
    EXPECT_EQ(collapse_spaces(second_line), "| 15 14 13 |");
}

TEST(MyMatrixInput, ReadsInRowMajorOrder) {
    MyMatrix matrix(2, 3);
    std::istringstream input("1 2 3 4 5 6");

    input >> matrix;

    expect_matrix(matrix, 2, 3, {1, 2, 3, 4, 5, 6});
}

TEST(MyMatrixInput, ReadsTwoMatricesInChain) {
    MyMatrix first(1, 2), second(2, 1);
    std::istringstream input("1 2 3 4");

    input >> first >> second;

    expect_matrix(first, 1, 2, {1, 2});
    expect_matrix(second, 2, 1, {3, 4});
}

// =====================================================================
// 과제 예시 시나리오
// =====================================================================

TEST(MyMatrixScenario, MatchesAssignmentExample) {
    const int MAX_CNT = 8;
    MyMatrix matArr[MAX_CNT];
    matArr[0] = MyMatrix(2, 2);
    matArr[1] = MyMatrix(2, 2);
    matArr[2] = MyMatrix(2, 3);

    int arr0[] = {1, 2, 3, 4}, arr1[] = {1, -1, 0, 2};
    int arr2[] = {1, 2, 3, 3, 2, 1};
    matArr[0].init(arr0);
    matArr[1].init(arr1);
    matArr[2].init(arr2);

    matArr[3] = matArr[0] + matArr[1];
    matArr[4] = matArr[0] + matArr[2];
    matArr[5] = matArr[0] * matArr[1];
    matArr[6] = matArr[2] * matArr[0];
    matArr[7] = matArr[0] * matArr[2];

    std::ostringstream output;
    for (int i = 0; i < MAX_CNT; i++) {
        output << "-- Matrix " << i << " --\n";
        output << matArr[i] << std::endl;
    }

    EXPECT_EQ(collapse_spaces(output.str()),
              "-- Matrix 0 --\n| 1 2 |\n| 3 4 |\n\n"
              "-- Matrix 1 --\n| 1 -1 |\n| 0 2 |\n\n"
              "-- Matrix 2 --\n| 1 2 3 |\n| 3 2 1 |\n\n"
              "-- Matrix 3 --\n| 2 1 |\n| 3 6 |\n\n"
              "-- Matrix 4 --\n| -999 |\n\n"
              "-- Matrix 5 --\n| 1 3 |\n| 3 5 |\n\n"
              "-- Matrix 6 --\n| -999 |\n\n"
              "-- Matrix 7 --\n| 7 6 5 |\n| 15 14 13 |\n\n");
}
