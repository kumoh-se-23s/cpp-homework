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
        for (;num / 2 > 0 ; num /= 2) {
            arr[idx++] = num % 2;
        }
        arr[idx] = num; // 이거 반복문 안에 못담으려나

        for (int index = idx; index >= 0 ; index--) {
            if ((index+1) % 4 == 0) { // 4자리 끊어주기
                cout << " ";
            }
            cout << arr[index];
        }
    }
    return 0;
}