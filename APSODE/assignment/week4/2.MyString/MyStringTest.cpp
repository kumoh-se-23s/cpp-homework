#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "MyString.h"

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

TEST(MyStringConstructor, DefaultCreatesEmptyString) {
    MyString string;

    EXPECT_EQ(0, string.length());
    EXPECT_TRUE(string.empty());
}

TEST(MyStringConstructor, FromCharArray) {
    MyString string("hello");

    EXPECT_EQ(5, string.length());
    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringConstructor, FromLongCharArrayIsTruncated) {
    MyString string("1234567890123456789");

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringConstructor, FromCharSequence) {
    CharSequence sequence("sequence");
    MyString string(sequence);

    EXPECT_EQ(8, string.length());
    EXPECT_TRUE(string.equals(sequence));
}

TEST(MyStringConstructor, CopyCopiesContent) {
    MyString original("copy");
    MyString copied(original);

    EXPECT_TRUE(original == copied);
    EXPECT_EQ("copy", to_std_string(copied));
}

TEST(MyStringConstructor, CopyIsIndependentFromOriginal) {
    MyString original("abc");
    MyString copied(original);

    copied.set('z', 0);

    EXPECT_EQ('a', original.at(0));
    EXPECT_EQ('z', copied.at(0));
}

// =====================================================================
// 상속받은 기능 (CharSequence)
// =====================================================================

TEST(MyStringInherited, AtAndSetWork) {
    MyString string("abc");

    EXPECT_EQ('b', string.at(1));
    EXPECT_TRUE(string.set('x', 1));
    EXPECT_EQ('x', string.at(1));
    EXPECT_FALSE(string.set('x', 3));
}

TEST(MyStringInherited, CanBeUsedThroughCharSequenceReference) {
    MyString string("base");
    const CharSequence &reference = string;

    EXPECT_EQ(4, reference.length());
    EXPECT_EQ('b', reference.at(0));
}

TEST(MyStringInherited, AppendTruncatedWorks) {
    MyString string("1234567890");

    CharSequence result = string.append_truncated("abcdefghij");

    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

// =====================================================================
// operator==
// =====================================================================

TEST(MyStringEquality, TrueForSameContent) {
    MyString a("hello");
    MyString b("hello");

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == a);
}

TEST(MyStringEquality, FalseForDifferentContent) {
    MyString a("hello");
    MyString b("world");

    EXPECT_FALSE(a == b);
}

TEST(MyStringEquality, FalseForDifferentLength) {
    MyString a("hello");
    MyString b("hello!");

    EXPECT_FALSE(a == b);
}

TEST(MyStringEquality, TrueForSelf) {
    MyString a("self");

    EXPECT_TRUE(a == a);
}

TEST(MyStringEquality, TrueForTwoEmptyStrings) {
    MyString a;
    MyString b;

    EXPECT_TRUE(a == b);
}

// =====================================================================
// operator=
// =====================================================================

TEST(MyStringAssignment, CopiesContent) {
    MyString a("source");
    MyString b("target");

    b = a;

    EXPECT_TRUE(a == b);
    EXPECT_EQ("source", to_std_string(b));
}

TEST(MyStringAssignment, FromEmptyMakesTargetEmpty) {
    MyString empty;
    MyString target("something");

    target = empty;

    EXPECT_TRUE(target.empty());
}

TEST(MyStringAssignment, ToSelfKeepsContent) {
    MyString a("self");
    MyString &reference = a;

    a = reference;

    EXPECT_EQ("self", to_std_string(a));
}

TEST(MyStringAssignment, ReturnsReferenceForChaining) {
    MyString a("chain");
    MyString b;
    MyString c;

    c = b = a;

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == c);
}

TEST(MyStringAssignment, IsIndependentFromSource) {
    MyString a("abc");
    MyString b;

    b = a;
    b.set('z', 0);

    EXPECT_EQ('a', a.at(0));
    EXPECT_EQ('z', b.at(0));
}

// =====================================================================
// operator+
// =====================================================================

TEST(MyStringPlus, WithMyString) {
    MyString a("abc");
    MyString b("def");

    MyString result = a + b;

    EXPECT_EQ("abcdef", to_std_string(result));
    EXPECT_EQ(6, result.length());
}

TEST(MyStringPlus, WithCharSequence) {
    MyString a("abc");
    CharSequence b("def");

    MyString result = a + b;

    EXPECT_EQ("abcdef", to_std_string(result));
}

TEST(MyStringPlus, WithCharArray) {
    MyString a("abc");

    MyString result = a + "def";

    EXPECT_EQ("abcdef", to_std_string(result));
}

TEST(MyStringPlus, DoesNotModifyOperands) {
    MyString a("abc");
    MyString b("def");

    MyString result = a + b;

    EXPECT_EQ("abc", to_std_string(a));
    EXPECT_EQ("def", to_std_string(b));
}

TEST(MyStringPlus, WithEmptyReturnsSameContent) {
    MyString a("abc");
    MyString empty;

    EXPECT_TRUE((a + empty) == a);
    EXPECT_TRUE((empty + a) == a);
}

TEST(MyStringPlus, TruncatesAtMaxLength) {
    MyString a("1234567890");
    MyString b("abcdefghij");

    MyString result = a + b;

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(MyStringPlus, CharArrayTruncatesAtMaxLength) {
    MyString a("1234567890");

    MyString result = a + "abcdefghij";

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(MyStringPlus, CanBeChained) {
    MyString a("a");
    MyString b("b");

    MyString result = a + b + "c";

    EXPECT_EQ("abc", to_std_string(result));
}

// =====================================================================
// operator>>
// =====================================================================

TEST(MyStringInputOperator, ReadsWord) {
    std::istringstream input("hello");
    MyString string;

    input >> string;

    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringInputOperator, StopsAtWhitespace) {
    std::istringstream input("hello world");
    MyString first;
    MyString second;

    input >> first >> second;

    EXPECT_EQ("hello", to_std_string(first));
    EXPECT_EQ("world", to_std_string(second));
}

TEST(MyStringInputOperator, TruncatesLongInput) {
    std::istringstream input("1234567890123456789");
    MyString string;

    input >> string;

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringInputOperator, OverwritesExistingContent) {
    std::istringstream input("new");
    MyString string("previous");

    input >> string;

    EXPECT_EQ("new", to_std_string(string));
}

TEST(MyStringInputOperator, ReturnsStreamForChaining) {
    std::istringstream input("a b");
    MyString first;
    MyString second;

    std::istream &returned = (input >> first);
    returned >> second;

    EXPECT_EQ("a", to_std_string(first));
    EXPECT_EQ("b", to_std_string(second));
}

// =====================================================================
// 길이 경계 (최대 15글자)
// =====================================================================

TEST(MyStringBoundary, ExactlyMaxLengthIsKept) {
    MyString string("123456789012345");

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringBoundary, OneOverMaxLengthIsTruncated) {
    MyString string("1234567890123456");

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringBoundary, EmptyCharArrayCreatesEmptyString) {
    MyString string("");

    EXPECT_EQ(0, string.length());
    EXPECT_TRUE(string.empty());
}

TEST(MyStringBoundary, NonEmptyStringIsNotEmpty) {
    MyString string("a");

    EXPECT_EQ(1, string.length());
    EXPECT_FALSE(string.empty());
}

TEST(MyStringBoundary, LengthIsContentLengthNotCapacity) {
    // 과제 조건: 전체 용량이 아니라 문자열 자체의 길이를 반환
    MyString string("abc");

    EXPECT_EQ(3, string.length());
}

// =====================================================================
// operator!=
// =====================================================================

TEST(MyStringInequality, FalseForSameContent) {
    MyString a("hello");
    MyString b("hello");

    EXPECT_FALSE(a != b);
}

TEST(MyStringInequality, TrueForDifferentContent) {
    MyString a("hello");
    MyString b("world");

    EXPECT_TRUE(a != b);
    EXPECT_TRUE(b != a);
}

TEST(MyStringInequality, TrueForDifferentLength) {
    MyString a("hello");
    MyString b("hello!");

    EXPECT_TRUE(a != b);
}

TEST(MyStringInequality, TrueForEmptyAndNonEmpty) {
    MyString empty;
    MyString text("a");

    EXPECT_TRUE(empty != text);
    EXPECT_TRUE(text != empty);
}

TEST(MyStringInequality, FalseForTwoEmptyStrings) {
    MyString a;
    MyString b;

    EXPECT_FALSE(a != b);
}

TEST(MyStringInequality, AlwaysOppositeOfEquality) {
    const char *samples[] = {"", "a", "ab", "abc", "hello", "123456789012345"};

    for (const char *left_text: samples) {
        for (const char *right_text: samples) {
            MyString left(left_text);
            MyString right(right_text);

            EXPECT_EQ(!(left == right), left != right)
                << "left: " << left_text << ", right: " << right_text;
        }
    }
}

TEST(MyStringInequality, DetectsDifferenceAtLastCharacter) {
    MyString a("abcde");
    MyString b("abcdf");

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

// =====================================================================
// operator<< (출력)
// =====================================================================

TEST(MyStringOutputOperator, PrintsContent) {
    MyString string("hello");
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ("hello", stream.str());
}

TEST(MyStringOutputOperator, PrintsNothingForEmptyString) {
    MyString string;
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ("", stream.str());
}

TEST(MyStringOutputOperator, DoesNotAppendNewline) {
    MyString string("abc");
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ(std::string::npos, stream.str().find('\n'));
}

TEST(MyStringOutputOperator, CanBeChainedWithOtherOutput) {
    MyString a("abc");
    MyString b("def");
    std::ostringstream stream;

    stream << a << "," << b << "!";

    EXPECT_EQ("abc,def!", stream.str());
}

TEST(MyStringOutputOperator, PrintsConcatenatedResult) {
    MyString a("abc");
    std::ostringstream stream;

    stream << (a + "def");

    EXPECT_EQ("abcdef", stream.str());
}

TEST(MyStringOutputOperator, PrintsTruncatedContent) {
    MyString string("1234567890123456789");
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ("123456789012345", stream.str());
}

// =====================================================================
// operator>> 추가 규칙
// =====================================================================

TEST(MyStringInputOperator, SkipsLeadingWhitespace) {
    std::istringstream input("   \n\t hello");
    MyString string;

    input >> string;

    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringInputOperator, ReadsExactlyMaxLengthFully) {
    std::istringstream input("123456789012345");
    MyString string;

    input >> string;

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

// 과제 조건: 범위를 넘는 글자는 버퍼에 그대로 둔다.
// (넘친 글자가 16번째부터 스트림에 남는다는 해석에 의존합니다.)
TEST(MyStringInputOperator, CharactersBeyondLimitStayInStream) {
    std::istringstream input("1234567890123456789");
    MyString first;
    MyString second;

    input >> first >> second;

    EXPECT_EQ("123456789012345", to_std_string(first));
    EXPECT_EQ("6789", to_std_string(second));
}

TEST(MyStringInputOperator, ReadsThreeWords) {
    std::istringstream input("one two three");
    MyString a;
    MyString b;
    MyString c;

    input >> a >> b >> c;

    EXPECT_EQ("one", to_std_string(a));
    EXPECT_EQ("two", to_std_string(b));
    EXPECT_EQ("three", to_std_string(c));
}

TEST(MyStringInputOperator, StopsAtNewline) {
    std::istringstream input("first\nsecond");
    MyString a;
    MyString b;

    input >> a >> b;

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ("second", to_std_string(b));
}

// =====================================================================
// get_line
// =====================================================================

TEST(MyStringGetLine, ReadsWholeLineIncludingSpaces) {
    std::istringstream input("hello world\n");
    MyString string;

    get_line(input, string);

    EXPECT_EQ("hello world", to_std_string(string));
}

TEST(MyStringGetLine, DoesNotIncludeDelimiter) {
    std::istringstream input("abc\n");
    MyString string;

    get_line(input, string);

    EXPECT_EQ(3, string.length());
    EXPECT_EQ(std::string::npos, to_std_string(string).find('\n'));
}

TEST(MyStringGetLine, ConsumesDelimiterSoNextLineIsRead) {
    std::istringstream input("first\nsecond\n");
    MyString a;
    MyString b;

    get_line(input, a);
    get_line(input, b);

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ("second", to_std_string(b));
}

TEST(MyStringGetLine, KeepsLeadingWhitespace) {
    std::istringstream input("  hi\n");
    MyString string;

    get_line(input, string);

    EXPECT_EQ("  hi", to_std_string(string));
}

TEST(MyStringGetLine, EmptyLineGivesEmptyString) {
    std::istringstream input("\nnext\n");
    MyString a;
    MyString b;

    get_line(input, a);
    get_line(input, b);

    EXPECT_TRUE(a.empty());
    EXPECT_EQ("next", to_std_string(b));
}

TEST(MyStringGetLine, LastLineWithoutDelimiterIsRead) {
    std::istringstream input("only line");
    MyString string;

    get_line(input, string);

    EXPECT_EQ("only line", to_std_string(string));
}

TEST(MyStringGetLine, CustomDelimiter) {
    std::istringstream input("a,b,c");
    MyString a;
    MyString b;
    MyString c;

    get_line(input, a, ',');
    get_line(input, b, ',');
    get_line(input, c, ',');

    EXPECT_EQ("a", to_std_string(a));
    EXPECT_EQ("b", to_std_string(b));
    EXPECT_EQ("c", to_std_string(c));
}

TEST(MyStringGetLine, NewlineIsNotDelimiterWhenCustomDelimiterGiven) {
    std::istringstream input("ab\ncd,ef");
    MyString string;

    get_line(input, string, ',');

    EXPECT_EQ("ab\ncd", to_std_string(string));
}

TEST(MyStringGetLine, ExactlyMaxLengthIsKept) {
    std::istringstream input("123456789012345\nnext\n");
    MyString a;
    MyString b;

    get_line(input, a);
    get_line(input, b);

    EXPECT_EQ("123456789012345", to_std_string(a));
    EXPECT_EQ("next", to_std_string(b));
}

// 과제 조건: delim까지 모든 글자를 읽되 16번째 글자부터는 버린다.
TEST(MyStringGetLine, DiscardsCharactersBeyondLimit) {
    std::istringstream input("1234567890123456789\nnext\n");
    MyString a;
    MyString b;

    get_line(input, a);
    get_line(input, b);

    EXPECT_EQ(CharSequence::MAX_LENGTH, a.length());
    EXPECT_EQ("123456789012345", to_std_string(a));
    EXPECT_EQ("next", to_std_string(b));
}

TEST(MyStringGetLine, OverwritesExistingContent) {
    std::istringstream input("new\n");
    MyString string("previous");

    get_line(input, string);

    EXPECT_EQ("new", to_std_string(string));
}

TEST(MyStringGetLine, ReturnsStreamForChaining) {
    std::istringstream input("a\nb\n");
    MyString first;
    MyString second;

    std::istream &returned = get_line(input, first);
    get_line(returned, second);

    EXPECT_EQ("a", to_std_string(first));
    EXPECT_EQ("b", to_std_string(second));
}

TEST(MyStringGetLine, MixesWithExtractionOperator) {
    // >>로 단어를 읽은 뒤 같은 스트림에서 get_line이 다음 줄을 읽을 수 있어야 함
    std::istringstream input("word\nrest of line\n");
    MyString word;
    MyString line;

    input >> word;
    input.get();    // >>가 남긴 개행 소비
    get_line(input, line);

    EXPECT_EQ("word", to_std_string(word));
    EXPECT_EQ("rest of line", to_std_string(line));
}

// =====================================================================
// substr
// =====================================================================

TEST(MyStringSubstr, FromStart) {
    MyString string("hello world");

    EXPECT_EQ("hello", to_std_string(string.substr(0, 5)));
}

TEST(MyStringSubstr, FromMiddle) {
    MyString string("hello world");

    EXPECT_EQ("world", to_std_string(string.substr(6, 5)));
}

TEST(MyStringSubstr, SingleCharacter) {
    MyString string("abc");

    EXPECT_EQ("b", to_std_string(string.substr(1, 1)));
}

TEST(MyStringSubstr, WholeString) {
    MyString string("abcde");

    EXPECT_EQ("abcde", to_std_string(string.substr(0, 5)));
}

TEST(MyStringSubstr, LastCharacter) {
    MyString string("abcde");

    EXPECT_EQ("e", to_std_string(string.substr(4, 1)));
}

TEST(MyStringSubstr, ZeroLengthGivesEmpty) {
    MyString string("hello");

    EXPECT_TRUE(string.substr(2, 0).empty());
}

// 과제 조건: len이 맞지 않으면 std::string::substr처럼 끝까지만 반환
TEST(MyStringSubstr, LengthBeyondEndStopsAtEnd) {
    MyString string("hello world");

    EXPECT_EQ("world", to_std_string(string.substr(6, 100)));
}

TEST(MyStringSubstr, LengthOneBeyondEndStopsAtEnd) {
    MyString string("abc");

    EXPECT_EQ("bc", to_std_string(string.substr(1, 3)));
}

// 과제 조건: pos가 음수이면 0으로 취급
TEST(MyStringSubstr, NegativePositionIsTreatedAsZero) {
    MyString string("hello");

    EXPECT_EQ("he", to_std_string(string.substr(-3, 2)));
}

TEST(MyStringSubstr, VeryNegativePositionIsTreatedAsZero) {
    MyString string("hello");

    EXPECT_EQ("hello", to_std_string(string.substr(-100, 5)));
}

// 과제 조건: 문자열 범위 밖이면 빈 문자열
TEST(MyStringSubstr, PositionBeyondEndGivesEmpty) {
    MyString string("hello");

    EXPECT_TRUE(string.substr(10, 2).empty());
}

TEST(MyStringSubstr, PositionAtEndGivesEmpty) {
    // std::string::substr(size(), n)도 빈 문자열을 반환
    MyString string("hello");

    EXPECT_TRUE(string.substr(5, 3).empty());
}

TEST(MyStringSubstr, EmptySourceGivesEmpty) {
    MyString empty;

    EXPECT_TRUE(empty.substr(0, 3).empty());
}

TEST(MyStringSubstr, DoesNotModifyOriginal) {
    MyString string("hello");

    MyString result = string.substr(1, 3);
    (void) result;

    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringSubstr, ResultIsIndependentFromOriginal) {
    MyString string("hello");

    MyString result = string.substr(0, 3);
    result.set('z', 0);

    EXPECT_EQ('h', string.at(0));
    EXPECT_EQ('z', result.at(0));
}

TEST(MyStringSubstr, ResultLengthMatchesContent) {
    MyString string("hello world");

    EXPECT_EQ(5, string.substr(0, 5).length());
    EXPECT_EQ(5, string.substr(6, 100).length());
    EXPECT_EQ(0, string.substr(20, 3).length());
}

TEST(MyStringSubstr, FromMaxLengthString) {
    MyString string("123456789012345");

    EXPECT_EQ("12345", to_std_string(string.substr(10, 5)));
    EXPECT_EQ("5", to_std_string(string.substr(14, 1)));
}

TEST(MyStringSubstr, MatchesStdStringForManyRanges) {
    const std::string text = "hello world";
    MyString string(text.c_str());

    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
        for (int len = 0; len <= 15; ++len) {
            EXPECT_EQ(text.substr(pos, len), to_std_string(string.substr(pos, len)))
                << "pos: " << pos << ", len: " << len;
        }
    }
}

TEST(MyStringSubstr, CanBeConcatenated) {
    MyString string("hello world");

    MyString result = string.substr(0, 5) + string.substr(5, 6);

    EXPECT_EQ("hello world", to_std_string(result));
}

// =====================================================================
// operator+ 추가 경계
// =====================================================================

TEST(MyStringPlus, ExactlyMaxLengthIsNotTruncated) {
    MyString a("12345678");
    MyString b("1234567");

    MyString result = a + b;

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("123456781234567", to_std_string(result));
}

TEST(MyStringPlus, OneOverMaxLengthIsTruncated) {
    MyString a("12345678");
    MyString b("12345678");

    MyString result = a + b;

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("123456781234567", to_std_string(result));
}

TEST(MyStringPlus, SelfConcatenation) {
    MyString a("ab");

    MyString result = a + a;

    EXPECT_EQ("abab", to_std_string(result));
    EXPECT_EQ("ab", to_std_string(a));
}

TEST(MyStringPlus, EmptyCharArray) {
    MyString a("abc");

    MyString result = a + "";

    EXPECT_EQ("abc", to_std_string(result));
}

TEST(MyStringPlus, ResultIsIndependentFromOperands) {
    MyString a("abc");
    MyString b("def");

    MyString result = a + b;
    result.set('z', 0);

    EXPECT_EQ('a', a.at(0));
    EXPECT_EQ('z', result.at(0));
}

TEST(MyStringPlus, AssignedResultMatchesDirectResult) {
    MyString a("abc");
    MyString b("def");
    MyString assigned;

    assigned = a + b;

    EXPECT_TRUE(assigned == (a + b));
}
