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

TEST(StringAppEncryptTest, OppositeGapRestoresOriginalText) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto encrypted = run_string_app(REFERENCE_TEXT, gap);
        ASSERT_EQ(encrypted.size(), 3u) << "gap: " << gap;

        const auto restored = run_string_app(encrypted[2], -gap);
        ASSERT_EQ(restored.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(restored[2], REFERENCE_TEXT) << "gap: " << gap;
    }
}
