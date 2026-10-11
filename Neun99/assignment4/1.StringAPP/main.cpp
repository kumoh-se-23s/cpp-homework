#include <cstring>
#include <iostream>

using namespace std;

int main() {
    void countAlphabet(const char str[], int usingSize);
    void normalize(const char str[],int usingSize);
    void encryption(const char str[], int usingSize, int gap);

    static constexpr int MAX_INPUT_LENGTH = 128;
    char str[MAX_INPUT_LENGTH];
    int usingSize;

    cin >> str;
    usingSize = strlen(str);

}

//주기능 -----------------------------------------------------------
//알파벳 개수
void countAlphabet(const char str[], int usingSize) {
    constexpr int NUMBER_OF_ALPHABET = 26;
    char alphabetCount[NUMBER_OF_ALPHABET];

    char lowerStr[] = {};
    strcpy(lowerStr, str);
    for (int idx = 0; idx < usingSize; idx++) {
        if (!isDigit(str[idx])) {}
    }
}

//첫 글자 대문자, 나머지 소문자 변환
void normalize(const char str[], int usingSize) {

}

//gap만큼 이동 암호화
void encryption(const char str[], int usingSize, int gap) {

}

//util---------------------------------------------------------
static void toLower(char str[], int size) {
    for (int idx = 0; idx < size; idx++) {

    }
}

static bool isAlphabet(char text) {
    int result = text - 'a';
    return (result >= 'a' && result <= 'z');
}

static bool isDigit(char text) {
    int result = text - '0';
    return (result >= 0 && result <= 9);
}