
#include <iostream>

void printBits(int num){
    using namespace std;
    constexpr auto SIZE = sizeof(int) * 8;

    int arr[SIZE]{0};
    int startIdx;
    for(startIdx = SIZE - 1; startIdx >= 0 && num > 0; --startIdx){
        arr[startIdx] = num & 1;
        num >>= 1;
    }
    
    ++startIdx;
    
    for(; startIdx < SIZE; ++startIdx){
        cout << arr[startIdx];

        if((startIdx & 3) == 3){
            cout << ' ';
        }
    }
    cout << endl;
}

int main(){
    using namespace std;

    int executionCount;

    cin >> executionCount;

    for(int i = 0; i < executionCount; ++i){
        int num;
        cin >> num;
        printBits(num);
    }


    return 0;
}