//
// Created by apsode on 26. 10. 12..
//

#include <iostream>

void myCopy(int *source, int *dest, int size) {
    for (int index = 0; index < size; ++index) {
        dest[index] = source[index];
    }
}

void print1DArr(int *arr, int size) {
    for (int index = 0; index < size; ++index) {
        std::cout << arr[index] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int MAX = 5;
    int arr1[MAX] = {1, 2, 3, 4, 5}, arr2[MAX], *p;

    /*
     * arr2 = arr1;
     * 해당 코드의 경우 arr1, arr2 모두 고정된 주소(정적배열)를 가리키는 포인터이다.
     * 이때, arr2 = arr1을 진행할 경우 고정된 공간을 전부 덮어씌우는게 불가능하기에,
     * Compile Time에서 에러가 발생하게 된다.
     *
     * p = arr1
     * p는 포인터 타입이고, arr1는 고정된 주소(정적배열)를 가리키는 포인터이다.
     * 따라서 p = arr1을 진행할 경우 이는 p에 정적배열이 복사되는 것이 아닌,
     * p가 arr1의 주소값을 가지게 되는 것이기에 동일한 위치를 가리키게 된다.
     * 따라서 99 2 3 4 5라는 출력결과가 만들어진다.
     */

    // arr2 = arr1; // arr2가 arr1의 복사본이 되도록 함수 myCopy(int *, int *, size) 작성
    myCopy(arr1, arr2, MAX);

    // p = arr1; // p는 arr1의 복사본이 되도록, 동적 할당 후 myCopy() 호출
    p = new int[MAX];
    myCopy(arr1, p, MAX);


    p[0] = 99; // 위의 수정이 제대로 되면 arr2은 안 바뀌면서 p[0]만 수정됨
    print1DArr(arr1, MAX); // print1DArr(int *, int)를 추가 구현할 것
    print1DArr(arr2, MAX);
    print1DArr(p, MAX);

    delete[] p;
    return 0;
}


