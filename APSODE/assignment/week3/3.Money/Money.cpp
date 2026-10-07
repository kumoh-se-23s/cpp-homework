//
// Created by leegu on 26. 10. 7..
//

#include "Money.h"
#include <iostream>

Money::Money()
    : dollar(0), cent(0) {
}

Money::Money(int dollar, int cent)
    : dollar(dollar), cent(cent) {
    this->normalize();
}

int Money::get_dollar() const {
    return this->dollar;
}

int Money::get_cent() const {
    return this->cent;
}

Money Money::operator+(const Money &money_delta) const {
    return this->plus(money_delta);
}

Money Money::operator-(const Money &money_delta) const {
    return this->minus(money_delta);
}

bool Money::operator==(const Money &other_money) const {
    return this->dollar == other_money.dollar && this->cent == other_money.cent;
}

bool Money::operator<=(const Money &other_money) const {
    return this->compare_to(other_money) <= 0;
}

bool Money::operator>=(const Money &other_money) const {
    return this->compare_to(other_money) >= 0;
}

bool Money::operator<(const Money &other_money) const {
    return this->compare_to(other_money) < 0;
}

bool Money::operator>(const Money &other_money) const {
    return this->compare_to(other_money) > 0;

}

Money &Money::operator=(const Money &money) {
    this->set(
        money.dollar,
        money.cent
    );

    return *this;
}

void Money::set(int dollar, int cent) {
    this->dollar = dollar;
    this->cent = cent;

    this->normalize();
}

Money Money::plus(const Money &money_delta) const {
    Money new_money = Money();
    new_money.dollar = this->dollar + money_delta.dollar;
    new_money.cent = this->cent + money_delta.cent;

    new_money.normalize();
    return new_money;
}

Money Money::minus(const Money &money_delta) const {
    Money new_money = Money();

    new_money.dollar = this->dollar - money_delta.dollar;
    new_money.cent = this->cent - money_delta.cent;

    new_money.normalize();
    return new_money;
}

int Money::compare_to(const Money &other_money) const {
    Money compared_money = this->minus(other_money);


    if (compared_money.dollar == 0 && compared_money.cent == 0) {
        return 0;
    }

    return compared_money.dollar > 0 || compared_money.cent > 0 ? 1 : -1;
}

void Money::normalize() {
    this->dollar += this->cent / 100;
    this->cent %= 100;

    if (this->dollar * this->cent < 0) {
        int temp = dollar / std::abs(dollar) * 100;
        this->dollar -= temp / 100;
        this->cent += temp;
    }
}

std::ostream &operator<<(std::ostream &output_stream, const Money &money) {

    bool is_negative = money.get_dollar() < 0 || money.get_cent() < 0;

    bool is_short_digit_cent = money.get_cent() % 100 < 10;

    output_stream << (is_negative ? "-$" : "$") << money.get_dollar()
                  << (is_short_digit_cent ? ".0" : ".") << money.get_cent();

    return output_stream;
}

std::istream &operator>>(std::istream &input_stream, Money &money) {
    int dollar, cent;
    input_stream >> dollar >> cent;
    money.set(dollar, cent);

    return input_stream;
}
