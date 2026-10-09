#pragma once

class Fraction {
    public:
        Fraction();
        Fraction(int numerator, int denominator);
        void set(int num, int den);

        int getNumerator() const;
        int getDenominator() const;

        const Fraction operator+(const Fraction& a) const;
        Fraction operator=(const Fraction& a);


    private:
        int calcGCD(int num, int den);
        void simplify();
        int abs(int num);
        int max(int num1, int num2);
        int min(int num1, int num2);


        int numerator;
        int denominator;
};

std::ostream &operator<<(std::ostream& ostream, const Fraction& a);