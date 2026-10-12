#include <gtest/gtest.h>
#include <string>

#include "TextFormatter.h"

namespace {
    const int BUFFER_SIZE = 127;
    const int MAX_OF_ALPHABETS = 26;
    const int MAX_OF_DIGITS = 10;


    std::string capitalize(const char *text) {
        char result[BUFFER_SIZE] = {};
        TextFormatter::capitalizeFirstLetter(text, result);
        return result;
    }

    std::string encrypt(const char *text, int gap) {
        char result[BUFFER_SIZE] = {};
        TextFormatter::encryptSentence(text, result, gap);
        return result;
    }

    // 알파벳 개수를 "[a:2] [b:1]" 형태로 바꿔서 비교하기 쉽게 만든다.
    std::string count_to_string(const char *text) {
        int counts[MAX_OF_ALPHABETS] = {};
        TextFormatter::countAlphabet(text, counts);

        std::string result;
        for (int index = 0; index < MAX_OF_ALPHABETS; ++index) {
            if (counts[index] > 0) {
                if (!result.empty()) {
                    result += " ";
                }
                result += "[";
                result += static_cast<char>('a' + index);
                result += ":" + std::to_string(counts[index]) + "]";
            }
        }
        return result;
    }

    const char *REFERENCE_TEXT = "This is A My 1-st Test PROGram.";
}

// =====================================================================
// 상수
// =====================================================================

TEST(TextFormatterConstants, AlphabetAndDigitCounts) {
    EXPECT_EQ(26, MAX_OF_ALPHABETS);
    EXPECT_EQ(10, MAX_OF_DIGITS);
}

// =====================================================================
// isAlphabet / isDigit
// =====================================================================

TEST(TextFormatterIsAlphabet, TrueForLowerAndUpperCase) {
    for (char chr = 'a'; chr <= 'z'; ++chr) {
        EXPECT_TRUE(TextFormatter::isAlphabet(chr)) << "char: " << chr;
    }
    for (char chr = 'A'; chr <= 'Z'; ++chr) {
        EXPECT_TRUE(TextFormatter::isAlphabet(chr)) << "char: " << chr;
    }
}

TEST(TextFormatterIsAlphabet, FalseForDigitsAndSymbols) {
    EXPECT_FALSE(TextFormatter::isAlphabet('0'));
    EXPECT_FALSE(TextFormatter::isAlphabet('9'));
    EXPECT_FALSE(TextFormatter::isAlphabet(' '));
    EXPECT_FALSE(TextFormatter::isAlphabet('-'));
    EXPECT_FALSE(TextFormatter::isAlphabet('.'));
    EXPECT_FALSE(TextFormatter::isAlphabet('\0'));
}

TEST(TextFormatterIsAlphabet, FalseForCharactersAdjacentToRanges) {
    EXPECT_FALSE(TextFormatter::isAlphabet('@'));    // 'A' 바로 앞
    EXPECT_FALSE(TextFormatter::isAlphabet('['));    // 'Z' 바로 뒤
    EXPECT_FALSE(TextFormatter::isAlphabet('`'));    // 'a' 바로 앞
    EXPECT_FALSE(TextFormatter::isAlphabet('{'));    // 'z' 바로 뒤
}

TEST(TextFormatterIsDigit, TrueForAllDigits) {
    for (char chr = '0'; chr <= '9'; ++chr) {
        EXPECT_TRUE(TextFormatter::isDigit(chr)) << "char: " << chr;
    }
}

TEST(TextFormatterIsDigit, FalseForLettersAndSymbols) {
    EXPECT_FALSE(TextFormatter::isDigit('a'));
    EXPECT_FALSE(TextFormatter::isDigit('Z'));
    EXPECT_FALSE(TextFormatter::isDigit(' '));
    EXPECT_FALSE(TextFormatter::isDigit('-'));
    EXPECT_FALSE(TextFormatter::isDigit('\0'));
}

TEST(TextFormatterIsDigit, FalseForCharactersAdjacentToRange) {
    EXPECT_FALSE(TextFormatter::isDigit('/'));    // '0' 바로 앞
    EXPECT_FALSE(TextFormatter::isDigit(':'));    // '9' 바로 뒤
}

// =====================================================================
// toUpper / toLower
// =====================================================================

TEST(TextFormatterToUpper, ConvertsLowerCase) {
    EXPECT_EQ('A', TextFormatter::toUpper('a'));
    EXPECT_EQ('M', TextFormatter::toUpper('m'));
    EXPECT_EQ('Z', TextFormatter::toUpper('z'));
}

TEST(TextFormatterToUpper, KeepsUpperCase) {
    EXPECT_EQ('A', TextFormatter::toUpper('A'));
    EXPECT_EQ('Z', TextFormatter::toUpper('Z'));
}

TEST(TextFormatterToUpper, KeepsDigitsAndSymbols) {
    EXPECT_EQ('1', TextFormatter::toUpper('1'));
    EXPECT_EQ(' ', TextFormatter::toUpper(' '));
    EXPECT_EQ('-', TextFormatter::toUpper('-'));
    EXPECT_EQ('{', TextFormatter::toUpper('{'));
    EXPECT_EQ('`', TextFormatter::toUpper('`'));
}

TEST(TextFormatterToLower, ConvertsUpperCase) {
    EXPECT_EQ('a', TextFormatter::toLower('A'));
    EXPECT_EQ('m', TextFormatter::toLower('M'));
    EXPECT_EQ('z', TextFormatter::toLower('Z'));
}

TEST(TextFormatterToLower, KeepsLowerCase) {
    EXPECT_EQ('a', TextFormatter::toLower('a'));
    EXPECT_EQ('z', TextFormatter::toLower('z'));
}

TEST(TextFormatterToLower, KeepsDigitsAndSymbols) {
    EXPECT_EQ('1', TextFormatter::toLower('1'));
    EXPECT_EQ(' ', TextFormatter::toLower(' '));
    EXPECT_EQ('-', TextFormatter::toLower('-'));
    EXPECT_EQ('@', TextFormatter::toLower('@'));
    EXPECT_EQ('[', TextFormatter::toLower('['));
}

TEST(TextFormatterCase, UpperAndLowerAreInverseForLetters) {
    for (char chr = 'a'; chr <= 'z'; ++chr) {
        EXPECT_EQ(chr, TextFormatter::toLower(TextFormatter::toUpper(chr))) << "char: " << chr;
    }
}

// =====================================================================
// countAlphabet
// =====================================================================

TEST(TextFormatterCountAlphabet, CountsEachLetter) {
    EXPECT_EQ("[a:1] [b:1] [c:1]", count_to_string("abc"));
}

TEST(TextFormatterCountAlphabet, MergesUpperAndLowerCase) {
    EXPECT_EQ("[a:2] [b:2]", count_to_string("AaBb"));
}

TEST(TextFormatterCountAlphabet, CountsRepeatedLetters) {
    EXPECT_EQ("[d:1] [e:1] [h:1] [l:3] [o:2] [r:1] [w:1]", count_to_string("Hello World"));
}

TEST(TextFormatterCountAlphabet, IgnoresDigitsAndSymbols) {
    EXPECT_EQ("[a:1] [b:1] [c:1]", count_to_string("a-b, c!"));
}

TEST(TextFormatterCountAlphabet, NoLettersLeavesAllZero) {
    EXPECT_EQ("", count_to_string("2026 10 09"));
}

TEST(TextFormatterCountAlphabet, EmptyTextLeavesAllZero) {
    EXPECT_EQ("", count_to_string(""));
}

TEST(TextFormatterCountAlphabet, FillsArrayByAlphabetIndex) {
    int counts[MAX_OF_ALPHABETS] = {};

    TextFormatter::countAlphabet("azAZz", counts);

    EXPECT_EQ(2, counts[0]);     // a, A
    EXPECT_EQ(3, counts[25]);    // z, Z, z
    EXPECT_EQ(0, counts[1]);
}

TEST(TextFormatterCountAlphabet, ReferenceSentence) {
    EXPECT_EQ("[a:2] [e:1] [g:1] [h:1] [i:2] [m:2] [o:1] [p:1] [r:2] [s:4] [t:4] [y:1]",
              count_to_string(REFERENCE_TEXT));
}

TEST(TextFormatterCountAlphabet, TotalEqualsNumberOfLetters) {
    int counts[MAX_OF_ALPHABETS] = {};
    const std::string text = "The quick brown fox, 123 jumps over the lazy dog!";

    TextFormatter::countAlphabet(text.c_str(), counts);

    int total = 0;
    for (int count: counts) {
        total += count;
    }
    int expected = 0;
    for (const char chr: text) {
        if (TextFormatter::isAlphabet(chr)) {
            ++expected;
        }
    }
    EXPECT_EQ(expected, total);
}

TEST(TextFormatterCountAlphabet, DoesNotModifyInput) {
    char text[] = "Hello";
    int counts[MAX_OF_ALPHABETS] = {};

    TextFormatter::countAlphabet(text, counts);

    EXPECT_STREQ("Hello", text);
}

// =====================================================================
// capitalizeFirstLetter
// =====================================================================

TEST(TextFormatterCapitalize, UppersFirstLowerCaseLetter) {
    EXPECT_EQ("Hello", capitalize("hello"));
}

TEST(TextFormatterCapitalize, LowersTheRest) {
    EXPECT_EQ("Hello world", capitalize("HELLO WORLD"));
}

TEST(TextFormatterCapitalize, KeepsAlreadyCapitalizedText) {
    EXPECT_EQ("Hello world", capitalize("Hello world"));
}

TEST(TextFormatterCapitalize, SingleLetter) {
    EXPECT_EQ("A", capitalize("a"));
    EXPECT_EQ("A", capitalize("A"));
}

TEST(TextFormatterCapitalize, KeepsDigitsAndSymbols) {
    EXPECT_EQ("2026 10 09", capitalize("2026 10 09"));
    EXPECT_EQ("-- !!", capitalize("-- !!"));
}

TEST(TextFormatterCapitalize, EmptyTextGivesEmptyResult) {
    EXPECT_EQ("", capitalize(""));
}

TEST(TextFormatterCapitalize, ReferenceSentence) {
    EXPECT_EQ("This is a my 1-st test program.", capitalize(REFERENCE_TEXT));
}

// 가정: 문장이 숫자/기호로 시작하면 처음 나오는 알파벳이 대문자가 된다. (getFirstLetterIndex)
TEST(TextFormatterCapitalize, SkipsLeadingNonLetters) {
    EXPECT_EQ("123 abc", capitalize("123 abc"));
    EXPECT_EQ("  Hello", capitalize("  hELLO"));
}

TEST(TextFormatterCapitalize, OnlyFirstLetterIsUpper) {
    const std::string result = capitalize("aBcDeF gHiJ");

    EXPECT_EQ("Abcdef ghij", result);
}

TEST(TextFormatterCapitalize, KeepsLength) {
    const std::string result = capitalize(REFERENCE_TEXT);

    EXPECT_EQ(std::string(REFERENCE_TEXT).size(), result.size());
}

TEST(TextFormatterCapitalize, DoesNotModifyInput) {
    char text[] = "hELLO";
    char result[BUFFER_SIZE] = {};

    TextFormatter::capitalizeFirstLetter(text, result);

    EXPECT_STREQ("hELLO", text);
    EXPECT_STREQ("Hello", result);
}

// =====================================================================
// encryptSentence
// =====================================================================

TEST(TextFormatterEncrypt, GapZeroKeepsText) {
    EXPECT_EQ("Hello 123", encrypt("Hello 123", 0));
}

TEST(TextFormatterEncrypt, ShiftsLettersAndKeepsCase) {
    EXPECT_EQ("Ifmmp Xpsme", encrypt("Hello World", 1));
}

TEST(TextFormatterEncrypt, WrapsUpperCase) {
    EXPECT_EQ("DEF ABC", encrypt("ABC XYZ", 3));
}

TEST(TextFormatterEncrypt, WrapsLowerCase) {
    EXPECT_EQ("abc", encrypt("xyz", 3));
}

TEST(TextFormatterEncrypt, NegativeGapShiftsBackward) {
    EXPECT_EQ("abc", encrypt("def", -3));
}

TEST(TextFormatterEncrypt, NegativeGapWrapsBackward) {
    EXPECT_EQ("xyz", encrypt("abc", -3));
    EXPECT_EQ("XYZ", encrypt("ABC", -3));
}

TEST(TextFormatterEncrypt, MaximumGapValues) {
    EXPECT_EQ("zab", encrypt("abc", 25));
    EXPECT_EQ("bcd", encrypt("abc", -25));
}

TEST(TextFormatterEncrypt, ShiftsDigitsWithinTenValues) {
    EXPECT_EQ("7571 65 54", encrypt("2026 10 09", 5));
}

TEST(TextFormatterEncrypt, DigitsWrapForward) {
    EXPECT_EQ("0", encrypt("7", 3));
    EXPECT_EQ("2", encrypt("9", 3));
}

TEST(TextFormatterEncrypt, DigitsWrapBackward) {
    EXPECT_EQ("9", encrypt("0", -1));
    EXPECT_EQ("7", encrypt("0", -3));
}

TEST(TextFormatterEncrypt, LeavesSymbolsUnchanged) {
    EXPECT_EQ("b-c, d!", encrypt("a-b, c!", 1));
}

// 과제 조건 예시: gap == -11 일 때 'a' -> 'p', '0' -> '9' (대문자도 동일)
TEST(TextFormatterEncrypt, MinusElevenExamples) {
    EXPECT_EQ("p", encrypt("a", -11));
    EXPECT_EQ("P", encrypt("A", -11));
    EXPECT_EQ("9", encrypt("0", -11));
}

TEST(TextFormatterEncrypt, ReferenceSentenceGapThree) {
    EXPECT_EQ("Wklv lv D Pb 4-vw Whvw SURJudp.", encrypt(REFERENCE_TEXT, 3));
}

TEST(TextFormatterEncrypt, ReferenceSentenceGapMinusTwentyThree) {
    EXPECT_EQ("Wklv lv D Pb 8-vw Whvw SURJudp.", encrypt(REFERENCE_TEXT, -23));
}

TEST(TextFormatterEncrypt, EmptyTextGivesEmptyResult) {
    EXPECT_EQ("", encrypt("", 5));
}

TEST(TextFormatterEncrypt, KeepsLength) {
    for (int gap = -25; gap <= 25; ++gap) {
        EXPECT_EQ(std::string(REFERENCE_TEXT).size(), encrypt(REFERENCE_TEXT, gap).size()) << "gap: " << gap;
    }
}

TEST(TextFormatterEncrypt, KeepsCaseOfEveryLetter) {
    const std::string source = REFERENCE_TEXT;

    for (int gap = -25; gap <= 25; ++gap) {
        const std::string result = encrypt(REFERENCE_TEXT, gap);
        for (std::size_t index = 0; index < source.size(); ++index) {
            const char original = source[index];
            if (original >= 'a' && original <= 'z') {
                EXPECT_TRUE(result[index] >= 'a' && result[index] <= 'z') << "gap: " << gap << ", index: " << index;
            } else if (original >= 'A' && original <= 'Z') {
                EXPECT_TRUE(result[index] >= 'A' && result[index] <= 'Z') << "gap: " << gap << ", index: " << index;
            } else if (original >= '0' && original <= '9') {
                EXPECT_TRUE(result[index] >= '0' && result[index] <= '9') << "gap: " << gap << ", index: " << index;
            } else {
                EXPECT_EQ(original, result[index]) << "gap: " << gap << ", index: " << index;
            }
        }
    }
}

TEST(TextFormatterEncrypt, OppositeGapRestoresOriginalText) {
    for (int gap = -25; gap <= 25; ++gap) {
        const std::string encrypted = encrypt(REFERENCE_TEXT, gap);

        EXPECT_EQ(REFERENCE_TEXT, encrypt(encrypted.c_str(), -gap)) << "gap: " << gap;
    }
}

TEST(TextFormatterEncrypt, AlphabetGapIsPeriodicWithTwentySix) {
    // 알파벳만 있는 문장은 gap과 gap - 26이 같은 결과여야 한다.
    for (int gap = 1; gap <= 25; ++gap) {
        EXPECT_EQ(encrypt("Hello World", gap), encrypt("Hello World", gap - 26)) << "gap: " << gap;
    }
}

TEST(TextFormatterEncrypt, DoesNotModifyInput) {
    char text[] = "Hello";
    char result[BUFFER_SIZE] = {};

    TextFormatter::encryptSentence(text, result, 2);

    EXPECT_STREQ("Hello", text);
    EXPECT_STREQ("Jgnnq", result);
}

TEST(TextFormatterEncrypt, ResultIsNullTerminated) {
    char result[BUFFER_SIZE] = {};

    TextFormatter::encryptSentence("abc", result, 1);

    EXPECT_STREQ("bcd", result);
}