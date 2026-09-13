#include <iostream>

using namespace std;

void readNumbers(int arr[], int maxSize, int endCond, int& readSize);
double getAverage(int arr[], int arrSize);

int main() {
    const int MAX_STD_NUM = 10;
    int stdScoreList[MAX_STD_NUM], stdNum;

    cout << "학생 점수 입력 (최대 "  << MAX_STD_NUM << "개. 종료는 -1) : ";
    readNumbers(stdScoreList, MAX_STD_NUM, -1, stdNum);
    cout << "평균점수는" << getAverage(stdScoreList, stdNum) << endl;

    return 0;
}

void readNumbers(int arr[], int maxSize, int endCond, int& readSize) {
    for (readSize = 0; readSize < maxSize; readSize++) {
        cin >> arr[readSize];
        if (arr[readSize] == endCond) {
            break;
        }
    }
}

double getAverage(int arr[], int size) {
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / size;
}