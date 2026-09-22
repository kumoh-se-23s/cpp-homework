#include <iostream>
using namespace std;

void printDigit(int);
void printReverseDigit(int);

int main()
{
    int cnt, val;
    cin >> cnt;
    for (int i = 0; i < cnt; ++i)
    {
        cin >> val;
        cout << "Backward : ";
        printReverseDigit(val);
        cout << endl
             << "Forward : ";
        printDigit(val);
        cout << endl;
    }
    return 0;
}
void printReverseDigit(int n)
{
    if (n > 0)
    {
        cout << n % 10 << " ";
        printReverseDigit(n / 10);
    }
}

void printDigit(int n)
{
    if (n > 0)
    {
        printDigit(n / 10);
        cout << n % 10 << " ";
    }
}
