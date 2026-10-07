#include<iostream>
#include "Fraction.h"

using namespace std;

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

Fraction::Fraction(int n, int d) {
    set(n, d);
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

Fraction Fraction::add(const Fraction& fraction) const{
    int firstNumerator = this->numerator * fraction.denominator;
    int secondNumerator = fraction.numerator * this->denominator;

    int resultDenominator = fraction.denominator * this->denominator;

    int resultNumerator = firstNumerator + secondNumerator;

    Fraction resultFraction = Fraction(resultNumerator, resultDenominator);
    return resultFraction;
}
void Fraction::set(int n, int d) {
    numerator = n;
    denominator = d;
    this->organizeFraction();
}

Fraction Fraction::operator +(const Fraction& fraction) const {
    return add(fraction);
}
Fraction& Fraction::operator =(const Fraction& fraction) {
    set(fraction.numerator, fraction.denominator);
    return *this;
}

ostream& operator <<(ostream& outputStream, const Fraction& fraction) {
    outputStream << fraction.getNumerator();
    if (fraction.getDenominator() != 1) {
        outputStream << "/" << fraction.getDenominator();
    }

    return outputStream;
}


