#include <iostream>
#include <cmath>
#include <iomanip>

int main(){
    using namespace std;

    int executionCount;
    cin >> executionCount;
    for(int i = 0; i < executionCount; ++i){
        int maxMultiplicand;
        int maxMultiplier;
        int cols;
        cin >> maxMultiplicand;
        cin >> maxMultiplier;
        cin >> cols;

        for(int multiplicand = 1; multiplicand <= maxMultiplicand; multiplicand += cols){
            for(int multiplier = 1; multiplier <= maxMultiplier; ++multiplier){
                for(int j = 0; j < cols && multiplicand + j <= maxMultiplicand; ++j){
                    cout << setw(2) << multiplicand + j;
                    cout << " * ";
                    cout << setw(2) << multiplier;
                    cout << " = ";
                    cout << setw(3) << (multiplicand + j) * multiplier; 
                    cout << setw(10) << " ";
                }        
                cout << endl;
            }
            cout << endl;
        }
    }

    return 0;
}