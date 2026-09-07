#include <iostream>
#include <MyArrayList.h>

using namespace dynamic_array;

int testMyArrayList() {
    MyArrayList<int> temp = MyArrayList<int>();
    temp.append(10);
    temp.append(20);
    temp.append(30);
    temp.append(40);
    temp.append(50);

    for (int i = 0; i < 5; ++i) {
        std::cout << temp.get(i) << std::endl;
    }
    return 0;
}

int main() {
    testMyArrayList();
}