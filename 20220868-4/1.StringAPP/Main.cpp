#include <iostream>


bool isAlphabet(char ch, int *idx = nullptr, bool *isLower = nullptr){
    constexpr int ALPHABETS = 26;
    int i = ch - 'a';
    int j = ch - 'A';
    if((i < 0 || i >= ALPHABETS) && (j < 0 || j >= ALPHABETS)) return false;   
    if(idx) *idx = i >= 0 ? i : j;
    if(isLower) *isLower = (ch - 'A') >> 5;
    return true;
}

int main(){
    constexpr int MAX_STR_LEN = 127;
    constexpr int ALPHABETS = 26;
    char buf[MAX_STR_LEN + 1]; // no initialization 
    char ceasar[MAX_STR_LEN + 1];
    int gap;
    int alphabetCnt[ALPHABETS]{};
    char ch;
    int currLen = 0;
    while((ch = std::cin.get()) != '\n' && currLen < MAX_STR_LEN) {
        buf[currLen++] = ch;
        int idx;
        if(isAlphabet(ch, &idx)) ++alphabetCnt[idx];
    }

    std::cin >> gap;
    bool firstAlphabetAppeared = false;
    for(int i = 0; i < ALPHABETS; ++i){
        if(alphabetCnt[i] > 0) std::cout << "[" << static_cast<char>(i + 'a') << ":" << alphabetCnt[i] << "] ";
    }
    
    for(int i = 0; buf[i] != '\0'; ++i){
        char ch1 = buf[i];
        int num = ch1 - '0';
        int idx;
        bool lower;
        if(isAlphabet(ch1, &idx, &lower)){

            if(firstAlphabetAppeared){
                ch1 = idx + 'a';
            }else{
                ch1 = idx + 'A';
                firstAlphabetAppeared = true;
            }
            buf[i] = ch1; //fix the buffer
            const int newChIdx = (idx + gap + 26) % 26;
            ceasar[i] = newChIdx + (lower ? 'a' : 'A');

        }else if(num >= 0 && num <= 9){
            num = (num + 30 + gap) % 10;
            ceasar[i] = num + '0';
        }else{
            ceasar[i] = ch1;
        }
    }
    std::cout << std::endl;
    
    buf[currLen] = '\0';
    ceasar[currLen] = '\0';
    std::cout << buf << std::endl;
    std::cout << ceasar << std::endl;



    return 0;
}