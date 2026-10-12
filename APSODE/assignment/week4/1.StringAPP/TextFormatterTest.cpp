//
// Created by leegu on 26. 10. 12..
//
#include <gtest/gtest.h>
#include <string>
#include "TextFormatter.h"

namespace {
    constexpr int BUFFER_SIZE = TextFormatter::MAX_INPUT_LENGTH;

    std::string build_text(TextFormatter &formatter) {
        char buffer[BUFFER_SIZE] = {};
        formatter.build(buffer);
        return std::string(buffer);
    }

    std::string capitalized(const char text[]) {
        TextFormatter formatter(text);
        formatter.capitalize_first();
        return build_text(formatter);
    }

    std::string encrypted(const char text[], const int gap) {
        TextFormatter formatter(text);
        formatter.encrypt(gap);
        return build_text(formatter);
    }

    const char REFERENCE_TEXT[] = "This is A My 1-st Test PROGram.";
}

// ---------- 상수 ----------

TEST(TextFormatterConstantsTest, MatchSpecification) {
    EXPECT_EQ(128, TextFormatter::MAX_INPUT_LENGTH);
    EXPECT_EQ(-25, TextFormatter::MIN_GAP);
    EXPECT_EQ(25, TextFormatter::MAX_GAP);
}

// ---------- 생성 / build ----------

// 기본 생성자가 빈 문자열로 초기화한다는 가정
TEST(TextFormatterBuildTest, DefaultConstructorBuildsEmptyString) {
    TextFormatter formatter;

    EXPECT_EQ("", build_text(formatter));
}

TEST(TextFormatterBuildTest, KeepsContentWithoutFormatting) {
    TextFormatter formatter("Hello, World! 123");

    EXPECT_EQ("Hello, World! 123", build_text(formatter));
}

TEST(TextFormatterBuildTest, EmptyTextBuildsEmptyString) {
    TextFormatter formatter("");

    EXPECT_EQ("", build_text(formatter));
}

TEST(TextFormatterBuildTest, CopiesSourceText) {
    char source[] = "abc";
    TextFormatter formatter(source);

    source[0] = 'z';

    EXPECT_EQ("abc", build_text(formatter));
}

TEST(TextFormatterBuildTest, CanBeCalledRepeatedlyWithSameResult) {
    TextFormatter formatter("Hello");

    EXPECT_EQ("Hello", build_text(formatter));
    EXPECT_EQ("Hello", build_text(formatter));
}

TEST(TextFormatterBuildTest, MaximumLengthTextIsKept) {
    // 널 문자를 제외한 최대 길이는 127
    const std::string text(TextFormatter::MAX_INPUT_LENGTH - 1, 'a');
    TextFormatter formatter(text.c_str());

    EXPECT_EQ(text, build_text(formatter));
}

// 최대 길이를 넘는 입력은 127글자까지만 저장한다는 가정
TEST(TextFormatterBuildTest, LongerTextIsTruncated) {
    const std::string text(200, 'a');
    TextFormatter formatter(text.c_str());

    EXPECT_EQ(std::string(TextFormatter::MAX_INPUT_LENGTH - 1, 'a'), build_text(formatter));
}

// ---------- 첫 글자 대문자화 ----------

TEST(TextFormatterCapitalizeTest, UppersLowerCaseFirstLetter) {
    EXPECT_EQ("Hello", capitalized("hello"));
}

TEST(TextFormatterCapitalizeTest, LowersTheRest) {
    EXPECT_EQ("Hello world", capitalized("HELLO WORLD"));
}

TEST(TextFormatterCapitalizeTest, KeepsAlreadyCapitalizedText) {
    EXPECT_EQ("Hello world", capitalized("Hello world"));
}

TEST(TextFormatterCapitalizeTest, SingleLetter) {
    EXPECT_EQ("A", capitalized("a"));
    EXPECT_EQ("A", capitalized("A"));
}

TEST(TextFormatterCapitalizeTest, ReferenceSentence) {
    EXPECT_EQ("This is a my 1-st test program.", capitalized(REFERENCE_TEXT));
}

TEST(TextFormatterCapitalizeTest, DigitsAndSymbolsAreUnchanged) {
    EXPECT_EQ("2026 10 09", capitalized("2026 10 09"));
    EXPECT_EQ("!@# $%^", capitalized("!@# $%^"));
}

TEST(TextFormatterCapitalizeTest, EmptyTextStaysEmpty) {
    EXPECT_EQ("", capitalized(""));
}

TEST(TextFormatterCapitalizeTest, IsIdempotent) {
    const std::string once = capitalized(REFERENCE_TEXT);

    EXPECT_EQ(once, capitalized(once.c_str()));
}

TEST(TextFormatterCapitalizeTest, PreservesLength) {
    EXPECT_EQ(std::string(REFERENCE_TEXT).size(), capitalized(REFERENCE_TEXT).size());
}

// 아래 두 개는 get_first_alphabet_index라는 이름을 근거로, 맨 앞이 알파벳이 아니면
// "처음 나오는 알파벳"을 대문자로 만든다고 가정한 테스트입니다.
TEST(TextFormatterCapitalizeTest, CapitalizesFirstAlphabetAfterDigits) {
    EXPECT_EQ("123Abc def", capitalized("123abc DEF"));
}

TEST(TextFormatterCapitalizeTest, CapitalizesFirstAlphabetAfterSpaces) {
    EXPECT_EQ("  Hello", capitalized("  hello"));
}

TEST(TextFormatterCapitalizeTest, ReturnsSelfForChaining) {
    TextFormatter formatter("hello");

    EXPECT_EQ(&formatter, &formatter.capitalize_first());
}

// ---------- 암호화 ----------

TEST(TextFormatterEncryptTest, ZeroGapKeepsText) {
    EXPECT_EQ("Hello 123", encrypted("Hello 123", 0));
}

TEST(TextFormatterEncryptTest, ShiftsLettersAndKeepsCase) {
    EXPECT_EQ("Ifmmp Xpsme", encrypted("Hello World", 1));
}

TEST(TextFormatterEncryptTest, ReferenceSentenceWithPositiveGap) {
    EXPECT_EQ("Wklv lv D Pb 4-vw Whvw SURJudp.", encrypted(REFERENCE_TEXT, 3));
}

TEST(TextFormatterEncryptTest, ReferenceSentenceWithNegativeGap) {
    EXPECT_EQ("Wklv lv D Pb 8-vw Whvw SURJudp.", encrypted(REFERENCE_TEXT, -23));
}

TEST(TextFormatterEncryptTest, WrapsAroundAlphabet) {
    EXPECT_EQ("DEF ABC", encrypted("ABC XYZ", 3));
    EXPECT_EQ("abc", encrypted("def", -3));
}

TEST(TextFormatterEncryptTest, BoundaryGapValues) {
    EXPECT_EQ("zab", encrypted("abc", TextFormatter::MAX_GAP));
    EXPECT_EQ("bcd", encrypted("abc", TextFormatter::MIN_GAP));
}

TEST(TextFormatterEncryptTest, ShiftsDigitsWithinTenValues) {
    EXPECT_EQ("7571 65 54", encrypted("2026 10 09", 5));
}

TEST(TextFormatterEncryptTest, LeavesSymbolsUnchanged) {
    EXPECT_EQ("b-c, d!", encrypted("a-b, c!", 1));
}

TEST(TextFormatterEncryptTest, EmptyTextStaysEmpty) {
    EXPECT_EQ("", encrypted("", 5));
}

TEST(TextFormatterEncryptTest, PreservesLengthForEveryGap) {
    for (int gap = TextFormatter::MIN_GAP; gap <= TextFormatter::MAX_GAP; ++gap) {
        EXPECT_EQ(std::string(REFERENCE_TEXT).size(), encrypted(REFERENCE_TEXT, gap).size())
            << "gap: " << gap;
    }
}

TEST(TextFormatterEncryptTest, OppositeGapRestoresOriginalForEveryGap) {
    for (int gap = TextFormatter::MIN_GAP; gap <= TextFormatter::MAX_GAP; ++gap) {
        TextFormatter formatter(REFERENCE_TEXT);
        formatter.encrypt(gap).encrypt(-gap);

        EXPECT_EQ(std::string(REFERENCE_TEXT), build_text(formatter)) << "gap: " << gap;
    }
}

TEST(TextFormatterEncryptTest, ConsecutiveEncryptionsAddTheirGaps) {
    const int first_gaps[] = {10, -3, 7, -20};
    const int second_gaps[] = {-3, 8, 12, 15};

    for (int i = 0; i < 4; ++i) {
        TextFormatter stepwise(REFERENCE_TEXT);
        stepwise.encrypt(first_gaps[i]).encrypt(second_gaps[i]);

        EXPECT_EQ(encrypted(REFERENCE_TEXT, first_gaps[i] + second_gaps[i]), build_text(stepwise))
            << "gaps: " << first_gaps[i] << ", " << second_gaps[i];
    }
}

TEST(TextFormatterEncryptTest, ReturnsSelfForChaining) {
    TextFormatter formatter("hello");

    EXPECT_EQ(&formatter, &formatter.encrypt(1));
}

// ---------- 대문자화와 암호화 조합 ----------

TEST(TextFormatterChainTest, CapitalizeThenEncrypt) {
    TextFormatter formatter("hELLO");
    formatter.capitalize_first().encrypt(1);

    EXPECT_EQ("Ifmmp", build_text(formatter));
}

TEST(TextFormatterChainTest, EncryptThenCapitalize) {
    TextFormatter formatter("hELLO");
    formatter.encrypt(1).capitalize_first();

    EXPECT_EQ("Ifmmp", build_text(formatter));
}

TEST(TextFormatterChainTest, OrderDoesNotMatterForEveryGap) {
    // 암호화는 알파벳을 알파벳으로, 숫자를 숫자로만 바꾸므로 순서와 무관해야 함
    for (int gap = TextFormatter::MIN_GAP; gap <= TextFormatter::MAX_GAP; ++gap) {
        TextFormatter capitalize_first(REFERENCE_TEXT);
        capitalize_first.capitalize_first().encrypt(gap);

        TextFormatter encrypt_first(REFERENCE_TEXT);
        encrypt_first.encrypt(gap).capitalize_first();

        EXPECT_EQ(build_text(capitalize_first), build_text(encrypt_first)) << "gap: " << gap;
    }
}

TEST(TextFormatterChainTest, SeparateFormattersDoNotAffectEachOther) {
    // StringApp이 같은 입력으로 포맷터를 두 개 만들어 쓰는 방식
    TextFormatter for_capitalize(REFERENCE_TEXT);
    TextFormatter for_encrypt(REFERENCE_TEXT);

    for_capitalize.capitalize_first();
    for_encrypt.encrypt(3);

    EXPECT_EQ("This is a my 1-st test program.", build_text(for_capitalize));
    EXPECT_EQ("Wklv lv D Pb 4-vw Whvw SURJudp.", build_text(for_encrypt));
}