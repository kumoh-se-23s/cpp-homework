#include <iostream>
#include <sstream>
#include <string>
#include "MySet.h"

using namespace std;

// [주의] 음수(-1) 없이 문자열 끝(EOF)에서 입력을 마치려면
// operator>> 구현부의 반복문을 아래와 같이 작성해야 무한루프에 빠지지 않습니다.
/*
istream& operator>>(istream& is, MySet& set) {
    int value;
    // 값을 정상적으로 읽어왔을 때만 반복 (EOF 도달 시 종료)
    while (is >> value) {
        if (value < 0) break;
        set.insert(value);
    }
    return is;
}
*/

string setToString(const MySet& set) {
    stringstream ss;
    ss << set;
    return ss.str();
}

void assertTest(const string& testName, const MySet& result, const string& expected) {
    string actual = setToString(result);
    if (actual == expected) {
        cout << "  [PASS] " << testName << " : " << actual << "\n";
    } else {
        cout << "  [FAIL] " << testName << "\n";
        cout << "         Expected : " << expected << "\n";
        cout << "         Actual   : " << actual << "\n";
    }
}

void runTestCase(const string& testName, const string& input1, const string& input2,
                 const string& expUnion, const string& expInter,
                 const string& expDiff1, const string& expDiff2) {

    MySet s1, s2, resultSet;
    stringstream ss1(input1);
    stringstream ss2(input2);

    ss1 >> s1;
    ss2 >> s2;

    cout << "========== " << testName << " ==========\n";
    cout << "[입력된 데이터]\n";
    cout << "s1 입력: " << input1 << "\n";
    cout << "s2 입력: " << input2 << "\n\n";

    cout << "[결과 검증]\n";
    assertTest("Union (s1 + s2)", s1 + s2, expUnion);
    assertTest("Inter (s1 & s2)", s1 & s2, expInter);
    assertTest("Diff  (s1 - s2)", s1 - s2, expDiff1);
    assertTest("Diff  (s2 - s1)", s2 - s1, expDiff2);
    cout << "========================================================\n\n";
}

int main() {
    // 1. 소수와 짝수의 만남 (음수 완전 제거)
    runTestCase("테스트 1: 소수와 짝수 집합",
                "19 2 17 3 13 5 11 7",
                "20 18 16 14 12 10 8 6 4 2",
                "{2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20}",
                "{2}",
                "{3, 5, 7, 11, 13, 17, 19}",
                "{4, 6, 8, 10, 12, 14, 16, 18, 20}");

    // 2. 극단적인 중복 폭격 (음수 완전 제거)
    runTestCase("테스트 2: 대량의 중복 데이터",
                "100 50 100 200 50 300 400 400 500 500 500",
                "200 400 600 800",
                "{50, 100, 200, 300, 400, 500, 600, 800}",
                "{200, 400}",
                "{50, 100, 300, 500}",
                "{600, 800}");

    // 3. 교집합이 아예 없는 극단적 케이스
    runTestCase("테스트 3: 상호 배타적(Disjoint) 집합",
                "999 99 9 7 5 3 1",
                "888 88 8 6 4 2 10",
                "{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 88, 99, 888, 999}",
                "{}",
                "{1, 3, 5, 7, 9, 99, 999}",
                "{2, 4, 6, 8, 10, 88, 888}");

    // 4. 완벽히 동일한 집합 입력
    runTestCase("테스트 4: 동일 집합 입력 및 자기 차집합",
                "42 42 42",
                "42",
                "{42}",
                "{42}",
                "{}",
                "{}");

    // 5. 대입 연산자 및 깊은 복사 검증
    cout << "========== 테스트 5: 대입 연산자(깊은 복사) 검증 ==========\n";
    MySet s_orig, s_copy;
    stringstream ss("777 999 888");
    ss >> s_orig;

    // 6. 한쪽 집합이 완전히 비어있는 경우 (s2가 공집합)
    runTestCase("테스트 6: 한쪽이 공집합인 경우",
                "10 20 30 -1", // s1
                "-1",          // s2 (바로 종료)
                "{10, 20, 30}", // Union
                "{}",           // Inter
                "{10, 20, 30}", // Diff (s1 - s2)
                "{}");          // Diff (s2 - s1)

    // 7. 양쪽 모두 비어있는 공집합인 경우
    runTestCase("테스트 7: 양쪽 모두 공집합",
                "-1", // s1
                "-1", // s2
                "{}", // Union
                "{}", // Inter
                "{}", // Diff (s1 - s2)
                "{}"); // Diff (s2 - s1)

    // 8. 완벽한 부분 집합 (s1이 s2에 완전히 포함됨)
    runTestCase("테스트 8: 완벽한 부분 집합 (Subset)",
                "2 4 6 -1", // s1
                "1 2 3 4 5 6 7 -1", // s2
                "{1, 2, 3, 4, 5, 6, 7}", // Union
                "{2, 4, 6}",             // Inter
                "{}",                    // Diff (s1 - s2: 모두 빼앗김)
                "{1, 3, 5, 7}");         // Diff (s2 - s1)

    // 9. 단일 원소(Single element) 집합 간의 연산
    runTestCase("테스트 9: 단일 원소 집합",
                "99 -1",
                "99 -1",
                "{99}",
                "{99}",
                "{}",
                "{}");

    // 10. 완전히 역순으로 입력된 연속된 숫자들 (Shift 연산 극한 테스트)
    runTestCase("테스트 10: 역순 입력 (Shift 부하 테스트)",
                "5 4 3 2 1 -1", // 계속 맨 앞으로 밀어내야 함
                "10 9 8 7 6 -1",
                "{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}",
                "{}",
                "{1, 2, 3, 4, 5}",
                "{6, 7, 8, 9, 10}");

    s_copy = s_orig;
    s_copy = s_copy;

    assertTest("원본 객체 (s_orig)", s_orig, "{777, 888, 999}");
    assertTest("복사 객체 (s_copy)", s_copy, "{777, 888, 999}");
    cout << "========================================================\n";

    return 0;
}