#include <cstring>
#include <iostream>

using namespace std;

void countAlphabet(const char str[], int usingSize);
void normalize(const char str[],int usingSize);
void encryption(const char str[], int usingSize, int gap);

static constexpr int MAX_INPUT_LENGTH = 127;
int main() {
    char str[MAX_INPUT_LENGTH + 1];
    int usingSize;
    int gap;

    cin.getline(str, MAX_INPUT_LENGTH + 1);
    usingSize = strlen(str);

    cin >> gap;

    countAlphabet(str, usingSize);
    normalize(str, usingSize);
    encryption(str, usingSize, gap);
}

//기능 -----------------------------------------------------------
static char toLower(char ch);
static char toUpper(char ch);
static bool isAlphabet(char ch);
static bool isUpper(char ch);
static bool isLower(char ch);
static bool isDigit(char ch);
static int findFirstLetterIdx(const char str[], int size);
static char moveAlphabet(char ch, int gap);
static char moveDigit(char ch, int gap);

//알파벳 개수
void countAlphabet(const char str[], int usingSize) {
    constexpr int NUMBER_OF_ALPHABET = 26;
    int alphabetCount[NUMBER_OF_ALPHABET] = {};

    //알파벳 담기
    for (int idx = 0; idx < usingSize; idx++) {
        if (isAlphabet(str[idx])) {
            char alphabet = toLower(str[idx]);
            alphabetCount[alphabet - 'a']++;
        }
    }
    //출력
    for (int idx = 0; idx < NUMBER_OF_ALPHABET; idx++) {
        if (alphabetCount[idx] != 0) {
            char alphabet = 'a' + idx;
            cout << "[" << alphabet << ":" << alphabetCount[idx] << "]" << " ";
        }
    }

    cout << endl;
}

//첫 글자 대문자, 나머지 소문자 변환
void normalize(const char str[], int usingSize) {
    char lowerStr[MAX_INPUT_LENGTH + 1];
    int startIdx = findFirstLetterIdx(str, usingSize);
    if (startIdx == -1) { //빈 문자열 관리
        cout << endl;
        return;
    }
    strcpy(lowerStr, str);

    //변환
    lowerStr[startIdx] = toUpper(lowerStr[startIdx]); //첫 글자 대문자
    for (int idx = startIdx + 1; idx < usingSize; idx++) { //이후로 소문자
        lowerStr[idx] = toLower(lowerStr[idx]);
    }

    //출력
    cout << lowerStr << endl;
}

//gap만큼 이동 암호화
void encryption(const char str[], int usingSize, int gap) {
    //결과 배열
    char resultStr[MAX_INPUT_LENGTH + 1];
    strcpy(resultStr, str);

    //동작
    for (int idx = 0; idx < usingSize; idx++) {
        if (isAlphabet(resultStr[idx])) {
            resultStr[idx] = moveAlphabet(resultStr[idx], gap);
        } else if (isDigit(resultStr[idx])) {
            resultStr[idx] = moveDigit(resultStr[idx], gap);
        }
    }

    //출력
    cout << resultStr << endl;
}

//util---------------------------------------------------------
static char toLower(char ch) {
    if (isUpper(ch))
        return ch - 'A' + 'a';
    return ch;
}

static char toUpper(char ch) {
    if (isLower(ch))
        return ch - 'a' + 'A';
    return ch;
}

static bool isAlphabet(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static bool isUpper(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

static bool isLower(char ch) {
    return ch >= 'a' && ch <= 'z';
}

static bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

static int findFirstLetterIdx(const char str[], int size) {
    for (int idx = 0; idx < size; idx++) {
        if (str[idx] != ' ')
            return idx;
    }
    return -1;
}

static char moveAlphabet(char ch, int gap) {
    //동작상 편의 위해 소문자로 돌린 뒤 계산 후 원복
    int ascii= toLower(ch) + gap;
    if (ascii < 'a')
        ascii += 26;
    else if (ascii > 'z')
        ascii -= 26;

    //대문자였으면 다시 올려서 리턴
    char result = static_cast<char>(ascii);
    if (isUpper(ch))
        return toUpper(result);
    return result;
}

static char moveDigit(char ch, int gap) {
    int result = ch - '0';
    result += gap;
    //음수 정리
    while (result < 0) {
        result += 10;
    }
    //한 자리 수로 정리
    result %= 10;

    return result + '0';
}