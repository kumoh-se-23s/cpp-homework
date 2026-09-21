#pragma once

class Fraction {
public:
    Fraction();
    Fraction(int numerator, int denominator);
    Fraction add(Fraction a);
    void set(int num, int den);
    void print();


private:
    int calcGcd(int num, int den);
    void simplify();
    int abs(int num);
    int max(int num1, int num2);
    int min(int num1, int num2);

    int numerator;
    int denominator;
};