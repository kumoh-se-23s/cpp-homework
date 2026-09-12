#include <iostream>

using namespace std;

const int RANGE = 100;

int main() {
    /*
    아래 srand를 사용하지 않는 경우, rand 함수에서 매 실행 같은 결과가 리턴되어 정답이 고정되는 현상이 일어났습니다.
    srand를 사용하되 인자(시드)로 임의의 상수를 넣는 경우에도 당연하지만 같은 결과가 고정되었습니다.
    반면 srand에 time을 넣고 실행할 시 매 실행별로 답이 달라지는 현상을 확인할 수 있었습니다.
    */
    srand(time(NULL));

    int answer = rand() % RANGE + 1;
    cout << "I have specified one natural number less than or equal to 100." << endl;
    cout << "Guess the number and enter it : ";
    
    bool isCorrect = false;
    int tryCnt = 0;
    //답 맞추면 탈출, 못 맞추면 힌트 주고 Cnt + 1
    while(!isCorrect) {
        int userInput;
        cin >> userInput;

        if(userInput > answer)
            printf("The number I specified is a number less than %d.\n", userInput);
        else if (userInput < answer)
            printf("The number I specified is a number greater than %d.\n", userInput);
        else
            isCorrect = true;
        tryCnt++;
    }
    printf("Got it!!! You've succeeded in the %d-th times!", tryCnt);

    return 0;
}