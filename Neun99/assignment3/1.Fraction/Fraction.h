#pragma once
#include <iostream>

using namespace std;

class Fraction {
public:
    //생성자
    Fraction();
    Fraction(int, int);

    //getter
    int getNum() const;
    int getDen() const;

    //연산자 오버로딩
    const Fraction operator+(const Fraction&) const;

private:
    int numerator = 1;
    int denominator = 1;
    void normalize();
    static int getGCD(int, int);
};

ostream& operator<<(ostream&, const Fraction&);