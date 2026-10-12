#pragma once
#include <iosfwd>

class Fraction {
    public:
        Fraction();
        Fraction(int numerator, int denominator);

        void set(int num, int den);
        int getNumerator() const;
        int getDenominator() const;

        Fraction operator+(const Fraction& a) const;
        Fraction& operator=(const Fraction& a);

    private:
        static int calcGCD(int num, int den);
        void simplify();

        static int abs(int num);

        static int max(int num1, int num2);

        static int min(int num1, int num2);

        int numerator;
        int denominator;
};

std::ostream &operator<<(std::ostream& ostream, const Fraction& a);