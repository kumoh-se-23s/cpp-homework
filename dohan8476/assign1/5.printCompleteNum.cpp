#include <iostream>
using namespace std;

int main() {
    int tc;
    cin >> tc;

    while(tc--){
        int num;
        cin >> num;

        bool isFirstPrime = true;

        for (int i = 2; i <= num; i++) {
            int sum = 1; // 모든 약수는 항상 1과 본인을 포함

            //약수들의 합을 구하는 부분
            for (int j = 2; j < i; j++) {
                if (i % j == 0) {
                    sum += j;
                }
            }

            //소수 출력 부분 (10 이상 소수)
            if (sum == 1 && i > 10) {
                if (isFirstPrime) {
                    cout << "[P] ";
                    isFirstPrime = false;
                }
                cout << i << " ";
            }

            //완전수 출력 부분
            if (sum == i){
                // [P] 정상 출력되게 다시 true로 초기화
                if (!isFirstPrime) {
                    cout << endl;
                    isFirstPrime = true;
                }

                cout << "[C] " << sum << " =  1";
                for (int j = 2; j < i; j++) {
                    if (i % j == 0) {
                        cout << " + " << j;
                    }
                }
                cout << endl;
            }
        }
    }
}