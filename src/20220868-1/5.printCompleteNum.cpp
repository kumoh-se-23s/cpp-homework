#include <iostream>
#include <cmath>

enum class State{
    PRIME, COMPLETE
};


int main(){
    using namespace std;

    int executionCount;
    cin >> executionCount;
    for(int i = 0; i < executionCount; ++i){
        
        float input;
        cin >> input;

        State currentState = State::COMPLETE;

        for(int i = 3; i <= input; ++i){
            if(i & 1 && i > 10){
                //소수 판별
                bool isPrime = true;
                for(int j = sqrt(i); j > 1; --j){
                    if(i % j == 0) isPrime = false;
                }
                if(isPrime){
                    if(currentState != State::PRIME){
                        currentState = State::PRIME;
                        cout << endl;
                        cout << "[P] ";
                    }else{
                        cout << ", ";
                    }
                    cout << i;
                }
            }
            //완전수 판별
            int sum = 1;
            for(int j = 2; j <= i / 2; ++j){
                if(i % j == 0) sum += j;
            }

            if(sum == i){
                currentState = State::COMPLETE;
                cout << endl;
                cout << "[C] " << i << " = ";
                for(int j = 1; j <= i / 2; ++j){ 
                    if(i % j == 0) {
                        if(j != 1)  cout << " + ";
                        cout << j;
                    }
                }
            }
            
        }
        cout << endl;
    }

    return 0;
}