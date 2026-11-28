#ifndef AUTOUID_H
#define AUTOUID_H
#include "../../../base/string/MyString.h"

// Mixin을 한번 구현해보고 싶었습니다..
template<typename Derived, char Prefix>
class AutoUID {
    static constexpr int DIGITS = 5;
    static constexpr int RADIX = 10;

    static int s_last;

    protected:
        static MyString issue();

        static void observe(const MyString &uid);

        static void release_if_last(const MyString &uid);

    private:
        static MyString format(int number);

        static int number_of(const MyString &uid);
};

#include "AutoUID.tpp"

#endif //AUTOUID_H
