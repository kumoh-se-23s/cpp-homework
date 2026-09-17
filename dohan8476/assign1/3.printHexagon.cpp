#include <iostream>
using namespace std;

int main() {
    int tc;
    cin >> tc;

    // for (int rep = 0; rep < tc; rep++) {
    while (tc--) { 
        // tc가 0이되면 false로 위와 같이도 사용 가능
        int num;
        cin >> num;

        // 상단 부분
        for (int i = 0; i < num - 1; i++) {
            for (int blank = num - 1 ; blank > i; blank--) {
                cout << " ";
            }
            for (int star = 0 ; star < num + 2*i ; star++) {
                cout << "*";
            }
            cout << endl;
        }

        // 중간 부분
        for (int i = 0; i < num; i++) {
            for (int star = 0 ; star < 3 * num - 2; star++) {
                cout << "*";
            }
            cout << endl;
        }

        // 아랫 부분
        for (int i = 0; i < num - 1; i++) {
            for (int blank = 0 ; blank < i + 1; blank++) {
                cout << " ";
            }
            for (int star = 0 ; star < 3 * num - 4 -2 * i ; star++) {
                cout << "*";
            }
            cout << endl;
        }
    }
    return 0;
}