#include <gtest/gtest.h>
#include <string>

#include "CharSequence.h"

namespace {
    // CharSequence에는 문자 배열을 받는 생성자가 없으므로,
    // protected 멤버(fillArray, CAPACITY, charArr, arrLength)에 접근하는 하위 클래스로 내용을 채운다.
    class CharSequenceProbe : public CharSequence {
        public:
            using CharSequence::CAPACITY;

            CharSequenceProbe() = default;

            explicit CharSequenceProbe(const char *text) {
                fill(text);
            }

            void fill(const char *text) {
                // fillArray가 const가 아닌 char[]를 받으므로 수정 가능한 복사본을 넘긴다.
                std::string buffer(text);
                fillArray(&buffer[0]);
            }

            const char *raw() const { return charArr; }

            int raw_length() const { return arrLength; }
    };

    constexpr int MAX_LENGTH = CharSequenceProbe::CAPACITY;

    // operator<<가 없으므로 length()와 at()으로 내용을 꺼낸다.
    std::string to_std_string(const CharSequence &sequence) {
        std::string result;
        for (int index = 0; index < sequence.length(); ++index) {
            result += sequence.at(index);
        }
        return result;
    }

    // std::string::find와 같은 결과를 기대값으로 계산한다.
    // (과제 조건: start가 음수이면 0으로 취급, 문자열 길이를 넘으면 -1)
    int expected_find(const std::string &text, const std::string &pattern, int start) {
        if (start < 0) {
            start = 0;
        }
        if (start > static_cast<int>(text.size())) {
            return -1;
        }
        const std::size_t found = text.find(pattern, static_cast<std::size_t>(start));
        return found == std::string::npos ? -1 : static_cast<int>(found);
    }

    // find의 두 오버로드(const char[] / const CharSequence&)가 같은 결과를 내는지 함께 검사한다.
    void expect_find(const char *text, const char *pattern, int start, int expected) {
        CharSequenceProbe sequence(text);

        EXPECT_EQ(expected, sequence.find(pattern, start))
            << "[char array] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
        EXPECT_EQ(expected, sequence.find(CharSequenceProbe(pattern), start))
            << "[CharSequence] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
    }
}

// =====================================================================
// 생성 / fillArray
// 가정: fillArray는 최대 CAPACITY 글자까지 복사하고 arrLength도 함께 갱신한다.
// =====================================================================

TEST(CharSequenceConstructor, DefaultCreatesEmptySequence) {
    CharSequence sequence;

    EXPECT_EQ(0, sequence.length());
    EXPECT_TRUE(sequence.empty());
}

TEST(CharSequenceFillArray, FromCharArray) {
    CharSequenceProbe sequence("hello");

    EXPECT_EQ(5, sequence.length());
    EXPECT_FALSE(sequence.empty());
    EXPECT_EQ('h', sequence.at(0));
    EXPECT_EQ('o', sequence.at(4));
}

TEST(CharSequenceFillArray, FromEmptyCharArray) {
    CharSequenceProbe sequence("");

    EXPECT_EQ(0, sequence.length());
    EXPECT_TRUE(sequence.empty());
}

TEST(CharSequenceFillArray, FromExactMaxLengthCharArray) {
    CharSequenceProbe sequence("123456789012345");

    EXPECT_EQ(MAX_LENGTH, sequence.length());
    EXPECT_EQ('5', sequence.at(MAX_LENGTH - 1));
}

TEST(CharSequenceFillArray, FromLongCharArrayIsTruncated) {
    CharSequenceProbe sequence("1234567890123456789");

    EXPECT_EQ(MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence));
}

// 가정: 내용이 있는 상태에서 다시 fillArray를 호출하면 이전 내용을 대체한다.
TEST(CharSequenceFillArray, ReplacesPreviousContent) {
    CharSequenceProbe sequence("long content");

    sequence.fill("abc");

    EXPECT_EQ(3, sequence.length());
    EXPECT_EQ("abc", to_std_string(sequence));
    EXPECT_EQ('\0', sequence.raw()[3]);
}

TEST(CharSequenceConstructor, CopyCopiesContent) {
    CharSequenceProbe original("copy me");
    CharSequenceProbe copied(original);

    EXPECT_EQ(original.length(), copied.length());
    EXPECT_EQ("copy me", to_std_string(copied));
}

TEST(CharSequenceConstructor, CopyIsIndependentFromOriginal) {
    CharSequenceProbe original("abc");
    CharSequenceProbe copied(original);

    copied.setChar('z', 0);

    EXPECT_EQ('a', original.at(0));
    EXPECT_EQ('z', copied.at(0));
}

// =====================================================================
// length / empty
// =====================================================================

TEST(CharSequenceLength, IsContentLengthNotCapacity) {
    CharSequenceProbe sequence("abc");

    EXPECT_EQ(3, sequence.length());
}

TEST(CharSequenceEmpty, FalseForNonEmptySequence) {
    CharSequenceProbe sequence("a");

    EXPECT_EQ(1, sequence.length());
    EXPECT_FALSE(sequence.empty());
}

// =====================================================================
// at
// =====================================================================

TEST(CharSequenceAt, ReturnsEachCharacter) {
    CharSequenceProbe sequence("abc");

    EXPECT_EQ('a', sequence.at(0));
    EXPECT_EQ('b', sequence.at(1));
    EXPECT_EQ('c', sequence.at(2));
}

// 가정: 범위를 벗어난 인덱스는 '\0'을 반환한다.
TEST(CharSequenceAt, NegativeIndexReturnsNullCharacter) {
    CharSequenceProbe sequence("abc");

    EXPECT_EQ('\0', sequence.at(-1));
    EXPECT_EQ('\0', sequence.at(-100));
}

TEST(CharSequenceAt, IndexAtOrBeyondLengthReturnsNullCharacter) {
    CharSequenceProbe sequence("abc");

    EXPECT_EQ('\0', sequence.at(3));
    EXPECT_EQ('\0', sequence.at(MAX_LENGTH + 5));
}

// =====================================================================
// setChar
// =====================================================================

TEST(CharSequenceSetChar, ChangesCharacter) {
    CharSequenceProbe sequence("abc");

    sequence.setChar('x', 1);

    EXPECT_EQ('x', sequence.at(1));
    EXPECT_EQ("axc", to_std_string(sequence));
}

TEST(CharSequenceSetChar, DoesNotChangeLength) {
    CharSequenceProbe sequence("abc");

    sequence.setChar('x', 0);
    sequence.setChar('y', 2);

    EXPECT_EQ(3, sequence.length());
    EXPECT_EQ("xby", to_std_string(sequence));
}

// =====================================================================
// 다형성 (CharSequence 참조)
// =====================================================================

TEST(CharSequencePolymorphism, WorksThroughBaseReference) {
    CharSequenceProbe sequence("abc");
    const CharSequence &base = sequence;

    EXPECT_EQ(3, base.length());
    EXPECT_FALSE(base.empty());
    EXPECT_EQ('b', base.at(1));
    EXPECT_EQ(1, base.find("bc", 0));
}

TEST(CharSequencePolymorphism, EmptyThroughBaseReference) {
    CharSequenceProbe sequence;
    const CharSequence &base = sequence;

    EXPECT_TRUE(base.empty());
    EXPECT_EQ(0, base.length());
}

// =====================================================================
// 내부 상태 (protected 멤버)
// =====================================================================

TEST(CharSequenceInternals, ContainerIsNullTerminated) {
    CharSequenceProbe probe("abc");

    EXPECT_EQ('\0', probe.raw()[3]);
    EXPECT_EQ(3, probe.raw_length());
}

TEST(CharSequenceInternals, MaxLengthContainerIsNullTerminated) {
    CharSequenceProbe probe("1234567890123456789");

    EXPECT_EQ(MAX_LENGTH, probe.raw_length());
    EXPECT_EQ('\0', probe.raw()[MAX_LENGTH]);
}

TEST(CharSequenceInternals, CapacityIsFifteen) {
    EXPECT_EQ(15, MAX_LENGTH);
}

// =====================================================================
// find (두 오버로드를 모두 검사)
// =====================================================================

TEST(CharSequenceFind, FindsAtStart) {
    expect_find("hello world", "hello", 0, 0);
}

TEST(CharSequenceFind, FindsInMiddle) {
    expect_find("hello world", "lo w", 0, 3);
}

TEST(CharSequenceFind, FindsAtEnd) {
    expect_find("hello world", "world", 0, 6);
}

TEST(CharSequenceFind, FindsSingleCharacter) {
    expect_find("hello", "l", 0, 2);
}

TEST(CharSequenceFind, FindsWholeString) {
    expect_find("hello", "hello", 0, 0);
}

TEST(CharSequenceFind, ReturnsMinusOneWhenNotFound) {
    expect_find("hello", "xyz", 0, -1);
}

TEST(CharSequenceFind, ReturnsMinusOneWhenPatternIsLongerThanText) {
    expect_find("abc", "abcd", 0, -1);
}

TEST(CharSequenceFind, ReturnsMinusOneOnEmptyText) {
    expect_find("", "a", 0, -1);
}

TEST(CharSequenceFind, DefaultStartIsZero) {
    CharSequenceProbe sequence("abcabc");

    EXPECT_EQ(0, sequence.find("abc"));
    EXPECT_EQ(1, sequence.find(CharSequenceProbe("bca")));
}

TEST(CharSequenceFind, ReturnsFirstOccurrence) {
    expect_find("abcabcabc", "abc", 0, 0);
    expect_find("xabcabc", "abc", 0, 1);
}

TEST(CharSequenceFind, StartPositionSkipsEarlierMatch) {
    expect_find("abcabcabc", "abc", 1, 3);
    expect_find("abcabcabc", "abc", 3, 3);
    expect_find("abcabcabc", "abc", 4, 6);
}

TEST(CharSequenceFind, StartPositionAfterLastMatchReturnsMinusOne) {
    expect_find("abcabc", "abc", 4, -1);
}

TEST(CharSequenceFind, NegativeStartIsTreatedAsZero) {
    expect_find("hello", "he", -1, 0);
    expect_find("hello", "he", -100, 0);
    expect_find("hello", "lo", -3, 3);
}

TEST(CharSequenceFind, StartBeyondLengthReturnsMinusOne) {
    expect_find("hello", "h", 6, -1);
    expect_find("hello", "h", 100, -1);
}

TEST(CharSequenceFind, StartAtLengthReturnsMinusOne) {
    expect_find("hello", "o", 5, -1);
}

TEST(CharSequenceFind, StartAtLastCharacter) {
    expect_find("hello", "o", 4, 4);
}

TEST(CharSequenceFind, RepeatedCharacters) {
    expect_find("aaaaaaaaaa", "aaa", 0, 0);
    expect_find("aaaaaaaaaa", "aaa", 7, 7);
    expect_find("aaaaaaaaaa", "aaa", 8, -1);
}

TEST(CharSequenceFind, PartialMatchBeforeRealMatch) {
    expect_find("aaaaab", "aab", 0, 3);
    expect_find("abababc", "abc", 0, 4);
}

TEST(CharSequenceFind, SamePrefixDifferentTail) {
    expect_find("abcabd", "abd", 0, 3);
    expect_find("abcabd", "abe", 0, -1);
}

TEST(CharSequenceFind, SameCharactersInDifferentOrderAreNotFound) {
    expect_find("abcdef", "cba", 0, -1);
    expect_find("abcdef", "fed", 0, -1);
}

TEST(CharSequenceFind, IsCaseSensitive) {
    expect_find("Hello", "hello", 0, -1);
    expect_find("Hello", "Hello", 0, 0);
}

TEST(CharSequenceFind, FindsSpacesAndSymbols) {
    expect_find("a b-c_d", " ", 0, 1);
    expect_find("a b-c_d", "-c_", 0, 3);
}

TEST(CharSequenceFind, FindsDigits) {
    expect_find("2026/10/12", "10", 0, 5);
    expect_find("2026/10/12", "10", 6, -1);
}

TEST(CharSequenceFind, HighAsciiCharactersDoNotCauseFalseMatch) {
    expect_find("~}|{zyxwvutsrqp", "~}|", 0, 0);
    expect_find("~}|{zyxwvutsrqp", "rqp", 0, 12);
    expect_find("~}|{zyxwvutsrqp", "}~", 0, -1);
}

TEST(CharSequenceFind, WorksOnMaxLengthSequence) {
    expect_find("123456789012345", "5", 0, 4);
    expect_find("123456789012345", "5", 5, 14);
    expect_find("123456789012345", "12345", 0, 0);
    expect_find("123456789012345", "12345", 1, 10);
}

TEST(CharSequenceFind, WorksOnTruncatedSequence) {
    // 15글자로 잘린 뒤의 내용만 검색 대상이 된다.
    expect_find("123456789012345678", "345", 0, 2);
    expect_find("123456789012345678", "678", 0, -1);
}

TEST(CharSequenceFind, FindsPatternAtEveryPosition) {
    const std::string text = "abcdefghijklmno";

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        const std::string pattern = text.substr(static_cast<std::size_t>(i), 1);
        expect_find(text.c_str(), pattern.c_str(), 0, i);
    }
}

TEST(CharSequenceFind, DoesNotModifySequence) {
    CharSequenceProbe sequence("hello");

    sequence.find("ll", 0);
    sequence.find(CharSequenceProbe("ll"), 0);

    EXPECT_EQ("hello", to_std_string(sequence));
    EXPECT_EQ(5, sequence.length());
}

TEST(CharSequenceFind, WorksAfterSetChar) {
    CharSequenceProbe sequence("abcd");

    sequence.setChar('x', 1);

    EXPECT_EQ(1, sequence.find("x", 0));
    EXPECT_EQ(-1, sequence.find("b", 0));
    EXPECT_EQ(2, sequence.find("cd", 0));
}

TEST(CharSequenceFind, CharArrayAndCharSequenceOverloadsAgree) {
    const char *texts[] = {"", "a", "abc", "abcabc", "aaaa", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "b", "abc", "aa", "ll", "o w", "345", "zzz", "123456789012345"};

    for (const char *text: texts) {
        CharSequenceProbe sequence(text);
        for (const char *pattern: patterns) {
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(sequence.find(pattern, start), sequence.find(CharSequenceProbe(pattern), start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}

TEST(CharSequenceFind, MatchesStdStringForManyInputs) {
    const char *texts[] = {"abcabcabc", "aaaaaaaaaa", "hello world", "2026/10/12", "abababababababa", "123456789012345"};
    const char *patterns[] = {"a", "ab", "aba", "abc", "aa", "o", "lo w", "10", "/", "5", "345", "zz", "ba"};

    for (const char *text: texts) {
        for (const char *pattern: patterns) {
            for (int start = -3; start <= 17; ++start) {
                expect_find(text, pattern, start, expected_find(text, pattern, start));
            }
        }
    }
}
