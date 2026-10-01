#include "MyString.h"


MyString::MyString(const MyString &other) {
    size = other.size;
    for(int i = 0; i < other.size; ++i) {
        data[i] = other[i];
    }
    data[size] = '\0';
};
MyString &MyString::operator=(const MyString &other) {
    size = other.size;
    for(int i = 0; i < other.size; ++i) {
        data[i] = other[i];
    }
    data[size] = '\0';
    return *this;
};
MyString &MyString::operator=(const char str2nd[]) {
    size = 0;
    for(int i = 0; str2nd[i] != '\0' && size < LEN; ++i) {
        data[i] = str2nd[i];
        ++size;
    }
    data[size] = '\0';
    return *this;
};

MyString MyString::operator+(const char str2nd[])
{
    MyString result;
    for(int i = 0; i < size; ++i){
        result.data[i] = data[i];
    }
    
    int total;
    for(total = size; total < LEN && str2nd[total - size] != '\0'; ++total){
        result.data[total] = data[total];
    }

    result.data[total] = '\0';
    return result;
}

MyString MyString::operator+(const MyString &other)
{
    MyString result;
    for(int i = 0; i < size; ++i){
        result.data[i] = data[i];
    }
    
    for(int i = 0; i < other.size; ++i){
        result.data[i] = other.data[i];
    }

    result.data[size + other.size] = '\0';
    return result;
}

bool MyString::operator==(const MyString &str) const
{
    if(size != str.size) return false;
    for(int i = 0; i < size; ++i) {
        if(data[i] != str.data[i]) return false;
    }
    return true;
};

bool MyString::operator!=(const MyString &str) const{
    return !(*this == str);
}

int MyString::find(const MyString &subStr, int pos) const {

    pos = std::max(0, pos);
    for(int i = pos; i < size; ++i){
        int idx = i;
        for(int j = 0; j < subStr.size; ++j){
            if(i + j >= size) return -1;
            if(data[i + j] != subStr[j]) idx = -1;
        }
        if(idx >= 0) return idx;
    }
    return -1;
};
int MyString::find(const char *subStr, int pos) const {
    pos = std::max(0, pos);
    for(int i = pos; i < size; ++i){
        int idx = i;
        for(int j = 0; subStr[j] != '\0'; ++j){
            if(i + j >= size) return -1;
            if(data[i + j] != subStr[j]) idx = -1;
        }
        if(idx >= 0) return idx;
    }
    return -1;
}
MyString MyString::subStr(int pos, int len) const
{   
    pos = std::max(0, pos);
    char buf[LEN + 1];
    int actualLen;
    for(actualLen = 0; actualLen < len && pos + actualLen < size; ++actualLen){
        buf[actualLen] = data[pos + actualLen];
    }
    buf[actualLen] = '\0';
    MyString result;
    result = buf;
    return result;
};

int MyString::length() const {
    return size;
};
bool MyString::empty() const {
    return size == 0;
};
char MyString::at(int pos) const {
    if(pos >= size) return -1;
    return data[pos];
};
char MyString::operator[](int pos) const {
    if(pos >= size) return -1;
    return data[pos];
}
std::istream &MyString::getline(std::istream &in, MyString &str, char delim)
{
    char buf[LEN + 1];
    char ch;
    int len;
    for(len = 0; len < LEN && (ch = in.get()) != delim; ++len){
        buf[len] = ch;
    }
    buf[len] = '\0';
    str = buf;
    return in;
};

// int main(){
//     MyString myString;
//     MyString myString2 = myString;
//     MyString myString3 = myString2;
//     std::cin >> myString;
//     std::cin >> myString2;
//     std::cin >> myString3;

//     std::cout << myString << std::endl;
//     std::cout << myString2 << std::endl;
//     std::cout << myString3 << std::endl;


//     std::cout << "aaaa location in \"" << myString << "\" : " << myString.find("aaaa") << std::endl;
//     std::cout << "aaa location in \"" << myString2 << "\" : " << myString2.find("aaa") << std::endl;
//     std::cout << "aa location in \"" << myString3 << "\" : " << myString3.find("aa") << std::endl;
//     std::cout << "a location in \"" << myString3 << "\" : " << myString3.find("a") << std::endl;
//     std::cout << "substr (2, 8) in \"" << myString3 << "\" : " << myString3.subStr(2, 8) << std::endl;
//     std::cout << myString2 << " location in \"" << myString << "\" : " << myString.find(myString2) << std::endl;
//     std::cout << myString3 << " location in \"" << myString << "\" : " << myString.find(myString3) << std::endl;
//     std::cout << "4-th value in \"" << myString << "\" : " << myString.at(4) << std::endl;
//     std::cout << "5-th value location in \"" << myString << "\" : " << myString.at(5) << std::endl;
    
//     return 0;
// }