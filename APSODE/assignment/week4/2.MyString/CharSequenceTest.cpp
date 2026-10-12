#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "CharSequence.h"

namespace {
    std::string to_std_string(const CharSequence &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
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
        CharSequence sequence(text);

        EXPECT_EQ(expected, sequence.find(pattern, start))
            << "[char array] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
        EXPECT_EQ(expected, sequence.find(CharSequence(pattern), start))
            << "[CharSequence] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
    }

    // protected 멤버를 테스트하기 위한 접근용 하위 클래스
    class CharSequenceProbe : public CharSequence {
        public:
            using CharSequence::CharSequence;
            using CharSequence::flush;
            using CharSequence::is_correct_index_bound;

            const char *raw() const { return container; }

            int raw_length() const { return length_; }
    };
}

// =====================================================================
// 생성자
// =====================================================================

TEST(CharSequenceConstructor, DefaultCreatesEmptySequence) {
    CharSequence sequence;

    EXPECT_EQ(0, sequence.length());
    EXPECT_TRUE(sequence.empty());
}

TEST(CharSequenceConstructor, FromCharArray) {
    CharSequence sequence("hello");

    EXPECT_EQ(5, sequence.length());
    EXPECT_FALSE(sequence.empty());
    EXPECT_EQ('h', sequence.at(0));
    EXPECT_EQ('o', sequence.at(4));
}

TEST(CharSequenceConstructor, FromEmptyCharArray) {
    CharSequence sequence("");

    EXPECT_EQ(0, sequence.length());
    EXPECT_TRUE(sequence.empty());
}

TEST(CharSequenceConstructor, FromExactMaxLengthCharArray) {
    CharSequence sequence("123456789012345");

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ('5', sequence.at(CharSequence::MAX_LENGTH - 1));
}

TEST(CharSequenceConstructor, FromLongCharArrayIsTruncated) {
    CharSequence sequence("1234567890123456789");

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence));
}

TEST(CharSequenceConstructor, CopyCopiesContent) {
    CharSequence original("copy me");
    CharSequence copied(original);

    EXPECT_TRUE(original.equals(copied));
    EXPECT_EQ(original.length(), copied.length());
}

TEST(CharSequenceConstructor, CopyIsIndependentFromOriginal) {
    CharSequence original("abc");
    CharSequence copied(original);

    copied.set('z', 0);

    EXPECT_EQ('a', original.at(0));
    EXPECT_EQ('z', copied.at(0));
}

// =====================================================================
// at
// =====================================================================

TEST(CharSequenceAt, ReturnsEachCharacter) {
    CharSequence sequence("abc");

    EXPECT_EQ('a', sequence.at(0));
    EXPECT_EQ('b', sequence.at(1));
    EXPECT_EQ('c', sequence.at(2));
}

// =====================================================================
// set
// =====================================================================

TEST(CharSequenceSet, ChangesCharacterAndReturnsTrue) {
    CharSequence sequence("abc");

    EXPECT_TRUE(sequence.set('x', 1));
    EXPECT_EQ('x', sequence.at(1));
    EXPECT_EQ(3, sequence.length());
}

TEST(CharSequenceSet, NegativeIndexReturnsFalse) {
    CharSequence sequence("abc");

    EXPECT_FALSE(sequence.set('x', -1));
    EXPECT_EQ("abc", to_std_string(sequence));
}

TEST(CharSequenceSet, IndexEqualToLengthReturnsFalse) {
    CharSequence sequence("abc");

    EXPECT_FALSE(sequence.set('x', 3));
    EXPECT_EQ("abc", to_std_string(sequence));
}

TEST(CharSequenceSet, IndexBeyondMaxLengthReturnsFalse) {
    CharSequence sequence("abc");

    EXPECT_FALSE(sequence.set('x', CharSequence::MAX_LENGTH + 5));
}

TEST(CharSequenceSet, OnEmptySequenceReturnsFalse) {
    CharSequence sequence;

    EXPECT_FALSE(sequence.set('x', 0));
    EXPECT_TRUE(sequence.empty());
}

// =====================================================================
// equals
// =====================================================================

TEST(CharSequenceEquals, TrueForSameContent) {
    CharSequence a("hello");
    CharSequence b("hello");

    EXPECT_TRUE(a.equals(b));
    EXPECT_TRUE(b.equals(a));
}

TEST(CharSequenceEquals, FalseForDifferentContent) {
    CharSequence a("hello");
    CharSequence b("world");

    EXPECT_FALSE(a.equals(b));
}

TEST(CharSequenceEquals, FalseForDifferentLength) {
    CharSequence a("hello");
    CharSequence b("hell");

    EXPECT_FALSE(a.equals(b));
    EXPECT_FALSE(b.equals(a));
}

TEST(CharSequenceEquals, TrueForTwoEmptySequences) {
    CharSequence a;
    CharSequence b("");

    EXPECT_TRUE(a.equals(b));
}

// =====================================================================
// append_truncated
// =====================================================================

TEST(CharSequenceAppendTruncated, WithCharArray) {
    CharSequence base("abc");

    CharSequence result = base.append_truncated("def");

    EXPECT_EQ("abcdef", to_std_string(result));
    EXPECT_EQ(6, result.length());
}

TEST(CharSequenceAppendTruncated, WithCharSequence) {
    CharSequence base("abc");
    CharSequence other("def");

    CharSequence result = base.append_truncated(other);

    EXPECT_EQ("abcdef", to_std_string(result));
}

TEST(CharSequenceAppendTruncated, DoesNotModifyOriginal) {
    CharSequence base("abc");

    base.append_truncated("def");

    EXPECT_EQ("abc", to_std_string(base));
    EXPECT_EQ(3, base.length());
}

TEST(CharSequenceAppendTruncated, CutsAtMaxLength) {
    CharSequence base("1234567890");

    CharSequence result = base.append_truncated("abcdefghij");

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(CharSequenceAppendTruncated, ExactlyMaxLengthIsNotCut) {
    CharSequence base("1234567890");

    CharSequence result = base.append_truncated("abcde");

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(CharSequenceAppendTruncated, ToFullSequenceKeepsContent) {
    CharSequence base("123456789012345");

    CharSequence result = base.append_truncated("xyz");

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("123456789012345", to_std_string(result));
}

TEST(CharSequenceAppendTruncated, WithEmptyReturnsSameContent) {
    CharSequence base("abc");

    CharSequence result = base.append_truncated("");

    EXPECT_TRUE(base.equals(result));
}

TEST(CharSequenceAppendTruncated, OnEmptyBase) {
    CharSequence base;

    CharSequence result = base.append_truncated("abc");

    EXPECT_EQ("abc", to_std_string(result));
}

// =====================================================================
// copy_from_other
// =====================================================================

TEST(CharSequenceCopyFromOther, CharArrayReplacesContent) {
    CharSequence sequence("old");

    sequence.copy_from_other("new content");

    EXPECT_EQ("new content", to_std_string(sequence));
    EXPECT_EQ(11, sequence.length());
}

TEST(CharSequenceCopyFromOther, CharArrayTruncatesLongInput) {
    CharSequence sequence;

    sequence.copy_from_other("1234567890123456789");

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence));
}

TEST(CharSequenceCopyFromOther, CharSequenceReplacesContent) {
    CharSequence sequence("old");
    CharSequence other("replacement");

    sequence.copy_from_other(other);

    EXPECT_TRUE(sequence.equals(other));
}

TEST(CharSequenceCopyFromOther, EmptyMakesSequenceEmpty) {
    CharSequence sequence("something");
    CharSequence empty;

    sequence.copy_from_other(empty);

    EXPECT_TRUE(sequence.empty());
    EXPECT_EQ(0, sequence.length());
}

TEST(CharSequenceCopyFromOther, SelfKeepsContent) {
    CharSequence sequence("self");

    sequence.copy_from_other(sequence);

    EXPECT_EQ("self", to_std_string(sequence));
}

// =====================================================================
// operator<<
// =====================================================================

TEST(CharSequenceOutputOperator, PrintsContent) {
    CharSequence sequence("print");
    std::ostringstream stream;

    stream << sequence;

    EXPECT_EQ("print", stream.str());
}

TEST(CharSequenceOutputOperator, PrintsNothingForEmpty) {
    CharSequence sequence;
    std::ostringstream stream;

    stream << sequence;

    EXPECT_EQ("", stream.str());
}

TEST(CharSequenceOutputOperator, IsChainable) {
    CharSequence a("ab");
    CharSequence b("cd");
    std::ostringstream stream;

    stream << a << "-" << b;

    EXPECT_EQ("ab-cd", stream.str());
}

// =====================================================================
// add_last
// =====================================================================

TEST(CharSequenceAddLast, AddsToEmptySequence) {
    CharSequence sequence;

    sequence.add_last('a');

    EXPECT_EQ(1, sequence.length());
    EXPECT_FALSE(sequence.empty());
    EXPECT_EQ('a', sequence.at(0));
}

TEST(CharSequenceAddLast, AppendsAfterExistingContent) {
    CharSequence sequence("ab");

    sequence.add_last('c');

    EXPECT_EQ("abc", to_std_string(sequence));
    EXPECT_EQ(3, sequence.length());
}

TEST(CharSequenceAddLast, CanBeRepeated) {
    CharSequence sequence;

    sequence.add_last('x');
    sequence.add_last('y');
    sequence.add_last('z');

    EXPECT_EQ("xyz", to_std_string(sequence));
}

TEST(CharSequenceAddLast, CanFillUpToMaxLength) {
    CharSequence sequence;

    for (int i = 0; i < CharSequence::MAX_LENGTH; ++i) {
        sequence.add_last(static_cast<char>('a' + i));
    }

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("abcdefghijklmno", to_std_string(sequence));
}

// 가정: 가득 찬 상태에서 add_last를 호출하면 무시된다. (구현이 다르면 이 테스트를 조정하세요.)
TEST(CharSequenceAddLast, IgnoredWhenFull) {
    CharSequence sequence("123456789012345");

    sequence.add_last('x');

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence));
}

TEST(CharSequenceAddLast, DoesNotAffectCopiedSequence) {
    CharSequence original("ab");
    CharSequence copied(original);

    copied.add_last('c');

    EXPECT_EQ("ab", to_std_string(original));
    EXPECT_EQ("abc", to_std_string(copied));
}

// =====================================================================
// 다형성 (Sequence<char> 인터페이스)
// =====================================================================

TEST(CharSequencePolymorphism, WorksThroughSequenceReference) {
    CharSequence sequence("abc");
    Sequence<char> &base = sequence;

    EXPECT_EQ(3, base.length());
    EXPECT_FALSE(base.empty());
    EXPECT_EQ('b', base.at(1));
}

TEST(CharSequencePolymorphism, SetAndAddLastThroughSequenceReference) {
    CharSequence sequence("abc");
    Sequence<char> &base = sequence;

    EXPECT_TRUE(base.set('x', 0));
    base.add_last('d');

    EXPECT_EQ("xbcd", to_std_string(sequence));
}

TEST(CharSequencePolymorphism, EmptyThroughSequenceReference) {
    CharSequence sequence;
    const Sequence<char> &base = sequence;

    EXPECT_TRUE(base.empty());
    EXPECT_EQ(0, base.length());
}

TEST(CharSequencePolymorphism, DeleteThroughBasePointerIsSafe) {
    Sequence<char> *base = new CharSequence("heap");

    EXPECT_EQ(4, base->length());
    EXPECT_EQ('h', base->at(0));

    delete base;    // 소멸자가 virtual이어야 안전하다.
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

    EXPECT_EQ(CharSequence::MAX_LENGTH, probe.raw_length());
    EXPECT_EQ('\0', probe.raw()[CharSequence::MAX_LENGTH]);
}

TEST(CharSequenceInternals, AddLastKeepsNullTermination) {
    CharSequenceProbe probe("ab");

    probe.add_last('c');

    EXPECT_EQ('\0', probe.raw()[3]);
    EXPECT_EQ(3, probe.raw_length());
}

// 가정: flush()는 내용을 비워 길이를 0으로 만든다.
TEST(CharSequenceInternals, FlushMakesSequenceEmpty) {
    CharSequenceProbe probe("something");

    probe.flush();

    EXPECT_TRUE(probe.empty());
    EXPECT_EQ(0, probe.length());
    EXPECT_EQ('\0', probe.raw()[0]);
}

TEST(CharSequenceInternals, FlushedSequenceCanBeReused) {
    CharSequenceProbe probe("old");

    probe.flush();
    probe.add_last('n');

    EXPECT_EQ("n", to_std_string(probe));
}

// 가정: 유효한 인덱스는 0 <= index < length 이다.
TEST(CharSequenceInternals, IndexBoundAcceptsValidIndexes) {
    CharSequenceProbe probe("abc");

    EXPECT_TRUE(probe.is_correct_index_bound(0));
    EXPECT_TRUE(probe.is_correct_index_bound(1));
    EXPECT_TRUE(probe.is_correct_index_bound(2));
}

TEST(CharSequenceInternals, IndexBoundRejectsInvalidIndexes) {
    CharSequenceProbe probe("abc");

    EXPECT_FALSE(probe.is_correct_index_bound(-1));
    EXPECT_FALSE(probe.is_correct_index_bound(3));
    EXPECT_FALSE(probe.is_correct_index_bound(CharSequence::MAX_LENGTH));
}

TEST(CharSequenceInternals, IndexBoundRejectsEverythingWhenEmpty) {
    CharSequenceProbe probe;

    EXPECT_FALSE(probe.is_correct_index_bound(0));
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
    // 해시 값이 큰 문자들로 구성된 경우에도 정확히 비교해야 한다.
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
    CharSequence sequence("hello");

    sequence.find("ll", 0);
    sequence.find(CharSequence("ll"), 0);

    EXPECT_EQ("hello", to_std_string(sequence));
    EXPECT_EQ(5, sequence.length());
}

TEST(CharSequenceFind, WorksAfterModification) {
    CharSequence sequence("abc");

    sequence.set('x', 1);
    sequence.add_last('d');

    EXPECT_EQ(1, sequence.find("x", 0));
    EXPECT_EQ(-1, sequence.find("b", 0));
    EXPECT_EQ(2, sequence.find("cd", 0));
}

TEST(CharSequenceFind, WorksOnAppendedResult) {
    CharSequence base("abc");

    CharSequence result = base.append_truncated("def");

    EXPECT_EQ(2, result.find("cd", 0));
    EXPECT_EQ(5, result.find("f", 0));
}

TEST(CharSequenceFind, CharArrayAndCharSequenceOverloadsAgree) {
    const char *texts[] = {"", "a", "abc", "abcabc", "aaaa", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "b", "abc", "aa", "ll", "o w", "345", "zzz", "123456789012345"};

    for (const char *text: texts) {
        CharSequence sequence(text);
        for (const char *pattern: patterns) {
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(sequence.find(pattern, start), sequence.find(CharSequence(pattern), start))
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
