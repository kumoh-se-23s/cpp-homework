//
// Created by leegu on 26. 9. 18..
//

#ifndef CPP_HOMEWORK_9_RUNFRACTION_H
#define CPP_HOMEWORK_9_RUNFRACTION_H
#include <iosfwd>

class Fraction {
    public:
        Fraction();

        Fraction(int numerator);

        Fraction(int numerator, int denominator);

        void set(int new_numerator, int new_denominator);

        // Fraction add(Fraction &other_fraction);

        Fraction add(const Fraction &other_fraction) const;

        void simplify();

        void normalize_sign();

        int get_denominator() const;

        int get_numerator() const;

        void set_denominator(int new_denominator);

        void set_numerator(int new_numerator);

        void print() const;

        Fraction operator +(const Fraction &fraction) const;

        Fraction &operator =(const Fraction &fraction);

    private:
        int numerator;
        int denominator;

        static int get_max(int num1, int num2);

        static int get_min(int num1, int num2);

        static int calc_gcd(int num1, int num2);
};

std::ostream& operator <<(std::ostream& output_stream, const Fraction& fraction);



#endif //CPP_HOMEWORK_9_RUNFRACTION_H
