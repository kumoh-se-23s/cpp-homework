#include <iostream>
#include <cmath>

int main(){
    using namespace std;

    int executionCount;
    cin >> executionCount;
    for(int i = 0; i < executionCount; ++i){
        
        int num;
        cin >> num;
        int size = num + 2 * (num - 1);

        
        
        for(int j = 0; j < size; ++j) {
            for(int k = 0; k < size; ++k){
                if(k < max(num - j - 1, num - size + j) || k > (num - 1) * 2 + min(size - j - 1, j)){
                    cout << " ";
                }else{
                    cout << "*";
                }
            }
            cout << endl;
        }
    }

    return 0;
}