#include <iostream>

using namespace std;

int main()
{
    int repeat;
    cin >> repeat;
    for (int i = 0; i < repeat; i++)
    {
        int inputNum;
        int cnt = 1;
        cin >> inputNum;
        while (inputNum > 9)
        {
            inputNum /= 10;
            cnt++;
        }
        while (inputNum < -9)
        {
            inputNum /= 10;
            cnt++;
        }
        cout << inputNum << " @ " << "[" << cnt << "]" << endl;
    }
    return 0;
}