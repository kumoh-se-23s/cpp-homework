#include <iostream>

using namespace std;

int main()
{
    int repeat;
    cin >> repeat;

    for (int i = 0; i < repeat; i++)
    {
        double inputNumber;
        int integerNumber;
        double decimalNumber;
        cin >> inputNumber;

        integerNumber = (int)inputNumber;
        decimalNumber = inputNumber - integerNumber;

        cout << integerNumber << " + " << decimalNumber << endl;
    }
    return 0;
}