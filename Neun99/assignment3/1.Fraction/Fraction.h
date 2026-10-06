#pragma once
#include <iostream>

using namespace std;

class Fraction {
public:
    Fraction();
    Fraction(int, int);

    const Fraction operator+(const Fraction&) const;
    int getNum() const;
    int getDen() const;
private:
    int numerator = 1;
    int denominator = 1;
    void normalize();
    static int getGCD(int, int);
};

ostream& operator<<(ostream&, const Fraction&);