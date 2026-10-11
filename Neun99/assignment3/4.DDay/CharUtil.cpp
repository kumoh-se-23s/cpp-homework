#include "CharUtil.h"

//배열의 지정된 범위 int로 변환
int CharUtil::toInt(const char arr[], int startIdx, int endIdx) {
    int result = 0;
    for (int idx = startIdx; idx < endIdx; idx++) {
        result = result * 10 + (arr[idx] - '0');
    }
    return result;
}

//배열에서 실제로 사용중인 길이 반환
int CharUtil::getLength(const char arr[], int maxSize){
    int idx;
    for (idx = 0; idx < maxSize && arr[idx] != '\0';idx++) {} //범위 안에서 null이 아닌 동안 idx++
    return idx;
}

//글자 하나 숫자인지 반환
bool CharUtil::isDigit(char ch){
    int result = ch - '0';
    return (result >= 0 && result <= 9);
}

//배열에서 지정한 범위가 다 숫자인지 반환
bool CharUtil::isAllDigit(const char arr[], int startIdx, int endIdx){
    for (int idx = startIdx; idx < endIdx; idx++) {
        if (!isDigit(arr[idx]))
            return false;
    }
    return true;
}