#include <iostream>
using namespace std;

int main() {
    const int SIZE = 26;
    int arr[SIZE] = {};
    char input[100];
    cin >> input;

    int cnt = 1;
    for (int i = 0; i < 100; i++, cnt++) {
        if (input[i] == '\0') {
            break;
        }
        if (arr[input[i] - 'a'] == 0) {
            arr[input[i] - 'a'] = cnt;
        }
    }

    for (int i : arr) {
        cout << i << " ";
    }

    return 0;
}