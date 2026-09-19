#include <iostream>
#include <iomanip>
using namespace std ;

const int SIZE = 3 ; // 전역변수는 사용하지 않아야 하나, 이번 과제에서만 사용


void transpose(int m1[][SIZE], int result[][SIZE], int SIZE){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            result[i][j] = m1[j][i];
        }
    }
    
}

void add(int m1[][SIZE], int m2[][SIZE], int result[][SIZE], int SIZE){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            result[i][j] = m1[i][j] + m2[i][j];
        }
    }

}

void multiply(int m1[][SIZE], int m2[][SIZE], int result[][SIZE], int SIZE){

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
        }
    }
    
    for (int k = 0; k < SIZE; k++){ 
        for (int i = 0; i < SIZE; i++){ 
            for (int j = 0; j < SIZE; j++){ 
                result[i][k] += m1[i][j]*m2[j][k];
            }
        }
    }
} 

void printMatrix(int result[][SIZE], int SIZE){

    for (int i = 0; i < SIZE; i++){
        cout << "|";
        for (int j = 0; j < SIZE; j++){
             cout << right << setw(4) << result[i][j];
        }
        cout << "|" << endl;
    }
    cout << endl;
}

