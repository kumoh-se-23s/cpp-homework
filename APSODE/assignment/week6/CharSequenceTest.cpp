#include <gtest/gtest.h>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

#include "CharSequence.h"

namespace {
    CharSequence make(const char *text) {
        return CharSequence(text, static_cast<int>(std::strlen(text)));
    }

    std::string to_std_string(const CharSequence &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    // std::string::find 결과를 기대값으로 계산한다 (start 음수 → 0, 못 찾으면 -1)
    int expected_find(const std::string &text, const std::string &pattern, int start) {
        if (start < 0) {
            start = 0;
        }
        if (pattern.empty() || start > static_cast<int>(text.size())) {
            return -1;
        }
        const std::size_t found = text.find(pattern, static_cast<std::size_t>(start));
        return found == std::string::npos ? -1 : static_cast<int>(found);
    }

    // find의 두 오버로드(const char* / const CharSequence&)를 함께 검사한다
    void expect_find(const char *text, const char *pattern, int start, int expected) {
        const CharSequence sequence = make(text);

        EXPECT_EQ(expected, sequence.find(pattern, start))
            << "[const char*] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
        EXPECT_EQ(expected, sequence.find(make(pattern), start))
            << "[CharSequence] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
    }
}

// =====================================================================
// 생성자 / 복사
// =====================================================================

TEST(CharSequenceConstructor, DefaultIsEmpty) {
    CharSequence sequence;

    EXPECT_EQ(0, sequence.size());
    EXPECT_TRUE(sequence.empty());
    EXPECT_EQ("", to_std_string(sequence));
}

TEST(CharSequenceConstructor, FromCharArray) {
    CharSequence sequence("hello", 5);

    EXPECT_EQ(5, sequence.size());
    EXPECT_FALSE(sequence.empty());
    EXPECT_EQ('h', sequence.at(0));
    EXPECT_EQ('o', sequence.at(4));
    EXPECT_EQ("hello", to_std_string(sequence));
}

TEST(CharSequenceConstructor, FromCharArrayUsesOnlyGivenSize) {
    CharSequence sequence("hello world", 5);

    EXPECT_EQ(5, sequence.size());
    EXPECT_EQ("hello", to_std_string(sequence));
}

TEST(CharSequenceConstructor, IsNullTerminated) {
    CharSequence sequence("hello world", 5);

    EXPECT_EQ('\0', sequence[5]);
    EXPECT_STREQ("hello", &sequence[0]);
}

TEST(CharSequenceConstructor, LongTextGrowsCapacity) {
    const std::string text(100, 'x');
    CharSequence sequence(text.c_str(), 100);

    EXPECT_EQ(100, sequence.size());
    EXPECT_GE(sequence.capacity(), 100);
    EXPECT_EQ(text, to_std_string(sequence));
}

TEST(CharSequenceConstructor, NegativeSizeThrows) {
    EXPECT_THROW(CharSequence("abc", -1), std::invalid_argument);
}

TEST(CharSequenceConstructor, NullElementsThrows) {
    EXPECT_THROW(CharSequence(nullptr, 3), std::invalid_argument);
}

TEST(CharSequenceCopy, CopyConstructorIsDeep) {
    CharSequence original = make("hello");
    CharSequence copy(original);

    copy[0] = 'j';

    EXPECT_EQ("hello", to_std_string(original));
    EXPECT_EQ("jello", to_std_string(copy));
}

TEST(CharSequenceCopy, CopyInitializationWorks) {
    const CharSequence original = make("hello");
    const CharSequence copy = original;   // explicit이 아니어야 컴파일된다

    EXPECT_EQ("hello", to_std_string(copy));
}

TEST(CharSequenceCopy, AssignmentIsDeep) {
    CharSequence source = make("hello");
    CharSequence target = make("bye");

    target = source;
    target[0] = 'j';

    EXPECT_EQ("hello", to_std_string(source));
    EXPECT_EQ("jello", to_std_string(target));
}

// =====================================================================
// 원소 접근
// =====================================================================

TEST(CharSequenceAccess, AtIsWritable) {
    CharSequence sequence = make("abc");

    sequence.at(1) = 'X';

    EXPECT_EQ("aXc", to_std_string(sequence));
}

TEST(CharSequenceAccess, AtThrowsOutOfRange) {
    const CharSequence sequence = make("abc");

    EXPECT_THROW(sequence.at(-1), std::out_of_range);
    EXPECT_THROW(sequence.at(3), std::out_of_range);
}

// =====================================================================
// operator<<
// =====================================================================

TEST(CharSequenceOutput, ChainsWithOtherOutput) {
    const CharSequence sequence = make("world");
    std::ostringstream stream;

    stream << "hello " << sequence << '!';

    EXPECT_EQ("hello world!", stream.str());
}

TEST(CharSequenceOutput, PrintsOnlyUpToSize) {
    const CharSequence sequence("abcdef", 3);
    std::ostringstream stream;

    stream << sequence;

    EXPECT_EQ("abc", stream.str());
}

// =====================================================================
// find
// =====================================================================

TEST(CharSequenceFind, FindsAtBeginning) {
    expect_find("hello world", "hello", 0, 0);
}

TEST(CharSequenceFind, FindsInMiddle) {
    expect_find("hello world", "lo w", 0, 3);
}

TEST(CharSequenceFind, FindsAtEnd) {
    expect_find("hello world", "world", 0, 6);
}

TEST(CharSequenceFind, FindsSingleCharacter) {
    expect_find("hello world", "w", 0, 6);
}

TEST(CharSequenceFind, PatternEqualsWholeText) {
    expect_find("hello", "hello", 0, 0);
}

TEST(CharSequenceFind, ReturnsFirstOccurrence) {
    expect_find("abcabcabc", "abc", 0, 0);
}

TEST(CharSequenceFind, StartSkipsEarlierOccurrences) {
    expect_find("abcabcabc", "abc", 1, 3);
    expect_find("abcabcabc", "abc", 3, 3);
    expect_find("abcabcabc", "abc", 4, 6);
}

TEST(CharSequenceFind, StartAtLastPossiblePosition) {
    expect_find("abcabc", "bc", 4, 4);
}

TEST(CharSequenceFind, OverlappingOccurrences) {
    expect_find("aaaa", "aa", 0, 0);
    expect_find("aaaa", "aa", 1, 1);
    expect_find("aaaa", "aa", 2, 2);
    expect_find("aaaa", "aa", 3, -1);
}

TEST(CharSequenceFind, NotFound) {
    expect_find("hello world", "xyz", 0, -1);
    expect_find("hello world", "worlds", 0, -1);
}

TEST(CharSequenceFind, PatternLongerThanText) {
    expect_find("abc", "abcd", 0, -1);
}

TEST(CharSequenceFind, NegativeStartIsTreatedAsZero) {
    expect_find("hello world", "hello", -5, 0);
}

TEST(CharSequenceFind, StartPastEndReturnsMinusOne) {
    expect_find("hello", "o", 5, -1);
    expect_find("hello", "o", 100, -1);
}

TEST(CharSequenceFind, EmptyPatternReturnsMinusOne) {
    expect_find("hello", "", 0, -1);
}

TEST(CharSequenceFind, EmptyTextReturnsMinusOne) {
    expect_find("", "a", 0, -1);
}

TEST(CharSequenceFind, IgnoresCharactersPastSize) {
    // "abc" 뒤의 "def"는 시퀀스에 들어있지 않으므로 찾으면 안 된다
    const CharSequence sequence("abcdef", 3);

    EXPECT_EQ(-1, sequence.find("cd", 0));
    EXPECT_EQ(2, sequence.find("c", 0));
}

TEST(CharSequenceFind, NearMissPrefixes) {
    // 앞부분만 일치하는 후보가 여러 번 나오는 경우
    expect_find("aaaaaaaab", "aaab", 0, 5);
    expect_find("abababac", "ababac", 0, 2);
}

TEST(CharSequenceFind, NonAsciiBytes) {
    // UTF-8 한글은 char가 signed일 때 음수 바이트가 된다
    const char *text = "가나다라마바사";
    const char *pattern = "라마";

    expect_find(text, pattern, 0, expected_find(text, pattern, 0));
    expect_find(text, "사", 0, expected_find(text, "사", 0));
    expect_find(text, "가", 1, -1);
}

TEST(CharSequenceFind, MatchesStdStringOnLongText) {
    // 해시가 여러 번 굴러가도 정확한지 std::string::find와 비교한다
    std::string text;
    for (int index = 0; index < 2000; ++index) {
        text += static_cast<char>('a' + (index * 7 + index / 13) % 5);
    }
    const CharSequence sequence(text.c_str(), static_cast<int>(text.size()));

    for (int length = 1; length <= 8; ++length) {
        for (int offset = 0; offset < 1500; offset += 97) {
            const std::string pattern = text.substr(offset, length);
            for (int start : {0, offset, offset + 1, 1000}) {
                EXPECT_EQ(expected_find(text, pattern, start), sequence.find(pattern.c_str(), start))
                    << "pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}

TEST(CharSequenceFind, DoesNotModifySequence) {
    const CharSequence sequence = make("hello world");

    sequence.find("world", 0);

    EXPECT_EQ("hello world", to_std_string(sequence));
}
