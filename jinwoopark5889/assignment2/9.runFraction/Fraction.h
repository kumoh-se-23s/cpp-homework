#pragma once

class Fraction
{
public:
    Fraction();
    Fraction(int numerator, int denominator);
    void set(int numerator, int denomiator);
    int getDenominator();
    int getNumerator();
    Fraction add(Fraction fraction);
    void print();

private:
    int numerator;
    int denominator;
};