#pragma once
#include <iosfwd>

class MyString {
    public:
        static constexpr int DEFAULT_CAPACITY = 8;
        //기본 기능 난이도 하
        MyString(); // 초기 크기 8!!byte의 문자열 공간을 생성하고 빈 문자열로 초기화
        MyString(const MyString& srcStr);
        ~MyString();

        MyString& operator =(const MyString& srcStr);

        [[nodiscard]] int length() const;
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] int getCapacity() const;

        [[nodiscard]] char at(int pos) const;

        char operator[](int pos) const;
        // char& operator[](int pos) const;
        // const char& operator[](int pos) const;

        //기본 기능 난이도 중
        MyString(const char srcStr[]); // 전달되는 문자열에 대한 깊은 복사로 복사생성자 구현
        MyString& operator =(const char cstr2nd[]); // str2nd로부터의 깊은 복사로 대입연산 구현

        MyString operator +(const char str2nd[]) const;
        MyString operator +(const MyString& srcStr) const;

        bool operator ==(const MyString& srcStr) const;
        bool operator ==(const char cstr2nd[]) const;

        bool operator !=(const MyString& srcStr) const;
        bool operator !=(const char cstr2nd[]) const;

        int find(const char subStr[], int pos = 0) const; // 부분 문자열 없음 -1 리턴
        int find(const MyString& subStr, int pos = 0) const;

        [[nodiscard]] MyString substr(int pos, int length) const;

        //기본 난이도 중상
        MyString& operator +=(const MyString& srcStr);
        MyString& operator +=(const char cstr2nd[]);

        MyString& append(const MyString& srcStr);
        MyString& append(const char cstr2nd[]);

        //사전 순서 기준 bool값 반환 (compareTo 만들어야할듯)
        bool operator <(const MyString & srcStr) const;
        bool operator <(const char cstr2nd[]) const;
        bool operator >(const MyString& srcStr) const;
        bool operator >(const char cstr2nd[]) const;

        bool operator <=(const MyString & srcStr) const;
        bool operator <=(const char cstr2nd[]) const;
        bool operator >=(const MyString& srcStr) const;
        bool operator >=(const char cstr2nd[]) const;

        MyString& insert(int pos, const MyString& srcStr);
        MyString& insert(int pos, const char subStr[]);

        MyString& replace(int pos, int len, const MyString& subStr);
        MyString& replace(int pos, int len, const char subStr[]);

        MyString& erase(int pos, int len);

    private:
        int size = 0;
        int capacity = DEFAULT_CAPACITY;
        char *str = nullptr;

        void resize();

        int cstrlen(const char str[]) const;
        int compareTo(const MyString& srcStr) const;

        int findHelper(const char subStr[], int subLen, int pos) const;

        bool compareHelper(const char srcStr[], int strLen) const;

        MyString plusHelper(const char srcStr[], int strLen) const;

        MyString& appendHelper(const char srcStr[], int strLen);
        MyString& insertHelper(int pos, int len, const char srcStr[]);
        MyString& replaceHelper(int pos, int len, const char srcStr[]);
};


//io operator
std::ostream& operator<<(std::ostream& os, const MyString& str);
std::istream& operator>>(std::istream& is, MyString& str);
std::istream& getline(std::istream& is, MyString& str, char delimiter = '\n');

//baseConverter 만들어야함
int stoi(const MyString& str, int pos = 0, int base = 10) const;