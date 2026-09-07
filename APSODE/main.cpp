//
// Created by apsode on 26. 9. 4..
//

#include <iostream>
#include <ptrutil.h>
#include <dynamic_array.h>

using namespace dynamic_array;

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

    for (int i = 0; i < da.getCurrentSize(); i++) {
        printf("%d", da.get(i));
    }

    return 0;
}

int main() {
    // ScopedPointer test
    // int* danglingPtr = dangerousTest();
    // std::cout << *danglingPtr << std::endl;

    // ReferenceCountedPointer test
    // refCountTest();

    dynamicArrayTest();

    return 0;
}