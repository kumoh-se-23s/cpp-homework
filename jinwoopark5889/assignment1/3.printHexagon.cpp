#include <iostream>

using namespace std;

int main()
{
    int repeat;
    cin >> repeat;

    for (int i = 0; i < repeat; i++)
    {
        int lineCnt = 0;
        int inputNumber;
        cin >> inputNumber;
        for (int j = 0; j < inputNumber - 1; j++)
        {
            for (int k = inputNumber - (j + 1); k > 0; k--)
            {
                cout << " ";
            }
            for (int k = 0; k < inputNumber + 2 * (j); k++)
            {
                cout << "*";
            }
            lineCnt++;
            cout << endl;
        }
        for (int j = 0; j < inputNumber; j++)
        {
            for (int k = 0; k < 2 * lineCnt + inputNumber; k++)
            {
                cout << "*";
            }
            cout << endl;
        }
        for (int j = 0; j < inputNumber - 1; j++)
        {
            for (int k = 0 ; k < j + 1; k++)
            {
                cout << " ";
            }
            for (int k = 2 * (lineCnt - j - 1) + inputNumber; k > 0; k--)
            {
                cout << "*";
            }
            cout << endl;
        }
    }
    return 0;
}