#include <iostream>
using namespace std;

void printCharPosition(char[], int);

int main()
{
    const int STR_MAX_LEN = 100;
    char myCharArr[STR_MAX_LEN + 1];  // null저장을 위해 문자열의 최대 길이보다
                                      // 하나 더 큰 배열을 사용해야 함
    cin >> myCharArr;
    printCharPosition(myCharArr, STR_MAX_LEN);
    return 0;
}

void printCharPosition(char str[], int MAX_LEN)
{
    const int ALPHABET_CNT = 26;
    int alphabetFirstPosition[ALPHABET_CNT] = {};
    int cnt = 0;
    for (int pos = 0; pos < MAX_LEN && str[pos] != '\0'; pos++)
    {
        ++cnt;
        // 위치 저장하는 코드 구현
        if (alphabetFirstPosition[str[pos] - 'a'] == 0)
            alphabetFirstPosition[str[pos] - 'a'] = cnt;
    }
    // 출력 코드 구현
    for (int idx = 0; idx < ALPHABET_CNT; idx++)
    {
        cout << alphabetFirstPosition[idx] << " ";
    }
}

// int main()
// {
//     int alphabetCnt[26] = {};
//     char inStr[100] = {};
//     cin >> inStr;
//     int idx = 0;
//     int cnt = 1;
//     while (inStr[idx] != '\0')
//     {
//         if (alphabetCnt[inStr[idx] - 'a'] == 0)
//             alphabetCnt[inStr[idx] - 'a'] = cnt;
//         idx++;
//         cnt++;
//     }
//     for (int i = 0; i < 26; i++)
//         cout << alphabetCnt[i] << " ";
//     return 0;
// }