#include <iostream>

using namespace std;

#pragma once
class Fraction {
public:
    Fraction();
    Fraction(int, int);

    void set(int, int);
    const Fraction operator+(const Fraction&) const;
    int getNum() const;
    int getDen() const;
private:
    int numerator = 1;
    int denominator = 1;
    void normalize(int&, int&);
    int getGCD(int, int);
};

ostream& operator<<(ostream&, const Fraction&);