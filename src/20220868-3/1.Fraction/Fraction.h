#pragma once
#include <cmath>
#include <cstdint>
#include <iostream>

class Fraction {

    int32_t a = 0;
    int32_t b = 0;

    public:
    Fraction(int32_t a = 1, int32_t b = 1);

    void set(int32_t newA, int32_t newB);

    Fraction &operator=(const Fraction other);

    Fraction operator+(const Fraction other) const;

    Fraction operator*(const Fraction other) const;

    std::string toString() const;

    static uint32_t gcd(uint32_t m, uint32_t n);
};

std::ostream &operator<<(std::ostream &out, const Fraction &v){
    return out << v.toString();
}

