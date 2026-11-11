//
// Created by Merutilm on 11/10/26.
//

#pragma once
#include "MyString.h"

struct StringHasher {
    static size_t hash(const MyString &value) {
        const char *data = value.getData();
        size_t seed = 0;
        for (int i = 0; data[i] != '\0'; ++i) {
            //using boost-hasher
            seed ^= data[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
