#include <iostream>
using namespace std;

int main() {
    const int SIZE = 26;
    int arr[SIZE] = {};

    const int STR_MAX_LEN = 100;
    char input[STR_MAX_LEN+1];
    cin >> input;

    int cnt = 1;
    for (int i = 0; i < STR_MAX_LEN; i++, cnt++) {
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