#pragma once
class Account {
private:
    long accNumber;
    double balance;
    const static int base = 100000;
    static int accountCNT;
public:
    Account(double bal);
    ~Account();
    void print() const;
    void deposit(double amount);
    void withdraw(double amount);
};
