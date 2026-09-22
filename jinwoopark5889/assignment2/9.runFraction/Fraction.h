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
    void print() const;
    int getGcd(int a, int b);
    void simplify();

private:
    int numerator;
    int denominator;
};