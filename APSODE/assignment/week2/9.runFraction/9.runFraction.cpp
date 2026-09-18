//
// Created by leegu on 26. 9. 18..
//

#include "9.runFraction.h"

#include <iostream>


Fraction::Fraction()
    : numerator(1), denominator(1) {
}

Fraction::Fraction(const int numerator)
    : numerator(numerator), denominator(1) {
}

Fraction::Fraction(const int numerator, const int denominator) {
    this->set(numerator, denominator);
}


void Fraction::set(const int new_numerator, const int new_denominator) {
    if (new_denominator == 0) {
        std::cout << "ERR\n";
    }

    this->numerator = new_numerator;
    this->denominator = new_denominator == 0 ? 1 : new_denominator;
    this->simplify();
}

Fraction Fraction::add(Fraction& other_fraction) {
    if (!is_same_denominator(*this, other_fraction)) {
        make_denominator_to_same(*this, other_fraction);
    }

    Fraction result_fraction = Fraction(
        this->numerator + other_fraction.get_numerator(),
        this->denominator
    );

    result_fraction.simplify();
    return result_fraction;
}

void Fraction::simplify() {
    const int gcd = calc_gcd(std::abs(this->numerator), std::abs(this->denominator));
    this->numerator /= gcd;
    this->denominator /= gcd;

    this->normalize_sign();
}

void Fraction::normalize_sign() {
    if (this->denominator < 0) {
        this->numerator *= -1;
        this->denominator *= -1;
    }
}

void Fraction::print() const {
    std::cout << this->numerator;
    if (this->denominator != 1) {
        std::cout << "/" << this->denominator << std::endl;
    }
}

int Fraction::get_denominator() const {
    return this->denominator;
}

int Fraction::get_numerator() const {
    return this->numerator;
}

void Fraction::set_denominator(const int new_denominator) {
    this->denominator = new_denominator;
}

void Fraction::set_numerator(const int new_numerator) {
    this->numerator = new_numerator;
}

int Fraction::get_max(const int num1, const int num2) {
    return num1 >= num2 ? num1 : num2;
}

int Fraction::get_min(const int num1, const int num2) {
    return num1 <= num2 ? num1 : num2;
}

int Fraction::calc_gcd(const int num1, const int num2) {
    const int larger = get_max(num1, num2);
    const int smaller = get_min(num1, num2);

    if (smaller == 0) {
        return larger;
    }

    const int temp = larger % smaller;
    if (temp == 0) {
        return smaller;
    }

    return calc_gcd(smaller, temp);
}

void Fraction::make_denominator_to_same(Fraction& fraction1, Fraction& fraction2) {
    fraction1.set_numerator(fraction2.get_denominator() * fraction1.get_numerator());
    fraction2.set_numerator(fraction1.get_denominator() * fraction2.get_numerator());

    const int new_denominator = fraction1.get_denominator() * fraction2.get_denominator();
    fraction1.set_denominator(new_denominator);
    fraction2.set_denominator(new_denominator);
}

bool Fraction::is_same_denominator(const Fraction fraction1, const Fraction fraction2) {
    return fraction1.get_denominator() == fraction2.get_denominator();
}

int main() {
    // Fraction f1, f2(1, 4), f3; // f2 = 2/5
    // f1.set(1, -2); // f1 = 2/3
    // f3 = f1.add(f2); // f1 = 2/3
    // f3.print();


    Fraction f1 = Fraction(1, 2);
    Fraction f2 = Fraction(10, 0);
    Fraction f3 = f1.add(f2);
    f3.print();
    // f1.print();
    return 0;
}
