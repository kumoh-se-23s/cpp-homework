#include <iostream>
#include "Fraction.h"

using namespace std;

//생성자
Fraction::Fraction() {}
Fraction::Fraction(int num, int den): numerator(num), denominator(den) {
    normalize();
}

//덧셈
const Fraction Fraction::operator+(const Fraction& fra) const{
    int result_numerator = numerator * fra.getDen() + fra.getNum() * denominator;
    int result_denominator = denominator * fra.getDen();

    return Fraction(result_numerator, result_denominator);
}

//out
ostream& operator<<(ostream& out, const Fraction& fra) {
    out << fra.getNum();
    if (fra.getDen() != 1)
        out << "/" << fra.getDen();
    return out;
}

//분자 반환
int Fraction::getNum() const{
    return numerator;
}

//분모 반환
int Fraction::getDen() const{
    return denominator;
}

//입력값 정리
void Fraction::normalize() {
    //--음수 정리--
    if (denominator < 0) {
        numerator *= -1;
        denominator *= -1;
    }

    //분모 0 처리
    if (denominator == 0) {
        cout << "ERR ";
        denominator = 1;
    }

    //약분
    int gcd = getGCD(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
}

//최대공약수 반환
int Fraction::getGCD(int num1, int num2) {
    if (num1 < 0)
        num1 *= -1;

    int remainder = num1 % num2;
    while (remainder != 0 ) {
        num1 = num2;
        num2 = remainder;
        remainder = num1 % num2;
    }

    return num2;
}
