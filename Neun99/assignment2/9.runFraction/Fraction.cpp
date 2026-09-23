#include <iostream>
#include "Fraction.h"

using namespace std;

//생성자
Fraction::Fraction() {}
Fraction::Fraction(int num, int den) {
    set(num, den);
}

//값 변경
void Fraction::set(int num, int den) {
    normalize(num, den);
    numerator = num;
    denominator = den;
}

//덧셈
Fraction Fraction::add(Fraction fra2) {
    int result_numerator = getNum() * fra2.getDen() + fra2.getNum() * getDen();
    int result_denominator = getDen() * fra2.getDen();

    Fraction result(result_numerator, result_denominator);
    return result;
}

void Fraction::print() {
    cout << numerator;
    if (denominator != 1)
        cout << "/" << denominator << endl;
}

//분자 반환
int Fraction::getNum() {
    return numerator;
}

//분모 반환
int Fraction::getDen() {
    return denominator;
}

//입력값 정리
void Fraction::normalize(int& num, int& den) {
    //--음수 정리--
    if (den < 0) {
        num *= -1;
        den *= -1;
    }

    //분모 0 처리
    if (den == 0) {
        cout << "ERR ";
        den = 1;
    }

    //약분
    int gcd = getGCD(num, den);
    num /= gcd;
    den /= gcd;
}

//최대공약수 반환
int Fraction::getGCD(int num1, int num2) {
    euclidNormalize(num1, num2);

    int remainder = num1 % num2;
    while (remainder != 0 ) {
        num1 = num2;
        num2 = remainder;
        remainder = num1 % num2;
    }

    return num2;
}

//유클리드 사용 전 전처리
void Fraction::euclidNormalize(int& num1, int& num2) {
    //분모 음수는 이미 처리되었으므로 분자만
    if (num1 < 0)
        num1 *= -1;

    //num1 >= num2가 되도록
    if (num1 < num2) {
        int temp = num1;
        num1 = num2;
        num2 = temp;
    }
}
