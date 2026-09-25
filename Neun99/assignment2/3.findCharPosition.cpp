#include <iostream>

using namespace std;

int main() {
    const int INPUT_ARR_MAX_SIZE = 101;
    char userInput[INPUT_ARR_MAX_SIZE];

    //결과 저장용 배열
    const int ALPHABET_COUNT = 26;
    int charPositions[ALPHABET_COUNT] = {}; //0으로 초기화

    //입력받고 용량 확인
    cin >> userInput;

    for (int idx = 0; idx < INPUT_ARR_MAX_SIZE && userInput[idx] != '\0'; idx++) {
        if(charPositions[userInput[idx] - 'a'] == 0) //이미 나온 알파벳이면 덮어씌우지 않음
            charPositions[userInput[idx] - 'a'] = idx + 1;
    }

    //출력
    cout << charPositions[0];
    for (int idx = 1; idx < ALPHABET_COUNT; idx++) {
        cout << " " << charPositions[idx];
    }
}