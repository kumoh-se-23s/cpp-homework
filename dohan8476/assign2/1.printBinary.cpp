#include <iostream>
using namespace std;

int main() {
    int tc;
    cin >> tc;

    const int SIZE = 32;
    int arr[SIZE];

    while (tc--) {
        int num;
        cin >> num;

        int idx = 0;
        //do-while로 0 입력 문제 해결
        do {
            arr[idx++] = num % 2;
            num /= 2;
        } while (num > 0);

        for (int index = idx - 1; index >= 0 ; index--) {
            cout << arr[index];
            if (index % 4 == 0) { // 4자리 끊어주기
                cout << " ";
            }
        }
    }
    return 0;
}