//
// Created by leegu on 26. 10. 10..
//

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "StringApp.h"

namespace {
    // std::cin의 버퍼를 문자열로 바꿔치기하고, 소멸 시 원래대로 되돌림
    class CinRedirect {
        public:
            explicit CinRedirect(const std::string &text)
                : stream(text), original(std::cin.rdbuf(stream.rdbuf())) {
            }

            ~CinRedirect() {
                std::cin.rdbuf(original);
                std::cin.clear();
            }

        private:
            std::istringstream stream;
            std::streambuf *original;
    };

    std::string rtrim(std::string text) {
        while (!text.empty() && (text.back() == ' ' || text.back() == '\r')) {
            text.pop_back();
        }
        return text;
    }

    // 입력: 첫 줄 텍스트, 둘째 줄 gap. 출력: 줄 단위로 분리해서 반환
    std::vector<std::string> run_string_app(const std::string &text, const int gap) {
        CinRedirect redirect(text + "\n" + std::to_string(gap) + "\n");
        testing::internal::CaptureStdout();
        StringApp app;
        app.run();
        std::cout.flush();
        const std::string output = testing::internal::GetCapturedStdout();

        std::vector<std::string> lines;
        std::istringstream stream(output);
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(rtrim(line));
        }
        return lines;
    }

    // 대문자화 규칙: 앞의 공백을 모두 건너뛰고 처음 나온 문자가 영어일 때만 대문자로 바꾸고,
    // 나머지 영어는 모두 소문자로 바꾼다. (처음 나온 문자가 숫자/기호이면 대문자로 바꾸지 않는다.)
    std::string expected_capitalized(std::string text) {
        for (char &chr: text) {
            if (chr >= 'A' && chr <= 'Z') {
                chr = static_cast<char>(chr - 'A' + 'a');
            }
        }
        std::size_t first = 0;
        while (first < text.size() && text[first] == ' ') {
            ++first;
        }
        if (first < text.size() && text[first] >= 'a' && text[first] <= 'z') {
            text[first] = static_cast<char>(text[first] - 'a' + 'A');
        }
        return text;
    }

    const std::string REFERENCE_TEXT = "This is A My 1-st Test PROGram.";
    const std::string REFERENCE_COUNTS =
            "[a:2] [e:1] [g:1] [h:1] [i:2] [m:2] [o:1] [p:1] [r:2] [s:4] [t:4] [y:1]";
    const std::string REFERENCE_CAPITALIZED = "This is a my 1-st test program.";
}

// ---------- 사진의 예시 ----------

TEST(StringAppReferenceTest, GapThree) {
    const auto lines = run_string_app(REFERENCE_TEXT, 3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], REFERENCE_COUNTS);
    EXPECT_EQ(lines[1], REFERENCE_CAPITALIZED);
    EXPECT_EQ(lines[2], "Wklv lv D Pb 4-vw Whvw SURJudp.");
}

TEST(StringAppReferenceTest, GapMinusTwentyThree) {
    const auto lines = run_string_app(REFERENCE_TEXT, -23);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], REFERENCE_COUNTS);
    EXPECT_EQ(lines[1], REFERENCE_CAPITALIZED);
    EXPECT_EQ(lines[2], "Wklv lv D Pb 8-vw Whvw SURJudp.");
}

TEST(StringAppReferenceTest, PrintsExactlyThreeLines) {
    EXPECT_EQ(run_string_app(REFERENCE_TEXT, 3).size(), 3u);
}

// ---------- 알파벳 개수 ----------

TEST(StringAppCountTest, MergesUpperAndLowerCase) {
    const auto lines = run_string_app("AaBb", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:2] [b:2]");
}

TEST(StringAppCountTest, ListsInAlphabeticalOrderAndSkipsAbsentLetters) {
    const auto lines = run_string_app("zebra", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:1] [b:1] [e:1] [r:1] [z:1]");
}

TEST(StringAppCountTest, CountsRepeatedLetters) {
    const auto lines = run_string_app("Hello World", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[d:1] [e:1] [h:1] [l:3] [o:2] [r:1] [w:1]");
}

TEST(StringAppCountTest, IgnoresDigitsAndSymbols) {
    const auto lines = run_string_app("a-b, c!", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:1] [b:1] [c:1]");
}

TEST(StringAppCountTest, NoLettersGivesEmptyLine) {
    const auto lines = run_string_app("2026 10 09", 5);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "");
}

TEST(StringAppCountTest, DoesNotDependOnGap) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto lines = run_string_app("Hello World", gap);

        ASSERT_EQ(lines.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(lines[0], "[d:1] [e:1] [h:1] [l:3] [o:2] [r:1] [w:1]") << "gap: " << gap;
    }
}

// ---------- 첫 글자 대문자화 ----------

TEST(StringAppCapitalizeTest, LowersTheRest) {
    const auto lines = run_string_app("HELLO WORLD", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello world");
}

TEST(StringAppCapitalizeTest, UppersLowerCaseFirstLetter) {
    const auto lines = run_string_app("hello", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello");
}

TEST(StringAppCapitalizeTest, KeepsDigitsAndSymbols) {
    const auto lines = run_string_app("2026 10 09", 5);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "2026 10 09");
}

// 앞의 공백은 건너뛰고, 처음 나온 문자가 영어이면 대문자로 바꾼다.
TEST(StringAppCapitalizeTest, SkipsLeadingBlanksAndUppersFirstLetter) {
    const auto lines = run_string_app("   hello", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "   Hello");
}

TEST(StringAppCapitalizeTest, SkipsLeadingBlanksAndLowersTheRest) {
    const auto lines = run_string_app("  hELLO wORLD", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "  Hello world");
}

TEST(StringAppCapitalizeTest, SingleLeadingBlank) {
    const auto lines = run_string_app(" abc", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], " Abc");
}

TEST(StringAppCapitalizeTest, OnlyBlanksStayBlank) {
    const auto lines = run_string_app("     ", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "");    // 오른쪽 공백은 테스트에서 잘라내므로 빈 줄로 보인다.
}

// 처음 나온 문자가 숫자이면 대문자로 바꾸지 않고, 뒤의 영어는 모두 소문자여야 한다.
TEST(StringAppCapitalizeTest, DigitFirstDoesNotUpperFollowingLetter) {
    const auto lines = run_string_app("1st", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "1st");    // "1St" 이면 안 된다.
}

TEST(StringAppCapitalizeTest, DigitFirstLowersTheRest) {
    const auto lines = run_string_app("1ST PLACE", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "1st place");
}

TEST(StringAppCapitalizeTest, DigitFirstThenWords) {
    const auto lines = run_string_app("2nd Test PROGram", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "2nd test program");
}

TEST(StringAppCapitalizeTest, DigitThenSpaceThenLetter) {
    const auto lines = run_string_app("123 abc", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "123 abc");    // 첫 글자가 숫자이므로 뒤의 abc 는 대문자가 되지 않는다.
}

TEST(StringAppCapitalizeTest, LeadingBlanksThenDigitDoesNotUpperLetter) {
    const auto lines = run_string_app("  1st", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "  1st");
}

TEST(StringAppCapitalizeTest, LeadingBlanksThenDigitLowersTheRest) {
    const auto lines = run_string_app("   9LIVES Are", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "   9lives are");
}

// 처음 나온 문자가 기호인 경우에도 영어가 아니므로 대문자로 바꾸지 않는다.
TEST(StringAppCapitalizeTest, SymbolFirstDoesNotUpperFollowingLetter) {
    const auto lines = run_string_app("-abc DEF", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "-abc def");
}

TEST(StringAppCapitalizeTest, SymbolFirstLowersTheRest) {
    const auto lines = run_string_app("!HELLO", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "!hello");
}

TEST(StringAppCapitalizeTest, LeadingBlanksThenSymbol) {
    const auto lines = run_string_app("  .Hi There", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "  .hi there");
}

// 대문자가 되는 글자는 처음 나온 영어 하나뿐이다.
TEST(StringAppCapitalizeTest, OnlyOneLetterIsUpper) {
    const auto lines = run_string_app("aBcDeF gHiJ", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Abcdef ghij");
}

TEST(StringAppCapitalizeTest, AlreadyCapitalizedStaysTheSame) {
    const auto lines = run_string_app("Hello world", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello world");
}

TEST(StringAppCapitalizeTest, KeepsLengthAndNonLetterPositions) {
    const std::string text = "  1ST-place, 9 LIVES!";
    const auto lines = run_string_app(text, 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "  1st-place, 9 lives!");
    EXPECT_EQ(lines[1].size(), text.size());
}

TEST(StringAppCapitalizeTest, MatchesRuleForManyInputs) {
    const char *inputs[] = {
        "a", "A", "1", "1a", "1A", "a1", "A1", " a", " A", " 1", " 1a", " 1A",
        "   hello WORLD", "1st", "1ST", "1St", "2nd Test", "-a", "- a", ". A", "a b c", "A B C",
        "  a  b  c", "z", "Z", "0Z", "  0Z  z", "x-ray", "X-RAY", "3D PRINTER", "e-mail ADDRESS"
    };

    for (const char *input: inputs) {
        const auto lines = run_string_app(input, 0);

        ASSERT_EQ(lines.size(), 3u) << "input: \"" << input << "\"";
        EXPECT_EQ(lines[1], rtrim(expected_capitalized(input))) << "input: \"" << input << "\"";
    }
}

TEST(StringAppCapitalizeTest, DoesNotDependOnGap) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto lines = run_string_app("Hello World", gap);

        ASSERT_EQ(lines.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(lines[1], "Hello world") << "gap: " << gap;
    }
}

// ---------- 암호화 ----------

TEST(StringAppEncryptTest, GapZeroKeepsText) {
    const auto lines = run_string_app("Hello 123", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "Hello 123");
}

TEST(StringAppEncryptTest, ShiftsLettersAndKeepsCase) {
    const auto lines = run_string_app("Hello World", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "Ifmmp Xpsme");
}

TEST(StringAppEncryptTest, WrapsUpperCase) {
    const auto lines = run_string_app("ABC XYZ", 3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "DEF ABC");
}

TEST(StringAppEncryptTest, NegativeGapShiftsBackward) {
    const auto lines = run_string_app("def", -3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "abc");
}

TEST(StringAppEncryptTest, MaximumGapValues) {
    EXPECT_EQ(run_string_app("abc", 25).at(2), "zab");
    EXPECT_EQ(run_string_app("abc", -25).at(2), "bcd");
}

TEST(StringAppEncryptTest, ShiftsDigitsWithinTenValues) {
    const auto lines = run_string_app("2026 10 09", 5);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "7571 65 54");
}

TEST(StringAppEncryptTest, LeavesSymbolsUnchanged) {
    const auto lines = run_string_app("a-b, c!", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "b-c, d!");
}

TEST(StringAppEncryptTest, UsesOriginalTextNotCapitalizedText) {
    // 대문자화된 문장이 아니라 입력 원본의 대소문자가 유지되어야 함
    const auto lines = run_string_app("hELLO", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello");
    EXPECT_EQ(lines[2], "hELLO");
}

TEST(StringAppEncryptTest, UsesOriginalTextWhenDigitIsFirst) {
    // 대문자화 결과("1st")가 아니라 입력 원본("1ST")의 대소문자가 유지되어야 함
    const auto lines = run_string_app("1ST", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "1st");
    EXPECT_EQ(lines[2], "1ST");
}

TEST(StringAppEncryptTest, KeepsLeadingBlanks) {
    const auto lines = run_string_app("  abc", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "  bcd");
}

TEST(StringAppEncryptTest, OppositeGapRestoresOriginalText) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto encrypted = run_string_app(REFERENCE_TEXT, gap);
        ASSERT_EQ(encrypted.size(), 3u) << "gap: " << gap;

        const auto restored = run_string_app(encrypted[2], -gap);
        ASSERT_EQ(restored.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(restored[2], REFERENCE_TEXT) << "gap: " << gap;
    }
}

// 널 문자를 제외하고 실제로 보관되는 최대 글자 수 (MAX_INPUT_LENGTH는 널 문자를 포함한 칸 수)
constexpr int kMaxVisibleLength = TextFormatter::MAX_INPUT_LENGTH;

// ---------- 입력 길이 제한 (널 문자 포함 MAX_INPUT_LENGTH = 128칸, 최대 127글자) ----------

TEST(StringAppLimitTest, MaximumLengthInputIsKept) {
    const std::string text(kMaxVisibleLength, 'a');
    const auto lines = run_string_app(text, 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:" + std::to_string(kMaxVisibleLength) + "]");
    EXPECT_EQ(lines[1], "A" + std::string(kMaxVisibleLength - 1, 'a'));
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'b'));
}

TEST(StringAppLimitTest, OneOverLimitInputIsTruncated) {
    const std::string text(kMaxVisibleLength + 1, 'a');
    const auto lines = run_string_app(text, 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:" + std::to_string(kMaxVisibleLength) + "]");
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'b'));
}

TEST(StringAppLimitTest, OverLimitInputIsTruncated) {
    const std::string text(200, 'a');
    const auto lines = run_string_app(text, 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:" + std::to_string(kMaxVisibleLength) + "]");
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'b'));
}

TEST(StringAppLimitTest, GapIsStillReadAfterOverLimitInput) {
    const std::string text(200, 'a');
    const auto lines = run_string_app(text, 3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'd'));
}

// ---------- 초과 입력이 버퍼에 남은 채 gap 단계로 넘어가는 경우 ----------
// 널 문자를 제외한 최대 보관 글자 수는 kMaxVisibleLength(= MAX_INPUT_LENGTH - 1)입니다.

// // 경우 1: 남은 글자가 gap 입력으로 소비되어 gap이 유효하지 않게 되는 구현
// // (남은 'a'가 숫자로 읽히지 않으므로 입력 실패가 되고, 출력이 3줄이 아닐 수 있음)
// TEST(StringAppLimitTest, LeftoverBufferBreaksGapReading) {
//     const std::string text(kMaxVisibleLength + 10, 'a');
//     const auto lines = run_string_app(text, 1);
//
//     // 앞의 126글자만 처리되었는지 확인 (첫 줄은 입력 직후 상태이므로 구현과 무관하게 동일)
//     ASSERT_FALSE(lines.empty());
//     EXPECT_EQ(lines[0], "[a:" + std::to_string(kMaxVisibleLength) + "]");
//
//     // gap이 정상 처리되었다면 3줄이 나오고 암호화 결과는 'b'로 채워짐.
//     // 남은 글자 때문에 gap 읽기가 실패하는 구현이라면 이 단언이 실패해야 정상.
//     EXPECT_NE(lines.size(), 3u);
// }

// 경우 2: 남은 글자를 비우고 gap까지 정상 처리하는 구현
// (input()이 초과분을 ignore 등으로 버리거나, gap 읽기 전에 버퍼를 비우는 경우)
TEST(StringAppLimitTest, LeftoverBufferIsClearedBeforeGap) {
    const std::string text(kMaxVisibleLength + 10, 'a');
    const auto lines = run_string_app(text, 2);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:" + std::to_string(kMaxVisibleLength) + "]");
    EXPECT_EQ(lines[1], "A" + std::string(kMaxVisibleLength - 1, 'a'));
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'c'));
}

// 경계값: 정확히 한 글자만 넘친 경우에도 gap이 정상 처리되는지
TEST(StringAppLimitTest, SingleLeftoverCharacterDoesNotCorruptGap) {
    const std::string text(kMaxVisibleLength + 1, 'a');
    const auto lines = run_string_app(text, 3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'd'));
}

// gap이 여러 자리 숫자이거나 음수일 때도 초과분 처리와 섞이지 않는지
TEST(StringAppLimitTest, NegativeGapAfterOverLimitInput) {
    const std::string text(200, 'b');
    const auto lines = run_string_app(text, -1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], std::string(kMaxVisibleLength, 'a'));
}