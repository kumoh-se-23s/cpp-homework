#include <iostream>

using namespace std;

int main()
{
    int repeat;
    cin >> repeat;

    for (int i = 0; i < repeat; i++)
    {
        int dan;
        int multiplyNum;
        int num;
        cin >> dan >> multiplyNum >> num;

        for (int l = 0; l < dan; l += num)
        {
            for (int j = 1; j <= multiplyNum; j++)
            {
                for (int k = 1; k <= num && k + l != dan + 1; k++)
                {
                    printf("%2d * %2d = %-4d", k + l, j, (k + l) * j);
                }
                cout << endl;
            }
            cout << endl
                 << endl;
        }
    }
    return 0;
}