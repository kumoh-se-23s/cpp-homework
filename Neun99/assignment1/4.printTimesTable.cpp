#include <iostream>

using namespace std;

int main() {
    int runCntGoal;
    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; runCnt++) {
        int base; //몇단까지 할지
        int times; //각 단에 몇까지 곱할지
        int width; //한 줄에 출력될 수
        cin >> base;
        cin >> times;
        cin >> width;

        //층 구분
        for(int heightIdx = 0, startBase = 1; startBase <= base; startBase = width * ++heightIdx + 1) {
            //각 층별로 times만큼 출력
            for(int timesIdx = 1; timesIdx <= times; ++timesIdx) {
                printf("%2d * %2d = %4d",startBase, timesIdx, startBase * timesIdx);
                //층 너비가 2 이상이면 사이에 공백 추가하면서 출력
                for(int baseIdx = 1; baseIdx < width && (startBase + baseIdx) <= base; ++baseIdx) {
                    cout << "    ";
                    printf("%2d * %2d = %4d", startBase + baseIdx, timesIdx, (startBase + baseIdx) * timesIdx);
                }
                cout << endl; //한 줄 마무리
            }
            cout << endl; //한 층 마무리
        }
    }
    return 0;
}