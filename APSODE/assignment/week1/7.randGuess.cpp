#include <iostream>

int challenge_random_number(int challenge_answer, int *try_count) {
    int challenge_target;
    printf("\nGuess the number and enter it : ");
    std::cin >> challenge_target;
    (*try_count)++;
    int challenge_result = challenge_answer - challenge_target;

    if (challenge_result != 0) {
        printf("The number I specified is a number ");
        printf(challenge_result > 0 ? "greater than " : "less than ");
        printf("%d.", challenge_target);

        return challenge_random_number(challenge_answer, try_count);
    }

    printf("Got it!!! You've succeeded in the %d-th times!", *try_count);
    return 0;
}


int main() {
    printf("I have specified one natural number less than or equal to 100.\n");
    /*
     * rand()의 경우 의사난수라고 하며, 이는 완벽한 난수가 아닌 환경에 따라 달라지는 난수이다.
     * 해당 함수의 경우 프로그램의 시작 시점에서 난수가 결정되며 이때문에 여러번 실행하여도
     * 동일한 난수가 사용되어 프로그램 구현 의도와 달라진다.
     *
     * 따라서 srand(unsigned int _Seed)를 통해 rand()가 의사난수를 생성하기 위한 seed값에
     * 변화를 주어 rand로 생성되는 난수가 매번 바뀌도록 설정하는 것으로 rand()에서 발생한
     * 반복되는 난수 문제를 해결할 수 있다.
     */
    srand(1000);
    int random_number = rand() % 100 + 1;
    printf("%d", random_number);
    int try_count = 1;
    return challenge_random_number(random_number, &try_count);
}
