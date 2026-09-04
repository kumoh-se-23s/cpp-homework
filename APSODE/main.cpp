//
// Created by apsode on 26. 9. 4..
//

#include <iostream>
#include <ptrutil.h>

int* dangerous_test() {
    auto testInteger = ptr_util::make_scoped_pointer<int>(10);
    return testInteger.get();
}

int* ref_count_test() {
    auto testInteger = ptr_util::make_counted_pointer<int>(10);
    std::cout << testInteger.useCount() << std::endl;

    {
        auto copiedTestInteger = testInteger;
        std::cout << testInteger.useCount() << std::endl;
        std::cout << copiedTestInteger.useCount() << std::endl;
    }

    std::cout << testInteger.useCount() << std::endl;
}

int main() {
    // ScopedPointer test
    int* danglingPtr = dangerous_test();
    std::cout << *danglingPtr << std::endl;

    // ReferenceCountedPointer test
    ref_count_test();
}