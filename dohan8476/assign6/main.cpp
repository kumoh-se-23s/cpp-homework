#include "MyString.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>

int main() {
    // 1. 기본 생성자, C-string 생성자, 복사 생성자, 용량(8의 배수) 검증
    MyString s0;
    assert(s0.length() == 0 && s0.isEmpty() && s0.getCapacity() == 8);

    MyString s1("hello");
    assert(s1.length() == 5 && !s1.isEmpty() && s1.getCapacity() == 8);
    assert(s1 == "hello");

    MyString s2("12345678"); // 길이 8 -> 널 문자 포함해 capacity 16이어야 함
    assert(s2.length() == 8 && s2.getCapacity() == 16);

    MyString s3(s2); // 깊은 복사 생성자
    assert(s3 == s2 && s3.length() == 8 && s3.getCapacity() == 16);

    // 2. 대입 연산자 및 자기 대입
    s0 = s1;
    assert(s0 == "hello" && s0.length() == 5);
    s0 = "world!";
    assert(s0 == "world!" && s0.length() == 6);
    s0 = s0;
    assert(s0 == "world!");

    // 3. at() 예외 처리 및 operator[] 참조 수정/범위 밖 '\0' 반환
    s1[0] = 'H';
    assert(s1 == "Hello");
    assert(s1[-1] == '\0' && s1[100] == '\0');

    // main.cpp의 34~41번째 줄(try-catch 블록)을 아래 한 줄로 교체
    assert(s1.at(-1) == '\0' && s1.at(5) == '\0');

    // 4. 입출력 (>>, <<, getline)
    std::stringstream ss("   hello\tworld\nline1,line2\n");
    MyString in1, in2, line1, line2;
    ss >> in1 >> in2;
    assert(in1 == "hello" && in2 == "world");
    getline(ss, line1, '\n'); // 남은 개행 소비
    getline(ss, line1, ',');
    getline(ss, line2, '\n');
    assert(line1 == "line1" && line2 == "line2");

    // 5. +, +=, append 및 자기 자신 append
    MyString s4 = s1 + " " + s0;
    assert(s4 == "Hello world!" && s4.length() == 12 && s4.getCapacity() == 16);
    s1 += "!";
    assert(s1 == "Hello!");
    s1.append(s1);
    assert(s1 == "Hello!Hello!" && s1.length() == 12);

    // 6. find 및 substr (음수 pos, 초과 len 포함)
    MyString target("abcde12345abcde");
    assert(target.find("abc") == 0);
    assert(target.find("abc", 1) == 10);
    assert(target.find("xyz") == -1);
    assert(target.find("abc", -5) == 0);

    MyString sub = target.substr(5, 5);
    assert(sub == "12345" && sub.length() == 5);
    assert(target.substr(-1, 1) == "e");   // 음수 pos: 맨 뒷글자
    assert(target.substr(-5, 100) == "abcde"); // 초과 len: 최대 길이

    // 7. 비교 연산자 (==, !=, <, <=, >, >=)
    MyString a("abc"), b("abcd"), c("abd");
    assert(a == "abc" && a != b);
    assert(a < b && b < c && a <= "abc" && c > b && b >= a);

    // 8. insert, replace, erase 및 역-resize (8의 배수 유지 확인)
    MyString mod("ac");
    mod.insert(1, "b");
    assert(mod == "abc");
    mod.insert(-10, "0"); // 음수 pos -> 0 위치 삽입
    assert(mod == "0abc");
    mod.insert(100, "9"); // 범위 초과 pos -> 마지막 위치 삽입
    assert(mod == "0abc9");

    mod.replace(1, 3, "XYZ");
    assert(mod == "0XYZ9");

    mod.append("12345678901234567890");
    assert(mod.getCapacity() % 8 == 0);
    mod.erase(1, 100); // 대량 삭제 후 축소 확인
    assert(mod == "0" && mod.length() == 1 && mod.getCapacity() == 8);

    // 9. stoi 진법 변환 (명세서 예시 케이스 검증)
    assert(stoi(MyString("123axyz"), 3, 16) == 10);
    assert(stoi(MyString("123ab"), 3, 16) == 171);
    assert(stoi(MyString("-123defg")) == 0);
    assert(stoi(MyString("123ab456"), 0, 10) == 123);

    std::cout << "모든 테스트를 성공적으로 통과했습니다!" << std::endl;
    return 0;
}