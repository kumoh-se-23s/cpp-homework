#pragma once
#include <iostream>

using namespace std;

class MyString {
public:
    //생성자--------------------
    MyString(); //초기 8byte, 빈 문자열
    MyString(const MyString &srcStr); //복사생성자
    MyString(const char *cstr2nd); //cstring 복사생성자
    ~MyString(); //소멸자

    //기능----------------------
    int length() const;
    bool isEmpty() const;
    int getCapacity() const;
    char& at(int pos) const; //범위 외 예외처리
    int find(const char *cSubStr, int pos = 0) const; //pos 음수 시 0, 범위 밖이면 마지막 위치
    int find(const MyString &subStr, int pos = 0) const;
    MyString subStr(int pos, int len) const; //pos 음수시 뒷글자부터 앞으로, 범위 밖이면 find와 동일, len이 이상하면 최대길이
    MyString& append(const char *cSubStr);
    MyString& append(const MyString &subStr);

    MyString& insert(int pos, const char *csubStr); //pos에 낑가넣기
    MyString& insert(int pos, const MyString &subStr);
    MyString& replace(int pos, const char *csubStr); //pos부터 앞으로 교체
    MyString& replace(int pos, const MyString &subStr);
    MyString& erase(int pos, int cnt); //pos부터 cnt까지 삭제

    //연산자 오버로딩-------------
    const MyString operator+(const char *cstr2nd) const;
    const MyString operator+(const MyString &str2nd) const;
    MyString& operator+=(const char *cstr2nd);
    MyString& operator+=(const MyString &str2nd);
    MyString& operator=(const char *cstr2nd);
    MyString& operator=(const MyString &srcStr);
    char& operator[](int pos); //범위 외 \0 리턴
    const char& operator[](int pos) const;

    bool operator==(const char *cstr2nd);
    bool operator==(const MyString &str2nd);
    bool operator!=(const char *cstr2nd);
    bool operator!=(const MyString &str2nd);
    bool operator<(const char *cstr2nd); //사전순 비교
    bool operator<(const MyString &str2nd);
    bool operator>(const char *cstr2nd);
    bool operator>(const MyString &str2nd);
    bool operator<=(const char *cstr2nd);
    bool operator<=(const MyString &str2nd);
    bool operator>=(const char *str2nd);
    bool operator>=(const MyString &str2nd);


private:
    static constexpr int DEFAULT_CAPACITY = 8;
    char *str;
    int capacity;
    int size;

    void resize();
    static void copyArr(char *copy, const char *src, int srcSize);
    static int getLengthOfChArr(const char *chArr);
    static int getEnoughCapacity(int size);
    void substitute(const char *chArr, int newCapacity, int newSize);
    int findChArr(const char *chArr, int arrSize, int pos) const;
    const MyString subStrImpl(int pos, int len) const;
    void appendCh(char ch);
    void appendChArr(const char *chArr, int chArrSize);
    void insertChArr(const char *chArr, int chArrSize);
    void replaceChArr(const char *chArr, int chArrSize);
    int compareTo(const char *chArr, int chArrSize);
};

ostream& operator<<(ostream& out, const MyString &str);
istream& operator>>(istream& in, MyString &str);
istream& getline(istream& in, MyString &str, char delim = '\n');
//str[pos]부터의 숫자부분을 정수로 변환해 return
//음수처리x, 적절한 내용 아닌 글자면 0
int stoi(const MyString &str, int pos = 0, int base = 10);