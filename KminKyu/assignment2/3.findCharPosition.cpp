#include<iostream>

using namespace std;

int main() {
    const int MAX_ALPHABET_LENGTH = 26, MAX_STR_LENGTH = 102;
    int alphabetArray[MAX_ALPHABET_LENGTH] = {0,};
    char str[MAX_STR_LENGTH];
    cin >> str;
    for (int nowIndex = 0; nowIndex < MAX_STR_LENGTH; ++nowIndex) {
        if (str[nowIndex] == '\0') {
            break;
        }
        int arrayIndex = str[nowIndex] - 'a';
        if (alphabetArray[arrayIndex] == 0){
            alphabetArray[arrayIndex] = nowIndex + 1;
        }
        
    }
    for (int nowIndex = 0; nowIndex < MAX_ALPHABET_LENGTH; ++nowIndex) {
        cout << alphabetArray[nowIndex] << " ";
    }
    cout << endl;

    return 0;
}