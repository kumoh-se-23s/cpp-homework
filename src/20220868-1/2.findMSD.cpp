#include <iostream>


int main(){
    using namespace std;

    int executionCount;
    cin >> executionCount;
    for(int i = 0; i < executionCount; ++i){
        
        int num;
        cin >> num;
        int cnt = 0;
        int msd = 0;
        while(num != 0){
            msd = num;
            num /= 10;
            cnt++;
        }
        cout << msd << " @ [" << cnt << "]" << endl;
    }

    return 0;
}