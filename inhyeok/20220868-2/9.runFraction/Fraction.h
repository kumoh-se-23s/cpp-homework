#pragma once
#include <cmath>
#include <cstdint>

class Fraction {

    int32_t a = 0;
    int32_t b = 0;

    public:
    Fraction(int32_t a = 1, int32_t b = 1);

    void set(int32_t newA, int32_t newB);

    Fraction add(const Fraction other);

    Fraction mul(const Fraction other);

    void print() const;

    uint32_t gcd(uint32_t m, uint32_t n);
};