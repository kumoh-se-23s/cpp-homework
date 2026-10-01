#include <iostream>
#include <iomanip>

int main(){
    using namespace std;

    int executionCount;
    cin >> executionCount;
    for(int i = 0; i < executionCount; ++i){
        
        double input;
        cin >> input;

        int integer = static_cast<int>(input);
        double decimal = input - integer;

        cout << integer << " + " << setprecision(15) << decimal << endl;
    }

    return 0;
}