#include <iostream>
using namespace std ;

bool readArray(int [], int, int); //단조 증가와 단소 갑소가 맞는지 파악 
bool getMinMax(int [], int [], int, ???, ???); // min, max에 저장할 수 있도록(바로)

int main() {
    const int SIZE = 5, ASCENDING = 1, DESCENDING = 2 ;
    int cnt, arr1[SIZE], arr2[SIZE], min=0, max=0 ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( readArray(arr1, SIZE, ASCENDING) && readArray(arr2, SIZE, DESCENDING) ){
            if (getMinMax(arr1, arr2, SIZE, min, max)){
                cout << "Min : " << min << ", Max : " << max << "\n" ;
            }
            else
                cout << "ALL SAME :" << min << "\n" ;
        }
        else 
            cout << "[Error] Unsorted input\n" ;
    }  
    return 0 ;
}