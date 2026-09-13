#include<iostream>
using namespace std;

struct CDAccountV1 {
    double balance;
    double interestRate;
    int term;
};

CDAccountV1 getData();

int main() {
    CDAccountV1 account;
    account = getData();

    double rateFraction, interest;
    rateFraction = account.interestRate / 100.0;
    interest = account.balance * (rateFraction * (account.term / 12.0));
    account.balance += interest;

    cout << account.term << "개월 후 만기 예상 총액은" << account.balance << "원" << endl;

    return 0;

}

CDAccountV1 getData() {
    CDAccountV1 theAccount;
    cout << "계좌 잔고: ";
    cin >> theAccount.balance;
    cout << "연이율 : ";
    cin >> theAccount.interestRate;
    cout << "만기까지의 남은 개월수";
    cin >> theAccount.term;

    return theAccount;
}