#pragma once

class Fraction {
    public:
        Fraction();
        Fraction(int numerator, int denominator);
        Fraction add(Fraction a);
        void set(int num, int den);
        void print();


    private:
        int gcd(int num, int den);
        void simplify();
        int numerator;
        int denominator;
};