#pragma once
#include <iostream>

using namespace std;

class Fraction {
public:
    //생성자
    Fraction();
    Fraction(int num, int den);

    //getter setter
    int getNum() const;
    int getDen() const;
    void set(int num, int den);

    //연산자 오버로딩
    const Fraction operator+(const Fraction& fra2) const;

private:
    int numerator = 1;
    int denominator = 1;
    void normalize();
    static int getGCD(int, int);
};

ostream& operator<<(ostream& out, const Fraction& fra);