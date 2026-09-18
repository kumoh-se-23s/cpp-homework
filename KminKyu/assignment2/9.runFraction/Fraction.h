#pragma once

class Fraction {
public:
    Fraction(int n = 1, int d = 1);
    Fraction add(const Fraction& fraction) const;
    void set(int n, int d);
    int getNumerator() const;
    int getDenominator() const;
    void print() const;
private:
    int numerator;
    int denominator;
    void organizeFraction();
    static void swap(int&, int&);
    static int getGCD(int n, int d);
};