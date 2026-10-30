#include <iostream>
#include "Fraction.h"
using namespace std;

Fraction::Fraction() {
    numerator = 1;
    denominator = 1;
}

Fraction::Fraction(int numerator, int denominator) {

    if(denominator != 0) {
        int dividend = numerator > denominator ? numerator : denominator;
        int divisor = numerator < denominator ? numerator : denominator;
        int remainder;

        while(divisor !=0) {
            remainder = dividend % divisor;
            dividend = divisor;
            divisor = remainder;
        }

        if(dividend != 1) {
            this->numerator = denominator / dividend < 0 ?  - (numerator / dividend) : numerator / dividend;
            this->denominator = denominator / dividend < 0 ?  - (denominator / dividend) : denominator / dividend;
        } else {
            this->numerator = denominator < 0 ?  -numerator : numerator;
            this->denominator = denominator < 0 ? -denominator : denominator;
        }
    } 

    else {
        cout << "ERR" << "\n";

        this->numerator = numerator;
        this->denominator = 1;
    }
}

int Fraction::getNumerator() const {
    return numerator;
}

void Fraction::setNumerator(int numerator) {
    this->numerator = numerator;
}

int Fraction::getDenominator() const {
    return denominator;
}

void Fraction::setDenominator(int denominator) {
    this->denominator = denominator;
}

const Fraction Fraction::operator+(const Fraction& f1) const {
    return Fraction(
        (f1.getNumerator() * denominator) + (numerator * f1.getDenominator()), 
        f1.getDenominator() *denominator
    );
}

void Fraction::set(int numerator, int denominator) {

    if(denominator != 0) {
        int dividend = numerator > denominator ? numerator : denominator;
        int divisor = numerator < denominator ? numerator : denominator;
        int remainder;

        while(divisor !=0) {
            remainder = dividend % divisor;
            dividend = divisor;
            divisor = remainder;
        }

        if(dividend != 1) {
            this->numerator = denominator / dividend < 0 ?  - (numerator / dividend) : numerator / dividend;
            this->denominator = denominator / dividend < 0 ?  - (denominator / dividend) : denominator / dividend;
        } else {
            this->numerator = denominator < 0 ?  -numerator : numerator;
            this->denominator = denominator < 0 ? -denominator : denominator;
        }
    } 

    else {
        cout << "ERR" << "\n";

        this->numerator = numerator;
        this->denominator = 1;
    }
}

ostream& operator<<(ostream& outputStream, const Fraction& f1) {
    if(f1.getDenominator() > 1) {
        outputStream << f1.getNumerator() << "/" << f1.getDenominator(); 
    } else {
        outputStream << f1.getNumerator();
    }

    return outputStream;
} 