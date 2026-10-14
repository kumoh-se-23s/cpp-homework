#include <iostream>
using namespace std;

static void countAlphabets(const char input[]);
static void convertCase(const char input[]);
static void encryptInput(const char input[], int gap);

static char toUpper(char c);
static char toLower(char c);
static bool isUpper(char c);
static bool isLower(char c);
static bool isAlphabet(char c);
static bool isDigit(char c);

int main() {
    const int STR_MAX_LEN = 127;
    char input[STR_MAX_LEN + 1];
    int gap;

    cin.getline(input, STR_MAX_LEN + 1);

    cin >> gap;

    countAlphabets(input);
    convertCase(input);
    encryptInput(input, gap);
}

void countAlphabets(const char input[]) {
    int count[26] = {};

    for (int i = 0; input[i] != '\0'; i++) {
        if (isAlphabet(input[i])) {
            count[toLower(input[i]) - 'a']++;
        }
    }
    for (int i = 0; i < 26; i++) {
        if (count[i] > 0) {
            cout << "[" << (char)('a' + i) << ":" << count[i] << "] ";
        }
    }
    cout << endl;
}

void convertCase(const char input[]) {
    bool firstFound = false;

    for (int i = 0; input[i] != '\0'; i++) {
        if (isAlphabet(input[i])) {
            if (!firstFound) {
                cout << toUpper(input[i]);
                firstFound = true;
            } else {
                cout << toLower(input[i]);
            }
        } else {
            cout << input[i];
        }
    }
    cout << endl;
}
// 원형 queue에서 배열 앞으로 하듯이 %26하면 맞춰질려나
// -23 -> +3이랑 동일
void encryptInput(const char input[], const int gap) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (isUpper(input[i])) {
            char c = ((input[i] - 'A' + gap) % 26 + 26) % 26 + 'A';
            cout << c;
        } else if (isLower(input[i])) {
            char c = ((input[i] - 'a' + gap) % 26 + 26) % 26 + 'a';
            cout << c;
        } else if (isDigit(input[i])) {
            char c = ((input[i] - '0' + gap) % 10 + 10) % 26 + '0';
            cout << c;
        } else {
            cout << input[i];
        }
    }
    cout << endl;
}

bool isUpper(const char c) {
    return c >= 'A' && c <= 'Z';
}

bool isLower(const char c) {
    return c >= 'a' && c <= 'z';
}

bool isAlphabet(const char c) {
    return isUpper(c) || isLower(c);
}

bool isDigit(const char c) {
    return c >= '0' && c <= '9';
}

char toUpper(const char c) {
    if (isLower(c)) {
        return c - 'a' + 'A';
    }
    return c;
}

char toLower(const char c) {
    if (isUpper(c)) {
        return c - 'A' + 'a';
    }
    return c;
}