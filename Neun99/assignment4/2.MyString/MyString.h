#pragma once
#include <iostream>

using namespace std;

class MyString {
public:
    //생성자
    MyString();
    MyString(const MyString& srcStr);

    //연산자
    MyString& operator=(const MyString& srcStr);
    MyString& operator=(const char str2nd[]);
    const MyString operator+(const MyString& str2nd) const;
    const MyString operator+(const char str2nd[]) const;
    bool operator==(const MyString& str2nd) const;
    bool operator!=(const MyString& str2nd) const;
    char operator[](int pos) const;

    //기능
    int length() const;
    bool empty() const;
    char at(int pos) const;
    int find(const MyString& subStr, int pos = 0) const;
    int find(const char subStr[], int pos = 0) const;
    MyString substr(int pos, int len) const;

    void append(char ch);

    static constexpr int MAX_INPUT_SIZE = 15;
private:
    char str[MAX_INPUT_SIZE + 1];
    int usingSize = 0;
};

ostream& operator<<(ostream& out, const MyString& str);
istream& operator>>(istream& in, MyString& str );
istream& getline(istream& in, MyString& str, char delim = '\n');