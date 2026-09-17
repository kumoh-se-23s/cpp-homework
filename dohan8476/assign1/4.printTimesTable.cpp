#include <iostream>
using namespace std;

int main() {
    int tc;
    cin >> tc;

    while (tc--) {
        int dan, num, colsPerLine;
        cin >> dan >> num >> colsPerLine;

        for (int i = 1; i <= dan; i += colsPerLine) {
            // num만큼 다 반복했으니 5단부터 시작하기 위해 colsPerLine(4) 더해서 5단 시작으로 맞추기
            for (int j = 1; j <= num; j++, i -= colsPerLine) {
                // 1~4 출력하면 i == 5가 된 다음 다음줄로 넘어감 colsPerLine(4)빼서 다시 1단으로 맞춰주기
                for (int k = 1; k <= colsPerLine; k++) {
                    if (i <= dan) {
                        printf(" %2d * %2d = %4d ", i , j , i*j);
                    }
                    ++i;
                }
                cout << endl;
            }
            cout << endl;
        }
    }
}