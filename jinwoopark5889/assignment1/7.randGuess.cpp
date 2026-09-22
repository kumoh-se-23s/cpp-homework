#include <iostream>

using namespace std;

void randMatchRun();

int main()
{
    randMatchRun();
    return 0;
}

void randMatchRun()
{
    // rand 사용시에는 시드값이 1로 고정되어서 프로그램을 실행시킬 때마다 같은 값이 출력되었고
    // srand 사용시에는 time과 같은 계속 변하는 값을 넣어 시드 값이 계속 변하게되어 계속 다른 값이 출력됨을 확인해볼 수 있었습니다.
    srand((unsigned int)time(NULL));

    int flag = 1;
    int count = 0;
    int randNumber = rand() % 100 + 1;

    cout << "I have specified one natural number less than or equal to 100. " << endl;

    while (flag)
    {
        int number;
        cout << "Guess the number and enter it :";
        cin >> number;

        if (number < randNumber)
        {
            count++;
            cout << "The number I specified is a number greater than " << number << "." << endl;
        }
        else if (number > randNumber)
        {
            count++;
            cout << "The number I specified is a number less than " << number << "." << endl;
        }
        else if (number == randNumber)
        {
            count++;
            flag = 0;
            cout << "Got it!!! You've succeeded in the " << count << "-th" << " times!";
        }
    }
}