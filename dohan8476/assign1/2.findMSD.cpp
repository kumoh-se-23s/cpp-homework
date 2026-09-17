#include <iostream>
using namespace std;

int main () {
    int tc;
    cin >> tc;

    for (int i = 0; i < tc; i++) {
        int num;
        cin >> num;

        int cnt = 1; // 자릿수는 1부터 시작

        while (num / 10 != 0 ) {
            num = num / 10;
            ++cnt;
            //cnt++은 기존 값을 복사하는 임시 객체를 만들어 ++cnt보다 약간 느림
        }

        cout << num << " @ [" << cnt <<"]" << endl;
    }
}