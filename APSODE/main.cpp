//
// Created by apsode on 26. 9. 4..
//

#include <iostream>
#include "ptrutil.h"
#include "arrays.h"
#include "lists.h"

using namespace arrays;
using namespace lists;

int* dangerousTest() {
    auto testInteger = ptr_util::make_scoped_pointer<int>(10);
    return testInteger.get();
}

int* refCountTest() {
    auto testInteger = ptr_util::make_counted_pointer<int>(10);
    std::cout << testInteger.useCount() << std::endl;

    {
        auto copiedTestInteger = testInteger;
        std::cout << testInteger.useCount() << std::endl;
        std::cout << copiedTestInteger.useCount() << std::endl;
    }

    std::cout << testInteger.useCount() << std::endl;
}

int dynamicArrayTest() {
    DynamicArray<int> da = DynamicArray<int>();
    da.add(10);
    da.add(20);
    da.add(30);
    da.add(40);
    da.add(50);
    da.add(60);
    for (int i = 0; i < da.getSize(); i++) {
        printf("%d\n", da[i]);
    }

    return 0;
}

int arrayListTest() {
    ArrayList<int> integerList = ArrayList<int>();

    for (int i = 0; i < 10; ++i) {
        integerList.append(10 * i);
    }

    return 0;
}

int main() {
    // ScopedPointer test
    // int* danglingPtr = dangerousTest();
    // std::cout << *danglingPtr << std::endl;

    // ReferenceCountedPointer test
    // refCountTest();

    // dynamicArrayTest();

    arrayListTest();

    return 0;
}
