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

    // MyString::find(const MyString&, int)를 편하게 호출하기 위한 도우미
    int find_in(const char *text, const char *pattern, int start) {
        MyString string(text);
        return string.find(MyString(pattern), start);
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

// =====================================================================
// find
// =====================================================================

TEST(MyStringFind, FindsAtStart) {
    EXPECT_EQ(0, find_in("hello world", "hello", 0));
}

TEST(MyStringFind, FindsInMiddle) {
    EXPECT_EQ(3, find_in("hello world", "lo w", 0));
}

TEST(MyStringFind, FindsAtEnd) {
    EXPECT_EQ(6, find_in("hello world", "world", 0));
}

TEST(MyStringFind, FindsSingleCharacter) {
    EXPECT_EQ(2, find_in("hello", "l", 0));
}

TEST(MyStringFind, FindsWholeString) {
    EXPECT_EQ(0, find_in("hello", "hello", 0));
}

TEST(MyStringFind, ReturnsMinusOneWhenNotFound) {
    EXPECT_EQ(-1, find_in("hello", "xyz", 0));
}

TEST(MyStringFind, ReturnsMinusOneWhenPatternIsLongerThanText) {
    EXPECT_EQ(-1, find_in("abc", "abcd", 0));
}

TEST(MyStringFind, ReturnsMinusOneOnEmptyText) {
    EXPECT_EQ(-1, find_in("", "a", 0));
}

TEST(MyStringFind, ReturnsFirstOccurrence) {
    EXPECT_EQ(0, find_in("abcabcabc", "abc", 0));
    EXPECT_EQ(1, find_in("xabcabc", "abc", 0));
}

TEST(MyStringFind, StartPositionSkipsEarlierMatch) {
    EXPECT_EQ(3, find_in("abcabcabc", "abc", 1));
    EXPECT_EQ(3, find_in("abcabcabc", "abc", 3));
    EXPECT_EQ(6, find_in("abcabcabc", "abc", 4));
}

TEST(MyStringFind, StartPositionAfterLastMatchReturnsMinusOne) {
    EXPECT_EQ(-1, find_in("abcabc", "abc", 4));
}

// 과제 조건: start가 음수이면 0으로 취급
TEST(MyStringFind, NegativeStartIsTreatedAsZero) {
    EXPECT_EQ(0, find_in("hello", "he", -1));
    EXPECT_EQ(0, find_in("hello", "he", -100));
    EXPECT_EQ(3, find_in("hello", "lo", -3));
}

// 과제 조건: start가 문자열 길이를 넘으면 -1
TEST(MyStringFind, StartBeyondLengthReturnsMinusOne) {
    EXPECT_EQ(-1, find_in("hello", "h", 6));
    EXPECT_EQ(-1, find_in("hello", "h", 100));
}

TEST(MyStringFind, StartAtLengthReturnsMinusOne) {
    EXPECT_EQ(-1, find_in("hello", "o", 5));
}

TEST(MyStringFind, StartAtLastCharacter) {
    EXPECT_EQ(4, find_in("hello", "o", 4));
}

TEST(MyStringFind, RepeatedCharacters) {
    EXPECT_EQ(0, find_in("aaaaaaaaaa", "aaa", 0));
    EXPECT_EQ(7, find_in("aaaaaaaaaa", "aaa", 7));
    EXPECT_EQ(-1, find_in("aaaaaaaaaa", "aaa", 8));
}

TEST(MyStringFind, PartialMatchBeforeRealMatch) {
    EXPECT_EQ(3, find_in("aaaaab", "aab", 0));
    EXPECT_EQ(4, find_in("abababc", "abc", 0));
}

TEST(MyStringFind, SamePrefixDifferentTail) {
    EXPECT_EQ(3, find_in("abcabd", "abd", 0));
    EXPECT_EQ(-1, find_in("abcabd", "abe", 0));
}

TEST(MyStringFind, SameCharactersInDifferentOrderAreNotFound) {
    EXPECT_EQ(-1, find_in("abcdef", "cba", 0));
    EXPECT_EQ(-1, find_in("abcdef", "fed", 0));
}

TEST(MyStringFind, IsCaseSensitive) {
    EXPECT_EQ(-1, find_in("Hello", "hello", 0));
    EXPECT_EQ(0, find_in("Hello", "Hello", 0));
}

TEST(MyStringFind, FindsSpacesAndSymbols) {
    EXPECT_EQ(1, find_in("a b-c_d", " ", 0));
    EXPECT_EQ(3, find_in("a b-c_d", "-c_", 0));
}

TEST(MyStringFind, FindsDigits) {
    EXPECT_EQ(5, find_in("2026/10/12", "10", 0));
    EXPECT_EQ(-1, find_in("2026/10/12", "10", 6));
}

TEST(MyStringFind, HighAsciiCharactersDoNotCauseFalseMatch) {
    // 해시 값이 큰 문자들로 구성된 경우에도 정확히 비교해야 한다.
    EXPECT_EQ(0, find_in("~}|{zyxwvutsrqp", "~}|", 0));
    EXPECT_EQ(12, find_in("~}|{zyxwvutsrqp", "rqp", 0));
    EXPECT_EQ(-1, find_in("~}|{zyxwvutsrqp", "}~", 0));
}

TEST(MyStringFind, WorksOnMaxLengthString) {
    EXPECT_EQ(4, find_in("123456789012345", "5", 0));
    EXPECT_EQ(14, find_in("123456789012345", "5", 5));
    EXPECT_EQ(0, find_in("123456789012345", "12345", 0));
    EXPECT_EQ(10, find_in("123456789012345", "12345", 1));
}

TEST(MyStringFind, WorksOnTruncatedString) {
    // 15글자로 잘린 뒤의 내용만 검색 대상이 된다.
    EXPECT_EQ(2, find_in("123456789012345678", "345", 0));
    EXPECT_EQ(-1, find_in("123456789012345678", "678", 0));
}

TEST(MyStringFind, FindsPatternAtEveryPosition) {
    const std::string text = "abcdefghijklmno";

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        const std::string pattern = text.substr(static_cast<std::size_t>(i), 1);
        EXPECT_EQ(i, find_in(text.c_str(), pattern.c_str(), 0)) << "pattern: " << pattern;
    }
}

TEST(MyStringFind, DoesNotModifyString) {
    MyString string("hello");

    string.find(MyString("ll"), 0);

    EXPECT_EQ("hello", to_std_string(string));
    EXPECT_EQ(5, string.length());
}

TEST(MyStringFind, DoesNotModifyPattern) {
    MyString string("hello");
    MyString pattern("ll");

    string.find(pattern, 0);

    EXPECT_EQ("ll", to_std_string(pattern));
}

TEST(MyStringFind, StringCanBeUsedAsItsOwnPattern) {
    MyString string("hello");

    EXPECT_EQ(0, string.find(string, 0));
}

TEST(MyStringFind, WorksOnConcatenatedResult) {
    MyString a("hello ");
    MyString b("world");

    MyString joined = a + b;

    EXPECT_EQ(6, joined.find(MyString("world"), 0));
    EXPECT_EQ(4, joined.find(MyString("o w"), 0));
}

TEST(MyStringFind, WorksOnSubstrResult) {
    MyString string("hello world");

    MyString part = string.substr(6, 5);

    EXPECT_EQ(2, part.find(MyString("rl"), 0));
    EXPECT_EQ(-1, part.find(MyString("hello"), 0));
}

TEST(MyStringFind, WorksOnAssignedString) {
    MyString source("find me");
    MyString target("other");

    target = source;

    EXPECT_EQ(5, target.find(MyString("me"), 0));
}

TEST(MyStringFind, WorksOnInputString) {
    std::istringstream input("needle haystack");
    MyString first;
    MyString second;

    input >> first >> second;

    EXPECT_EQ(3, second.find(MyString("st"), 0));
    EXPECT_EQ(-1, first.find(MyString("st"), 0));
}

TEST(MyStringFind, WorksOnGetLineString) {
    std::istringstream input("find in line\n");
    MyString line;

    get_line(input, line);

    EXPECT_EQ(5, line.find(MyString("in"), 0));
}

TEST(MyStringFind, MatchesStdStringForManyInputs) {
    const char *texts[] = {"abcabcabc", "aaaaaaaaaa", "hello world", "2026/10/12", "abababababababa", "123456789012345"};
    const char *patterns[] = {"a", "ab", "aba", "abc", "aa", "o", "lo w", "10", "/", "5", "345", "zz", "ba"};

    for (const char *text: texts) {
        for (const char *pattern: patterns) {
            for (int start = -3; start <= 17; ++start) {
                EXPECT_EQ(expected_find(text, pattern, start), find_in(text, pattern, start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}

// =====================================================================
// 상속받은 find (CharSequence 참조를 통한 호출)
// MyString::find가 기반 클래스의 find를 가리므로 기반 참조로 호출해 검사한다.
// =====================================================================

TEST(MyStringInheritedFind, CharArrayOverloadThroughBaseReference) {
    MyString string("hello world");
    const CharSequence &base = string;

    EXPECT_EQ(2, base.find("ll", 0));
    EXPECT_EQ(-1, base.find("xyz", 0));
}

TEST(MyStringInheritedFind, CharSequenceOverloadThroughBaseReference) {
    MyString string("hello world");
    const CharSequence &base = string;

    EXPECT_EQ(6, base.find(CharSequence("world"), 0));
    EXPECT_EQ(-1, base.find(CharSequence("xyz"), 0));
}

TEST(MyStringInheritedFind, StartRulesApplyThroughBaseReference) {
    MyString string("abcabc");
    const CharSequence &base = string;

    EXPECT_EQ(0, base.find("abc", -5));
    EXPECT_EQ(3, base.find("abc", 1));
    EXPECT_EQ(-1, base.find("abc", 10));
}

TEST(MyStringInheritedFind, BaseAndDerivedFindAgree) {
    const char *texts[] = {"", "a", "abcabc", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "abc", "ll", "o w", "345", "zzz"};

    for (const char *text: texts) {
        MyString string(text);
        const CharSequence &base = string;
        for (const char *pattern: patterns) {
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(base.find(CharSequence(pattern), start), string.find(MyString(pattern), start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}
