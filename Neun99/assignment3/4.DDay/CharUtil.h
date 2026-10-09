#pragma once

class CharUtil {
public:
    static int toInt(const char arr[], int startIdx, int endIdx);
    static int getLength(const char arr[], int maxSize);
    static bool isDigit(char ch);
    static bool isAllDigit(const char arr[], int startIdx, int endIdx);
};