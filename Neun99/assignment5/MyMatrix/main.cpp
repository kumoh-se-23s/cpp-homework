#include "MyMatrix.h"

int main() {
    const int MAX_CNT = 8;
    MyMatrix matArr[MAX_CNT];
    matArr[0] = MyMatrix(2,2) ; // 행렬 크기 초기화
    matArr[1] = MyMatrix(2,2) ;
    matArr[2] = MyMatrix(2,3) ;

    int arr0[] = {1,2,3,4}, arr1[] = {1, -1, 0, 2} ;
    int arr2[] = {1,2,3,3,2,1} ; // 배열 내용 초기화

    matArr[0].init(arr0) ; // arr0으로 시작하는 배열에
    matArr[1].init(arr1) ; // 적절한 개수의 값이 있다고
    matArr[2].init(arr2) ; // 가정하고 행렬값 행우선 초기화

    matArr[3] = matArr[0] + matArr[1] ;
    matArr[4] = matArr[0] + matArr[2] ;
    matArr[5] = matArr[0] * matArr[1] ;
    matArr[6] = matArr[2] * matArr[0] ;
    matArr[7] = matArr[0] * matArr[2] ;

    for (int i = 0 ; i < MAX_CNT ; i++) { // 출력
        cout << "-- Matrix " << i << " --\n" ;
        cout << matArr[i] << endl;
    }

}