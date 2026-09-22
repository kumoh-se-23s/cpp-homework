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
        short numArr[32] = {};
        int size = 0;
        int cnt;
        for (cnt = 0; inputNum > 0; cnt++)
        {
            numArr[size++] = inputNum % 2;
            inputNum /= 2;
        }
        cnt %= 4;
        for (; cnt > 0; cnt--)
            cout << numArr[size-- - 1];
        cout << " ";
        for (int i = size - 1; i >= 0; i--)
        {
            cout << numArr[i];
            cnt++;
            if (cnt % 4 == 0)
                cout << " ";
        }
        cout << endl;
    }
    return 0;
}
