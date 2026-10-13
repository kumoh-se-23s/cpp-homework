#pragma once
#include <iostream>

using namespace std;

class MyString {
public:
    //생성자
    MyString();
    MyString(const MyString& srcStr);

    //연산자 (15글자 제한 준수)
    MyString operator=(const MyString& srcStr) const; //대입
    MyString& operator=(const char str2nd[]) const; //내용 복사
    const MyString operator+(const MyString& str2nd) const; //이어붙이기
    const MyString operator+(const char str2nd[]) const;
    bool operator==(const MyString& str) const; //동일여부
    bool operator!=(const MyString& str) const; //다름여부
    char operator[](int pos) const;

    //기능
    int length() const; //사용중 길이 리턴
    bool empty() const; //비었는지
    char at(int pos) const; //위치의 값 반환
    int find(const MyString& subStr, int pos = 0) const; //int 이후부터 겹친 문자열 시작 위치 반환
    int find(const char subStr[], int pos = 0) const;
    MyString substr(int pos, int len) const; //뒷값 default 0, 시작점 음수면 0취급, 앞값이 범위 밖이면 -1리턴
    static constexpr int MAX_SIZE = 15;
private:
    char str[MAX_SIZE + 1];
    int usingSize = 0;
};

ostream& operator<<(ostream& out, const MyString& str);
istream& operator>>(istream& in, MyString& str ); //cin.get으로, 공백까지, 여분 버퍼 유지
istream& getline(istream& in, MyString& str, char delim = '\n'); //char까지 저장, 기본 \n, 여분 처분