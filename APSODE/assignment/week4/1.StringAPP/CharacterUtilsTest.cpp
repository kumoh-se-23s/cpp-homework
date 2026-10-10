//
// Created by leegu on 26. 10. 10..
//

#include <gtest/gtest.h>
#include <cstring>
#include "CharacterUtils.h"

using namespace char_utils;

// ---------- 문자 판별 ----------

TEST(CharUtilsNumericCharacterTest, DigitsAreNumeric) {
    for (char ch = '0'; ch <= '9'; ++ch) {
        EXPECT_TRUE(is_numeric_character(ch)) << "문자: " << ch;
    }
}

TEST(CharUtilsNumericCharacterTest, NonDigitsAreNotNumeric) {
    // '/'와 ':'는 ASCII 상 '0' 바로 앞, '9' 바로 뒤
    const char samples[] = {'a', 'Z', ' ', '+', '-', '.', '/', ':'};
    for (const char ch: samples) {
        EXPECT_FALSE(is_numeric_character(ch)) << "문자: " << ch;
    }
}

TEST(CharUtilsAlphabetCharacterTest, LowerAlphabet) {
    EXPECT_TRUE(is_lower_alphabet('a'));
    EXPECT_TRUE(is_lower_alphabet('m'));
    EXPECT_TRUE(is_lower_alphabet('z'));
}

TEST(CharUtilsAlphabetCharacterTest, LowerAlphabetRejectsOthers) {
    // '`'와 '{'는 ASCII 상 'a' 바로 앞, 'z' 바로 뒤
    const char samples[] = {'A', 'Z', '0', ' ', '`', '{'};
    for (const char ch: samples) {
        EXPECT_FALSE(is_lower_alphabet(ch)) << "문자: " << ch;
    }
}

TEST(CharUtilsAlphabetCharacterTest, UpperAlphabet) {
    EXPECT_TRUE(is_upper_alphabet('A'));
    EXPECT_TRUE(is_upper_alphabet('M'));
    EXPECT_TRUE(is_upper_alphabet('Z'));
}

TEST(CharUtilsAlphabetCharacterTest, UpperAlphabetRejectsOthers) {
    // '@'와 '['는 ASCII 상 'A' 바로 앞, 'Z' 바로 뒤
    const char samples[] = {'a', 'z', '0', ' ', '@', '['};
    for (const char ch: samples) {
        EXPECT_FALSE(is_upper_alphabet(ch)) << "문자: " << ch;
    }
}

TEST(CharUtilsAlphabetCharacterTest, AlphabetCharacterAcceptsBothCases) {
    EXPECT_TRUE(is_alphabet_character('a'));
    EXPECT_TRUE(is_alphabet_character('Z'));
    EXPECT_TRUE(is_alphabet_character('q'));
}

TEST(CharUtilsAlphabetCharacterTest, AlphabetCharacterRejectsOthers) {
    const char samples[] = {'0', '9', ' ', '+', '@', '[', '`', '{'};
    for (const char ch: samples) {
        EXPECT_FALSE(is_alphabet_character(ch)) << "문자: " << ch;
    }
}

// ---------- 문자열 판별 ----------

TEST(CharUtilsNumericOnlyTest, AcceptsDigitStrings) {
    EXPECT_TRUE(is_numeric_only("0"));
    EXPECT_TRUE(is_numeric_only("1234567890"));
    EXPECT_TRUE(is_numeric_only("20261225"));
}

TEST(CharUtilsNumericOnlyTest, RejectsNonDigitContent) {
    EXPECT_FALSE(is_numeric_only("12a4"));
    EXPECT_FALSE(is_numeric_only("a"));
    EXPECT_FALSE(is_numeric_only("12 3"));
    EXPECT_FALSE(is_numeric_only("1.5"));
}

TEST(CharUtilsNumericOnlyTest, RejectsSignedNumbers) {
    // DDayApp이 날짜(yyyymmdd)와 D-day(+/-int)를 구분하는 기준
    EXPECT_FALSE(is_numeric_only("+5"));
    EXPECT_FALSE(is_numeric_only("-5"));
}

TEST(CharUtilsSignedNumericTest, AcceptsSignedDigits) {
    EXPECT_TRUE(is_signed_numeric("+1004"));
    EXPECT_TRUE(is_signed_numeric("-8826"));
    EXPECT_TRUE(is_signed_numeric("+0"));
    EXPECT_TRUE(is_signed_numeric("-1"));
}

TEST(CharUtilsSignedNumericTest, RejectsInvalidContent) {
    EXPECT_FALSE(is_signed_numeric("+abc"));
    EXPECT_FALSE(is_signed_numeric("-a1"));
    EXPECT_FALSE(is_signed_numeric("+12a"));
    EXPECT_FALSE(is_signed_numeric("12-3"));
}

TEST(CharUtilsSignedNumericTest, RejectsRepeatedSigns) {
    EXPECT_FALSE(is_signed_numeric("--1"));
    EXPECT_FALSE(is_signed_numeric("+-1"));
}

TEST(CharUtilsAlphabetOnlyTest, AcceptsLetters) {
    EXPECT_TRUE(is_alphabet_only("abc"));
    EXPECT_TRUE(is_alphabet_only("ABC"));
    EXPECT_TRUE(is_alphabet_only("aBcD"));
}

TEST(CharUtilsAlphabetOnlyTest, RejectsNonLetters) {
    EXPECT_FALSE(is_alphabet_only("abc1"));
    EXPECT_FALSE(is_alphabet_only("a b"));
    EXPECT_FALSE(is_alphabet_only("a_b"));
}

// ---------- 범위 검사 ----------

TEST(CharUtilsRangeTest, AcceptsValidRanges) {
    // 포함/미포함 end 해석과 무관하게 유효한 값들
    EXPECT_TRUE(is_correct_range(0, 3, 8));
    EXPECT_TRUE(is_correct_range(4, 5, 8));
    EXPECT_TRUE(is_correct_range(6, 7, 8));
    EXPECT_TRUE(is_correct_range(0, 7, 8));
}

TEST(CharUtilsRangeTest, RejectsNegativeStart) {
    EXPECT_FALSE(is_correct_range(-1, 3, 8));
}

TEST(CharUtilsRangeTest, RejectsEndBeyondSize) {
    EXPECT_FALSE(is_correct_range(0, 9, 8));
}

TEST(CharUtilsRangeTest, RejectsStartAfterEnd) {
    EXPECT_FALSE(is_correct_range(5, 2, 8));
}

// ---------- 정수 변환 ----------

TEST(CharUtilsToIntegerTest, ConvertsUnsignedDigits) {
    EXPECT_EQ(to_integer("0"), 0);
    EXPECT_EQ(to_integer("7"), 7);
    EXPECT_EQ(to_integer("1234"), 1234);
    EXPECT_EQ(to_integer("20261225"), 20261225);
}

TEST(CharUtilsToIntegerTest, ConvertsPositiveSign) {
    EXPECT_EQ(to_integer("+1004"), 1004);
    EXPECT_EQ(to_integer("+0"), 0);
}

TEST(CharUtilsToIntegerTest, ConvertsNegativeSign) {
    EXPECT_EQ(to_integer("-8826"), -8826);
    EXPECT_EQ(to_integer("-1"), -1);
}

TEST(CharUtilsToIntegerRangeTest, SplitsDateText) {
    // end가 포함 인덱스라고 가정: yyyymmdd를 연/월/일로 분리
    const char date[] = "20261225";
    EXPECT_EQ(to_integer(date, 0, 3, 8), 2026);
    EXPECT_EQ(to_integer(date, 4, 5, 8), 12);
    EXPECT_EQ(to_integer(date, 6, 7, 8), 25);
}

TEST(CharUtilsToIntegerRangeTest, ConvertsWholeText) {
    const char date[] = "20261225";
    EXPECT_EQ(to_integer(date, 0, 7, 8), 20261225);
}

TEST(CharUtilsToIntegerRangeTest, ConvertsSingleCharacter) {
    // end가 포함 인덱스라는 가정에 의존
    const char date[] = "20261225";
    EXPECT_EQ(to_integer(date, 3, 3, 8), 6);
}

TEST(CharUtilsToIntegerRangeTest, KeepsLeadingZero) {
    const char date[] = "20260105";
    EXPECT_EQ(to_integer(date, 4, 5, 8), 1);
    EXPECT_EQ(to_integer(date, 6, 7, 8), 5);
}

TEST(CharUtilsParseIntegerTest, SplitsDateText) {
    // end가 포함 인덱스라고 가정
    const char date[] = "20261225";
    EXPECT_EQ(parse_integer(date, 0, 3), 2026);
    EXPECT_EQ(parse_integer(date, 4, 5), 12);
    EXPECT_EQ(parse_integer(date, 6, 7), 25);
}

TEST(CharUtilsParseIntegerTest, KeepsLeadingZero) {
    const char date[] = "20260105";
    EXPECT_EQ(parse_integer(date, 4, 5), 1);
    EXPECT_EQ(parse_integer(date, 6, 7), 5);
}

TEST(CharUtilsParseIntegerTest, MatchesToIntegerRange) {
    const char date[] = "20240229";
    EXPECT_EQ(parse_integer(date, 0, 3), to_integer(date, 0, 3, 8));
    EXPECT_EQ(parse_integer(date, 4, 5), to_integer(date, 4, 5, 8));
    EXPECT_EQ(parse_integer(date, 6, 7), to_integer(date, 6, 7, 8));
}

// ---------- 길이 / 개수 ----------

TEST(CharUtilsLengthTest, EmptyString) {
    EXPECT_EQ(get_char_array_length(""), 0);
}

TEST(CharUtilsLengthTest, CountsCharactersBeforeNull) {
    EXPECT_EQ(get_char_array_length("a"), 1);
    EXPECT_EQ(get_char_array_length("abc"), 3);
    EXPECT_EQ(get_char_array_length("20261225"), 8);
}

TEST(CharUtilsLengthTest, IgnoresUnusedBufferSpace) {
    char buffer[9] = "abc";
    EXPECT_EQ(get_char_array_length(buffer), 3);
}

TEST(CharUtilsCountAlphabetTest, EmptyAndDigitsOnly) {
    EXPECT_EQ(count_alphabet(""), 0);
    EXPECT_EQ(count_alphabet("123"), 0);
}

TEST(CharUtilsCountAlphabetTest, CountsLetters) {
    EXPECT_EQ(count_alphabet("abc"), 3);
    EXPECT_EQ(count_alphabet("ABCabc"), 6);
}

TEST(CharUtilsCountAlphabetTest, IgnoresNonLetters) {
    EXPECT_EQ(count_alphabet("a1b2c3"), 3);
    EXPECT_EQ(count_alphabet("Hello, World!"), 10);
}

// ---------- 대소문자 변환 (문자 버전) ----------

TEST(CharUtilsToLowerCharTest, ConvertsEveryUpperCase) {
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        EXPECT_EQ(to_lower(ch), static_cast<char>(ch - 'A' + 'a')) << "문자: " << ch;
    }
}

TEST(CharUtilsToLowerCharTest, KeepsLowerCase) {
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        EXPECT_EQ(to_lower(ch), ch) << "문자: " << ch;
    }
}

TEST(CharUtilsToLowerCharTest, KnownValues) {
    EXPECT_EQ(to_lower('A'), 'a');
    EXPECT_EQ(to_lower('M'), 'm');
    EXPECT_EQ(to_lower('Z'), 'z');
}

TEST(CharUtilsToLowerCharTest, NonAlphabetIsUnchanged) {
    // 알파벳이 아닌 문자는 그대로 반환한다는 가정 (구현에 따라 달라질 수 있음)
    const char samples[] = {'0', '9', ' ', '+', '@', '[', '`', '{'};
    for (const char ch: samples) {
        EXPECT_EQ(to_lower(ch), ch) << "문자: " << ch;
    }
}

TEST(CharUtilsToUpperCharTest, ConvertsEveryLowerCase) {
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        EXPECT_EQ(to_upper(ch), static_cast<char>(ch - 'a' + 'A')) << "문자: " << ch;
    }
}

TEST(CharUtilsToUpperCharTest, KeepsUpperCase) {
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        EXPECT_EQ(to_upper(ch), ch) << "문자: " << ch;
    }
}

TEST(CharUtilsToUpperCharTest, KnownValues) {
    EXPECT_EQ(to_upper('a'), 'A');
    EXPECT_EQ(to_upper('m'), 'M');
    EXPECT_EQ(to_upper('z'), 'Z');
}

TEST(CharUtilsToUpperCharTest, NonAlphabetIsUnchanged) {
    // 알파벳이 아닌 문자는 그대로 반환한다는 가정 (구현에 따라 달라질 수 있음)
    const char samples[] = {'0', '9', ' ', '+', '@', '[', '`', '{'};
    for (const char ch: samples) {
        EXPECT_EQ(to_upper(ch), ch) << "문자: " << ch;
    }
}

TEST(CharUtilsCaseRoundTripTest, LowerThenUpperReturnsUpper) {
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        EXPECT_EQ(to_upper(to_lower(ch)), ch) << "문자: " << ch;
    }
}

TEST(CharUtilsCaseRoundTripTest, UpperThenLowerReturnsLower) {
    for (char ch = 'a'; ch <= 'z'; ++ch) {
        EXPECT_EQ(to_lower(to_upper(ch)), ch) << "문자: " << ch;
    }
}

// ---------- 대소문자 변환 (배열 버전, 제자리 변환) ----------

TEST(CharUtilsToLowerArrayTest, ConvertsUpperToLower) {
    char text[] = "ABC";
    EXPECT_TRUE(to_lower(text));
    EXPECT_STREQ(text, "abc");
}

TEST(CharUtilsToLowerArrayTest, ConvertsMixedCase) {
    char text[] = "aBcD";
    EXPECT_TRUE(to_lower(text));
    EXPECT_STREQ(text, "abcd");
}

TEST(CharUtilsToLowerArrayTest, KeepsLowerCase) {
    char text[] = "abc";
    EXPECT_TRUE(to_lower(text));
    EXPECT_STREQ(text, "abc");
}

TEST(CharUtilsToLowerArrayTest, ConvertsFullAlphabet) {
    char text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    EXPECT_TRUE(to_lower(text));
    EXPECT_STREQ(text, "abcdefghijklmnopqrstuvwxyz");
}


// 고민중, 현재는 false를 반환하지 않으며 변환이 필요없는 문자가 포함되어있더라도 변환이 필요한 문자만 변환함
// TEST(CharUtilsToLowerArrayTest, ReportsFailureForNonAlphabet) {
//     // 알파벳 외 문자가 섞이면 false를 반환한다는 가정 (구현에 따라 달라질 수 있음)
//     char text[] = "AB1";
//     EXPECT_FALSE(to_lower(text));
// }

TEST(CharUtilsToUpperArrayTest, ConvertsLowerToUpper) {
    char text[] = "abc";
    EXPECT_TRUE(to_upper(text));
    EXPECT_STREQ(text, "ABC");
}

TEST(CharUtilsToUpperArrayTest, ConvertsMixedCase) {
    char text[] = "aBcD";
    EXPECT_TRUE(to_upper(text));
    EXPECT_STREQ(text, "ABCD");
}

TEST(CharUtilsToUpperArrayTest, KeepsUpperCase) {
    char text[] = "ABC";
    EXPECT_TRUE(to_upper(text));
    EXPECT_STREQ(text, "ABC");
}

TEST(CharUtilsToUpperArrayTest, ConvertsFullAlphabet) {
    char text[] = "abcdefghijklmnopqrstuvwxyz";
    EXPECT_TRUE(to_upper(text));
    EXPECT_STREQ(text, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

// 고민중, 현재는 false를 반환하지 않으며 변환이 필요없는 문자가 포함되어있더라도 변환이 필요한 문자만 변환함
// TEST(CharUtilsToUpperArrayTest, ReportsFailureForNonAlphabet) {
//     char text[] = "ab1";
//     EXPECT_FALSE(to_upper(text));
// }

// ---------- 문자 밀어내기: 영어 (offset_alphabet) ----------

TEST(CharUtilsOffsetAlphabetTest, ExamplesFromSpec) {
    EXPECT_EQ(offset_alphabet('a', -11), 'p');
    EXPECT_EQ(offset_alphabet('a', 3), 'd');
}

TEST(CharUtilsOffsetAlphabetTest, UpperCaseWorksTheSame) {
    EXPECT_EQ(offset_alphabet('A', -11), 'P');
    EXPECT_EQ(offset_alphabet('A', 3), 'D');
}

TEST(CharUtilsOffsetAlphabetTest, ZeroGapKeepsCharacter) {
    EXPECT_EQ(offset_alphabet('a', 0), 'a');
    EXPECT_EQ(offset_alphabet('m', 0), 'm');
    EXPECT_EQ(offset_alphabet('Z', 0), 'Z');
}

TEST(CharUtilsOffsetAlphabetTest, WrapsForwardPastZ) {
    EXPECT_EQ(offset_alphabet('z', 1), 'a');
    EXPECT_EQ(offset_alphabet('y', 3), 'b');
    EXPECT_EQ(offset_alphabet('Z', 1), 'A');
    EXPECT_EQ(offset_alphabet('Y', 3), 'B');
}

TEST(CharUtilsOffsetAlphabetTest, WrapsBackwardBeforeA) {
    EXPECT_EQ(offset_alphabet('a', -1), 'z');
    EXPECT_EQ(offset_alphabet('b', -3), 'y');
    EXPECT_EQ(offset_alphabet('A', -1), 'Z');
    EXPECT_EQ(offset_alphabet('B', -3), 'Y');
}

TEST(CharUtilsOffsetAlphabetTest, HalfAlphabetBoundary) {
    EXPECT_EQ(offset_alphabet('m', 13), 'z');
    EXPECT_EQ(offset_alphabet('n', 13), 'a');
    EXPECT_EQ(offset_alphabet('M', 13), 'Z');
    EXPECT_EQ(offset_alphabet('N', 13), 'A');
}

TEST(CharUtilsOffsetAlphabetTest, MaximumGapValues) {
    EXPECT_EQ(offset_alphabet('a', 25), 'z');
    EXPECT_EQ(offset_alphabet('z', 25), 'y');
    EXPECT_EQ(offset_alphabet('a', -25), 'b');
    EXPECT_EQ(offset_alphabet('z', -25), 'a');
    EXPECT_EQ(offset_alphabet('A', 25), 'Z');
    EXPECT_EQ(offset_alphabet('Z', -25), 'A');
}

TEST(CharUtilsOffsetAlphabetTest, EveryLetterAndGapMatchesModularFormula) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            const char expected = static_cast<char>('a' + ((ch - 'a' + gap) % 26 + 26) % 26);
            EXPECT_EQ(offset_alphabet(ch, gap), expected) << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            const char expected = static_cast<char>('A' + ((ch - 'A' + gap) % 26 + 26) % 26);
            EXPECT_EQ(offset_alphabet(ch, gap), expected) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetAlphabetTest, KeepsCase) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            EXPECT_TRUE(is_lower_alphabet(offset_alphabet(ch, gap))) << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            EXPECT_TRUE(is_upper_alphabet(offset_alphabet(ch, gap))) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetAlphabetTest, OppositeGapRestoresCharacter) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            EXPECT_EQ(offset_alphabet(offset_alphabet(ch, gap), -gap), ch)
                << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            EXPECT_EQ(offset_alphabet(offset_alphabet(ch, gap), -gap), ch)
                << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetAlphabetTest, ShiftIsOneToOne) {
    // 같은 gap으로 서로 다른 26개 문자를 밀면 결과도 모두 달라야 함
    for (int gap = -25; gap <= 25; ++gap) {
        bool seen[26] = {};
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            seen[offset_alphabet(ch, gap) - 'a'] = true;
        }
        for (int i = 0; i < 26; ++i) {
            EXPECT_TRUE(seen[i]) << "gap: " << gap << ", 누락된 문자: " << static_cast<char>('a' + i);
        }
    }
}

// ---------- 문자 밀어내기: 숫자 (offset_numeric) ----------

TEST(CharUtilsOffsetNumericTest, ExamplesFromSpec) {
    EXPECT_EQ(offset_numeric('0', -11), '9');
    EXPECT_EQ(offset_numeric('0', 3), '3');
}

TEST(CharUtilsOffsetNumericTest, ZeroGapKeepsCharacter) {
    for (char ch = '0'; ch <= '9'; ++ch) {
        EXPECT_EQ(offset_numeric(ch, 0), ch) << "문자: " << ch;
    }
}

TEST(CharUtilsOffsetNumericTest, WrapsForwardPastNine) {
    EXPECT_EQ(offset_numeric('9', 1), '0');
    EXPECT_EQ(offset_numeric('7', 3), '0');
    EXPECT_EQ(offset_numeric('8', 5), '3');
}

TEST(CharUtilsOffsetNumericTest, WrapsBackwardBeforeZero) {
    EXPECT_EQ(offset_numeric('0', -1), '9');
    EXPECT_EQ(offset_numeric('2', -5), '7');
    EXPECT_EQ(offset_numeric('3', -11), '2');
}

TEST(CharUtilsOffsetNumericTest, FullCycleOfTenKeepsDigit) {
    // 숫자는 10개 순환이므로 gap이 ±10, ±20이면 제자리
    const int full_cycles[] = {-20, -10, 10, 20};
    for (const int gap: full_cycles) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            EXPECT_EQ(offset_numeric(ch, gap), ch) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetNumericTest, MaximumGapValues) {
    EXPECT_EQ(offset_numeric('0', 25), '5');
    EXPECT_EQ(offset_numeric('9', 25), '4');
    EXPECT_EQ(offset_numeric('0', -25), '5');
    EXPECT_EQ(offset_numeric('9', -25), '4');
}

TEST(CharUtilsOffsetNumericTest, EveryDigitAndGapMatchesModularFormula) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            const char expected = static_cast<char>('0' + ((ch - '0' + gap) % 10 + 10) % 10);
            EXPECT_EQ(offset_numeric(ch, gap), expected) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetNumericTest, ResultIsAlwaysDigit) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            EXPECT_TRUE(is_numeric_character(offset_numeric(ch, gap))) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetNumericTest, OppositeGapRestoresCharacter) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            EXPECT_EQ(offset_numeric(offset_numeric(ch, gap), -gap), ch)
                << "문자: " << ch << ", gap: " << gap;
        }
    }
}

// ---------- 문자 밀어내기: 통합 (offset) ----------

TEST(CharUtilsOffsetTest, ExamplesFromSpecWithGapMinusEleven) {
    EXPECT_EQ(offset('a', -11), 'p');
    EXPECT_EQ(offset('0', -11), '9');
}

TEST(CharUtilsOffsetTest, ExamplesFromSpecWithGapThree) {
    EXPECT_EQ(offset('0', 3), '3');
    EXPECT_EQ(offset('a', 3), 'd');
}

TEST(CharUtilsOffsetTest, UpperCaseWorksTheSame) {
    EXPECT_EQ(offset('A', -11), 'P');
    EXPECT_EQ(offset('A', 3), 'D');
    EXPECT_EQ(offset('Z', 1), 'A');
}

TEST(CharUtilsOffsetTest, DispatchesLowerCaseToAlphabet) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            EXPECT_EQ(offset(ch, gap), offset_alphabet(ch, gap)) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetTest, DispatchesUpperCaseToAlphabet) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            EXPECT_EQ(offset(ch, gap), offset_alphabet(ch, gap)) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetTest, DispatchesDigitToNumeric) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            EXPECT_EQ(offset(ch, gap), offset_numeric(ch, gap)) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetTest, DigitsStayDigitsAndLettersStayLetters) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            EXPECT_TRUE(is_numeric_character(offset(ch, gap))) << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            EXPECT_TRUE(is_lower_alphabet(offset(ch, gap))) << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            EXPECT_TRUE(is_upper_alphabet(offset(ch, gap))) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetTest, OppositeGapRestoresCharacter) {
    // 암호화(밀기) 후 같은 크기로 반대로 밀면 원래 문자로 돌아옴
    for (int gap = -25; gap <= 25; ++gap) {
        for (char ch = '0'; ch <= '9'; ++ch) {
            EXPECT_EQ(offset(offset(ch, gap), -gap), ch) << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'a'; ch <= 'z'; ++ch) {
            EXPECT_EQ(offset(offset(ch, gap), -gap), ch) << "문자: " << ch << ", gap: " << gap;
        }
        for (char ch = 'A'; ch <= 'Z'; ++ch) {
            EXPECT_EQ(offset(offset(ch, gap), -gap), ch) << "문자: " << ch << ", gap: " << gap;
        }
    }
}

TEST(CharUtilsOffsetTest, NonAlphanumericIsUnchanged) {
    // 영어/숫자가 아닌 문자는 그대로 반환한다는 가정 (구현에 따라 달라질 수 있음)
    const char samples[] = {' ', '+', '-', '@', '[', '`', '{', '/', ':'};
    for (const char ch: samples) {
        EXPECT_EQ(offset(ch, 3), ch) << "문자: " << ch;
        EXPECT_EQ(offset(ch, -11), ch) << "문자: " << ch;
    }
}

// ---------- 알파벳 개수 세기 (count_alphabet_by_array) ----------

namespace {
    constexpr int ALPHABET_COUNT = 26;

    // 배열을 0으로 채운 뒤 호출 (함수가 초기화를 하는지 여부와 무관하게 동작)
    void count_into(const char text[], int counts[]) {
        for (int i = 0; i < ALPHABET_COUNT; ++i) {
            counts[i] = 0;
        }
        count_alphabet_by_array(text, counts);
    }

    int sum_of(const int counts[]) {
        int sum = 0;
        for (int i = 0; i < ALPHABET_COUNT; ++i) {
            sum += counts[i];
        }
        return sum;
    }
}

TEST(CharUtilsCountByArrayTest, EmptyStringKeepsAllZero) {
    int counts[ALPHABET_COUNT];
    count_into("", counts);

    for (int i = 0; i < ALPHABET_COUNT; ++i) {
        EXPECT_EQ(counts[i], 0) << "인덱스: " << i;
    }
}

TEST(CharUtilsCountByArrayTest, SingleLetter) {
    int counts[ALPHABET_COUNT];
    count_into("a", counts);

    EXPECT_EQ(counts[0], 1);
    EXPECT_EQ(sum_of(counts), 1);
}

TEST(CharUtilsCountByArrayTest, LastLetterUsesLastIndex) {
    int counts[ALPHABET_COUNT];
    count_into("z", counts);

    EXPECT_EQ(counts[25], 1);
    EXPECT_EQ(sum_of(counts), 1);
}

TEST(CharUtilsCountByArrayTest, EachLetterOnce) {
    int counts[ALPHABET_COUNT];
    count_into("abcdefghijklmnopqrstuvwxyz", counts);

    for (int i = 0; i < ALPHABET_COUNT; ++i) {
        EXPECT_EQ(counts[i], 1) << "인덱스: " << i;
    }
}

TEST(CharUtilsCountByArrayTest, RepeatedLetter) {
    int counts[ALPHABET_COUNT];
    count_into("aaaa", counts);

    EXPECT_EQ(counts[0], 4);
    EXPECT_EQ(sum_of(counts), 4);
}

TEST(CharUtilsCountByArrayTest, MixedCounts) {
    int counts[ALPHABET_COUNT];
    count_into("banana", counts);

    EXPECT_EQ(counts['a' - 'a'], 3);
    EXPECT_EQ(counts['b' - 'a'], 1);
    EXPECT_EQ(counts['n' - 'a'], 2);
    EXPECT_EQ(sum_of(counts), 6);
}

TEST(CharUtilsCountByArrayTest, IgnoresDigitsSpacesAndSymbols) {
    int counts[ALPHABET_COUNT];
    count_into("a1b2!c d,e.", counts);

    EXPECT_EQ(counts['a' - 'a'], 1);
    EXPECT_EQ(counts['b' - 'a'], 1);
    EXPECT_EQ(counts['c' - 'a'], 1);
    EXPECT_EQ(counts['d' - 'a'], 1);
    EXPECT_EQ(counts['e' - 'a'], 1);
    EXPECT_EQ(sum_of(counts), 5);
}

TEST(CharUtilsCountByArrayTest, NoLettersKeepsAllZero) {
    int counts[ALPHABET_COUNT];
    count_into("1234 !@#$ 5678", counts);

    EXPECT_EQ(sum_of(counts), 0);
}

TEST(CharUtilsCountByArrayTest, BoundaryCharactersAreNotCounted) {
    // '@', '[', '`', '{'는 알파벳 범위 바로 바깥의 ASCII 문자
    int counts[ALPHABET_COUNT];
    count_into("@[`{", counts);

    EXPECT_EQ(sum_of(counts), 0);
}

TEST(CharUtilsCountByArrayTest, PangramCounts) {
    int counts[ALPHABET_COUNT];
    count_into("the quick brown fox jumps over the lazy dog", counts);

    EXPECT_EQ(counts['o' - 'a'], 4);
    EXPECT_EQ(counts['e' - 'a'], 3);
    EXPECT_EQ(counts['u' - 'a'], 2);
    EXPECT_EQ(counts['h' - 'a'], 2);
    EXPECT_EQ(counts['r' - 'a'], 2);
    EXPECT_EQ(counts['t' - 'a'], 2);
    EXPECT_EQ(counts['a' - 'a'], 1);
    EXPECT_EQ(counts['z' - 'a'], 1);
    EXPECT_EQ(sum_of(counts), 35);
}

TEST(CharUtilsCountByArrayTest, TotalMatchesCountAlphabet) {
    const char *samples[] = {"", "abc", "Hello, World!", "a1b2c3", "the quick brown fox"};

    for (const char *text: samples) {
        int counts[ALPHABET_COUNT];
        count_into(text, counts);

        EXPECT_EQ(sum_of(counts), count_alphabet(text)) << "문자열: " << text;
    }
}

// 아래 세 개는 "대소문자를 구분하지 않고 같은 칸에 센다"는 가정에 의존합니다.

TEST(CharUtilsCountByArrayTest, UpperCaseCountsAtSameIndex) {
    int counts[ALPHABET_COUNT];
    count_into("ABC", counts);

    EXPECT_EQ(counts[0], 1);
    EXPECT_EQ(counts[1], 1);
    EXPECT_EQ(counts[2], 1);
    EXPECT_EQ(sum_of(counts), 3);
}

TEST(CharUtilsCountByArrayTest, MixedCaseIsMergedIntoOneCount) {
    int counts[ALPHABET_COUNT];
    count_into("aAaA", counts);

    EXPECT_EQ(counts[0], 4);
    EXPECT_EQ(sum_of(counts), 4);
}

TEST(CharUtilsCountByArrayTest, SentenceWithMixedCase) {
    int counts[ALPHABET_COUNT];
    count_into("Hello World", counts);

    EXPECT_EQ(counts['l' - 'a'], 3);
    EXPECT_EQ(counts['o' - 'a'], 2);
    EXPECT_EQ(counts['h' - 'a'], 1);
    EXPECT_EQ(counts['w' - 'a'], 1);
    EXPECT_EQ(sum_of(counts), 10);
}