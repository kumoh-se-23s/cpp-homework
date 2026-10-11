#pragma once
#include <iostream>

using namespace std;

class MyString {
public:
    //생성자
    MyString();
    MyString(const MyString&);

    //연산자 (15글자 제한 준수할 것)
    MyString operator=(const MyString&); //대입
    MyString& operator=(const char[]); //내용 복사
    const MyString operator+(const char[]); //이어붙이기
    const MyString operator+(const MyString&);
    bool operator==(const MyString&); //동일여부
    bool operator!=(const MyString&); //다름여부
    char operator[](int); //non-safty? 확인필요

    //기능
    int length() const; //사용중 길이 리턴
    bool empty() const; //비었는지
    char at(int) const; //위치의 값 반환 (safty)
    int find(const char[], int); //int 이후부터 겹친 문자열 시작 위치 반환
    int find(const MyString&, int);
    MyString substr(int, int); //뒷값 default 0, 시작점 음수면 0취급, 앞값이 범위 밖이면 -1리턴, 뒷값이 범위 밖이면 

    static const int MAX_SIZE = 16;
private:
    char str[MAX_SIZE];
    int usingSize = 0;
}

ostream& operator<<(ostream&, const MyString&);
istream& operator>>(istream&, MyString&); //cin.get으로, 공백까지, 여분 버퍼 유지
istream& getline(istream&, MyString&, char); //char까지 저장, 기본 \n, 여분 처분