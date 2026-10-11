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
