#include <iostream>
using namespace std;

class MyString {
    int capacity;
    int size;
    char* stringArr;

    void resize();
//     bool isCharacter(char inputChar);
    
    public:
        MyString();
        MyString(const MyString& srcStr);
        MyString(const char* sctr2nd); //필수 기능 
        ~MyString();

        void clear();

        int length() const;
        bool isEmpty() const;
        int getCapacity() const;

        void appendChar(char inputChar);

        char& at(int pos);
        char& operator[](int pos);
        const char& operator[](int pos) const;

        int find(const char* cSubStr, int pos=0);
        int find(const MyString& subStr, int pos=0);

        MyString substr(int pos, int len) const;

        MyString& insert(int pos, const char* subStr);
        MyString& inser(int pos, const MyString& subStr);
        MyString& replace(int pos, int len, const char* subStr);
        MyString& replace(int pos, int len, const MyString& subStr);
        MyString& erase(int pos, int cnt);

        const MyString operator+(const char* cstr2nd) const;
        const MyString operator+(const MyString& str2nd) const;

        bool operator==(const MyString& str);
        bool operator==(const char* cstr2nd);
        bool operator!=(const MyString& str);
        bool operator!=(const char* cstr2nd);

        //컴파일러에서 rvalue가 들어오면 이동 대입 연산자가 더 적합하다고 보고 이동 대입 연산자로 작동
        //일반적인 대입 연산자(복사 대입 연산자)는 lvalue(사라지지 않는 값)과 rvalue(사라지는 값) 모두 가능
        MyString operator=(const MyString& srcStr);
        //이동 대입 연산자는 rvalue(사라지는 값)만 가능
        MyString operator=(MyString&& srcStr) noexcept;

        MyString& operator=(const char* cstr2nd);

//---------------------------------------------------------------------

        MyString& operator+=(const char* cstr2nd);
        MyString& operator+=(const MyString& str);

        MyString& append(const char* cstr2nd);
        MyString& append(const MyString& str);

        bool operator<(const MyString& str) const;
        bool operator<=(const MyString& str) const;
        bool operator>(const MyString& str) const;
        bool operator>=(const MyString& str) const;
        
        bool operator<(const char* cstr2nd) const;
        bool operator<=(const char* cstr2nd) const;
        bool operator>(const char* cstr2nd) const;
        bool operator>=(const char* cstr2nd) const;
};

ostream& operator<<(ostream& outputStream, const MyString& str);
istream& operator>>(istream& inputStream, MyString& str);
istream& getline(istream& inputStream, MyString& str, char delim = '\n');

