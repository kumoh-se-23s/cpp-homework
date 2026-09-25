#pragma once
class Fraction {
public:
    Fraction();
    Fraction(int, int);
    void set(int, int);
    Fraction add(Fraction);
    void print();
    int getNum();
    int getDen();
private:
    int numerator = 1;
    int denominator = 1;
    void normalize(int&, int&);
    int getGCD(int, int);
};