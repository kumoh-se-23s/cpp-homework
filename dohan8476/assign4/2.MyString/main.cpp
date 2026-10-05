#include <iostream>
#include "MyString.h"
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001); // 출력 인코딩을 UTF-8로 고정
    SetConsoleCP(65001);       // 입력 인코딩을 UTF-8로 고정
#endif
    cout << "================ [1] 기본 생성자 및 입력 테스트 ================" << endl;
    MyString s1;
    cout << "초기 s1 (비어있어야 함): [" << s1 << "]" << endl;
    cout << "s1이 비어있는가? " << (s1.empty() ? "Yes" : "No") << endl;

    cout << "\n문자열을 입력해주세요 (띄어쓰기 포함 가능, 15글자 초과 시 버퍼 잔류): ";
    cin >> s1;
    cout << "입력된 s1: " << s1 << " (길이: " << s1.length() << ")" << endl;

    // MyString leftover;
    // getline(cin, leftover);
    // cout << "버퍼에 남아서 getline이 읽어간 찌꺼기: " << leftover << endl;

    cout << "\n================ [2] 복사 생성자 및 대입 연산자 테스트 ================" << endl;
    MyString s2 = s1; // 복사 생성자
    cout << "s2 (s1 복사본): " << s2 << endl;

    MyString s3;
    s3 = s2; // 대입 연산자 (MyString)
    cout << "s3 (s2 대입): " << s3 << endl;

    s3 = "Hello"; // 대입 연산자 (C 스타일 문자열 배열)
    cout << "s3에 \"Hello\" 대입: " << s3 << endl;

    cout << "\n================ [3] 문자열 연결 (+) 연산자 테스트 ================" << endl;
    MyString s4 = "C++ ";
    MyString s5 = "Programming";
    MyString s6 = s4 + s5; // MyString + MyString
    MyString s7 = s4 + "World"; // MyString + char[]
    cout << "s4 + s5 = " << s6 << endl;
    cout << "s4 + \"World\" = " << s7 << endl;

    cout << "\n================ [4] 동등성 비교 (==, !=) 테스트 ================" << endl;
    MyString alpha = "Test";
    MyString beta = "Test";
    MyString gamma = "Diff";
    cout << "alpha(Test)와 beta(Test)가 같은가? " << (alpha == beta ? "True" : "False") << endl;
    cout << "alpha(Test)와 gamma(Diff)가 다른가? " << (alpha != gamma ? "True" : "False") << endl;

    cout << "\n================ [5] 부분 문자열 검색 (find) 테스트 ================" << endl;
    MyString target = "banana apple banana";
    cout << "대상 문자열: " << target << endl;
    cout << "\"na\"의 위치 (기본 0번부터): " << target.find("na") << endl;
    cout << "\"na\"의 위치 (4번 인덱스부터): " << target.find("na", 4) << endl;
    cout << "\"na\"의 위치 (6번 인덱스 이후부터): " << target.find("na", 6) << endl;
    cout << "\"apple\"의 위치 (0번 인덱스부터): " << target.find("apple") << endl;
    cout << "없는 문자열(cat) 검색 결과: " << target.find("cat") << endl;

    cout << "\n================ [6] 부분 문자열 추출 (substr) 테스트 ================" << endl;
    MyString full = "HelloWorld";
    MyString sub = full.substr(5, 5); // 5번 인덱스부터 5글자
    cout << "원본: " << full << endl;
    cout << "substr(5, 5): " << sub << endl;

    cout << "\n================ [7] getline 테스트 ================" << endl;
    MyString lineStr;
    cout << "문장 전체를 입력하세요 (getline): ";
    // // 버퍼에 남아있을 수 있는 개행 문자 정리 후 테스트
    // cin.ignore(256, '\n');
    getline(cin, lineStr);
    cout << "입력된 문장: " << lineStr << " (길이: " << lineStr.length() << ")" << endl;

    cout << "\n================ 모든 테스트 완료 ================" << endl;
    return 0;
}
