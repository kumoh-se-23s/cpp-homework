#include <iostream>
#include "MyString.h"
#include <limits>

//생성자---------------------------
MyString::MyString() {
    str[usingSize] = '\0';
}

MyString::MyString(const MyString &srcStr) {
    *this = srcStr;
}

//연산자 오버로딩---------------------
//대입
MyString& MyString::operator=(const MyString& srcStr) {
    *this = srcStr.str;
    return *this;
}
MyString& MyString::operator=(const char str2nd[]) {
    int idx = 0; //usingSize 위해 따로 빼둠
    for (; idx < MAX_INPUT_SIZE && str2nd[idx] != '\0'; idx++) {
        str[idx] = str2nd[idx];
    }
    usingSize = idx;
    str[usingSize] = '\0';
    return *this;
}

//+
const MyString MyString::operator+(const char str2nd[]) const {
    MyString resultStr = *this;
    int idx = 0;
    for (; idx + usingSize < MAX_INPUT_SIZE && str2nd[idx] != '\0'; idx++) {
        resultStr.str[usingSize + idx] = str2nd[idx];
    }
    resultStr.usingSize += idx;
    resultStr.str[resultStr.usingSize] = '\0';
    return resultStr;
}
const MyString MyString::operator+(const MyString& str2nd) const {
    return *this + str2nd.str;
}

//==
bool MyString::operator==(const MyString& str2nd) const {
    if (usingSize != str2nd.usingSize) //길이 다르면 바로 false
        return false;
    for (int idx = 0; idx < usingSize; idx++) {
        if (str[idx] != str2nd.str[idx])
            return false;
    }
    return true;
}

//!=
bool MyString::operator!=(const MyString& str2nd) const {
    return !(*this == str2nd);
}

//[]
char MyString::operator[](int pos) const {
    if (pos >= 0 && pos < usingSize)
        return str[pos];
    return '\0';
}

//출력
ostream& operator<<(ostream & out, const MyString& str) {
    for (int idx = 0; idx < str.length(); idx++) {
        out << str.at(idx);
    }
    return out;
}

//whitespace 판별
static bool isWhiteSpace(int chCode) {
    return chCode == ' ' || chCode == '\n' || chCode == '\t' || chCode == '\r';
}

//입력
istream& operator>>(istream& in, MyString& str) {
    str = MyString(); //초기화

    //stream 앞부분 정리
    int inputCode = in.peek();
    for (;isWhiteSpace(inputCode); inputCode = in.peek()) {
        in.ignore();
    }

    //이후 append
    char ch;
    for (int idx = 0; idx < MyString::MAX_INPUT_SIZE && !isWhiteSpace(inputCode); idx++, inputCode = in.peek()) {
        in.get(ch);
        str.append(ch);
    }
    return in;
}

//getline
istream& getline(istream& in, MyString& str, char delim) {
    str = MyString(); //초기화

    int inputCode = in.peek();
    for (int idx = 0; idx < MyString::MAX_INPUT_SIZE && inputCode != delim; idx++, inputCode = in.peek()) {
        char ch;
        in.get(ch);
        str.append(ch);
    }

    in.ignore(numeric_limits<streamsize>::max(),delim);
    return in;
}

//기능------------------------------
int MyString::length() const {
    return usingSize;
}

bool MyString::empty() const {
    return usingSize == 0;
}

char MyString::at(int pos) const {
    if (pos >= 0 && pos < usingSize)
        return str[pos];
    return '\0';
}

int MyString::find(const MyString &subStr, int pos) const {
    if (pos < 0)
        pos = 0;
    if (subStr.usingSize == 0)
        return -1;
    for (int idx = 0; idx + pos < usingSize; idx++) {
        //첫 글자 같으면 substr로 범위 비교
        if (str[idx + pos] == subStr[0]) {
            if (substr(idx + pos, subStr.usingSize) == subStr)
                return idx + pos;
        }
    }
    return -1;
}

int MyString::find(const char subStr[], int pos) const {
    for (int idx = 0; subStr[idx] != '\0'; idx++) {
        if (idx == MAX_INPUT_SIZE) //찾고자 하는 것이 상한선을 넘어가면 무조건 없음
            return -1;
    }

    MyString target;
    target = subStr;
    return find(target, pos);
}

MyString MyString::substr(int pos, int len) const {
    //pos 처리
    if (pos < 0) //음수처리
        pos = 0;
    else if (pos > usingSize) //초과처리
        return MyString();

    //len 처리
    if (len < 0) //음수처리
        len = numeric_limits<int>::max();
    if (len > usingSize - pos) //초과처리
        len = usingSize - pos;

   //동작
    MyString resultStr;
    for (int idx = 0; idx < len; idx++) {
        resultStr.str[idx] = str[idx + pos];
    }
    resultStr.usingSize = len;
    resultStr.str[resultStr.usingSize] = '\0';

    return resultStr;
}

void MyString::append(char ch) {
    if (usingSize < MAX_INPUT_SIZE) {
        str[usingSize] = ch;
        str[++usingSize] = '\0';
    }
}
