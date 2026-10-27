#include <iostream>
#include <string>
#include "MyMatrix.h"

using namespace std;

// 행렬의 크기와 내부 데이터가 기대값(Expected)과 일치하는지 확인하는 검증 함수
void assertTest(const string& testName, const MyMatrix& actual, int expRow, int expCol, const int* expData) {
    bool isPass = true;

    // 1. 크기(Row, Col) 비교
    if (actual.getRow() != expRow || actual.getCol() != expCol) {
        isPass = false;
    } else {
        // 2. 내부 데이터 비교
        int idx = 0;
        for (int i = 0; i < expRow; i++) {
            for (int j = 0; j < expCol; j++) {
                if (actual.getMatrix(i, j) != expData[idx]) {
                    isPass = false;
                    break;
                }
                idx++;
            }
            if (!isPass) break;
        }
    }

    if (isPass) {
        cout << "  [PASS] " << testName << "\n";
    } else {
        cout << "  [FAIL] " << testName << "\n";
        cout << "         [기대하는 크기] " << expRow << "x" << expCol << "\n";
        cout << "         [실제 나온 결과]\n" << actual << "\n";
    }
}

int main() {

    cout << "====================================================\n";
    cout << "          MyMatrix 통합 자동화 테스트 (확장판)\n";
    cout << "====================================================\n\n";

    // [기본 데이터 셋업]
    MyMatrix m1(2, 2); int arr1[] = {1, 2, 3, 4}; m1.init(arr1);
    MyMatrix m2(2, 2); int arr2[] = {5, -6, -7, 8}; m2.init(arr2); // 음수 포함
    MyMatrix m3(2, 3); int arr3[] = {1, 2, 3, 4, 5, 6}; m3.init(arr3);
    MyMatrix m4(3, 2); int arr4[] = {1, 0, 0, 1, 0, 0}; m4.init(arr4);

    // [특수 데이터 셋업]
    MyMatrix mZero(2, 2); int arrZero[] = {0, 0, 0, 0}; mZero.init(arrZero);
    MyMatrix mIden(2, 2); int arrIden[] = {1, 0, 0, 1}; mIden.init(arrIden);
    MyMatrix m_1x3(1, 3); int arr_1x3[] = {1, 2, 3}; m_1x3.init(arr_1x3);
    MyMatrix m_3x1(3, 1); int arr_3x1[] = {4, 5, 6}; m_3x1.init(arr_3x1);
    MyMatrix m_1x1(1, 1); int arr_1x1[] = {7}; m_1x1.init(arr_1x1);


    cout << "========== [1] 기본 덧셈 연산 테스트 ==========\n";
    int expAdd1[] = {6, -4, -4, 12};
    assertTest("2x2 + 2x2 (음수 포함 덧셈)", m1 + m2, 2, 2, expAdd1);

    int expAdd2[] = {1, 2, 3, 4};
    assertTest("2x2 + 영행렬 (원본 유지)", m1 + mZero, 2, 2, expAdd2);

    int expErr[] = {-999};
    assertTest("크기가 다른 행렬 덧셈 (예외처리)", m1 + m3, 1, 1, expErr);
    cout << "\n";


    cout << "========== [2] 기본 곱셈 연산 테스트 ==========\n";
    int expMul1[] = {-9, 10, -13, 14};
    assertTest("2x2 * 2x2 (음수 포함 곱셈)", m1 * m2, 2, 2, expMul1);

    int expMul2[] = {1, 2, 4, 5};
    assertTest("2x3 * 3x2 직사각형 곱셈", m3 * m4, 2, 2, expMul2);

    assertTest("크기가 안 맞는 곱셈 (예외처리)", m1 * m4, 1, 1, expErr);
    cout << "\n";


    cout << "========== [3] 특수 행렬(영행렬, 단위행렬) 곱셈 ==========\n";
    int expZeroOut[] = {0, 0, 0, 0};
    assertTest("행렬 * 영행렬 = 영행렬", m1 * mZero, 2, 2, expZeroOut);
    assertTest("행렬 * 단위행렬 = 원본행렬", m1 * mIden, 2, 2, arr1);
    assertTest("단위행렬 * 행렬 = 원본행렬", mIden * m1, 2, 2, arr1);
    cout << "\n";


    cout << "========== [4] 극단적인 형태(1xN, Nx1) 교차 곱셈 ==========\n";
    int exp_1x1_cross[] = {32}; // 1*4 + 2*5 + 3*6
    assertTest("1x3 * 3x1 = 1x1 차원 축소", m_1x3 * m_3x1, 1, 1, exp_1x1_cross);

    int exp_3x3_cross[] = {4, 8, 12, 5, 10, 15, 6, 12, 18};
    assertTest("3x1 * 1x3 = 3x3 차원 확장", m_3x1 * m_1x3, 3, 3, exp_3x3_cross);

    int exp_1x1_mul[] = {49};
    assertTest("1x1 * 1x1 단일요소 곱셈", m_1x1 * m_1x1, 1, 1, exp_1x1_mul);
    cout << "\n";


    cout << "========== [5] 디폴트 생성자 & 메모리 안전성(깊은 복사) ==========\n";
    MyMatrix mDefault; // 디폴트 생성자
    assertTest("디폴트 생성자 (-999 초기화)", mDefault, 1, 1, expErr);

    MyMatrix mCopy = m1;
    MyMatrix mAssign;
    mAssign = m2;
    mAssign = mAssign; // 자기 자신 대입(Self-assignment) 방어 로직 검사

    // 원본 파괴
    m1.setMatrix(0, 0, -9999);
    m2.setMatrix(0, 0, -8888);

    assertTest("복사 생성자 깊은 복사 검증", mCopy, 2, 2, arr1);
    assertTest("대입 연산자 자기 대입 및 깊은 복사 검증", mAssign, 2, 2, arr2);

    // 체이닝 연산 (임시 객체 파괴 테스트)
    int expChain[] = {-6, -4, 32, 40};
    assertTest("(mCopy + mAssign) * mCopy 체이닝", (mCopy + mAssign) * mCopy, 2, 2, expChain);
    cout << "\n====================================================\n";

    return 0;
}