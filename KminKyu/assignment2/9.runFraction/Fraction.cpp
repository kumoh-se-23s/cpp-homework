#include<iostream>
#include "Fraction.h"


void Fraction::swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int Fraction::getGCD(int n, int d) { //유클리드 호제법 알고리즘
    if (d < n) {
        swap(d, n);
    }
    while (n != 0) {
        int temp = n;
        n = d % n;
        d = temp;
    }
    return d;
}

Fraction::Fraction(int n, int d)
    : numerator(n), denominator(d) {
    organizeFraction();
}

void Fraction::organizeFraction() {
    if (denominator == 0) {
        denominator = 1;
        std::cout << "ERR";
    }
    int gcd = getGCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;

    if (denominator < 0) {
        denominator *= -1;
        numerator *= -1;
    }

}

int Fraction::getNumerator() const {
    return numerator;
}

int Fraction::getDenominator() const {
    return denominator;
}
void Fraction::print() const {
    std::cout << numerator;
    if (denominator != 1) {
        std::cout << "/" << denominator;
    }
    std::cout << "\n";
}

Fraction Fraction::add(const Fraction& fraction) const{
    int firstNumerator = this->getNumerator() * fraction.getDenominator();
    int secondNumerator = fraction.getNumerator() * this->getDenominator();

    int resultDenominator = fraction.getDenominator() * this->getDenominator();

    int resultNumerator = firstNumerator + secondNumerator;

    return Fraction(resultNumerator, resultDenominator);
}
void Fraction::set(int n, int d) {
    numerator = n;
    denominator = d;
    this->organizeFraction();
}
