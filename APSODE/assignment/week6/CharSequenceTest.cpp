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

TEST(CharSequenceConstructor, FromCString) {
    CharSequence sequence("hello");

    EXPECT_EQ(5, sequence.size());
    EXPECT_STREQ("hello", &sequence[0]);
}

TEST(CharSequenceConstructor, FromEmptyCString) {
    CharSequence sequence("");

    EXPECT_TRUE(sequence.empty());
    EXPECT_STREQ("", &sequence[0]);
}

TEST(CharSequenceConstructor, FromCStringIsDeepCopy) {
    char source[] = "hello";
    CharSequence sequence(source);

    source[0] = 'j';

    EXPECT_STREQ("hello", &sequence[0]);
}

TEST(CharSequenceConstructor, ImplicitConversionFromCString) {
    const CharSequence sequence = "hello";   // explicit이 아니어야 컴파일된다

    EXPECT_EQ("hello", to_std_string(sequence));
}

TEST(CharSequenceConstructor, NullCStringThrows) {
    const char *null_chars = nullptr;

    // 소괄호로 쓰면 null_chars라는 변수 선언으로 해석되므로 중괄호를 쓴다
    EXPECT_THROW(CharSequence{null_chars}, std::invalid_argument);
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

// =====================================================================
// operator[]
// =====================================================================

TEST(CharSequenceSubscript, ReadsElement) {
    const CharSequence sequence = make("abc");

    EXPECT_EQ('a', sequence[0]);
    EXPECT_EQ('c', sequence[2]);
}

TEST(CharSequenceSubscript, IsWritable) {
    CharSequence sequence = make("abc");

    sequence[1] = 'X';

    EXPECT_EQ("aXc", to_std_string(sequence));
}

// =====================================================================
// append
// =====================================================================

TEST(CharSequenceAppend, AppendsToEnd) {
    CharSequence sequence = make("hello");

    sequence.append(" world");

    EXPECT_EQ(11, sequence.size());
    EXPECT_STREQ("hello world", &sequence[0]);
}

TEST(CharSequenceAppend, ToEmptySequence) {
    CharSequence sequence;

    sequence.append("abc");

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceAppend, EmptyTextChangesNothing) {
    CharSequence sequence = make("abc");

    sequence.append("");

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceAppend, ConsecutiveCallsAccumulate) {
    CharSequence sequence = make("a");

    sequence.append("b");
    sequence.append("c");

    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceAppend, DoublesCapacityWhenFull) {
    CharSequence sequence = make("1234567");
    ASSERT_EQ(8, sequence.capacity());

    sequence.append("8");

    EXPECT_EQ(16, sequence.capacity());
    EXPECT_STREQ("12345678", &sequence[0]);
}

TEST(CharSequenceAppend, LongTextKeepsCapacityMultipleOfEight) {
    const std::string text(100, 'x');
    CharSequence sequence = make("abc");

    sequence.append(text.c_str());

    EXPECT_EQ(103, sequence.size());
    EXPECT_GE(sequence.capacity(), 103);
    EXPECT_EQ(0, sequence.capacity() % 8);
    EXPECT_EQ("abc" + text, std::string(&sequence[0]));
}

TEST(CharSequenceAppend, OwnContentSurvivesGrowth) {
    // 재할당으로 기존 버퍼가 해제된 뒤에 읽으면 안 된다
    CharSequence sequence = make("abcde");

    sequence.append(&sequence[0]);

    EXPECT_STREQ("abcdeabcde", &sequence[0]);
}

TEST(CharSequenceAppend, SingleCharacter) {
    CharSequence sequence = make("ab");

    sequence.append('c');

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceAppend, SingleCharacterGrowsCapacity) {
    CharSequence sequence;
    std::string expected;

    for (int loop = 0; loop < 100; ++loop) {
        const char character = static_cast<char>('a' + loop % 26);
        sequence.append(character);
        expected += character;
    }

    EXPECT_EQ(100, sequence.size());
    EXPECT_EQ(0, sequence.capacity() % 8);
    EXPECT_EQ(expected, std::string(&sequence[0]));
}

TEST(CharSequenceAppend, OtherSequence) {
    CharSequence sequence = make("hello");
    const CharSequence other = make(" world");

    sequence.append(other);

    EXPECT_STREQ("hello world", &sequence[0]);
    EXPECT_STREQ(" world", &other[0]);
}

TEST(CharSequenceAppend, OtherSequenceUsesOnlyItsSize) {
    CharSequence sequence = make("a");
    const CharSequence other("bcdef", 2);

    sequence.append(other);

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceAppend, OwnSequence) {
    CharSequence sequence = make("abcde");

    sequence.append(sequence);

    EXPECT_EQ(10, sequence.size());
    EXPECT_STREQ("abcdeabcde", &sequence[0]);
}

TEST(CharSequenceAppend, NullThrows) {
    CharSequence sequence = make("abc");
    const char *null_chars = nullptr;

    EXPECT_THROW(sequence.append(null_chars), std::invalid_argument);
    EXPECT_STREQ("abc", &sequence[0]);
}

// =====================================================================
// insert
// =====================================================================

TEST(CharSequenceInsert, AtFront) {
    CharSequence sequence = make("world");

    sequence.insert(0, "hello ");

    EXPECT_EQ(11, sequence.size());
    EXPECT_STREQ("hello world", &sequence[0]);
}

TEST(CharSequenceInsert, InMiddle) {
    CharSequence sequence = make("held");

    sequence.insert(2, "llo wor");

    EXPECT_STREQ("hello world", &sequence[0]);
}

TEST(CharSequenceInsert, AtEnd) {
    CharSequence sequence = make("abc");

    sequence.insert(3, "def");

    EXPECT_STREQ("abcdef", &sequence[0]);
}

TEST(CharSequenceInsert, IntoEmpty) {
    CharSequence sequence;

    sequence.insert(0, "abc");

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceInsert, EmptyTextChangesNothing) {
    CharSequence sequence = make("abc");

    sequence.insert(1, "");

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceInsert, ConsecutiveCallsAccumulate) {
    CharSequence sequence = make("ad");

    sequence.insert(1, "c");
    sequence.insert(1, "b");

    EXPECT_STREQ("abcd", &sequence[0]);
}

TEST(CharSequenceInsert, GrowsCapacity) {
    CharSequence sequence = make("1234567");

    sequence.insert(3, "abc");

    EXPECT_GE(sequence.capacity(), 10);
    EXPECT_EQ(0, sequence.capacity() % 8);
    EXPECT_STREQ("123abc4567", &sequence[0]);
}

TEST(CharSequenceInsert, OwnContent) {
    // 원소를 뒤로 미는 동안 넣을 내용이 덮어써지면 안 된다
    CharSequence sequence = make("abc");

    sequence.insert(1, &sequence[0]);

    EXPECT_STREQ("aabcbc", &sequence[0]);
}

TEST(CharSequenceInsert, OtherSequence) {
    CharSequence sequence = make("held");
    const CharSequence other = make("llo wor");

    sequence.insert(2, other);

    EXPECT_STREQ("hello world", &sequence[0]);
    EXPECT_STREQ("llo wor", &other[0]);
}

TEST(CharSequenceInsert, OwnSequence) {
    CharSequence sequence = make("abc");

    sequence.insert(1, sequence);

    EXPECT_EQ(6, sequence.size());
    EXPECT_STREQ("aabcbc", &sequence[0]);
}

TEST(CharSequenceInsert, InvalidPositionThrows) {
    CharSequence sequence = make("abc");
    const CharSequence other = make("x");

    EXPECT_THROW(sequence.insert(-1, "x"), std::out_of_range);
    EXPECT_THROW(sequence.insert(4, "x"), std::out_of_range);
    EXPECT_THROW(sequence.insert(-1, other), std::out_of_range);
    EXPECT_THROW(sequence.insert(4, other), std::out_of_range);
    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequenceInsert, NullThrows) {
    CharSequence sequence = make("abc");
    const char *null_chars = nullptr;

    EXPECT_THROW(sequence.insert(0, null_chars), std::invalid_argument);
}

// =====================================================================
// operator+
// =====================================================================

TEST(CharSequencePlus, ReturnsConcatenation) {
    const CharSequence sequence = make("hello");

    const CharSequence result = sequence + " world";

    EXPECT_EQ(11, result.size());
    EXPECT_STREQ("hello world", &result[0]);
}

TEST(CharSequencePlus, DoesNotModifyOperand) {
    const CharSequence sequence = make("hello");

    const CharSequence result = sequence + " world";

    EXPECT_EQ(5, sequence.size());
    EXPECT_STREQ("hello", &sequence[0]);
    EXPECT_NE(&sequence[0], &result[0]);
}

TEST(CharSequencePlus, EmptyOperands) {
    const CharSequence empty;
    const CharSequence sequence = make("abc");

    EXPECT_STREQ("abc", &(empty + "abc")[0]);
    EXPECT_STREQ("abc", &(sequence + "")[0]);
    EXPECT_STREQ("", &(empty + "")[0]);
}

TEST(CharSequencePlus, CanBeChained) {
    const CharSequence result = make("a") + "b" + "c";

    EXPECT_STREQ("abc", &result[0]);
}

TEST(CharSequencePlus, GrowsCapacity) {
    const std::string text(100, 'x');
    const CharSequence sequence = make("abc");

    const CharSequence result = sequence + text.c_str();

    EXPECT_EQ(103, result.size());
    EXPECT_EQ(0, result.capacity() % 8);
    EXPECT_EQ("abc" + text, std::string(&result[0]));
}

TEST(CharSequencePlus, NullThrows) {
    const CharSequence sequence = make("abc");
    const char *null_chars = nullptr;

    EXPECT_THROW(sequence + null_chars, std::invalid_argument);
}

// =====================================================================
// operator= (const char*)
// =====================================================================

TEST(CharSequenceAssignChars, ReplacesContent) {
    CharSequence sequence = make("hello");

    sequence = "bye";

    EXPECT_EQ(3, sequence.size());
    EXPECT_STREQ("bye", &sequence[0]);
}

TEST(CharSequenceAssignChars, FromEmptyText) {
    CharSequence sequence = make("hello");

    sequence = "";

    EXPECT_TRUE(sequence.empty());
    EXPECT_STREQ("", &sequence[0]);
}

TEST(CharSequenceAssignChars, GrowsAndKeepsCapacityMultipleOfEight) {
    const std::string text(100, 'x');
    CharSequence sequence = make("abc");

    sequence = text.c_str();

    EXPECT_EQ(100, sequence.size());
    EXPECT_EQ(0, sequence.capacity() % 8);
    EXPECT_EQ(text, std::string(&sequence[0]));
}

TEST(CharSequenceAssignChars, ReturnsSelfForChaining) {
    CharSequence sequence;

    CharSequence &returned = (sequence = "abc");

    EXPECT_EQ(&sequence, &returned);
}

TEST(CharSequenceAssignChars, FromOwnContent) {
    CharSequence sequence = make("hello");

    sequence = &sequence[1];

    EXPECT_STREQ("ello", &sequence[0]);
}

TEST(CharSequenceAssignChars, NullThrowsAndKeepsContent) {
    CharSequence sequence = make("abc");
    const char *null_chars = nullptr;

    EXPECT_THROW(sequence = null_chars, std::invalid_argument);
    EXPECT_STREQ("abc", &sequence[0]);
}

// =====================================================================
// operator+=
// =====================================================================

TEST(CharSequencePlusAssign, AppendsToSelf) {
    CharSequence sequence = make("hello");

    sequence += " world";

    EXPECT_EQ(11, sequence.size());
    EXPECT_STREQ("hello world", &sequence[0]);
}

TEST(CharSequencePlusAssign, EmptyText) {
    CharSequence sequence = make("abc");

    sequence += "";

    EXPECT_STREQ("abc", &sequence[0]);
}

TEST(CharSequencePlusAssign, ReturnsSelfForChaining) {
    CharSequence sequence = make("a");

    CharSequence &returned = (sequence += "b");
    (returned += "c") += "d";

    EXPECT_EQ(&sequence, &returned);
    EXPECT_STREQ("abcd", &sequence[0]);
}

TEST(CharSequencePlusAssign, ManyAppendsKeepContent) {
    CharSequence sequence;
    std::string expected;

    for (int loop = 0; loop < 200; ++loop) {
        sequence += "ab";
        expected += "ab";
    }

    EXPECT_EQ(400, sequence.size());
    EXPECT_EQ(0, sequence.capacity() % 8);
    EXPECT_EQ(expected, std::string(&sequence[0]));
}

TEST(CharSequencePlusAssign, OwnContent) {
    CharSequence sequence = make("abcde");

    sequence += &sequence[0];

    EXPECT_STREQ("abcdeabcde", &sequence[0]);
}

TEST(CharSequencePlusAssign, NullThrowsAndKeepsContent) {
    CharSequence sequence = make("abc");
    const char *null_chars = nullptr;

    EXPECT_THROW(sequence += null_chars, std::invalid_argument);
    EXPECT_STREQ("abc", &sequence[0]);
}

// =====================================================================
// operator== / operator!=
// =====================================================================

TEST(CharSequenceEquality, SameContent) {
    const CharSequence sequence = make("abc");

    EXPECT_TRUE(sequence == "abc");
    EXPECT_FALSE(sequence != "abc");
}

TEST(CharSequenceEquality, DifferentContent) {
    const CharSequence sequence = make("abc");

    EXPECT_FALSE(sequence == "abd");
    EXPECT_TRUE(sequence != "abd");
}

TEST(CharSequenceEquality, DifferentLength) {
    const CharSequence sequence = make("abc");

    EXPECT_FALSE(sequence == "ab");
    EXPECT_FALSE(sequence == "abcd");
    EXPECT_TRUE(sequence != "ab");
    EXPECT_TRUE(sequence != "abcd");
}

TEST(CharSequenceEquality, Empty) {
    const CharSequence empty;

    EXPECT_TRUE(empty == "");
    EXPECT_FALSE(empty != "");
    EXPECT_FALSE(empty == "a");
}

TEST(CharSequenceEquality, WithOtherSequence) {
    const CharSequence sequence = make("abc");

    EXPECT_TRUE(sequence == make("abc"));
    EXPECT_FALSE(sequence != make("abc"));
    EXPECT_FALSE(sequence == make("abd"));
    EXPECT_TRUE(sequence != make("abcd"));
}

TEST(CharSequenceEquality, NullThrows) {
    const CharSequence sequence = make("abc");
    const char *null_chars = nullptr;

    EXPECT_THROW(sequence == null_chars, std::invalid_argument);
    EXPECT_THROW(sequence != null_chars, std::invalid_argument);
}

// =====================================================================
// compare
// =====================================================================

TEST(CharSequenceCompare, ReturnsSignOfOrder) {
    const CharSequence sequence = make("banana");

    EXPECT_EQ(0, sequence.compare("banana"));
    EXPECT_LT(sequence.compare("cherry"), 0);
    EXPECT_GT(sequence.compare("apple"), 0);

    EXPECT_EQ(0, sequence.compare(make("banana")));
    EXPECT_LT(sequence.compare(make("cherry")), 0);
    EXPECT_GT(sequence.compare(make("apple")), 0);
}

TEST(CharSequenceCompare, PrefixIsSmaller) {
    EXPECT_LT(make("abc").compare("abcd"), 0);
    EXPECT_GT(make("abcd").compare("abc"), 0);
    EXPECT_LT(CharSequence().compare("a"), 0);
    EXPECT_EQ(0, CharSequence().compare(""));
}

TEST(CharSequenceCompare, NonAsciiBytesCompareAsUnsigned) {
    EXPECT_GT(make("가").compare("a"), 0);
    EXPECT_LT(make("a").compare(make("가")), 0);
}

TEST(CharSequenceCompare, NullThrows) {
    const char *null_chars = nullptr;

    EXPECT_THROW(make("abc").compare(null_chars), std::invalid_argument);
}

// =====================================================================
// operator< / <= / > / >= (사전 순서, std::string과 동일)
// =====================================================================

namespace {
    // 네 비교 연산자의 결과를 const char*와 CharSequence 인자 모두 std::string 비교 결과와 대조한다
    void expect_ordering(const char *left, const char *right) {
        const CharSequence sequence = make(left);
        const CharSequence other = make(right);
        const std::string left_text(left);
        const std::string right_text(right);

        EXPECT_EQ(left_text < right_text, sequence < right) << '"' << left << "\" < \"" << right << '"';
        EXPECT_EQ(left_text <= right_text, sequence <= right) << '"' << left << "\" <= \"" << right << '"';
        EXPECT_EQ(left_text > right_text, sequence > right) << '"' << left << "\" > \"" << right << '"';
        EXPECT_EQ(left_text >= right_text, sequence >= right) << '"' << left << "\" >= \"" << right << '"';

        EXPECT_EQ(left_text < right_text, sequence < other) << '"' << left << "\" < \"" << right << '"';
        EXPECT_EQ(left_text <= right_text, sequence <= other) << '"' << left << "\" <= \"" << right << '"';
        EXPECT_EQ(left_text > right_text, sequence > other) << '"' << left << "\" > \"" << right << '"';
        EXPECT_EQ(left_text >= right_text, sequence >= other) << '"' << left << "\" >= \"" << right << '"';
    }
}

TEST(CharSequenceOrdering, EqualText) {
    expect_ordering("abc", "abc");
    expect_ordering("", "");
}

TEST(CharSequenceOrdering, FirstDifferenceDecides) {
    expect_ordering("abc", "abd");
    expect_ordering("abd", "abc");
    expect_ordering("abc", "bbc");
    expect_ordering("bbc", "abc");
}

TEST(CharSequenceOrdering, EarlierDifferenceBeatsLaterOnes) {
    expect_ordering("axa", "bab");
    expect_ordering("bab", "axa");
}

TEST(CharSequenceOrdering, PrefixIsSmaller) {
    expect_ordering("abc", "abcd");
    expect_ordering("abcd", "abc");
}

TEST(CharSequenceOrdering, DifferenceBeatsLength) {
    expect_ordering("b", "abcd");
    expect_ordering("abcd", "b");
}

TEST(CharSequenceOrdering, EmptyIsSmallest) {
    expect_ordering("", "a");
    expect_ordering("a", "");
}

TEST(CharSequenceOrdering, UppercaseBeforeLowercase) {
    expect_ordering("Zebra", "apple");
    expect_ordering("apple", "Zebra");
}

TEST(CharSequenceOrdering, NonAsciiBytesCompareAsUnsigned) {
    // UTF-8 한글은 signed char에서 음수지만 std::string은 unsigned로 비교한다
    expect_ordering("가", "a");
    expect_ordering("a", "가");
    expect_ordering("가", "나");
}
