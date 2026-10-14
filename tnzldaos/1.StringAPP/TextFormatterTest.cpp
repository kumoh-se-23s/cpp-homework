// main.cpp의 대소문자 변환 / 암호화 / 문장 출력 / run() 테스트
//
// main.cpp에는 헤더가 없고 main()이 들어 있으므로, 아래처럼 main 이름을 바꿔 컴파일한 뒤 링크한다.
//   clang++ -std=c++17 -Dmain=sentence_main -c main.cpp -o main.o
//   clang++ -std=c++17 CharacterUtilsTest.cpp TextFormatterTest.cpp main.o -lgtest -lgtest_main -o sentence_test

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ---------- main.cpp에 정의된 함수 원형 ----------
void toCapitalize(char inputCharArr[], char resultCharArr[]);
void toEncrypted(char inputCharArr[], int inputGap, char resultCharArr[]);
void printSentence(char inputSentenceArr[], int arrLength);
void run();

namespace {
    constexpr int INPUT_LENGTH = 128;      // 과제 조건: 문자배열 글자 최대수 127 + '\0'
    constexpr int MIN_GAP = -25;
    constexpr int MAX_GAP = 25;

    const char REFERENCE_TEXT[] = "This is A My 1-st Test PROGram.";

    // 함수들이 const가 아닌 char[]를 받으므로 입력을 수정 가능한 버퍼에 복사한다.
    // 결과 버퍼는 run()과 같이 0으로 초기화해서 넘긴다.
    std::string capitalized(const char text[]) {
        std::string input(text);
        char result[INPUT_LENGTH]{};

        toCapitalize(&input[0], result);
        return std::string(result);
    }

    std::string encrypted(const char text[], const int gap) {
        std::string input(text);
        char result[INPUT_LENGTH]{};

        toEncrypted(&input[0], gap, result);
        return std::string(result);
    }

    // std::cout 출력을 가로채는 도우미
    class CoutCapture {
        public:
            CoutCapture() : previous(std::cout.rdbuf(buffer.rdbuf())) {}
            ~CoutCapture() { std::cout.rdbuf(previous); }

            std::string str() const { return buffer.str(); }

        private:
            std::ostringstream buffer;
            std::streambuf *previous;
    };

    // std::cin 입력을 문자열로 바꿔 넣는 도우미
    class CinReplace {
        public:
            explicit CinReplace(const std::string &text) : buffer(text), previous(std::cin.rdbuf(buffer.rdbuf())) {}
            ~CinReplace() {
                std::cin.rdbuf(previous);
                std::cin.clear();
            }

        private:
            std::istringstream buffer;
            std::streambuf *previous;
    };

    // 줄 끝 공백을 제거한다. (알파벳 개수 출력의 마지막 공백 허용)
    std::string trim_trailing_spaces(const std::string &line) {
        std::size_t end = line.size();
        while (end > 0 && line[end - 1] == ' ') {
            --end;
        }
        return line.substr(0, end);
    }

    // run()에 입력을 주고 출력된 각 줄을 돌려준다.
    std::vector<std::string> run_with_input(const std::string &input) {
        CinReplace cin_replace(input);
        CoutCapture capture;

        run();

        std::vector<std::string> lines;
        std::istringstream output(capture.str());
        std::string line;
        while (std::getline(output, line)) {
            lines.push_back(trim_trailing_spaces(line));
        }
        return lines;
    }
}

// =====================================================================
// 기능 2) 첫 글자는 대문자, 나머지는 소문자 (toCapitalize)
// =====================================================================

TEST(ToCapitalizeTest, ReferenceSentence) {
    EXPECT_EQ("This is a my 1-st test program.", capitalized(REFERENCE_TEXT));
}

TEST(ToCapitalizeTest, UppersLowerCaseFirstLetter) {
    EXPECT_EQ("Hello", capitalized("hello"));
}

TEST(ToCapitalizeTest, LowersTheRest) {
    EXPECT_EQ("Hello world", capitalized("HELLO WORLD"));
    EXPECT_EQ("Hello world", capitalized("hELLO wORLD"));
}

TEST(ToCapitalizeTest, KeepsAlreadyCapitalizedText) {
    EXPECT_EQ("Hello world", capitalized("Hello world"));
}

TEST(ToCapitalizeTest, SingleLetter) {
    EXPECT_EQ("A", capitalized("a"));
    EXPECT_EQ("A", capitalized("A"));
}

// 과제 조건: 입력의 앞 부분에 공백이 있을 수 있음. "첫 글자"가 대문자가 되어야 함.
TEST(ToCapitalizeTest, CapitalizesFirstLetterAfterLeadingSpaces) {
    EXPECT_EQ("  Hello", capitalized("  hello"));
    EXPECT_EQ("   This is a test.", capitalized("   tHIS IS A TEST."));
}

TEST(ToCapitalizeTest, CapitalizesFirstLetterAfterDigits) {
    EXPECT_EQ("123Abc def", capitalized("123abc DEF"));
}

TEST(ToCapitalizeTest, OnlyFirstLetterIsUpperCase) {
    const std::string result = capitalized("aBC DEF GHI");

    EXPECT_EQ("Abc def ghi", result);
    for (std::size_t i = 1; i < result.size(); ++i) {
        EXPECT_FALSE('A' <= result[i] && result[i] <= 'Z') << "index: " << i;
    }
}

TEST(ToCapitalizeTest, DigitsAndSymbolsAreUnchanged) {
    EXPECT_EQ("2026 10 09", capitalized("2026 10 09"));
    EXPECT_EQ("!@# $%^", capitalized("!@# $%^"));
}

TEST(ToCapitalizeTest, EmptyTextStaysEmpty) {
    EXPECT_EQ("", capitalized(""));
}

TEST(ToCapitalizeTest, IsIdempotent) {
    const std::string once = capitalized(REFERENCE_TEXT);

    EXPECT_EQ(once, capitalized(once.c_str()));
}

TEST(ToCapitalizeTest, PreservesLength) {
    EXPECT_EQ(std::string(REFERENCE_TEXT).size(), capitalized(REFERENCE_TEXT).size());
}

TEST(ToCapitalizeTest, MaximumLengthSentence) {
    const std::string text(INPUT_LENGTH - 1, 'b');

    EXPECT_EQ("B" + std::string(INPUT_LENGTH - 2, 'b'), capitalized(text.c_str()));
}

TEST(ToCapitalizeTest, DoesNotModifyInput) {
    char input[] = "hELLO";
    char result[INPUT_LENGTH]{};

    toCapitalize(input, result);

    EXPECT_STREQ("hELLO", input);
}

// =====================================================================
// 기능 3) 간단 암호화 (toEncrypted)
// =====================================================================

TEST(ToEncryptedTest, ReferenceSentenceWithPositiveGap) {
    EXPECT_EQ("Wklv lv D Pb 4-vw Whvw SURJudp.", encrypted(REFERENCE_TEXT, 3));
}

TEST(ToEncryptedTest, ReferenceSentenceWithNegativeGap) {
    EXPECT_EQ("Wklv lv D Pb 8-vw Whvw SURJudp.", encrypted(REFERENCE_TEXT, -23));
}

TEST(ToEncryptedTest, ExamplesFromSpecWithGapMinusEleven) {
    // gap == -11 인 경우 'a'는 'p'로, '0'은 '9'로 변환
    EXPECT_EQ("p", encrypted("a", -11));
    EXPECT_EQ("P", encrypted("A", -11));
    EXPECT_EQ("9", encrypted("0", -11));
}

TEST(ToEncryptedTest, DigitTableFromSpec) {
    // 0 1 2 3 4 5 6 7 8 9 -> gap 3 : 3 4 5 6 7 8 9 0 1 2 / gap -23 : 7 8 9 0 1 2 3 4 5 6
    EXPECT_EQ("3456789012", encrypted("0123456789", 3));
    EXPECT_EQ("7890123456", encrypted("0123456789", -23));
}

TEST(ToEncryptedTest, AlphabetTableFromSpec) {
    EXPECT_EQ("DEFGHIJKLMNOPQRSTUVWXYZABC", encrypted("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 3));
    EXPECT_EQ("defghijklmnopqrstuvwxyzabc", encrypted("abcdefghijklmnopqrstuvwxyz", 3));
}

TEST(ToEncryptedTest, ZeroGapKeepsText) {
    EXPECT_EQ("Hello 123", encrypted("Hello 123", 0));
}

TEST(ToEncryptedTest, ShiftsLettersAndKeepsCase) {
    EXPECT_EQ("Ifmmp Xpsme", encrypted("Hello World", 1));
}

TEST(ToEncryptedTest, WrapsAroundAlphabet) {
    EXPECT_EQ("DEF ABC", encrypted("ABC XYZ", 3));
    EXPECT_EQ("abc", encrypted("def", -3));
    EXPECT_EQ("a", encrypted("z", 1));
    EXPECT_EQ("Z", encrypted("A", -1));
}

TEST(ToEncryptedTest, WrapsAroundDigits) {
    EXPECT_EQ("0", encrypted("9", 1));
    EXPECT_EQ("9", encrypted("0", -1));
}

TEST(ToEncryptedTest, BoundaryGapValues) {
    EXPECT_EQ("zab", encrypted("abc", MAX_GAP));
    EXPECT_EQ("bcd", encrypted("abc", MIN_GAP));
    EXPECT_EQ("5", encrypted("0", MAX_GAP));
    EXPECT_EQ("5", encrypted("0", MIN_GAP));
}

TEST(ToEncryptedTest, ShiftsDigitsWithinTenValues) {
    EXPECT_EQ("7571 65 54", encrypted("2026 10 09", 5));
}

TEST(ToEncryptedTest, LeavesSymbolsUnchanged) {
    // 과제 조건: 숫자와 알파벳 외에는 변환하지 않음
    EXPECT_EQ("b-c, d!", encrypted("a-b, c!", 1));
    EXPECT_EQ(" @[`{/:.", encrypted(" @[`{/:.", 7));
}

TEST(ToEncryptedTest, EmptyTextStaysEmpty) {
    EXPECT_EQ("", encrypted("", 5));
}

TEST(ToEncryptedTest, PreservesLengthForEveryGap) {
    for (int gap = MIN_GAP; gap <= MAX_GAP; ++gap) {
        EXPECT_EQ(std::string(REFERENCE_TEXT).size(), encrypted(REFERENCE_TEXT, gap).size())
            << "gap: " << gap;
    }
}

TEST(ToEncryptedTest, OppositeGapRestoresOriginalForEveryGap) {
    for (int gap = MIN_GAP; gap <= MAX_GAP; ++gap) {
        const std::string once = encrypted(REFERENCE_TEXT, gap);

        EXPECT_EQ(std::string(REFERENCE_TEXT), encrypted(once.c_str(), -gap)) << "gap: " << gap;
    }
}

TEST(ToEncryptedTest, ConsecutiveEncryptionsAddTheirGaps) {
    const int first_gaps[] = {10, -3, 7, -20};
    const int second_gaps[] = {-3, 8, 12, 15};

    for (int i = 0; i < 4; ++i) {
        const std::string stepwise = encrypted(encrypted(REFERENCE_TEXT, first_gaps[i]).c_str(), second_gaps[i]);

        EXPECT_EQ(encrypted(REFERENCE_TEXT, first_gaps[i] + second_gaps[i]), stepwise)
            << "gaps: " << first_gaps[i] << ", " << second_gaps[i];
    }
}

TEST(ToEncryptedTest, DoesNotModifyInput) {
    char input[] = "abc 123";
    char result[INPUT_LENGTH]{};

    toEncrypted(input, 3, result);

    EXPECT_STREQ("abc 123", input);
}

// =====================================================================
// 대소문자 변환과 암호화 조합
// =====================================================================

TEST(CapitalizeAndEncryptTest, OrderDoesNotMatterForEveryGap) {
    // 암호화는 알파벳을 같은 대소문자의 알파벳으로, 숫자를 숫자로만 바꾸므로 순서와 무관해야 함
    for (int gap = MIN_GAP; gap <= MAX_GAP; ++gap) {
        const std::string capitalize_first = encrypted(capitalized(REFERENCE_TEXT).c_str(), gap);
        const std::string encrypt_first = capitalized(encrypted(REFERENCE_TEXT, gap).c_str());

        EXPECT_EQ(capitalize_first, encrypt_first) << "gap: " << gap;
    }
}

TEST(CapitalizeAndEncryptTest, BothUseOriginalInput) {
    // run()처럼 같은 입력에서 각각 변환한 결과
    EXPECT_EQ("This is a my 1-st test program.", capitalized(REFERENCE_TEXT));
    EXPECT_EQ("Wklv lv D Pb 4-vw Whvw SURJudp.", encrypted(REFERENCE_TEXT, 3));
}

// =====================================================================
// 문장 출력 (printSentence)
// =====================================================================

TEST(PrintSentenceTest, PrintsGivenLengthAndNewline) {
    char sentence[] = "hello";
    CoutCapture capture;

    printSentence(sentence, 5);

    EXPECT_EQ("hello\n", capture.str());
}

TEST(PrintSentenceTest, PrintsOnlyGivenLength) {
    char sentence[] = "hello world";
    CoutCapture capture;

    printSentence(sentence, 5);

    EXPECT_EQ("hello\n", capture.str());
}

TEST(PrintSentenceTest, ZeroLengthPrintsEmptyLine) {
    char sentence[] = "";
    CoutCapture capture;

    printSentence(sentence, 0);

    EXPECT_EQ("\n", capture.str());
}

// =====================================================================
// 전체 실행 (run) : 과제 예시 입출력
// =====================================================================

TEST(RunTest, ReferenceExampleWithGapThree) {
    const std::vector<std::string> lines = run_with_input("This is A My 1-st Test PROGram.\n3\n");

    ASSERT_EQ(3u, lines.size());
    EXPECT_EQ("[a:2] [e:1] [g:1] [h:1] [i:2] [m:2] [o:1] [p:1] [r:2] [s:4] [t:4] [y:1]", lines[0]);
    EXPECT_EQ("This is a my 1-st test program.", lines[1]);
    EXPECT_EQ("Wklv lv D Pb 4-vw Whvw SURJudp.", lines[2]);
}

TEST(RunTest, ReferenceExampleWithGapMinusTwentyThree) {
    const std::vector<std::string> lines = run_with_input("This is A My 1-st Test PROGram.\n-23\n");

    ASSERT_EQ(3u, lines.size());
    EXPECT_EQ("[a:2] [e:1] [g:1] [h:1] [i:2] [m:2] [o:1] [p:1] [r:2] [s:4] [t:4] [y:1]", lines[0]);
    EXPECT_EQ("This is a my 1-st test program.", lines[1]);
    EXPECT_EQ("Wklv lv D Pb 8-vw Whvw SURJudp.", lines[2]);
}

TEST(RunTest, LeadingSpacesInSentence) {
    const std::vector<std::string> lines = run_with_input("  hELLO 9\n1\n");

    ASSERT_EQ(3u, lines.size());
    EXPECT_EQ("[e:1] [h:1] [l:2] [o:1]", lines[0]);
    EXPECT_EQ("  Hello 9", lines[1]);
    EXPECT_EQ("  iFMMP 0", lines[2]);
}

TEST(RunTest, MaximumLengthSentence) {
    // 과제 조건: 문자배열의 글자 최대수는 127글자
    const std::string sentence(INPUT_LENGTH - 1, 'a');
    const std::vector<std::string> lines = run_with_input(sentence + "\n1\n");

    ASSERT_EQ(3u, lines.size());
    EXPECT_EQ("[a:127]", lines[0]);
    EXPECT_EQ("A" + std::string(INPUT_LENGTH - 2, 'a'), lines[1]);
    EXPECT_EQ(std::string(INPUT_LENGTH - 1, 'b'), lines[2]);
}
