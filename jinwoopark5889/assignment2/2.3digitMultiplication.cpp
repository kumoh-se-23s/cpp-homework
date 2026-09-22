#include <iostream>
using namespace std;

int main()
{
    const int maxLength = 3;
    int inputNum1, inputNum2;
    cin >> inputNum1 >> inputNum2;

    int inputNum2Digit[maxLength];

    for (int i = 0; i < maxLength; i++)
    {
        inputNum2Digit[i] = inputNum2 % 10;
        inputNum2 /= 10;
    }

    int sum = 0;
    for (int i = 0, placeValue = 1; i < maxLength; i++, placeValue *= 10)
    {
        cout << inputNum1 * inputNum2Digit[i] << endl;
        sum += inputNum1 * inputNum2Digit[i] * placeValue;
    }

    cout << sum;
    return 0;
}

// const int numSize = 3;
//     int num1, num2;
//     cin >> num1 >> num2;
//     int digit1 = num2 % 10;
//     int digit2 = (num2 / 10) % 10;
//     int digit3 = num2 / 100;

//     int sum1 = num1 * digit1;
//     int sum2 = num1 * digit2;
//     int sum3 = num1 * digit3;

//     int resultArr[6] = {};
//     int tempArr[4] = {};
//     int startIdx = 0;
//     int tempIdx = 0;

//     for (int i = 0; i < 4; i++)
//     {
//         resultArr[i] = sum1 % 10;
//         sum1 /= 10;
//     }
//     startIdx++;
//     for (int i = 3; i >= 0; i--)
//     {
//         cout << resultArr[i];
//     }
//     cout << endl;

//     for (int i = startIdx; i < 4 + startIdx; i++)
//     {
//         int sum2Digit = sum2 % 10;
//         if ((resultArr[i] + sum2Digit) / 10 == 1)
//         {
//             resultArr[i + 1]++;
//             resultArr[i] = (resultArr[i] + sum2Digit) % 10;
//         }
//         else
//             resultArr[i] += sum2Digit;

//         tempArr[tempIdx++] = sum2Digit;
//         sum2 /= 10;
//     }
//     startIdx++;
//     for (int i = 3; i >= 0; i--)
//     {
//         cout << tempArr[i];
//     }
//     cout << endl;
//     tempIdx = 0;
//     for (int i = startIdx; i < 4 + startIdx; i++)
//     {
//         int sum3Digit = sum3 % 10;
//         if ((resultArr[i] + sum3Digit) / 10 == 1)
//         {
//             resultArr[i + 1]++;
//             resultArr[i] = (resultArr[i] + sum3Digit) % 10;
//         }
//         else
//             resultArr[i] += sum3Digit;
//         tempArr[tempIdx++] = sum3Digit;
//         sum3 /= 10;
//     }
//     startIdx++;
//     for (int i = 3; i >= 0; i--)
//     {
//         cout << tempArr[i];
//     }
//     cout << endl;
//     for (int i = 5; i >= 0; i--)
//     {
//         cout << resultArr[i];
//     }