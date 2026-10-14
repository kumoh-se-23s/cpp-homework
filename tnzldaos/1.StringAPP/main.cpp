#include <iostream>
#include <cstring>
using namespace std;

char toUpper(char inputChar) {
    if('a' <= inputChar && inputChar <= 'z') {
        int alphabetIndex = inputChar - 'a';
        return static_cast<char> ('A' + alphabetIndex);
    } 
    return inputChar;
}

char toLower(char inputChar) {
    if('A' <= inputChar && inputChar <= 'Z') {
        int alphabetIndex = inputChar - 'A';
        return static_cast<char> ('a' + alphabetIndex);
    } 
    return inputChar;
}

void countLetters(char inputCharArr[], int resultCharArr[]) {
    int index{0};

    while (inputCharArr[index] != '\0') {
        int resultArrIndex;

        if('a' <= inputCharArr[index] && inputCharArr[index] <= 'z') {
            resultArrIndex = inputCharArr[index] - 'a';
            resultCharArr[resultArrIndex] = resultCharArr[resultArrIndex] + 1;
        }
        else if ('A' <= inputCharArr[index] && inputCharArr[index] <= 'Z') {
            resultArrIndex = inputCharArr[index] - 'A';
            resultCharArr[resultArrIndex] = resultCharArr[resultArrIndex] + 1;
        }
        
        index++;
    }
}

void toCapitalize(char inputCharArr[], char resultCharArr[]) {
    int index{0};
    bool isFirst{true};

    while (inputCharArr[index] != '\0') {
        if(isFirst && 65 <= inputCharArr[index] && inputCharArr[index] <= 90) {
            resultCharArr[index] = inputCharArr[index];

            isFirst = !isFirst;
        } 
        else if (isFirst && 97 <= inputCharArr[index] && inputCharArr[index] <= 122) {
            resultCharArr[index] = toUpper(inputCharArr[index]);

            isFirst = !isFirst;
        } 
        else {
            resultCharArr[index] = toLower(inputCharArr[index]);
        }

        index++;
    }
}

int calculateCypherIndex(int charIndex, int inputGap, int cycleNum) {
    int tempNum = charIndex + inputGap;

    while (tempNum >= cycleNum || -tempNum >= cycleNum) {
        if (tempNum > 0) {
            tempNum = tempNum - cycleNum;
        }
        else if (tempNum < 0) {
            tempNum = tempNum + cycleNum;
        }
    }


    return tempNum + (tempNum < 0) * cycleNum;

}

void toEncrypted(char inputCharArr[], int inputGap, char resultCharArr[]) {
    constexpr int ALPHABET_CYCLE = 26;
    constexpr int NUM_CYCLE = 10;

    int index{0};

    while (inputCharArr[index] != '\0') {
        int cypherIndex;

        if('a' <= inputCharArr[index] && inputCharArr[index] <= 'z') {
            cypherIndex = calculateCypherIndex(inputCharArr[index] - 'a', inputGap, ALPHABET_CYCLE);

            resultCharArr[index] = static_cast<char> ('a' + cypherIndex);
        } 
        else if ('A' <= inputCharArr[index] && inputCharArr[index] <= 'Z') {
            cypherIndex = calculateCypherIndex(inputCharArr[index] - 'A', inputGap, ALPHABET_CYCLE);

            resultCharArr[index] = static_cast<char> ('A' + cypherIndex);
        }
        else if ('0' <= inputCharArr[index] && inputCharArr[index] <= '9') {
            cypherIndex = calculateCypherIndex(inputCharArr[index] - '0', inputGap, NUM_CYCLE);

            resultCharArr[index] = static_cast<char> ('0' + cypherIndex);
        } 
        else {
            resultCharArr[index] = inputCharArr[index];
        }
        index++;
    }
}

void printAlphabetCount(int alphabetCountArr[], int arrLength) {
    for(int index = 0 ; index < arrLength ; index++) {
        if(alphabetCountArr[index] > 0) {
            cout << "[" << (char) ('a' + index )<< ":" << alphabetCountArr[index] << "]";
            cout << " ";
        }
    }
    cout << endl; 
}

void printSentence(char inputSentenceArr[], int arrLength) {
    for(int index = 0 ; index < arrLength ; index++) {
        cout << inputSentenceArr[index];
    }
    cout << endl;
}

void run() {
    constexpr int INPUT_LENGTH = 128;
    constexpr int ALPHABET_LENGTH = 26;

    char inputCharArr[INPUT_LENGTH];
    int inputGap{0};

    int alphabetCountArr[ALPHABET_LENGTH]{};
    char resultSentenceArr[INPUT_LENGTH]{};

    cin.getline(inputCharArr, INPUT_LENGTH); //공백 상관 없이 다 받기 위함
    cin >> inputGap;

    countLetters(inputCharArr, alphabetCountArr);
    printAlphabetCount(alphabetCountArr, ALPHABET_LENGTH);

    toCapitalize(inputCharArr, resultSentenceArr);
    int arrLength = strlen(resultSentenceArr);
    printSentence(resultSentenceArr, arrLength);

    toEncrypted(inputCharArr, inputGap, resultSentenceArr);
    arrLength = strlen(resultSentenceArr);
    printSentence(resultSentenceArr, arrLength);
}
int main()
{
    run();

    return 0;
}
