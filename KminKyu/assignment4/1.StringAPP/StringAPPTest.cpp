#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "StringAPP.h"

namespace {
    // std::cin의 버퍼를 문자열로 바꿔치기하고, 소멸 시 원래대로 되돌린다.
    const int MAX_STR_SIZE = 128;
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

    std::vector<std::string> split_lines(const std::string &output) {
        std::vector<std::string> lines;
        std::istringstream stream(output);
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(rtrim(line));
        }
        return lines;
    }

    std::string make_input(const std::string &text, const int gap) {
        return text + "\n" + std::to_string(gap) + "\n";
    }

    // 입력: 첫 줄 텍스트, 둘째 줄 gap. run() 의 출력을 줄 단위로 반환
    std::vector<std::string> run_string_app(const std::string &text, const int gap) {
        CinRedirect redirect(make_input(text, gap));
        testing::internal::CaptureStdout();
        StringAPP app;
        app.run();
        std::cout.flush();
        return split_lines(testing::internal::GetCapturedStdout());
    }

    // readInput() 후 지정한 출력 함수 하나만 호출해서 그 출력을 줄 단위로 반환
    template<typename PrintFunction>
    std::vector<std::string> read_then_print(const std::string &text, const int gap, PrintFunction print) {
        CinRedirect redirect(make_input(text, gap));
        StringAPP app;
        app.readInput();
        testing::internal::CaptureStdout();
        print(app);
        std::cout.flush();
        return split_lines(testing::internal::GetCapturedStdout());
    }

    const std::string REFERENCE_TEXT = "This is A My 1-st Test PROGram.";
    const std::string REFERENCE_COUNTS =
            "[a:2] [e:1] [g:1] [h:1] [i:2] [m:2] [o:1] [p:1] [r:2] [s:4] [t:4] [y:1]";
    const std::string REFERENCE_CAPITALIZED = "This is a my 1-st test program.";
}

// ---------- 상수 ----------

TEST(StringAPPConstants, MaxStringSize) {
    EXPECT_EQ(128, MAX_STR_SIZE);
}

// ---------- 과제 예시 ----------

TEST(StringAPPReferenceTest, GapThree) {
    const auto lines = run_string_app(REFERENCE_TEXT, 3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], REFERENCE_COUNTS);
    EXPECT_EQ(lines[1], REFERENCE_CAPITALIZED);
    EXPECT_EQ(lines[2], "Wklv lv D Pb 4-vw Whvw SURJudp.");
}

TEST(StringAPPReferenceTest, GapMinusTwentyThree) {
    const auto lines = run_string_app(REFERENCE_TEXT, -23);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], REFERENCE_COUNTS);
    EXPECT_EQ(lines[1], REFERENCE_CAPITALIZED);
    EXPECT_EQ(lines[2], "Wklv lv D Pb 8-vw Whvw SURJudp.");
}

TEST(StringAPPReferenceTest, PrintsExactlyThreeLines) {
    EXPECT_EQ(run_string_app(REFERENCE_TEXT, 3).size(), 3u);
}

// ---------- 알파벳 개수 ----------

TEST(StringAPPCountTest, MergesUpperAndLowerCase) {
    const auto lines = run_string_app("AaBb", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:2] [b:2]");
}

TEST(StringAPPCountTest, ListsInAlphabeticalOrderAndSkipsAbsentLetters) {
    const auto lines = run_string_app("zebra", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:1] [b:1] [e:1] [r:1] [z:1]");
}

TEST(StringAPPCountTest, CountsRepeatedLetters) {
    const auto lines = run_string_app("Hello World", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[d:1] [e:1] [h:1] [l:3] [o:2] [r:1] [w:1]");
}

TEST(StringAPPCountTest, IgnoresDigitsAndSymbols) {
    const auto lines = run_string_app("a-b, c!", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:1] [b:1] [c:1]");
}

TEST(StringAPPCountTest, NoLettersGivesEmptyLine) {
    const auto lines = run_string_app("2026 10 09", 5);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "");
}

TEST(StringAPPCountTest, DoesNotDependOnGap) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto lines = run_string_app("Hello World", gap);

        ASSERT_EQ(lines.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(lines[0], "[d:1] [e:1] [h:1] [l:3] [o:2] [r:1] [w:1]") << "gap: " << gap;
    }
}

// ---------- 첫 글자 대문자화 ----------

TEST(StringAPPCapitalizeTest, LowersTheRest) {
    const auto lines = run_string_app("HELLO WORLD", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello world");
}

TEST(StringAPPCapitalizeTest, UppersLowerCaseFirstLetter) {
    const auto lines = run_string_app("hello", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello");
}

TEST(StringAPPCapitalizeTest, KeepsDigitsAndSymbols) {
    const auto lines = run_string_app("2026 10 09", 5);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "2026 10 09");
}

TEST(StringAPPCapitalizeTest, DoesNotDependOnGap) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto lines = run_string_app("Hello World", gap);

        ASSERT_EQ(lines.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(lines[1], "Hello world") << "gap: " << gap;
    }
}

// ---------- 암호화 ----------

TEST(StringAPPEncryptTest, GapZeroKeepsText) {
    const auto lines = run_string_app("Hello 123", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "Hello 123");
}

TEST(StringAPPEncryptTest, ShiftsLettersAndKeepsCase) {
    const auto lines = run_string_app("Hello World", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "Ifmmp Xpsme");
}

TEST(StringAPPEncryptTest, WrapsUpperCase) {
    const auto lines = run_string_app("ABC XYZ", 3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "DEF ABC");
}

TEST(StringAPPEncryptTest, NegativeGapShiftsBackward) {
    const auto lines = run_string_app("def", -3);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "abc");
}

TEST(StringAPPEncryptTest, MaximumGapValues) {
    EXPECT_EQ(run_string_app("abc", 25).at(2), "zab");
    EXPECT_EQ(run_string_app("abc", -25).at(2), "bcd");
}

TEST(StringAPPEncryptTest, ShiftsDigitsWithinTenValues) {
    const auto lines = run_string_app("2026 10 09", 5);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "7571 65 54");
}

TEST(StringAPPEncryptTest, LeavesSymbolsUnchanged) {
    const auto lines = run_string_app("a-b, c!", 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "b-c, d!");
}

TEST(StringAPPEncryptTest, UsesOriginalTextNotCapitalizedText) {
    // 대문자화된 문장이 아니라 입력 원본의 대소문자가 유지되어야 함
    const auto lines = run_string_app("hELLO", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[1], "Hello");
    EXPECT_EQ(lines[2], "hELLO");
}

TEST(StringAPPEncryptTest, OppositeGapRestoresOriginalText) {
    for (int gap = -25; gap <= 25; ++gap) {
        const auto encrypted = run_string_app(REFERENCE_TEXT, gap);
        ASSERT_EQ(encrypted.size(), 3u) << "gap: " << gap;

        const auto restored = run_string_app(encrypted[2], -gap);
        ASSERT_EQ(restored.size(), 3u) << "gap: " << gap;
        EXPECT_EQ(restored[2], REFERENCE_TEXT) << "gap: " << gap;
    }
}

// ---------- 입력 ----------

TEST(StringAPPInputTest, KeepsSpacesInsideFirstLine) {
    const auto lines = run_string_app("a b  c", 0);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "a b  c");
}

TEST(StringAPPInputTest, ReadsGapFromSecondLine) {
    const auto lines = run_string_app("abc", 2);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "cde");
}

TEST(StringAPPInputTest, NegativeGapIsRead) {
    const auto lines = run_string_app("cde", -2);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[2], "abc");
}

// 가정: 최대 입력 길이는 MAX_STR_SIZE - 1 (널 문자 자리 1칸 제외) 이다.
TEST(StringAPPInputTest, MaxLengthTextIsKeptFully) {
    const std::string text(MAX_STR_SIZE - 1, 'a');
    const auto lines = run_string_app(text, 1);

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "[a:" + std::to_string(text.size()) + "]");
    EXPECT_EQ(lines[1], "A" + std::string(text.size() - 1, 'a'));
    EXPECT_EQ(lines[2], std::string(text.size(), 'b'));
}

// ---------- 개별 함수 ----------

TEST(StringAPPMethodTest, PrintAlphabetCountsPrintsOnlyCounts) {
    const auto lines = read_then_print(REFERENCE_TEXT, 3, [](const StringAPP &app) {
        app.printAlphabetCounts();
    });

    ASSERT_EQ(lines.size(), 1u);
    EXPECT_EQ(lines[0], REFERENCE_COUNTS);
}

TEST(StringAPPMethodTest, PrintCapitalizedPrintsOnlyCapitalizedText) {
    const auto lines = read_then_print(REFERENCE_TEXT, 3, [](const StringAPP &app) {
        app.printCapitalized();
    });

    ASSERT_EQ(lines.size(), 1u);
    EXPECT_EQ(lines[0], REFERENCE_CAPITALIZED);
}

TEST(StringAPPMethodTest, PrintEncryptedSentencePrintsOnlyEncryptedText) {
    const auto lines = read_then_print(REFERENCE_TEXT, 3, [](const StringAPP &app) {
        app.printEncryptedSentence();
    });

    ASSERT_EQ(lines.size(), 1u);
    EXPECT_EQ(lines[0], "Wklv lv D Pb 4-vw Whvw SURJudp.");
}

TEST(StringAPPMethodTest, ReadInputDoesNotPrintAnything) {
    CinRedirect redirect(make_input("hello", 1));
    StringAPP app;

    testing::internal::CaptureStdout();
    app.readInput();
    std::cout.flush();
    const std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ("", output);
}

TEST(StringAPPMethodTest, PrintFunctionsCanBeCalledRepeatedly) {
    CinRedirect redirect(make_input("Hello", 1));
    StringAPP app;
    app.readInput();

    testing::internal::CaptureStdout();
    app.printEncryptedSentence();
    app.printEncryptedSentence();
    std::cout.flush();
    const auto lines = split_lines(testing::internal::GetCapturedStdout());

    ASSERT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines[0], "Ifmmp");
    EXPECT_EQ(lines[1], "Ifmmp");
}

TEST(StringAPPMethodTest, RunEqualsReadInputFollowedByThreePrints) {
    const auto expected = run_string_app(REFERENCE_TEXT, 7);

    CinRedirect redirect(make_input(REFERENCE_TEXT, 7));
    StringAPP app;
    testing::internal::CaptureStdout();
    app.readInput();
    app.printAlphabetCounts();
    app.printCapitalized();
    app.printEncryptedSentence();
    std::cout.flush();
    const auto actual = split_lines(testing::internal::GetCapturedStdout());

    EXPECT_EQ(expected, actual);
}

TEST(StringAPPMethodTest, SecondReadInputReplacesPreviousInput) {
    CinRedirect redirect("first\n1\nsecond\n2\n");
    StringAPP app;
    app.readInput();
    app.readInput();

    testing::internal::CaptureStdout();
    app.printCapitalized();
    app.printEncryptedSentence();
    std::cout.flush();
    const auto lines = split_lines(testing::internal::GetCapturedStdout());

    ASSERT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines[0], "Second");
    EXPECT_EQ(lines[1], "ugeqpf");
}

// 가정: readInput() 전에는 빈 문자열과 gap 0 으로 초기화되어 있다.
TEST(StringAPPMethodTest, PrintsEmptyLinesBeforeReadInput) {
    StringAPP app;

    testing::internal::CaptureStdout();
    app.printAlphabetCounts();
    app.printCapitalized();
    app.printEncryptedSentence();
    std::cout.flush();
    const auto lines = split_lines(testing::internal::GetCapturedStdout());

    ASSERT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines[0], "");
    EXPECT_EQ(lines[1], "");
    EXPECT_EQ(lines[2], "");
}