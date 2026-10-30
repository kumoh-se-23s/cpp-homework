
#include <iostream>

#include "MyString.h"

int main() {
    MyString myString3 = "21398102938";
    std::cout << "substr (-8, 8) in \"" << myString3 << "\" : " << myString3.substr(14, 8) << std::endl;

    return 0;
}
