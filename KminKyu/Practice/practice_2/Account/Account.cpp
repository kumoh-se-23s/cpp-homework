#include "Account.h"
#include<iostream>
#include<cassert>

using namespace std;

int Account::accountCNT = 0;

Account::Account(double bal) : balance(bal) {
    if (bal < 0.0) {
        cout << "음수 잔액 불가능. 프로그램을 종료.";
        assert(false);
    }
    accountCNT++;
    accNumber = accountCNT + base;
    cout << "게좌_#" << accNumber << "생성." << endl;
    cout << "잔액 $" << balance << endl << endl;
}

void Account::print() const {
    cout << "게좌_#" << accNumber << endl;
    cout << "트랜잭션 - 잔액 확인" << endl;
    cout << "잔액 $" << balance << endl << endl;
}

void Account::deposit(double amount) {
    if (amount > 0.0) {
        balance += amount;
        cout << "게좌_#" << accNumber << endl;
        cout << "트랜잭션 - 입금" << endl;
        cout << "변경 잔액: $" << balance << endl << endl;
    } else {
        cout << "트랜잭션 중단." << endl;
    }
}

void Account::withdraw(double amount) {
    if (amount < balance) amount = balance;
    balance -= amount;
    cout << "게좌_#" << accNumber << "생성." << endl;
    cout << "트랜잭션 - 인출" << endl;
    cout << "변경된 잔액: $" << balance << endl << endl;
}

Account::~Account() {
    cout << "계좌_#" << accNumber << "Closed" << endl;
    cout << "$" << balance << "를 고객에게 송금." << endl << endl;
}