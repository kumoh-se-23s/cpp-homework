#include <iostream>
#include <iomanip>
using namespace std;
const int SIZE = 3; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용

void transpose(int a[][SIZE], int result[][SIZE], int size) {
    for(int i = 0 ; i < size; ++i){
        for(int j = 0;j < size; ++j){
            result[j][i] = a[i][j];
        }
    }

}
void add(int a[][SIZE], int b[][SIZE], int result[][SIZE], int size) {
    for(int i = 0 ; i < size; ++i){
        for(int j = 0;j < size; ++j){
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}
void multiply(int a[][SIZE], int b[][SIZE], int result[][SIZE], int size) {
    for(int i = 0 ; i < size; ++i){
        for(int j = 0;j < size; ++j){
            int sum = 0;
            for(int k = 0; k < size; ++k){
                sum += a[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
}
void printMatrix(int a[][SIZE], int size) {
    for(int i = 0 ; i < size; ++i){
        cout << "|";
        for(int j = 0; j < size; ++j){
            cout << setw(4) << a[i][j];
        }
        cout << " |" << endl;
    }
}
