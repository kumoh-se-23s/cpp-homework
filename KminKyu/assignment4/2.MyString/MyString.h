#pragma once
#include<iostream>

using namespace std;
class MyString {
public:
    MyString();
    MyString(const MyString &srcStr);
    MyString(const char str2nd[]);
    MyString& operator =(const MyString &srcStr);
    MyString& operator =(const char str2nd[]);
    int length();
    bool empty();
    char at(int pos);
    char operator[](int pos);
    const MyString operator +(const char str2nd[]);
    const MyString operator +(const MyString& str2nd);
    bool operator ==(const MyString &str);
    bool operator !=(const MyString &str);
    int find(const char subStr[], int pos=0);
    int find(const MyString &subStr, int pos=0);
    MyString substr(int pos, int len);
 
    int getCapacity();
    int getNowSize();
    
    void append(char appendChr);
    void append(char appendStr);
    void clear();

private:
    const static int MAX_STR_SIZE = 16;
    char charArray[16];
    int nowSize;
    int capacity = 16 - 1;

    


};

ostream& operator <<(ostream& os, const MyString &str);
istream& operator >>(istream& is, MyString &str);
istream& getline(istream& is, MyString &str, char delim='\n');
