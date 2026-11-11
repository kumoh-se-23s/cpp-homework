#include "MyString.h"

int main() {
    MyString str;
    std::cin >> str;
    std::cout << str.substr(-5, -4) << std::endl;
    return 0;
}
