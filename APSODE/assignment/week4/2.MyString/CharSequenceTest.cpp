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

TEST(CharSequenceAddLast, OnEmptySequence) {
    CharSequence sequence;

    sequence.add_last('a');

    EXPECT_EQ(1, sequence.length());
    EXPECT_FALSE(sequence.empty());
    EXPECT_EQ('a', sequence.at(0));
    EXPECT_EQ("a", to_std_string(sequence));
}

TEST(CharSequenceAddLast, AppendsAtEnd) {
    CharSequence sequence("ab");

    sequence.add_last('c');

    EXPECT_EQ(3, sequence.length());
    EXPECT_EQ("abc", to_std_string(sequence));
    EXPECT_EQ('c', sequence.at(2));
}

TEST(CharSequenceAddLast, KeepsExistingCharacters) {
    CharSequence sequence("hello");

    sequence.add_last('!');

    EXPECT_EQ('h', sequence.at(0));
    EXPECT_EQ('o', sequence.at(4));
    EXPECT_EQ('!', sequence.at(5));
}

TEST(CharSequenceAddLast, CanBeCalledRepeatedly) {
    CharSequence sequence;

    sequence.add_last('a');
    sequence.add_last('b');
    sequence.add_last('c');

    EXPECT_EQ(3, sequence.length());
    EXPECT_EQ("abc", to_std_string(sequence));
}

TEST(CharSequenceAddLast, SameCharacterRepeatedly) {
    CharSequence sequence;

    for (int i = 0; i < 5; ++i) {
        sequence.add_last('z');
    }

    EXPECT_EQ(5, sequence.length());
    EXPECT_EQ("zzzzz", to_std_string(sequence));
}

TEST(CharSequenceAddLast, AcceptsSpacesDigitsAndSymbols) {
    CharSequence sequence;

    sequence.add_last(' ');
    sequence.add_last('7');
    sequence.add_last('#');

    EXPECT_EQ(3, sequence.length());
    EXPECT_EQ(" 7#", to_std_string(sequence));
}

TEST(CharSequenceAddLast, ResultEqualsSequenceBuiltFromLiteral) {
    CharSequence built;
    built.add_last('a');
    built.add_last('b');
    built.add_last('c');

    CharSequence literal("abc");

    EXPECT_TRUE(built.equals(literal));
    EXPECT_TRUE(literal.equals(built));
}

TEST(CharSequenceAddLast, FillsUpToMaxLength) {
    CharSequence sequence;

    for (int i = 0; i < CharSequence::MAX_LENGTH; ++i) {
        sequence.add_last(static_cast<char>('a' + i));
    }

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("abcdefghijklmno", to_std_string(sequence));
}

// 가득 찬 상태에서 추가하면 무시된다는 가정 (append_truncated와 같은 정책)
TEST(CharSequenceAddLast, IgnoredWhenFull) {
    CharSequence sequence("123456789012345");

    sequence.add_last('x');

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence));
}

TEST(CharSequenceAddLast, RepeatedWhenFullStillKeepsContent) {
    CharSequence sequence("123456789012345");

    for (int i = 0; i < 5; ++i) {
        sequence.add_last('x');
    }

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence));
}

TEST(CharSequenceAddLast, OneBelowMaxAcceptsExactlyOne) {
    CharSequence sequence("12345678901234");

    sequence.add_last('a');
    sequence.add_last('b');

    EXPECT_EQ(CharSequence::MAX_LENGTH, sequence.length());
    EXPECT_EQ("123456789012345", to_std_string(sequence).substr(0, 14) + "5");
    EXPECT_EQ('a', sequence.at(14));
}

TEST(CharSequenceAddLast, DoesNotAffectCopy) {
    CharSequence original("abc");
    CharSequence copied(original);

    copied.add_last('d');

    EXPECT_EQ("abc", to_std_string(original));
    EXPECT_EQ("abcd", to_std_string(copied));
}

TEST(CharSequenceAddLast, NewCharacterCanBeChangedWithSet) {
    CharSequence sequence("ab");

    sequence.add_last('c');

    EXPECT_TRUE(sequence.set('z', 2));
    EXPECT_EQ("abz", to_std_string(sequence));
}

TEST(CharSequenceAddLast, IndexOfNewCharacterBecomesValidForSet) {
    CharSequence sequence("ab");

    EXPECT_FALSE(sequence.set('x', 2));
    sequence.add_last('c');
    EXPECT_TRUE(sequence.set('x', 2));
}

TEST(CharSequenceAddLast, WorksAfterCopyFromOther) {
    CharSequence sequence("old");

    sequence.copy_from_other("new");
    sequence.add_last('!');

    EXPECT_EQ("new!", to_std_string(sequence));
}

TEST(CharSequenceAddLast, CanBeUsedWithAppendTruncated) {
    CharSequence sequence("ab");

    sequence.add_last('c');
    CharSequence result = sequence.append_truncated("de");

    EXPECT_EQ("abcde", to_std_string(result));
    EXPECT_EQ("abc", to_std_string(sequence));
}

// =====================================================================
// 부모 클래스(Sequence<char>)를 통한 호출
// =====================================================================

TEST(CharSequencePolymorphism, LengthAndAtThroughBaseReference) {
    CharSequence sequence("abc");
    Sequence<char> &base = sequence;

    EXPECT_EQ(3, base.length());
    EXPECT_EQ('a', base.at(0));
    EXPECT_EQ('c', base.at(2));
}

TEST(CharSequencePolymorphism, EmptyThroughBaseReference) {
    CharSequence empty;
    CharSequence filled("a");

    Sequence<char> &empty_base = empty;
    Sequence<char> &filled_base = filled;

    EXPECT_TRUE(empty_base.empty());
    EXPECT_FALSE(filled_base.empty());
}

TEST(CharSequencePolymorphism, SetThroughBaseReference) {
    CharSequence sequence("abc");
    Sequence<char> &base = sequence;

    EXPECT_TRUE(base.set('x', 1));
    EXPECT_FALSE(base.set('x', 3));
    EXPECT_EQ("axc", to_std_string(sequence));
}

TEST(CharSequencePolymorphism, AddLastThroughBaseReference) {
    CharSequence sequence("ab");
    Sequence<char> &base = sequence;

    base.add_last('c');

    EXPECT_EQ(3, base.length());
    EXPECT_EQ("abc", to_std_string(sequence));
}

TEST(CharSequencePolymorphism, DeleteThroughBasePointer) {
    // 부모 포인터로 삭제해도 소멸자가 정상 동작해야 함 (가상 소멸자)
    Sequence<char> *pointer = new CharSequence("abc");

    EXPECT_EQ(3, pointer->length());

    delete pointer;
}

TEST(CharSequencePolymorphism, MutationThroughBaseIsVisibleThroughDerived) {
    CharSequence sequence("abc");
    Sequence<char> &base = sequence;

    base.set('z', 0);
    base.add_last('d');

    EXPECT_EQ('z', sequence.at(0));
    EXPECT_EQ(4, sequence.length());
}

// =====================================================================
// protected 멤버 (테스트용 하위 클래스로 접근)
// =====================================================================

namespace {
    class CharSequenceProbe : public CharSequence {
        public:
            using CharSequence::CharSequence;
            using CharSequence::flush;
            using CharSequence::is_correct_index_bound;

            const char *raw() const { return container; }
            int raw_length() const { return length_; }
    };
}

TEST(CharSequenceInternals, LengthFieldMatchesLength) {
    CharSequenceProbe probe("hello");

    EXPECT_EQ(probe.length(), probe.raw_length());
}

TEST(CharSequenceInternals, ContainerIsNullTerminatedAfterConstruction) {
    CharSequenceProbe probe("abc");

    EXPECT_EQ('\0', probe.raw()[3]);
}

TEST(CharSequenceInternals, ContainerIsNullTerminatedAtMaxLength) {
    CharSequenceProbe probe("123456789012345");

    EXPECT_EQ('\0', probe.raw()[CharSequence::MAX_LENGTH]);
}

TEST(CharSequenceInternals, ContainerIsNullTerminatedAfterTruncation) {
    CharSequenceProbe probe("1234567890123456789");

    EXPECT_EQ('\0', probe.raw()[CharSequence::MAX_LENGTH]);
    EXPECT_EQ(CharSequence::MAX_LENGTH, probe.raw_length());
}

TEST(CharSequenceInternals, ContainerIsNullTerminatedAfterAddLast) {
    CharSequenceProbe probe("ab");

    probe.add_last('c');

    EXPECT_EQ('\0', probe.raw()[3]);
    EXPECT_EQ(3, probe.raw_length());
}

TEST(CharSequenceInternals, ContainerIsNullTerminatedAfterCopyFromOther) {
    CharSequenceProbe probe("a long previous text");

    probe.copy_from_other("hi");

    EXPECT_EQ('\0', probe.raw()[2]);
    EXPECT_EQ(2, probe.raw_length());
    EXPECT_EQ("hi", to_std_string(probe));
}

// flush는 내용을 모두 비운다는 가정 (이름 기준)
TEST(CharSequenceInternals, FlushMakesSequenceEmpty) {
    CharSequenceProbe probe("hello");

    probe.flush();

    EXPECT_TRUE(probe.empty());
    EXPECT_EQ(0, probe.length());
    EXPECT_EQ(0, probe.raw_length());
    EXPECT_EQ("", to_std_string(probe));
}

TEST(CharSequenceInternals, FlushClearsContainerHead) {
    CharSequenceProbe probe("hello");

    probe.flush();

    EXPECT_EQ('\0', probe.raw()[0]);
}

TEST(CharSequenceInternals, FlushOnEmptyKeepsEmpty) {
    CharSequenceProbe probe;

    probe.flush();

    EXPECT_TRUE(probe.empty());
}

TEST(CharSequenceInternals, SequenceIsReusableAfterFlush) {
    CharSequenceProbe probe("old content");

    probe.flush();
    probe.add_last('n');
    probe.add_last('w');

    EXPECT_EQ("nw", to_std_string(probe));
    EXPECT_EQ(2, probe.length());
}

TEST(CharSequenceInternals, FlushDoesNotAffectCopy) {
    CharSequenceProbe probe("keep");
    CharSequence copied(probe);

    probe.flush();

    EXPECT_EQ("keep", to_std_string(copied));
}

// is_correct_index_bound는 0 이상 length 미만이 유효하다는 가정
// (기존 set 테스트에서 index == length일 때 false인 점을 근거로 함)
TEST(CharSequenceInternals, IndexBoundAcceptsValidIndices) {
    CharSequenceProbe probe("abc");

    EXPECT_TRUE(probe.is_correct_index_bound(0));
    EXPECT_TRUE(probe.is_correct_index_bound(1));
    EXPECT_TRUE(probe.is_correct_index_bound(2));
}

TEST(CharSequenceInternals, IndexBoundRejectsNegative) {
    CharSequenceProbe probe("abc");

    EXPECT_FALSE(probe.is_correct_index_bound(-1));
}

TEST(CharSequenceInternals, IndexBoundRejectsLengthAndBeyond) {
    CharSequenceProbe probe("abc");

    EXPECT_FALSE(probe.is_correct_index_bound(3));
    EXPECT_FALSE(probe.is_correct_index_bound(100));
}

TEST(CharSequenceInternals, IndexBoundOnEmptyRejectsEverything) {
    CharSequenceProbe probe;

    EXPECT_FALSE(probe.is_correct_index_bound(0));
    EXPECT_FALSE(probe.is_correct_index_bound(-1));
}

TEST(CharSequenceInternals, IndexBoundAtMaxLength) {
    CharSequenceProbe probe("123456789012345");

    EXPECT_TRUE(probe.is_correct_index_bound(CharSequence::MAX_LENGTH - 1));
    EXPECT_FALSE(probe.is_correct_index_bound(CharSequence::MAX_LENGTH));
}

TEST(CharSequenceInternals, IndexBoundFollowsLengthChanges) {
    CharSequenceProbe probe("ab");

    EXPECT_FALSE(probe.is_correct_index_bound(2));
    probe.add_last('c');
    EXPECT_TRUE(probe.is_correct_index_bound(2));
    probe.flush();
    EXPECT_FALSE(probe.is_correct_index_bound(0));
}