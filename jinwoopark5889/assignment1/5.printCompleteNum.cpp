#include <iostream>

using namespace std;

int main()
{
    int repeat;
    cin >> repeat;
    for (int i = 0; i < repeat; i++)
    {
        int inputNum;
        cin >> inputNum;
        for (int j = 2; j <= inputNum; j++)
        {
            int sum = 1;
            int flag = 1;
            for (int k = 2; k <= j / 2; k++)
            {
                if (j % k == 0)
                {
                    sum += k;
                    flag = 0;
                }
            }
            if (j < 10)
            {
                flag = 0;
            }
            if (flag)
            {
                cout << j << " ";
            }
            if (sum == j)
            {
                for (int k = 1; k <= j / 2; k++)
                {
                    if (k == 1)
                    {
                        if (sum != 6)
                        {
                            cout << endl;
                        }
                        cout << "[C] " << sum << " = " << k;
                    }
                    else if (j % k == 0)
                    {
                        cout << " + " << k;
                    }
                }
                if (inputNum != sum && inputNum > 10)
                {
                    cout << endl
                         << "[P] ";
                }
                if (inputNum <= 10)
                {
                    cout << endl;
                }
            }
        }
    }
    return 0;
}