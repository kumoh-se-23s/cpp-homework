#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include "MyMatrix.h"

namespace {
    const int FAIL = -999;   // 과제 명세: 크기가 안 맞으면 -999만 저장된 1x1 행렬

    // 행렬이 rows x cols 크기이고 값이 행 우선 순서로 expected와 같은지 확인
    void expectMat(const MyMatrix &m, int rows, int cols, const std::vector<int> &expected) {
        ASSERT_EQ(m.getMaxRow(), rows);
        ASSERT_EQ(m.getMaxColumn(), cols);
        ASSERT_EQ(static_cast<int>(expected.size()), rows * cols);
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                EXPECT_EQ(m.getValue(r, c), expected[r * cols + c]) << "(" << r << "," << c << ")";
            }
        }
    }
    void expectSame(const MyMatrix &a, const MyMatrix &b) {
        ASSERT_EQ(a.getMaxRow(), b.getMaxRow());
        ASSERT_EQ(a.getMaxColumn(), b.getMaxColumn());
        for (int r = 0; r < a.getMaxRow(); ++r)
            for (int c = 0; c < a.getMaxColumn(); ++c)
                EXPECT_EQ(a.getValue(r, c), b.getValue(r, c)) << "(" << r << "," << c << ")";
    }

    void expectFail(const MyMatrix &m) {
        expectMat(m, 1, 1, {FAIL});
    }

    MyMatrix make(int rows, int cols, const std::vector<int> &values) {
        MyMatrix m(rows, cols);
        m.init(values.data());
        return m;
    }

    std::string str(const MyMatrix &m) {
        std::ostringstream oss;
        oss << m;
        return oss.str();
    }
}

// ---------- 생성자 ----------

TEST(MyMatrixConstructorTest, DefaultIsZeroByZero) {
    MyMatrix m;
    EXPECT_EQ(m.getMaxRow(), 0);
    EXPECT_EQ(m.getMaxColumn(), 0);
}

TEST(MyMatrixConstructorTest, SizedIsZeroFilled) {
    MyMatrix m(2, 3);
    expectMat(m, 2, 3, {0, 0, 0, 0, 0, 0});
}

TEST(MyMatrixConstructorTest, OneByOne) {
    expectMat(MyMatrix(1, 1), 1, 1, {0});
}

TEST(MyMatrixConstructorTest, CopyConstructor) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b(a);
    expectMat(b, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixConstructorTest, CopyIsIndependent) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b(a);
    b.setValue(0, 0, 99);
    expectMat(a, 2, 2, {1, 2, 3, 4});
    expectMat(b, 2, 2, {99, 2, 3, 4});
}

TEST(MyMatrixConstructorTest, CopyOfDefault) {
    MyMatrix a;
    MyMatrix b(a);
    EXPECT_EQ(b.getMaxRow(), 0);
    EXPECT_EQ(b.getMaxColumn(), 0);
}

TEST(MyMatrixConstructorTest, ArrayOfDefaultThenAssign) {
    // 과제 코드: MyMatrix matArr[8]; matArr[0] = MyMatrix(2,2);
    MyMatrix arr[3];
    arr[0] = MyMatrix(2, 2);
    arr[1] = MyMatrix(2, 3);
    expectMat(arr[0], 2, 2, {0, 0, 0, 0});
    expectMat(arr[1], 2, 3, {0, 0, 0, 0, 0, 0});
    EXPECT_EQ(arr[2].getMaxRow(), 0);
}

// ---------- init / getValue / setValue ----------

TEST(MyMatrixInitTest, RowMajorOrder) {
    MyMatrix m(2, 3);
    int arr[] = {1, 2, 3, 4, 5, 6};
    m.init(arr);
    EXPECT_EQ(m.getValue(0, 0), 1);
    EXPECT_EQ(m.getValue(0, 2), 3);
    EXPECT_EQ(m.getValue(1, 0), 4);
    EXPECT_EQ(m.getValue(1, 2), 6);
}

TEST(MyMatrixInitTest, UsesOnlyNeededValues) {
    MyMatrix m(2, 2);
    int arr[] = {1, 2, 3, 4, 5, 6, 7};   // 더 많은 값이 있어도 앞 4개만 사용
    m.init(arr);
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixInitTest, OverwritesPreviousValues) {
    MyMatrix m = make(2, 2, {9, 9, 9, 9});
    int arr[] = {1, 2, 3, 4};
    m.init(arr);
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAccessTest, GetValueOutOfRangeReturnsFail) {
    MyMatrix m = make(2, 2, {1, 2, 3, 4});
    EXPECT_EQ(m.getValue(-1, 0), FAIL);
    EXPECT_EQ(m.getValue(0, -1), FAIL);
    EXPECT_EQ(m.getValue(2, 0), FAIL);
    EXPECT_EQ(m.getValue(0, 2), FAIL);
    EXPECT_EQ(m.getValue(100, 100), FAIL);
}

TEST(MyMatrixAccessTest, GetValueOnDefaultReturnsFail) {
    MyMatrix m;
    EXPECT_EQ(m.getValue(0, 0), FAIL);
}

TEST(MyMatrixAccessTest, SetValueValid) {
    MyMatrix m(2, 2);
    m.setValue(1, 0, 7);
    EXPECT_EQ(m.getValue(1, 0), 7);
}

TEST(MyMatrixAccessTest, SetValueOutOfRangeIgnored) {
    MyMatrix m = make(2, 2, {1, 2, 3, 4});
    m.setValue(-1, 0, 99);
    m.setValue(0, 5, 99);
    m.setValue(2, 2, 99);
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

// ---------- operator= ----------

TEST(MyMatrixAssignTest, SameSize) {
    MyMatrix a(2, 2), b = make(2, 2, {1, 2, 3, 4});
    a = b;
    expectMat(a, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAssignTest, ToLargerMatrix) {
    MyMatrix a = make(1, 1, {5});
    MyMatrix b = make(2, 3, {1, 2, 3, 4, 5, 6});
    a = b;
    expectMat(a, 2, 3, {1, 2, 3, 4, 5, 6});
}

TEST(MyMatrixAssignTest, ToSmallerMatrix) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix b = make(1, 1, {5});
    a = b;
    expectMat(a, 1, 1, {5});
}

TEST(MyMatrixAssignTest, DeepCopy) {
    MyMatrix a, b = make(2, 2, {1, 2, 3, 4});
    a = b;
    a.setValue(0, 0, 99);
    expectMat(b, 2, 2, {1, 2, 3, 4});
    expectMat(a, 2, 2, {99, 2, 3, 4});
}

TEST(MyMatrixAssignTest, SelfAssignment) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix &ref = a;
    a = ref;
    expectMat(a, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAssignTest, ChainedAssignment) {
    MyMatrix a, b, c, src = make(2, 2, {1, 2, 3, 4});
    a = b = c = src;
    expectMat(a, 2, 2, {1, 2, 3, 4});
    expectMat(b, 2, 2, {1, 2, 3, 4});
    expectMat(c, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAssignTest, FromDefaultMatrix) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix empty;
    a = empty;
    EXPECT_EQ(a.getMaxRow(), 0);
    EXPECT_EQ(a.getMaxColumn(), 0);
}

TEST(MyMatrixAssignTest, RepeatedAssignmentNoCrash) {
    MyMatrix a;
    for (int i = 1; i <= 50; ++i) {
        a = MyMatrix(i % 7 + 1, i % 5 + 1);
    }
    SUCCEED();
}

// ---------- operator+ ----------

TEST(MyMatrixAddTest, Basic) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {1, -1, 0, 2});
    expectMat(a + b, 2, 2, {2, 1, 3, 6});     // 과제 Matrix 3
}

TEST(MyMatrixAddTest, NonSquare) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix b = make(2, 3, {6, 5, 4, 3, 2, 1});
    expectMat(a + b, 2, 3, {7, 7, 7, 7, 7, 7});
}

TEST(MyMatrixAddTest, Commutative) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {5, 6, 7, 8});
    expectMat(a + b, 2, 2, {6, 8, 10, 12});
    expectMat(b + a, 2, 2, {6, 8, 10, 12});
}

TEST(MyMatrixAddTest, ZeroMatrixIsIdentity) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    expectMat(a + MyMatrix(2, 2), 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixAddTest, DoesNotModifyOperands) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {1, 1, 1, 1});
    MyMatrix c = a + b;
    expectMat(a, 2, 2, {1, 2, 3, 4});
    expectMat(b, 2, 2, {1, 1, 1, 1});
    expectMat(c, 2, 2, {2, 3, 4, 5});
}

TEST(MyMatrixAddTest, Chained) {
    MyMatrix a = make(1, 2, {1, 2});
    expectMat(a + a + a, 1, 2, {3, 6});
}

TEST(MyMatrixAddTest, SizeMismatchBothDifferent) {
    expectFail(MyMatrix(2, 2) + MyMatrix(3, 3));
}

TEST(MyMatrixAddTest, SizeMismatchColumnOnly) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 3, {1, 2, 3, 3, 2, 1});
    expectFail(a + b);                         // 과제 Matrix 4
}

TEST(MyMatrixAddTest, SizeMismatchRowOnly) {
    // 행만 다르고 열은 같은 경우도 실패해야 함
    expectFail(MyMatrix(2, 3) + MyMatrix(3, 3));
    expectFail(MyMatrix(3, 2) + MyMatrix(2, 2));
}

TEST(MyMatrixAddTest, TransposedShapeMismatch) {
    expectFail(MyMatrix(2, 3) + MyMatrix(3, 2));
}

// ---------- operator* ----------

TEST(MyMatrixMultiTest, SquareFromSpec) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {1, -1, 0, 2});
    expectMat(a * b, 2, 2, {1, 3, 3, 5});      // 과제 Matrix 5
}

TEST(MyMatrixMultiTest, NonSquareFromSpec) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix c = make(2, 3, {1, 2, 3, 3, 2, 1});
    expectMat(a * c, 2, 3, {7, 6, 5, 15, 14, 13});   // 과제 Matrix 7
}

TEST(MyMatrixMultiTest, ResultSizeIsRowsOfLeftByColsOfRight) {
    MyMatrix a(2, 3), b(3, 4);
    MyMatrix r = a * b;
    EXPECT_EQ(r.getMaxRow(), 2);
    EXPECT_EQ(r.getMaxColumn(), 4);
}

TEST(MyMatrixMultiTest, RowTimesColumnIsOneByOne) {
    MyMatrix row = make(1, 3, {1, 2, 3});
    MyMatrix col = make(3, 1, {4, 5, 6});
    expectMat(row * col, 1, 1, {32});
}

TEST(MyMatrixMultiTest, ColumnTimesRow) {
    MyMatrix col = make(3, 1, {1, 2, 3});
    MyMatrix row = make(1, 2, {4, 5});
    expectMat(col * row, 3, 2, {4, 5, 8, 10, 12, 15});
}

TEST(MyMatrixMultiTest, IdentityLeftAndRight) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix id = make(2, 2, {1, 0, 0, 1});
    expectMat(a * id, 2, 2, {1, 2, 3, 4});
    expectMat(id * a, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixMultiTest, ZeroMatrix) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    expectMat(a * MyMatrix(2, 2), 2, 2, {0, 0, 0, 0});
}

TEST(MyMatrixMultiTest, NotCommutative) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {0, 1, 1, 0});
    expectMat(a * b, 2, 2, {2, 1, 4, 3});
    expectMat(b * a, 2, 2, {3, 4, 1, 2});
}

TEST(MyMatrixMultiTest, NegativeValues) {
    MyMatrix a = make(2, 2, {-1, -2, -3, -4});
    MyMatrix b = make(2, 2, {1, 1, 1, 1});
    expectMat(a * b, 2, 2, {-3, -3, -7, -7});
}

TEST(MyMatrixMultiTest, DoesNotModifyOperands) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {1, -1, 0, 2});
    MyMatrix c = a * b;
    expectMat(a, 2, 2, {1, 2, 3, 4});
    expectMat(b, 2, 2, {1, -1, 0, 2});
    (void) c;
}

TEST(MyMatrixMultiTest, Chained) {
    MyMatrix a = make(2, 2, {1, 1, 0, 1});
    expectMat(a * a * a, 2, 2, {1, 3, 0, 1});
}

TEST(MyMatrixMultiTest, MismatchFromSpec) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix c = make(2, 3, {1, 2, 3, 3, 2, 1});
    expectFail(c * a);                         // 과제 Matrix 6 (2x3 * 2x2)
}

TEST(MyMatrixMultiTest, MismatchSameShapeNonSquare) {
    expectFail(MyMatrix(2, 3) * MyMatrix(2, 3));
}

TEST(MyMatrixMultiTest, MixedAddAndMulti) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 2, {1, 0, 0, 1});
    expectMat((a + b) * b, 2, 2, {2, 2, 3, 5});
}

// ---------- operator! (transpose) ----------

TEST(MyMatrixTransposeTest, Square) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    expectMat(!a, 2, 2, {1, 3, 2, 4});
}

TEST(MyMatrixTransposeTest, NonSquare) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    expectMat(!a, 3, 2, {1, 4, 2, 5, 3, 6});
}

TEST(MyMatrixTransposeTest, DoubleTransposeIsOriginal) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    expectMat(!(!a), 2, 3, {1, 2, 3, 4, 5, 6});
}

TEST(MyMatrixTransposeTest, OneByOne) {
    expectMat(!make(1, 1, {7}), 1, 1, {7});
}

TEST(MyMatrixTransposeTest, RowBecomesColumn) {
    expectMat(!make(1, 3, {1, 2, 3}), 3, 1, {1, 2, 3});
}

TEST(MyMatrixTransposeTest, DoesNotModifyOriginal) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix t = !a;
    expectMat(a, 2, 3, {1, 2, 3, 4, 5, 6});
    (void) t;
}

TEST(MyMatrixTransposeTest, TransposeOfProduct) {
    // (AB)^T == B^T A^T
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix b = make(3, 2, {7, 8, 9, 10, 11, 12});
    MyMatrix left = !(a * b);
    MyMatrix right = (!b) * (!a);
    expectMat(left, 2, 2, {58, 139, 64, 154});
    expectMat(right, 2, 2, {58, 139, 64, 154});
}

// ---------- operator<< ----------

TEST(MyMatrixOutputTest, TwoByTwo) {
    EXPECT_EQ(str(make(2, 2, {1, 2, 3, 4})), "| 1 2 |\n| 3 4 |\n");
}

TEST(MyMatrixOutputTest, NegativeValueWidthPerColumn) {
    // 열마다 가장 긴 값 기준으로 폭을 맞춤 (과제 Matrix 1)
    EXPECT_EQ(str(make(2, 2, {1, -1, 0, 2})), "| 1 -1 |\n| 0  2 |\n");
}

TEST(MyMatrixOutputTest, TwoByThree) {
    EXPECT_EQ(str(make(2, 3, {1, 2, 3, 3, 2, 1})), "| 1 2 3 |\n| 3 2 1 |\n");
}

TEST(MyMatrixOutputTest, FailMatrix) {
    EXPECT_EQ(str(MyMatrix(2, 2) + MyMatrix(3, 3)), "| -999 |\n");
}

TEST(MyMatrixOutputTest, ColumnWidthAlignment) {
    // 과제 Matrix 7
    EXPECT_EQ(str(make(2, 3, {7, 6, 5, 15, 14, 13})), "|  7  6  5 |\n| 15 14 13 |\n");
}

TEST(MyMatrixOutputTest, ColumnsHaveIndependentWidths) {
    EXPECT_EQ(str(make(2, 2, {1, 100, 22, 3})), "|  1 100 |\n| 22   3 |\n");
}

TEST(MyMatrixOutputTest, AllZeroMatrix) {
    EXPECT_EQ(str(MyMatrix(2, 2)), "| 0 0 |\n| 0 0 |\n");
}

TEST(MyMatrixOutputTest, ZeroColumnNextToWiderColumn) {
    EXPECT_EQ(str(make(2, 2, {0, 10, 0, 20})), "| 0 10 |\n| 0 20 |\n");
}

TEST(MyMatrixOutputTest, OneByOne) {
    EXPECT_EQ(str(make(1, 1, {5})), "| 5 |\n");
}

TEST(MyMatrixOutputTest, Chained) {
    std::ostringstream oss;
    oss << make(1, 1, {1}) << "-" << make(1, 1, {2});
    EXPECT_EQ(oss.str(), "| 1 |\n-| 2 |\n");
}

TEST(MyMatrixOutputTest, EmptyMatrixPrintsNothing) {
    EXPECT_EQ(str(MyMatrix()), "");
}

// ---------- operator>> ----------

TEST(MyMatrixInputTest, ReadsRowMajor) {
    std::istringstream in("1 2 3 4");
    MyMatrix m(2, 2);
    in >> m;
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixInputTest, NonSquare) {
    std::istringstream in("1 2 3 4 5 6");
    MyMatrix m(2, 3);
    in >> m;
    expectMat(m, 2, 3, {1, 2, 3, 4, 5, 6});
}

TEST(MyMatrixInputTest, AcceptsNewlinesAndTabs) {
    std::istringstream in("1 2\n3\t4\n");
    MyMatrix m(2, 2);
    in >> m;
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixInputTest, NegativeValues) {
    std::istringstream in("-1 -2 -3 -4");
    MyMatrix m(2, 2);
    in >> m;
    expectMat(m, 2, 2, {-1, -2, -3, -4});
}

TEST(MyMatrixInputTest, OverwritesPreviousContent) {
    std::istringstream in("1 2 3 4");
    MyMatrix m = make(2, 2, {9, 9, 9, 9});
    in >> m;
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixInputTest, ReadsOnlyNeededValues) {
    std::istringstream in("1 2 3 4 5 6");
    MyMatrix a(2, 2);
    int rest = 0;
    in >> a >> rest;
    expectMat(a, 2, 2, {1, 2, 3, 4});
    EXPECT_EQ(rest, 5);
}

TEST(MyMatrixInputTest, TwoMatricesInRow) {
    std::istringstream in("1 2 3 4 5 6");
    MyMatrix a(1, 2), b(2, 2);
    in >> a >> b;
    expectMat(a, 1, 2, {1, 2});
    expectMat(b, 2, 2, {3, 4, 5, 6});
}

TEST(MyMatrixInputTest, ReturnsStream) {
    std::istringstream in("1 2 3 4");
    MyMatrix a(1, 2), b(1, 2);
    (in >> a) >> b;
    expectMat(a, 1, 2, {1, 2});
    expectMat(b, 1, 2, {3, 4});
}

TEST(MyMatrixInputTest, EmptyMatrixDoesNotCrash) {
    std::istringstream in("1 2 3");
    MyMatrix m;
    in >> m;
    EXPECT_EQ(m.getMaxRow(), 0);
}

// ---------- 과제 PDF 예제 그대로 ----------

TEST(MyMatrixSpecScenarioTest, FullAssignmentExample) {
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

    expectMat(matArr[0], 2, 2, {1, 2, 3, 4});
    expectMat(matArr[1], 2, 2, {1, -1, 0, 2});
    expectMat(matArr[2], 2, 3, {1, 2, 3, 3, 2, 1});
    expectMat(matArr[3], 2, 2, {2, 1, 3, 6});
    expectFail(matArr[4]);
    expectMat(matArr[5], 2, 2, {1, 3, 3, 5});
    expectFail(matArr[6]);
    expectMat(matArr[7], 2, 3, {7, 6, 5, 15, 14, 13});
}

TEST(MyMatrixSpecScenarioTest, FullAssignmentOutput) {
    MyMatrix m0 = make(2, 2, {1, 2, 3, 4});
    MyMatrix m1 = make(2, 2, {1, -1, 0, 2});
    MyMatrix m2 = make(2, 3, {1, 2, 3, 3, 2, 1});

    std::ostringstream oss;
    oss << m0 << "\n"
        << m1 << "\n"
        << m2 << "\n"
        << (m0 + m1) << "\n"
        << (m0 + m2) << "\n"
        << (m0 * m1) << "\n"
        << (m2 * m0) << "\n"
        << (m0 * m2) << "\n";

    const std::string expected =
        "| 1 2 |\n| 3 4 |\n\n"
        "| 1 -1 |\n| 0  2 |\n\n"
        "| 1 2 3 |\n| 3 2 1 |\n\n"
        "| 2 1 |\n| 3 6 |\n\n"
        "| -999 |\n\n"
        "| 1 3 |\n| 3 5 |\n\n"
        "| -999 |\n\n"
        "|  7  6  5 |\n| 15 14 13 |\n\n";
    EXPECT_EQ(oss.str(), expected);
}

// =====================================================
// 1. 크기 축: 0, 음수, 1x1, 모든 크기 조합
// =====================================================

TEST(MyMatrixEdgeSizeTest, ZeroRows) {
    MyMatrix m(0, 3);
    EXPECT_EQ(m.getMaxRow(), 0);
    EXPECT_EQ(m.getMaxColumn(), 3);
    EXPECT_EQ(m.getValue(0, 0), FAIL);
    EXPECT_EQ(str(m), "");
}

TEST(MyMatrixEdgeSizeTest, ZeroColumnsDoesNotCrash) {
    MyMatrix m(3, 0);
    EXPECT_EQ(m.getMaxRow(), 3);
    EXPECT_EQ(m.getMaxColumn(), 0);
    EXPECT_EQ(m.getValue(0, 0), FAIL);
    EXPECT_NO_THROW(str(m));
}

TEST(MyMatrixEdgeSizeTest, CopyAndAssignZeroSized) {
    MyMatrix a(3, 0);
    MyMatrix b(a);
    EXPECT_EQ(b.getMaxRow(), 3);
    EXPECT_EQ(b.getMaxColumn(), 0);

    MyMatrix c = make(2, 2, {1, 2, 3, 4});
    c = MyMatrix(0, 3);
    EXPECT_EQ(c.getMaxRow(), 0);
    EXPECT_EQ(c.getMaxColumn(), 3);
}

TEST(MyMatrixEdgeSizeTest, NegativeRowIsHandledSafely) {
    // 명세에 없는 입력: 예외를 던지거나, 던지지 않으면 음수 크기가 남아있지 않아야 함
    try {
        MyMatrix m(-1, 2);
        EXPECT_GE(m.getMaxRow(), 0);
        EXPECT_GE(m.getMaxColumn(), 0);
    } catch (const std::exception &) {
        SUCCEED();
    }
}

TEST(MyMatrixEdgeSizeTest, NegativeColumnIsHandledSafely) {
    // 주의: 현재 코드는 matrixArray를 먼저 할당한 뒤 new int[-1]에서 던지므로 메모리 누수 (ASAN leak 검출)
    try {
        MyMatrix m(2, -1);
        EXPECT_GE(m.getMaxRow(), 0);
        EXPECT_GE(m.getMaxColumn(), 0);
    } catch (const std::exception &) {
        SUCCEED();
    }
}

TEST(MyMatrixEdgeSizeTest, ZeroSizedAddAndMulti) {
    MyMatrix def;
    MyMatrix d2 = def + MyMatrix();
    EXPECT_EQ(d2.getMaxRow(), 0);
    EXPECT_EQ(d2.getMaxColumn(), 0);

    MyMatrix d3 = def * MyMatrix();
    EXPECT_EQ(d3.getMaxRow(), 0);
    EXPECT_EQ(d3.getMaxColumn(), 0);

    expectFail(def + MyMatrix(1, 1));
    expectFail(def * MyMatrix(1, 1));
}

TEST(MyMatrixEdgeSizeTest, AddZeroRowsDifferentColumns) {
    // [현재 코드 실패 예상] 행은 같고(0) 열만 다름 -> && 버그로 실패 처리가 안 됨
    expectFail(MyMatrix(0, 3) + MyMatrix(0, 2));
}

TEST(MyMatrixEdgeSizeTest, MultiWithZeroInnerDimension) {
    // (2x0) * (0x3) = 2x3 영행렬 (수학적으로 올바른 결과)
    expectMat(MyMatrix(2, 0) * MyMatrix(0, 3), 2, 3, {0, 0, 0, 0, 0, 0});
}

TEST(MyMatrixEdgeSizeTest, MultiWithZeroOuterDimension) {
    MyMatrix r = MyMatrix(0, 2) * MyMatrix(2, 3);
    EXPECT_EQ(r.getMaxRow(), 0);
    EXPECT_EQ(r.getMaxColumn(), 3);
}

TEST(MyMatrixEdgeSizeTest, TransposeZeroSized) {
    MyMatrix t = !MyMatrix(0, 3);
    EXPECT_EQ(t.getMaxRow(), 3);
    EXPECT_EQ(t.getMaxColumn(), 0);
    MyMatrix u = !MyMatrix(3, 0);
    EXPECT_EQ(u.getMaxRow(), 0);
    EXPECT_EQ(u.getMaxColumn(), 3);
}

TEST(MyMatrixEdgeSizeTest, AddFailsIffShapesDiffer_AllShapes1to3) {
    // [현재 코드 실패 예상] 행만/열만 다른 조합에서 실패 처리가 안 되고 범위 밖 읽기 발생
    for (int r1 = 1; r1 <= 3; ++r1)
        for (int c1 = 1; c1 <= 3; ++c1)
            for (int r2 = 1; r2 <= 3; ++r2)
                for (int c2 = 1; c2 <= 3; ++c2) {
                    SCOPED_TRACE("a=" + std::to_string(r1) + "x" + std::to_string(c1) +
                                 " b=" + std::to_string(r2) + "x" + std::to_string(c2));
                    MyMatrix r = MyMatrix(r1, c1) + MyMatrix(r2, c2);
                    if (r1 == r2 && c1 == c2) {
                        EXPECT_EQ(r.getMaxRow(), r1);
                        EXPECT_EQ(r.getMaxColumn(), c1);
                    } else {
                        expectFail(r);
                    }
                }
}

TEST(MyMatrixEdgeSizeTest, MultiFailsIffInnerDimsDiffer_AllShapes1to3) {
    for (int r1 = 1; r1 <= 3; ++r1)
        for (int c1 = 1; c1 <= 3; ++c1)
            for (int r2 = 1; r2 <= 3; ++r2)
                for (int c2 = 1; c2 <= 3; ++c2) {
                    SCOPED_TRACE("a=" + std::to_string(r1) + "x" + std::to_string(c1) +
                                 " b=" + std::to_string(r2) + "x" + std::to_string(c2));
                    MyMatrix r = MyMatrix(r1, c1) * MyMatrix(r2, c2);
                    if (c1 == r2) {
                        EXPECT_EQ(r.getMaxRow(), r1);
                        EXPECT_EQ(r.getMaxColumn(), c2);
                    } else {
                        expectFail(r);
                    }
                }
}

// =====================================================
// 2. 인덱스 경계
// =====================================================

TEST(MyMatrixEdgeIndexTest, LastCellIsAccessible) {
    MyMatrix m = make(2, 3, {1, 2, 3, 4, 5, 6});
    EXPECT_EQ(m.getValue(1, 2), 6);
    m.setValue(1, 2, 60);
    EXPECT_EQ(m.getValue(1, 2), 60);
}

TEST(MyMatrixEdgeIndexTest, OneAfterLastIsInvalid) {
    MyMatrix m = make(2, 3, {1, 2, 3, 4, 5, 6});
    EXPECT_EQ(m.getValue(2, 0), FAIL);
    EXPECT_EQ(m.getValue(0, 3), FAIL);
    EXPECT_EQ(m.getValue(2, 3), FAIL);
}

TEST(MyMatrixEdgeIndexTest, ExtremeIndices) {
    MyMatrix m = make(2, 2, {1, 2, 3, 4});
    EXPECT_EQ(m.getValue(INT_MIN, 0), FAIL);
    EXPECT_EQ(m.getValue(0, INT_MIN), FAIL);
    EXPECT_EQ(m.getValue(INT_MAX, 0), FAIL);
    EXPECT_EQ(m.getValue(0, INT_MAX), FAIL);
    m.setValue(INT_MAX, INT_MAX, 1);
    m.setValue(INT_MIN, INT_MIN, 1);
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

// =====================================================
// 3. 값 축: 극값, 0, 음수, 정렬
// =====================================================

TEST(MyMatrixEdgeValueTest, ExtremeValuesStoredAndPrinted) {
    EXPECT_EQ(str(make(1, 1, {INT_MIN})), "| -2147483648 |\n");
    EXPECT_EQ(str(make(1, 1, {INT_MAX})), "| 2147483647 |\n");
}

TEST(MyMatrixEdgeValueTest, ZeroAndNegativeInSameColumn) {
    EXPECT_EQ(str(make(2, 1, {0, -5})), "|  0 |\n| -5 |\n");
}

TEST(MyMatrixEdgeValueTest, NegativeThreeDigitAlignment) {
    EXPECT_EQ(str(make(2, 1, {-100, 5})), "| -100 |\n|    5 |\n");
}

TEST(MyMatrixEdgeValueTest, ManyDigitsPerColumn) {
    EXPECT_EQ(str(make(2, 2, {1, 123456, 100000, 2})),
              "|      1 123456 |\n| 100000      2 |\n");
}

TEST(MyMatrixEdgeValueTest, OutputDoesNotLeakStreamWidth) {
    std::ostringstream oss;
    oss << make(1, 1, {1}) << 7;
    EXPECT_EQ(oss.str(), "| 1 |\n7");
}

TEST(MyMatrixEdgeValueTest, AdditiveInverseGivesZero) {
    MyMatrix a = make(2, 2, {1, -2, 3, -4});
    MyMatrix neg = make(2, 2, {-1, 2, -3, 4});
    expectMat(a + neg, 2, 2, {0, 0, 0, 0});
}

// =====================================================
// 4. 상태 축: 별칭(aliasing), const, 반복
// =====================================================

TEST(MyMatrixEdgeAliasTest, AddToSelf) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    a = a + a;
    expectMat(a, 2, 2, {2, 4, 6, 8});
}

TEST(MyMatrixEdgeAliasTest, MultiplySquareBySelf) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    a = a * a;
    expectMat(a, 2, 2, {7, 10, 15, 22});
}

TEST(MyMatrixEdgeAliasTest, AssignTransposeChangesShape) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    a = !a;
    expectMat(a, 3, 2, {1, 4, 2, 5, 3, 6});
}

TEST(MyMatrixEdgeAliasTest, AssignProductChangesShape) {
    MyMatrix a = make(2, 3, {1, 0, 0, 0, 1, 0});
    MyMatrix b = make(3, 4, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    a = a * b;
    expectMat(a, 2, 4, {1, 2, 3, 4, 5, 6, 7, 8});
}

TEST(MyMatrixEdgeAliasTest, AssignFailResultToSelf) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    a = a + MyMatrix(5, 5);
    expectFail(a);
}

TEST(MyMatrixEdgeAliasTest, RepeatedAccumulate) {
    MyMatrix acc(2, 2);
    MyMatrix one = make(2, 2, {1, 2, 3, 4});
    for (int i = 0; i < 1000; ++i) acc = acc + one;
    expectMat(acc, 2, 2, {1000, 2000, 3000, 4000});
}

TEST(MyMatrixEdgeConstTest, ConstObjectsAreUsable) {
    const MyMatrix a = make(2, 2, {1, 2, 3, 4});
    const MyMatrix b = make(2, 2, {1, 0, 0, 1});
    EXPECT_EQ(a.getMaxRow(), 2);
    EXPECT_EQ(a.getMaxColumn(), 2);
    EXPECT_EQ(a.getValue(1, 1), 4);
    expectMat(a + b, 2, 2, {2, 2, 3, 5});
    expectMat(a * b, 2, 2, {1, 2, 3, 4});
    expectMat(!a, 2, 2, {1, 3, 2, 4});
    EXPECT_EQ(str(a), "| 1 2 |\n| 3 4 |\n");
}

TEST(MyMatrixEdgeConstTest, TemporaryBoundToConstRef) {
    const MyMatrix &r = make(1, 1, {1}) + make(1, 1, {2});
    expectMat(r, 1, 1, {3});
}

// =====================================================
// 5. 연산 성질 (property)
// =====================================================

TEST(MyMatrixEdgePropertyTest, MultiIsAssociative) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix b = make(3, 2, {1, 0, 2, 1, 0, 3});
    MyMatrix c = make(2, 2, {2, 1, 1, 3});
    expectSame((a * b) * c, a * (b * c));
}

TEST(MyMatrixEdgePropertyTest, MultiDistributesOverAdd) {
    MyMatrix a = make(2, 2, {1, 2, 3, 4});
    MyMatrix b = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix c = make(2, 3, {6, 5, 4, 3, 2, 1});
    expectSame(a * (b + c), a * b + a * c);
}

TEST(MyMatrixEdgePropertyTest, TransposeOfSum) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix b = make(2, 3, {7, 8, 9, 10, 11, 12});
    expectSame(!(a + b), (!a) + (!b));
}

TEST(MyMatrixEdgePropertyTest, TransposeOfProductReversesOrder) {
    MyMatrix a = make(2, 3, {1, 2, 3, 4, 5, 6});
    MyMatrix b = make(3, 2, {7, 8, 9, 10, 11, 12});
    expectSame(!(a * b), (!b) * (!a));
}

// =====================================================
// 6. 스트림 입력 실패
// =====================================================
// 권장 동작: 입력이 실패하면 in.fail()이 true이고 행렬은 바뀌지 않는다.

TEST(MyMatrixEdgeInputTest, NonNumericInput) {
    // [현재 코드 실패 예상] 초기화 안 된 inputValue가 행렬에 복사됨
    std::istringstream in("1 2 x 4");
    MyMatrix m = make(2, 2, {9, 9, 9, 9});
    in >> m;
    EXPECT_TRUE(in.fail());
    expectMat(m, 2, 2, {9, 9, 9, 9});
}

TEST(MyMatrixEdgeInputTest, TooFewValues) {
    // [현재 코드 실패 예상]
    std::istringstream in("1 2");
    MyMatrix m = make(2, 2, {9, 9, 9, 9});
    in >> m;
    EXPECT_TRUE(in.fail());
    expectMat(m, 2, 2, {9, 9, 9, 9});
}

TEST(MyMatrixEdgeInputTest, EmptyInput) {
    // [현재 코드 실패 예상]
    std::istringstream in("");
    MyMatrix m = make(1, 1, {9});
    in >> m;
    EXPECT_TRUE(in.fail());
    expectMat(m, 1, 1, {9});
}

TEST(MyMatrixEdgeInputTest, IntOverflowInput) {
    // [현재 코드 실패 예상]
    std::istringstream in("99999999999 2 3 4");
    MyMatrix m = make(2, 2, {9, 9, 9, 9});
    in >> m;
    EXPECT_TRUE(in.fail());
    expectMat(m, 2, 2, {9, 9, 9, 9});
}

TEST(MyMatrixEdgeInputTest, StreamAlreadyFailed) {
    // [현재 코드 실패 예상]
    std::istringstream in("1 2 3 4");
    in.setstate(std::ios::failbit);
    MyMatrix m = make(2, 2, {9, 9, 9, 9});
    in >> m;
    expectMat(m, 2, 2, {9, 9, 9, 9});
}

TEST(MyMatrixEdgeInputTest, LeadingAndTrailingWhitespace) {
    std::istringstream in("  \n 1 2 \n 3 4 \n\n");
    MyMatrix m(2, 2);
    in >> m;
    EXPECT_FALSE(in.fail());
    expectMat(m, 2, 2, {1, 2, 3, 4});
}

TEST(MyMatrixEdgeInputTest, PlusSignAccepted) {
    std::istringstream in("+1 -2 +3 4");
    MyMatrix m(2, 2);
    in >> m;
    expectMat(m, 2, 2, {1, -2, 3, 4});
}

TEST(MyMatrixEdgeInputTest, ExtremeValues) {
    std::istringstream in("-2147483648 2147483647");
    MyMatrix m(1, 2);
    in >> m;
    expectMat(m, 1, 2, {INT_MIN, INT_MAX});
}

// =====================================================
// 7. 수명 / 메모리 (ASAN 또는 valgrind와 함께 돌릴 것)
// =====================================================

TEST(MyMatrixEdgeLifetimeTest, CreateDestroyManySizes) {
    for (int i = 0; i < 2000; ++i) {
        MyMatrix m(i % 7 + 1, i % 5 + 1);
        MyMatrix copy(m);
        MyMatrix assigned;
        assigned = copy;
        EXPECT_EQ(assigned.getMaxRow(), m.getMaxRow());
    }
}

TEST(MyMatrixEdgeLifetimeTest, VectorOfMatricesReallocation) {
    // 벡터 재할당 시 복사 생성자가 올바르게 동작해야 함
    std::vector<MyMatrix> v;
    for (int i = 0; i < 30; ++i) v.push_back(make(1, 2, {i, i * 2}));
    for (int i = 0; i < 30; ++i) expectMat(v[i], 1, 2, {i, i * 2});
}

TEST(MyMatrixEdgeLifetimeTest, ArrayOfMatricesAssignedFromTemporaries) {
    MyMatrix arr[5];
    for (int i = 0; i < 5; ++i) arr[i] = make(1, 1, {i}) + make(1, 1, {10});
    for (int i = 0; i < 5; ++i) expectMat(arr[i], 1, 1, {i + 10});
}

TEST(MyMatrixEdgeLifetimeTest, ResultIndependentOfOperandsAfterTheyDie) {
    MyMatrix result;
    {
        MyMatrix a = make(2, 2, {1, 2, 3, 4});
        MyMatrix b = make(2, 2, {1, 1, 1, 1});
        result = a + b;
    }
    expectMat(result, 2, 2, {2, 3, 4, 5});
}

TEST(MyMatrixEdgeLifetimeTest, LargeMatrixIdentityProduct) {
    const int n = 100;
    MyMatrix id(n, n);
    for (int i = 0; i < n; ++i) id.setValue(i, i, 1);
    MyMatrix p = id * id;
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            ASSERT_EQ(p.getValue(r, c), r == c ? 1 : 0);
}
