// main.cpp의 알파벳 개수 세기 / 암호화 인덱스 계산 / 알파벳 개수 출력 함수 테스트
//
// main.cpp에는 헤더가 없고 main()이 들어 있으므로, 아래처럼 main 이름을 바꿔 컴파일한 뒤 링크한다.
//   clang++ -std=c++17 -Dmain=sentence_main -c main.cpp -o main.o
//   clang++ -std=c++17 CharacterUtilsTest.cpp TextFormatterTest.cpp main.o -lgtest -lgtest_main -o sentence_test

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

// ---------- main.cpp에 정의된 함수 원형 ----------
void countLetters(char inputCharArr[], int resultCharArr[]);
int calculateCypherIndex(int charIndex, int inputGap, int cycleNum);
void printAlphabetCount(int alphabetCountArr[], int arrLength);

namespace {
    constexpr int INPUT_LENGTH = 128;      // 과제 조건: 문자배열 글자 최대수 127 + '\0'
    constexpr int ALPHABET_COUNT = 26;
    constexpr int DIGIT_COUNT = 10;

    // countLetters가 const가 아닌 char[]를 받으므로 수정 가능한 버퍼에 복사한 뒤 호출한다.
    // 결과 배열은 0으로 채운 뒤 호출한다. (함수가 초기화를 하는지 여부와 무관하게 동작)
    void count_into(const char text[], int counts[]) {
        std::string buffer(text);
        for (int i = 0; i < ALPHABET_COUNT; ++i) {
            counts[i] = 0;
        }
        countLetters(&buffer[0], counts);
    }

    int sum_of(const int counts[]) {
        int sum = 0;
        for (int i = 0; i < ALPHABET_COUNT; ++i) {
            sum += counts[i];
        }
        return sum;
    }

    // 음수 gap에서도 0 이상 cycle 미만의 값을 돌려주는 기대값
    int expected_cypher_index(int char_index, int gap, int cycle) {
        return ((char_index + gap) % cycle + cycle) % cycle;
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

    // 줄 끝 공백은 무시하고 비교하기 위해 '\n' 앞의 공백을 제거한다.
    std::string trim_trailing_spaces(const std::string &line) {
        std::size_t end = line.size();
        while (end > 0 && line[end - 1] == ' ') {
            --end;
        }
        return line.substr(0, end);
    }

    // printAlphabetCount의 출력에서 첫 줄(개행 제외)을 꺼낸다.
    std::string printed_counts(const char text[]) {
        int counts[ALPHABET_COUNT];
        count_into(text, counts);

        CoutCapture capture;
        printAlphabetCount(counts, ALPHABET_COUNT);
        const std::string output = capture.str();

        return trim_trailing_spaces(output.substr(0, output.find('\n')));
    }
}

// =====================================================================
// 기능 1) 알파벳 개수 세기 (countLetters)
// =====================================================================

TEST(CountLettersTest, EmptyStringKeepsAllZero) {
    int counts[ALPHABET_COUNT];
    count_into("", counts);

    for (int i = 0; i < ALPHABET_COUNT; ++i) {
        EXPECT_EQ(0, counts[i]) << "인덱스: " << i;
    }
}

TEST(CountLettersTest, SingleLetter) {
    int counts[ALPHABET_COUNT];
    count_into("a", counts);

    EXPECT_EQ(1, counts[0]);
    EXPECT_EQ(1, sum_of(counts));
}

TEST(CountLettersTest, LastLetterUsesLastIndex) {
    int counts[ALPHABET_COUNT];
    count_into("z", counts);

    EXPECT_EQ(1, counts[ALPHABET_COUNT - 1]);
    EXPECT_EQ(1, sum_of(counts));
}

TEST(CountLettersTest, EachLetterOnce) {
    int counts[ALPHABET_COUNT];
    count_into("abcdefghijklmnopqrstuvwxyz", counts);

    for (int i = 0; i < ALPHABET_COUNT; ++i) {
        EXPECT_EQ(1, counts[i]) << "인덱스: " << i;
    }
}

TEST(CountLettersTest, RepeatedLetter) {
    int counts[ALPHABET_COUNT];
    count_into("aaaa", counts);

    EXPECT_EQ(4, counts[0]);
    EXPECT_EQ(4, sum_of(counts));
}

TEST(CountLettersTest, MixedCounts) {
    int counts[ALPHABET_COUNT];
    count_into("banana", counts);

    EXPECT_EQ(3, counts['a' - 'a']);
    EXPECT_EQ(1, counts['b' - 'a']);
    EXPECT_EQ(2, counts['n' - 'a']);
    EXPECT_EQ(6, sum_of(counts));
}

// 과제 조건: 대소문자 구분 없이 센다.
TEST(CountLettersTest, UpperCaseCountsAtSameIndex) {
    int counts[ALPHABET_COUNT];
    count_into("ABC", counts);

    EXPECT_EQ(1, counts[0]);
    EXPECT_EQ(1, counts[1]);
    EXPECT_EQ(1, counts[2]);
    EXPECT_EQ(3, sum_of(counts));
}

TEST(CountLettersTest, MixedCaseIsMergedIntoOneCount) {
    int counts[ALPHABET_COUNT];
    count_into("aAaA", counts);

    EXPECT_EQ(4, counts[0]);
    EXPECT_EQ(4, sum_of(counts));
}

TEST(CountLettersTest, IgnoresDigitsSpacesAndSymbols) {
    int counts[ALPHABET_COUNT];
    count_into("a1b2!c d,e.", counts);

    EXPECT_EQ(1, counts['a' - 'a']);
    EXPECT_EQ(1, counts['b' - 'a']);
    EXPECT_EQ(1, counts['c' - 'a']);
    EXPECT_EQ(1, counts['d' - 'a']);
    EXPECT_EQ(1, counts['e' - 'a']);
    EXPECT_EQ(5, sum_of(counts));
}

TEST(CountLettersTest, NoLettersKeepsAllZero) {
    int counts[ALPHABET_COUNT];
    count_into("1234 !@#$ 5678", counts);

    EXPECT_EQ(0, sum_of(counts));
}

TEST(CountLettersTest, BoundaryCharactersAreNotCounted) {
    // '@', '[', '`', '{'는 알파벳 범위 바로 바깥의 ASCII 문자
    int counts[ALPHABET_COUNT];
    count_into("@[`{", counts);

    EXPECT_EQ(0, sum_of(counts));
}

TEST(CountLettersTest, ReferenceSentence) {
    // 과제 예시: [a:2] [e:1] [g:1] [h:1] [i:2] [m:2] … [t:4] [y:1]
    int counts[ALPHABET_COUNT];
    count_into("This is A My 1-st Test PROGram.", counts);

    EXPECT_EQ(2, counts['a' - 'a']);
    EXPECT_EQ(1, counts['e' - 'a']);
    EXPECT_EQ(1, counts['g' - 'a']);
    EXPECT_EQ(1, counts['h' - 'a']);
    EXPECT_EQ(2, counts['i' - 'a']);
    EXPECT_EQ(2, counts['m' - 'a']);
    EXPECT_EQ(1, counts['o' - 'a']);
    EXPECT_EQ(1, counts['p' - 'a']);
    EXPECT_EQ(2, counts['r' - 'a']);
    EXPECT_EQ(4, counts['s' - 'a']);
    EXPECT_EQ(4, counts['t' - 'a']);
    EXPECT_EQ(1, counts['y' - 'a']);
    EXPECT_EQ(22, sum_of(counts));
}

TEST(CountLettersTest, PangramCounts) {
    int counts[ALPHABET_COUNT];
    count_into("The quick brown fox jumps over the lazy dog", counts);

    for (int i = 0; i < ALPHABET_COUNT; ++i) {
        EXPECT_GE(counts[i], 1) << "누락된 문자: " << static_cast<char>('a' + i);
    }
    EXPECT_EQ(4, counts['o' - 'a']);
    EXPECT_EQ(3, counts['e' - 'a']);
    EXPECT_EQ(2, counts['t' - 'a']);
    EXPECT_EQ(35, sum_of(counts));
}

TEST(CountLettersTest, MaximumLengthSentence) {
    // 과제 조건: 문자배열의 글자 최대수는 127글자
    const std::string text(INPUT_LENGTH - 1, 'q');
    int counts[ALPHABET_COUNT];
    count_into(text.c_str(), counts);

    EXPECT_EQ(INPUT_LENGTH - 1, counts['q' - 'a']);
    EXPECT_EQ(INPUT_LENGTH - 1, sum_of(counts));
}

TEST(CountLettersTest, DoesNotModifyInput) {
    char text[] = "Hello World";
    int counts[ALPHABET_COUNT] = {};

    countLetters(text, counts);

    EXPECT_STREQ("Hello World", text);
}

// =====================================================================
// 암호화 인덱스 계산 (calculateCypherIndex)
// 결과는 항상 0 이상 cycleNum 미만이어야 암호표 배열의 범위를 벗어나지 않는다.
// =====================================================================

TEST(CalculateCypherIndexTest, ZeroGapKeepsIndex) {
    for (int index = 0; index < ALPHABET_COUNT; ++index) {
        EXPECT_EQ(index, calculateCypherIndex(index, 0, ALPHABET_COUNT)) << "index: " << index;
    }
}

TEST(CalculateCypherIndexTest, ExamplesFromSpec) {
    // gap == 3 : 'a' -> 'd', '0' -> '3', '7' -> '0'
    EXPECT_EQ('d' - 'a', calculateCypherIndex('a' - 'a', 3, ALPHABET_COUNT));
    EXPECT_EQ(3, calculateCypherIndex(0, 3, DIGIT_COUNT));
    EXPECT_EQ(0, calculateCypherIndex(7, 3, DIGIT_COUNT));

    // gap == -11 : 'a' -> 'p', '0' -> '9'
    EXPECT_EQ('p' - 'a', calculateCypherIndex('a' - 'a', -11, ALPHABET_COUNT));
    EXPECT_EQ(9, calculateCypherIndex(0, -11, DIGIT_COUNT));
}

TEST(CalculateCypherIndexTest, ExactlyOneCycleWrapsToZero) {
    // 'z' + 1 -> 'a', '9' + 1 -> '0'
    EXPECT_EQ(0, calculateCypherIndex(ALPHABET_COUNT - 1, 1, ALPHABET_COUNT));
    EXPECT_EQ(0, calculateCypherIndex(DIGIT_COUNT - 1, 1, DIGIT_COUNT));
}

TEST(CalculateCypherIndexTest, WrapsForward) {
    EXPECT_EQ('b' - 'a', calculateCypherIndex('y' - 'a', 3, ALPHABET_COUNT));
    EXPECT_EQ(3, calculateCypherIndex(8, 5, DIGIT_COUNT));
}

TEST(CalculateCypherIndexTest, WrapsBackward) {
    EXPECT_EQ('z' - 'a', calculateCypherIndex('a' - 'a', -1, ALPHABET_COUNT));
    EXPECT_EQ('y' - 'a', calculateCypherIndex('b' - 'a', -3, ALPHABET_COUNT));
    EXPECT_EQ(9, calculateCypherIndex(0, -1, DIGIT_COUNT));
    EXPECT_EQ(7, calculateCypherIndex(2, -5, DIGIT_COUNT));
}

TEST(CalculateCypherIndexTest, MaximumGapValues) {
    // 과제 조건: gap은 -25 ~ 25
    EXPECT_EQ('z' - 'a', calculateCypherIndex('a' - 'a', 25, ALPHABET_COUNT));
    EXPECT_EQ('a' - 'a', calculateCypherIndex('z' - 'a', -25, ALPHABET_COUNT));
    EXPECT_EQ(5, calculateCypherIndex(0, 25, DIGIT_COUNT));
    EXPECT_EQ(5, calculateCypherIndex(0, -25, DIGIT_COUNT));
    EXPECT_EQ(4, calculateCypherIndex(9, 25, DIGIT_COUNT));
    EXPECT_EQ(4, calculateCypherIndex(9, -25, DIGIT_COUNT));
}

TEST(CalculateCypherIndexTest, EveryAlphabetIndexAndGapMatchesModularFormula) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (int index = 0; index < ALPHABET_COUNT; ++index) {
            EXPECT_EQ(expected_cypher_index(index, gap, ALPHABET_COUNT),
                      calculateCypherIndex(index, gap, ALPHABET_COUNT))
                << "index: " << index << ", gap: " << gap;
        }
    }
}

TEST(CalculateCypherIndexTest, EveryDigitIndexAndGapMatchesModularFormula) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (int index = 0; index < DIGIT_COUNT; ++index) {
            EXPECT_EQ(expected_cypher_index(index, gap, DIGIT_COUNT),
                      calculateCypherIndex(index, gap, DIGIT_COUNT))
                << "index: " << index << ", gap: " << gap;
        }
    }
}

TEST(CalculateCypherIndexTest, ResultIsAlwaysInsideCycle) {
    const int cycles[] = {ALPHABET_COUNT, DIGIT_COUNT};

    for (const int cycle: cycles) {
        for (int gap = -25; gap <= 25; ++gap) {
            for (int index = 0; index < cycle; ++index) {
                const int result = calculateCypherIndex(index, gap, cycle);
                EXPECT_GE(result, 0) << "index: " << index << ", gap: " << gap << ", cycle: " << cycle;
                EXPECT_LT(result, cycle) << "index: " << index << ", gap: " << gap << ", cycle: " << cycle;
            }
        }
    }
}

TEST(CalculateCypherIndexTest, OppositeGapRestoresIndex) {
    for (int gap = -25; gap <= 25; ++gap) {
        for (int index = 0; index < ALPHABET_COUNT; ++index) {
            const int shifted = calculateCypherIndex(index, gap, ALPHABET_COUNT);
            EXPECT_EQ(index, calculateCypherIndex(shifted, -gap, ALPHABET_COUNT))
                << "index: " << index << ", gap: " << gap;
        }
    }
}

TEST(CalculateCypherIndexTest, ShiftIsOneToOne) {
    // 같은 gap으로 서로 다른 26개 인덱스를 밀면 결과도 모두 달라야 함
    for (int gap = -25; gap <= 25; ++gap) {
        bool seen[ALPHABET_COUNT] = {};
        for (int index = 0; index < ALPHABET_COUNT; ++index) {
            const int result = calculateCypherIndex(index, gap, ALPHABET_COUNT);
            if (0 <= result && result < ALPHABET_COUNT) {
                seen[result] = true;
            }
        }
        for (int i = 0; i < ALPHABET_COUNT; ++i) {
            EXPECT_TRUE(seen[i]) << "gap: " << gap << ", 누락된 인덱스: " << i;
        }
    }
}

// =====================================================================
// 기능 1) 알파벳 개수 출력 (printAlphabetCount)
// 과제 예시 형식: [a:2] [e:1] [g:1] ... (줄 끝 공백은 무시하고 비교)
// =====================================================================

TEST(PrintAlphabetCountTest, ReferenceSentence) {
    EXPECT_EQ("[a:2] [e:1] [g:1] [h:1] [i:2] [m:2] [o:1] [p:1] [r:2] [s:4] [t:4] [y:1]",
              printed_counts("This is A My 1-st Test PROGram."));
}

TEST(PrintAlphabetCountTest, PrintsInAlphabeticalOrder) {
    EXPECT_EQ("[a:1] [b:1] [c:1]", printed_counts("cba"));
}

TEST(PrintAlphabetCountTest, PrintsOnlyIncludedLetters) {
    EXPECT_EQ("[a:3] [b:1] [n:2]", printed_counts("banana"));
}

TEST(PrintAlphabetCountTest, PrintsLowerCaseLetterForUpperCaseInput) {
    EXPECT_EQ("[a:2] [b:1]", printed_counts("AaB"));
}

TEST(PrintAlphabetCountTest, PrintsMultiDigitCount) {
    EXPECT_EQ("[z:12]", printed_counts("zzzzzzzzzzzz"));
}

TEST(PrintAlphabetCountTest, NoLettersPrintsEmptyLine) {
    EXPECT_EQ("", printed_counts("123 !?"));
}

TEST(PrintAlphabetCountTest, EndsWithNewline) {
    int counts[ALPHABET_COUNT] = {};
    counts[0] = 1;

    CoutCapture capture;
    printAlphabetCount(counts, ALPHABET_COUNT);
    const std::string output = capture.str();

    ASSERT_FALSE(output.empty());
    EXPECT_EQ('\n', output.back());
    EXPECT_EQ(output.size() - 1, output.find('\n'));    // 개행은 한 번만
}
