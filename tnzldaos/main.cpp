#include <iostream>
#include <cstring>
using namespace std;

void countLetters(char inputCharArr[], int resultCharArr[]) {
    int index{0};

    while (inputCharArr[index] != '\0') {
        int resultArrIndex = tolower(inputCharArr[index]) - 'a';

        if(0 <= resultArrIndex && resultArrIndex <= 25) {
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
            resultCharArr[index] = (char) toupper(inputCharArr[index]);

            isFirst = !isFirst;
        } 
        else {
            resultCharArr[index] = (char) tolower(inputCharArr[index]);
        }

        index++;
    }
}

int calculateCypherIndex(int charIndex, int inputGap, int cycleNum) {
    int tempNum = charIndex + inputGap;

    while (tempNum > cycleNum) {
        //tempNum = tempNum + (-(tempNum > 0)) * cycleNum;

        if (tempNum > 0) {
            tempNum = tempNum - cycleNum;
        }
        else if (tempNum < 0) {
            tempNum = tempNum + cycleNum;
        }
    }
    return tempNum;

}

void toEncrypted(char inputCharArr[], int inputGap, char resultCharArr[]) {
    constexpr int ALPHABET_CYCLE = 26;
    constexpr int NUM_CYCLE = 9;
    constexpr char CYPHER_TABLE[2][26] = {
    {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'},
    {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}
    };

    int index{0};

    while (inputCharArr[index] != '\0') {
        if('a' <= inputCharArr[index] && inputCharArr[index] <= 'z') {
            int cypherIndex = calculateCypherIndex(inputCharArr[index] - 'a', inputGap, ALPHABET_CYCLE);

            resultCharArr[index] = CYPHER_TABLE[0][cypherIndex];

        } 
        else if ('A' <= inputCharArr[index] && inputCharArr[index] <= 'Z') {
            int cypherIndex = calculateCypherIndex(inputCharArr[index] - 'A', inputGap, ALPHABET_CYCLE);

            resultCharArr[index] = CYPHER_TABLE[1][cypherIndex];
        }
        else if ('0' <= inputCharArr[index] && inputCharArr[index] <= '9') {
            int cypherIndex = calculateCypherIndex(inputCharArr[index] - '0', inputGap, NUM_CYCLE);

            resultCharArr[index] = (char) cypherIndex + '0';
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
            cout << "[" << (char) ('a' + index )<< " : " << alphabetCountArr[index] << "]";
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
    int arrLength = strlen(resultSentenceArr);
    printSentence(resultSentenceArr, arrLength);
}
int main()
{
    run();

    return 0;
}
