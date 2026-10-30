#include <iostream>


void myCopy(int *dst, int *src, int size){
    for(int i = 0; i < size; ++i){
        dst[i] = src[i];
    }
}


void print1DArr(int *arr, int size)
{
    for(int i = 0; i < size; ++i){
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    const int MAX = 5;
    int arr1[MAX] = { 1,2,3,4,5 }, arr2[MAX], *p;
    // arr2 = arr1; // 정적 배열은 수정 불가 포인터의 한 종류이므로 복사 대입 연산이 불가능하다
    myCopy(arr2, arr1, MAX);
    p = new int[MAX];
    myCopy(p, arr1, MAX); // p는 arr1의 복사본이 되도록, 동적 할당 후 myCopy() 호출
    p[0] = 99; // 위의 수정이 제대로 되면 arr2은 안 바뀌면서 p[0]만 수정됨
    print1DArr(arr1, MAX); // print1DArr(int *, int)를 추가 구현할 것
    print1DArr(arr2, MAX);
    print1DArr(p, MAX);
    delete[] p;
    return 0;
}