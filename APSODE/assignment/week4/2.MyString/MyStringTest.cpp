#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "OldMyString.h"

namespace {
    std::string to_std_string(const CharSequence &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    // OldMyString::find(const OldMyString&, int)를 편하게 호출하기 위한 도우미
    int find_in(const char *text, const char *pattern, int start) {
        OldMyString string(text);
        return string.find(OldMyString(pattern), start);
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
    OldMyString string;

    EXPECT_EQ(0, string.length());
    EXPECT_TRUE(string.empty());
}

TEST(MyStringConstructor, FromCharArray) {
    OldMyString string("hello");

    EXPECT_EQ(5, string.length());
    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringConstructor, FromLongCharArrayIsTruncated) {
    OldMyString string("1234567890123456789");

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringConstructor, FromCharSequence) {
    CharSequence sequence("sequence");
    OldMyString string(sequence);

    EXPECT_EQ(8, string.length());
    EXPECT_TRUE(string.equals(sequence));
}

TEST(MyStringConstructor, CopyCopiesContent) {
    OldMyString original("copy");
    OldMyString copied(original);

    EXPECT_TRUE(original == copied);
    EXPECT_EQ("copy", to_std_string(copied));
}

TEST(MyStringConstructor, CopyIsIndependentFromOriginal) {
    OldMyString original("abc");
    OldMyString copied(original);

    copied.set('z', 0);

    EXPECT_EQ('a', original.at(0));
    EXPECT_EQ('z', copied.at(0));
}

// =====================================================================
// 상속받은 기능 (CharSequence)
// =====================================================================

TEST(MyStringInherited, AtAndSetWork) {
    OldMyString string("abc");

    EXPECT_EQ('b', string.at(1));
    EXPECT_TRUE(string.set('x', 1));
    EXPECT_EQ('x', string.at(1));
    EXPECT_FALSE(string.set('x', 3));
}

TEST(MyStringInherited, CanBeUsedThroughCharSequenceReference) {
    OldMyString string("base");
    const CharSequence &reference = string;

    EXPECT_EQ(4, reference.length());
    EXPECT_EQ('b', reference.at(0));
}

TEST(MyStringInherited, AppendTruncatedWorks) {
    OldMyString string("1234567890");

    CharSequence result = string + "abcdefghij";

    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

// =====================================================================
// operator==
// =====================================================================

TEST(MyStringEquality, TrueForSameContent) {
    OldMyString a("hello");
    OldMyString b("hello");

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == a);
}

TEST(MyStringEquality, FalseForDifferentContent) {
    OldMyString a("hello");
    OldMyString b("world");

    EXPECT_FALSE(a == b);
}

TEST(MyStringEquality, FalseForDifferentLength) {
    OldMyString a("hello");
    OldMyString b("hello!");

    EXPECT_FALSE(a == b);
}

TEST(MyStringEquality, TrueForSelf) {
    OldMyString a("self");

    EXPECT_TRUE(a == a);
}

TEST(MyStringEquality, TrueForTwoEmptyStrings) {
    OldMyString a;
    OldMyString b;

    EXPECT_TRUE(a == b);
}

// =====================================================================
// operator=
// =====================================================================

TEST(MyStringAssignment, CopiesContent) {
    OldMyString a("source");
    OldMyString b("target");

    b = a;

    EXPECT_TRUE(a == b);
    EXPECT_EQ("source", to_std_string(b));
}

TEST(MyStringAssignment, FromEmptyMakesTargetEmpty) {
    OldMyString empty;
    OldMyString target("something");

    target = empty;

    EXPECT_TRUE(target.empty());
}

TEST(MyStringAssignment, ToSelfKeepsContent) {
    OldMyString a("self");
    OldMyString &reference = a;

    a = reference;

    EXPECT_EQ("self", to_std_string(a));
}

TEST(MyStringAssignment, ReturnsReferenceForChaining) {
    OldMyString a("chain");
    OldMyString b;
    OldMyString c;

    c = b = a;

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == c);
}

TEST(MyStringAssignment, IsIndependentFromSource) {
    OldMyString a("abc");
    OldMyString b;

    b = a;
    b.set('z', 0);

    EXPECT_EQ('a', a.at(0));
    EXPECT_EQ('z', b.at(0));
}

// =====================================================================
// operator+
// =====================================================================

TEST(MyStringPlus, WithMyString) {
    OldMyString a("abc");
    OldMyString b("def");

    OldMyString result = a + b;

    EXPECT_EQ("abcdef", to_std_string(result));
    EXPECT_EQ(6, result.length());
}

TEST(MyStringPlus, WithCharSequence) {
    OldMyString a("abc");
    CharSequence b("def");

    OldMyString result = a + b;

    EXPECT_EQ("abcdef", to_std_string(result));
}

TEST(MyStringPlus, WithCharArray) {
    OldMyString a("abc");

    OldMyString result = a + "def";

    EXPECT_EQ("abcdef", to_std_string(result));
}

TEST(MyStringPlus, DoesNotModifyOperands) {
    OldMyString a("abc");
    OldMyString b("def");

    OldMyString result = a + b;

    EXPECT_EQ("abc", to_std_string(a));
    EXPECT_EQ("def", to_std_string(b));
}

TEST(MyStringPlus, WithEmptyReturnsSameContent) {
    OldMyString a("abc");
    OldMyString empty;

    EXPECT_TRUE((a + empty) == a);
    EXPECT_TRUE((empty + a) == a);
}

TEST(MyStringPlus, TruncatesAtMaxLength) {
    OldMyString a("1234567890");
    OldMyString b("abcdefghij");

    OldMyString result = a + b;

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(MyStringPlus, CharArrayTruncatesAtMaxLength) {
    OldMyString a("1234567890");

    OldMyString result = a + "abcdefghij";

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(MyStringPlus, CanBeChained) {
    OldMyString a("a");
    OldMyString b("b");

    OldMyString result = a + b + "c";

    EXPECT_EQ("abc", to_std_string(result));
}

// =====================================================================
// operator>>
// =====================================================================

TEST(MyStringInputOperator, ReadsWord) {
    std::istringstream input("hello");
    OldMyString string;

    input >> string;

    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringInputOperator, StopsAtWhitespace) {
    std::istringstream input("hello world");
    OldMyString first;
    OldMyString second;

    input >> first >> second;

    EXPECT_EQ("hello", to_std_string(first));
    EXPECT_EQ("world", to_std_string(second));
}

TEST(MyStringInputOperator, TruncatesLongInput) {
    std::istringstream input("1234567890123456789");
    OldMyString string;

    input >> string;

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringInputOperator, OverwritesExistingContent) {
    std::istringstream input("new");
    OldMyString string("previous");

    input >> string;

    EXPECT_EQ("new", to_std_string(string));
}

TEST(MyStringInputOperator, ReturnsStreamForChaining) {
    std::istringstream input("a b");
    OldMyString first;
    OldMyString second;

    std::istream &returned = (input >> first);
    returned >> second;

    EXPECT_EQ("a", to_std_string(first));
    EXPECT_EQ("b", to_std_string(second));
}

// =====================================================================
// 길이 경계 (최대 15글자)
// =====================================================================

TEST(MyStringBoundary, ExactlyMaxLengthIsKept) {
    OldMyString string("123456789012345");

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringBoundary, OneOverMaxLengthIsTruncated) {
    OldMyString string("1234567890123456");

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

TEST(MyStringBoundary, EmptyCharArrayCreatesEmptyString) {
    OldMyString string("");

    EXPECT_EQ(0, string.length());
    EXPECT_TRUE(string.empty());
}

TEST(MyStringBoundary, NonEmptyStringIsNotEmpty) {
    OldMyString string("a");

    EXPECT_EQ(1, string.length());
    EXPECT_FALSE(string.empty());
}

TEST(MyStringBoundary, LengthIsContentLengthNotCapacity) {
    // 과제 조건: 전체 용량이 아니라 문자열 자체의 길이를 반환
    OldMyString string("abc");

    EXPECT_EQ(3, string.length());
}

// =====================================================================
// operator!=
// =====================================================================

TEST(MyStringInequality, FalseForSameContent) {
    OldMyString a("hello");
    OldMyString b("hello");

    EXPECT_FALSE(a != b);
}

TEST(MyStringInequality, TrueForDifferentContent) {
    OldMyString a("hello");
    OldMyString b("world");

    EXPECT_TRUE(a != b);
    EXPECT_TRUE(b != a);
}

TEST(MyStringInequality, TrueForDifferentLength) {
    OldMyString a("hello");
    OldMyString b("hello!");

    EXPECT_TRUE(a != b);
}

TEST(MyStringInequality, TrueForEmptyAndNonEmpty) {
    OldMyString empty;
    OldMyString text("a");

    EXPECT_TRUE(empty != text);
    EXPECT_TRUE(text != empty);
}

TEST(MyStringInequality, FalseForTwoEmptyStrings) {
    OldMyString a;
    OldMyString b;

    EXPECT_FALSE(a != b);
}

TEST(MyStringInequality, AlwaysOppositeOfEquality) {
    const char *samples[] = {"", "a", "ab", "abc", "hello", "123456789012345"};

    for (const char *left_text: samples) {
        for (const char *right_text: samples) {
            OldMyString left(left_text);
            OldMyString right(right_text);

            EXPECT_EQ(!(left == right), left != right)
                << "left: " << left_text << ", right: " << right_text;
        }
    }
}

TEST(MyStringInequality, DetectsDifferenceAtLastCharacter) {
    OldMyString a("abcde");
    OldMyString b("abcdf");

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

// =====================================================================
// operator<< (출력)
// =====================================================================

TEST(MyStringOutputOperator, PrintsContent) {
    OldMyString string("hello");
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ("hello", stream.str());
}

TEST(MyStringOutputOperator, PrintsNothingForEmptyString) {
    OldMyString string;
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ("", stream.str());
}

TEST(MyStringOutputOperator, DoesNotAppendNewline) {
    OldMyString string("abc");
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ(std::string::npos, stream.str().find('\n'));
}

TEST(MyStringOutputOperator, CanBeChainedWithOtherOutput) {
    OldMyString a("abc");
    OldMyString b("def");
    std::ostringstream stream;

    stream << a << "," << b << "!";

    EXPECT_EQ("abc,def!", stream.str());
}

TEST(MyStringOutputOperator, PrintsConcatenatedResult) {
    OldMyString a("abc");
    std::ostringstream stream;

    stream << (a + "def");

    EXPECT_EQ("abcdef", stream.str());
}

TEST(MyStringOutputOperator, PrintsTruncatedContent) {
    OldMyString string("1234567890123456789");
    std::ostringstream stream;

    stream << string;

    EXPECT_EQ("123456789012345", stream.str());
}

// =====================================================================
// operator>> 추가 규칙
// =====================================================================

TEST(MyStringInputOperator, SkipsLeadingWhitespace) {
    std::istringstream input("   \n\t hello");
    OldMyString string;

    input >> string;

    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringInputOperator, ReadsExactlyMaxLengthFully) {
    std::istringstream input("123456789012345");
    OldMyString string;

    input >> string;

    EXPECT_EQ(CharSequence::MAX_LENGTH, string.length());
    EXPECT_EQ("123456789012345", to_std_string(string));
}

// 과제 조건: 범위를 넘는 글자는 버퍼에 그대로 둔다.
// (넘친 글자가 16번째부터 스트림에 남는다는 해석에 의존합니다.)
TEST(MyStringInputOperator, CharactersBeyondLimitStayInStream) {
    std::istringstream input("1234567890123456789");
    OldMyString first;
    OldMyString second;

    input >> first >> second;

    EXPECT_EQ("123456789012345", to_std_string(first));
    EXPECT_EQ("6789", to_std_string(second));
}

TEST(MyStringInputOperator, ReadsThreeWords) {
    std::istringstream input("one two three");
    OldMyString a;
    OldMyString b;
    OldMyString c;

    input >> a >> b >> c;

    EXPECT_EQ("one", to_std_string(a));
    EXPECT_EQ("two", to_std_string(b));
    EXPECT_EQ("three", to_std_string(c));
}

TEST(MyStringInputOperator, StopsAtNewline) {
    std::istringstream input("first\nsecond");
    OldMyString a;
    OldMyString b;

    input >> a >> b;

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ("second", to_std_string(b));
}

// =====================================================================
// get_line
// =====================================================================

TEST(MyStringGetLine, ReadsWholeLineIncludingSpaces) {
    std::istringstream input("hello world\n");
    OldMyString string;

    getline(input, string);

    EXPECT_EQ("hello world", to_std_string(string));
}

TEST(MyStringGetLine, DoesNotIncludeDelimiter) {
    std::istringstream input("abc\n");
    OldMyString string;

    getline(input, string);

    EXPECT_EQ(3, string.length());
    EXPECT_EQ(std::string::npos, to_std_string(string).find('\n'));
}

TEST(MyStringGetLine, ConsumesDelimiterSoNextLineIsRead) {
    std::istringstream input("first\nsecond\n");
    OldMyString a;
    OldMyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ("second", to_std_string(b));
}

TEST(MyStringGetLine, KeepsLeadingWhitespace) {
    std::istringstream input("  hi\n");
    OldMyString string;

    getline(input, string);

    EXPECT_EQ("  hi", to_std_string(string));
}

TEST(MyStringGetLine, EmptyLineGivesEmptyString) {
    std::istringstream input("\nnext\n");
    OldMyString a;
    OldMyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_TRUE(a.empty());
    EXPECT_EQ("next", to_std_string(b));
}

TEST(MyStringGetLine, LastLineWithoutDelimiterIsRead) {
    std::istringstream input("only line");
    OldMyString string;

    getline(input, string);

    EXPECT_EQ("only line", to_std_string(string));
}

TEST(MyStringGetLine, CustomDelimiter) {
    std::istringstream input("a,b,c");
    OldMyString a;
    OldMyString b;
    OldMyString c;

    getline(input, a, ',');
    getline(input, b, ',');
    getline(input, c, ',');

    EXPECT_EQ("a", to_std_string(a));
    EXPECT_EQ("b", to_std_string(b));
    EXPECT_EQ("c", to_std_string(c));
}

TEST(MyStringGetLine, NewlineIsNotDelimiterWhenCustomDelimiterGiven) {
    std::istringstream input("ab\ncd,ef");
    OldMyString string;

    getline(input, string, ',');

    EXPECT_EQ("ab\ncd", to_std_string(string));
}

TEST(MyStringGetLine, ExactlyMaxLengthIsKept) {
    std::istringstream input("123456789012345\nnext\n");
    OldMyString a;
    OldMyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_EQ("123456789012345", to_std_string(a));
    EXPECT_EQ("next", to_std_string(b));
}

// 과제 조건: delim까지 모든 글자를 읽되 16번째 글자부터는 버린다.
TEST(MyStringGetLine, DiscardsCharactersBeyondLimit) {
    std::istringstream input("1234567890123456789\nnext\n");
    OldMyString a;
    OldMyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_EQ(CharSequence::MAX_LENGTH, a.length());
    EXPECT_EQ("123456789012345", to_std_string(a));
    EXPECT_EQ("next", to_std_string(b));
}

TEST(MyStringGetLine, OverwritesExistingContent) {
    std::istringstream input("new\n");
    OldMyString string("previous");

    getline(input, string);

    EXPECT_EQ("new", to_std_string(string));
}

TEST(MyStringGetLine, ReturnsStreamForChaining) {
    std::istringstream input("a\nb\n");
    OldMyString first;
    OldMyString second;

    std::istream &returned = getline(input, first);
    getline(returned, second);

    EXPECT_EQ("a", to_std_string(first));
    EXPECT_EQ("b", to_std_string(second));
}

TEST(MyStringGetLine, MixesWithExtractionOperator) {
    // >>로 단어를 읽은 뒤 같은 스트림에서 get_line이 다음 줄을 읽을 수 있어야 함
    std::istringstream input("word\nrest of line\n");
    OldMyString word;
    OldMyString line;

    input >> word;
    input.get();    // >>가 남긴 개행 소비
    getline(input, line);

    EXPECT_EQ("word", to_std_string(word));
    EXPECT_EQ("rest of line", to_std_string(line));
}

// =====================================================================
// substr
// =====================================================================

TEST(MyStringSubstr, FromStart) {
    OldMyString string("hello world");

    EXPECT_EQ("hello", to_std_string(string.substr(0, 5)));
}

TEST(MyStringSubstr, FromMiddle) {
    OldMyString string("hello world");

    EXPECT_EQ("world", to_std_string(string.substr(6, 5)));
}

TEST(MyStringSubstr, SingleCharacter) {
    OldMyString string("abc");

    EXPECT_EQ("b", to_std_string(string.substr(1, 1)));
}

TEST(MyStringSubstr, WholeString) {
    OldMyString string("abcde");

    EXPECT_EQ("abcde", to_std_string(string.substr(0, 5)));
}

TEST(MyStringSubstr, LastCharacter) {
    OldMyString string("abcde");

    EXPECT_EQ("e", to_std_string(string.substr(4, 1)));
}

TEST(MyStringSubstr, ZeroLengthGivesEmpty) {
    OldMyString string("hello");

    EXPECT_TRUE(string.substr(2, 0).empty());
}

// 과제 조건: len이 맞지 않으면 std::string::substr처럼 끝까지만 반환
TEST(MyStringSubstr, LengthBeyondEndStopsAtEnd) {
    OldMyString string("hello world");

    EXPECT_EQ("world", to_std_string(string.substr(6, 100)));
}

TEST(MyStringSubstr, LengthOneBeyondEndStopsAtEnd) {
    OldMyString string("abc");

    EXPECT_EQ("bc", to_std_string(string.substr(1, 3)));
}

// 과제 조건: pos가 음수이면 0으로 취급
TEST(MyStringSubstr, NegativePositionIsTreatedAsZero) {
    OldMyString string("hello");

    EXPECT_EQ("he", to_std_string(string.substr(-3, 2)));
}

TEST(MyStringSubstr, VeryNegativePositionIsTreatedAsZero) {
    OldMyString string("hello");

    EXPECT_EQ("hello", to_std_string(string.substr(-100, 5)));
}

// 과제 조건: 문자열 범위 밖이면 빈 문자열
TEST(MyStringSubstr, PositionBeyondEndGivesEmpty) {
    OldMyString string("hello");

    EXPECT_TRUE(string.substr(10, 2).empty());
}

TEST(MyStringSubstr, PositionAtEndGivesEmpty) {
    // std::string::substr(size(), n)도 빈 문자열을 반환
    OldMyString string("hello");

    EXPECT_TRUE(string.substr(5, 3).empty());
}

TEST(MyStringSubstr, EmptySourceGivesEmpty) {
    OldMyString empty;

    EXPECT_TRUE(empty.substr(0, 3).empty());
}

TEST(MyStringSubstr, DoesNotModifyOriginal) {
    OldMyString string("hello");

    OldMyString result = string.substr(1, 3);
    (void) result;

    EXPECT_EQ("hello", to_std_string(string));
}

TEST(MyStringSubstr, ResultIsIndependentFromOriginal) {
    OldMyString string("hello");

    OldMyString result = string.substr(0, 3);
    result.set('z', 0);

    EXPECT_EQ('h', string.at(0));
    EXPECT_EQ('z', result.at(0));
}

TEST(MyStringSubstr, ResultLengthMatchesContent) {
    OldMyString string("hello world");

    EXPECT_EQ(5, string.substr(0, 5).length());
    EXPECT_EQ(5, string.substr(6, 100).length());
    EXPECT_EQ(0, string.substr(20, 3).length());
}

TEST(MyStringSubstr, FromMaxLengthString) {
    OldMyString string("123456789012345");

    EXPECT_EQ("12345", to_std_string(string.substr(10, 5)));
    EXPECT_EQ("5", to_std_string(string.substr(14, 1)));
}

TEST(MyStringSubstr, MatchesStdStringForManyRanges) {
    const std::string text = "hello world";
    OldMyString string(text.c_str());

    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
        for (int len = 0; len <= 15; ++len) {
            EXPECT_EQ(text.substr(pos, len), to_std_string(string.substr(pos, len)))
                << "pos: " << pos << ", len: " << len;
        }
    }
}

TEST(MyStringSubstr, CanBeConcatenated) {
    OldMyString string("hello world");

    OldMyString result = string.substr(0, 5) + string.substr(5, 6);

    EXPECT_EQ("hello world", to_std_string(result));
}

// =====================================================================
// operator+ 추가 경계
// =====================================================================

TEST(MyStringPlus, ExactlyMaxLengthIsNotTruncated) {
    OldMyString a("12345678");
    OldMyString b("1234567");

    OldMyString result = a + b;

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("123456781234567", to_std_string(result));
}

TEST(MyStringPlus, OneOverMaxLengthIsTruncated) {
    OldMyString a("12345678");
    OldMyString b("12345678");

    OldMyString result = a + b;

    EXPECT_EQ(CharSequence::MAX_LENGTH, result.length());
    EXPECT_EQ("123456781234567", to_std_string(result));
}

TEST(MyStringPlus, SelfConcatenation) {
    OldMyString a("ab");

    OldMyString result = a + a;

    EXPECT_EQ("abab", to_std_string(result));
    EXPECT_EQ("ab", to_std_string(a));
}

TEST(MyStringPlus, EmptyCharArray) {
    OldMyString a("abc");

    OldMyString result = a + "";

    EXPECT_EQ("abc", to_std_string(result));
}

TEST(MyStringPlus, ResultIsIndependentFromOperands) {
    OldMyString a("abc");
    OldMyString b("def");

    OldMyString result = a + b;
    result.set('z', 0);

    EXPECT_EQ('a', a.at(0));
    EXPECT_EQ('z', result.at(0));
}

TEST(MyStringPlus, AssignedResultMatchesDirectResult) {
    OldMyString a("abc");
    OldMyString b("def");
    OldMyString assigned;

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
    EXPECT_EQ(5, find_in("123456789012345678", "678", 0));
}

TEST(MyStringFind, FindsPatternAtEveryPosition) {
    const std::string text = "abcdefghijklmno";

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        const std::string pattern = text.substr(static_cast<std::size_t>(i), 1);
        EXPECT_EQ(i, find_in(text.c_str(), pattern.c_str(), 0)) << "pattern: " << pattern;
    }
}

TEST(MyStringFind, DoesNotModifyString) {
    OldMyString string("hello");

    string.find(OldMyString("ll"), 0);

    EXPECT_EQ("hello", to_std_string(string));
    EXPECT_EQ(5, string.length());
}

TEST(MyStringFind, DoesNotModifyPattern) {
    OldMyString string("hello");
    OldMyString pattern("ll");

    string.find(pattern, 0);

    EXPECT_EQ("ll", to_std_string(pattern));
}

TEST(MyStringFind, StringCanBeUsedAsItsOwnPattern) {
    OldMyString string("hello");

    EXPECT_EQ(0, string.find(string, 0));
}

TEST(MyStringFind, WorksOnConcatenatedResult) {
    OldMyString a("hello ");
    OldMyString b("world");

    OldMyString joined = a + b;

    EXPECT_EQ(6, joined.find(OldMyString("world"), 0));
    EXPECT_EQ(4, joined.find(OldMyString("o w"), 0));
}

TEST(MyStringFind, WorksOnSubstrResult) {
    OldMyString string("hello world");

    OldMyString part = string.substr(6, 5);

    EXPECT_EQ(2, part.find(OldMyString("rl"), 0));
    EXPECT_EQ(-1, part.find(OldMyString("hello"), 0));
}

TEST(MyStringFind, WorksOnAssignedString) {
    OldMyString source("find me");
    OldMyString target("other");

    target = source;

    EXPECT_EQ(5, target.find(OldMyString("me"), 0));
}

TEST(MyStringFind, WorksOnInputString) {
    std::istringstream input("needle haystack");
    OldMyString first;
    OldMyString second;

    input >> first >> second;

    EXPECT_EQ(3, second.find(OldMyString("st"), 0));
    EXPECT_EQ(-1, first.find(OldMyString("st"), 0));
}

TEST(MyStringFind, WorksOnGetLineString) {
    std::istringstream input("find in line\n");
    OldMyString line;

    getline(input, line);

    EXPECT_EQ(1, line.find(OldMyString("in"), 0));
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
// OldMyString::find가 기반 클래스의 find를 가리므로 기반 참조로 호출해 검사한다.
// =====================================================================

TEST(MyStringInheritedFind, CharArrayOverloadThroughBaseReference) {
    OldMyString string("hello world");
    const CharSequence &base = string;

    EXPECT_EQ(2, base.find("ll", 0));
    EXPECT_EQ(-1, base.find("xyz", 0));
}

TEST(MyStringInheritedFind, CharSequenceOverloadThroughBaseReference) {
    OldMyString string("hello world");
    const CharSequence &base = string;

    EXPECT_EQ(6, base.find(CharSequence("world"), 0));
    EXPECT_EQ(-1, base.find(CharSequence("xyz"), 0));
}

TEST(MyStringInheritedFind, StartRulesApplyThroughBaseReference) {
    OldMyString string("abcabc");
    const CharSequence &base = string;

    EXPECT_EQ(0, base.find("abc", -5));
    EXPECT_EQ(3, base.find("abc", 1));
    EXPECT_EQ(-1, base.find("abc", 10));
}

TEST(MyStringInheritedFind, BaseAndDerivedFindAgree) {
    const char *texts[] = {"", "a", "abcabc", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "abc", "ll", "o w", "345", "zzz"};

    for (const char *text: texts) {
        OldMyString string(text);
        const CharSequence &base = string;
        for (const char *pattern: patterns) {
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(base.find(CharSequence(pattern), start), string.find(OldMyString(pattern), start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}
// operator>> 와 getline 을 하나의 스트림에서 함께 사용하는 경우를 검사한다.
// 규칙 (기존 OldMyString 과제 조건과 동일)
//  - >> : 앞쪽 공백을 건너뛰고 공백 전까지 읽는다. 15글자를 넘는 글자는 스트림에 남긴다.
//  - getline : delim 까지 읽고 delim 은 소비한다. 16번째 글자부터는 버린다.
//  - >> 는 뒤따르는 공백(개행 포함)을 소비하지 않는다.

// ---------------------------------------------------------------------
// >> 다음에 getline
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, GetlineAfterExtractionReadsRestOfSameLine) {
    // >> 가 단어 뒤 공백을 소비하지 않으므로 getline 은 그 공백부터 읽는다.
    std::istringstream input("word rest of line\nnext\n");
    OldMyString word;
    OldMyString rest;

    input >> word;
    ::getline(input, rest);

    EXPECT_EQ("word", to_std_string(word));
    EXPECT_EQ(" rest of line", to_std_string(rest));
}

TEST(MyStringStreamMixTest, GetlineRightAfterExtractionAtLineEndGivesEmptyString) {
    // >> 가 개행을 남기므로 바로 이어지는 getline 은 빈 줄을 읽는다.
    std::istringstream input("word\nrest\n");
    OldMyString word;
    OldMyString empty_line;
    OldMyString next_line;

    input >> word;
    ::getline(input, empty_line);
    ::getline(input, next_line);

    EXPECT_EQ("word", to_std_string(word));
    EXPECT_TRUE(empty_line.empty());
    EXPECT_EQ("rest", to_std_string(next_line));
}

TEST(MyStringStreamMixTest, GetlineAfterExtractionAndManualNewlineSkip) {
    std::istringstream input("word\nrest of line\n");
    OldMyString word;
    OldMyString line;

    input >> word;
    input.get();    // >> 가 남긴 개행 소비
    ::getline(input, line);

    EXPECT_EQ("word", to_std_string(word));
    EXPECT_EQ("rest of line", to_std_string(line));
}

TEST(MyStringStreamMixTest, GetlineAfterExtractionAndIgnore) {
    std::istringstream input("word\nrest of line\n");
    OldMyString word;
    OldMyString line;

    input >> word;
    input.ignore(1000, '\n');
    ::getline(input, line);

    EXPECT_EQ("rest of line", to_std_string(line));
}

TEST(MyStringStreamMixTest, ExtractionThenGetlineThenExtraction) {
    std::istringstream input("first second\nthird fourth\n");
    OldMyString a;
    OldMyString b;
    OldMyString c;

    input >> a;
    ::getline(input, b);    // " second"
    input >> c;

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ(" second", to_std_string(b));
    EXPECT_EQ("third", to_std_string(c));
}

// ---------------------------------------------------------------------
// getline 다음에 >>
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, ExtractionAfterGetlineReadsNextLineWord) {
    std::istringstream input("whole line here\nword other\n");
    OldMyString line;
    OldMyString word;

    ::getline(input, line);
    input >> word;

    EXPECT_EQ("whole line here", to_std_string(line));
    EXPECT_EQ("word", to_std_string(word));
}

TEST(MyStringStreamMixTest, ExtractionSkipsEmptyLinesLeftByGetline) {
    std::istringstream input("line\n\n\n   word\n");
    OldMyString line;
    OldMyString word;

    ::getline(input, line);
    input >> word;

    EXPECT_EQ("line", to_std_string(line));
    EXPECT_EQ("word", to_std_string(word));
}

TEST(MyStringStreamMixTest, AlternatesBetweenGetlineAndExtraction) {
    std::istringstream input("title line\nkeyword extra\nsecond title\nkeyword2\n");
    OldMyString title1;
    OldMyString keyword1;
    OldMyString title2;
    OldMyString keyword2;

    ::getline(input, title1);
    input >> keyword1;
    input.ignore(1000, '\n');
    ::getline(input, title2);
    input >> keyword2;

    EXPECT_EQ("title line", to_std_string(title1));
    EXPECT_EQ("keyword", to_std_string(keyword1));
    EXPECT_EQ("second title", to_std_string(title2));
    EXPECT_EQ("keyword2", to_std_string(keyword2));
}

// ---------------------------------------------------------------------
// 길이 제한이 걸린 경우의 조합
// ---------------------------------------------------------------------

// 과제 조건: >> 는 넘친 글자를 스트림에 남기고, getline 이 그 나머지를 읽는다.
TEST(MyStringStreamMixTest, OverflowLeftByExtractionIsReadByGetline) {
    std::istringstream input("1234567890123456789\nnext\n");
    OldMyString head;
    OldMyString tail;
    OldMyString next;

    input >> head;
    ::getline(input, tail);
    ::getline(input, next);

    EXPECT_EQ("123456789012345", to_std_string(head));
    EXPECT_EQ("6789", to_std_string(tail));
    EXPECT_EQ("next", to_std_string(next));
}

TEST(MyStringStreamMixTest, OverflowLeftByExtractionIsReadByNextExtraction) {
    std::istringstream input("1234567890123456789 word\n");
    OldMyString head;
    OldMyString tail;
    OldMyString word;

    input >> head >> tail >> word;

    EXPECT_EQ("123456789012345", to_std_string(head));
    EXPECT_EQ("6789", to_std_string(tail));
    EXPECT_EQ("word", to_std_string(word));
}

// 과제 조건: getline 은 16번째 글자부터 버리고 delim 까지 소비한다.
TEST(MyStringStreamMixTest, GetlineDiscardsOverflowThenExtractionReadsNextLine) {
    std::istringstream input("1234567890123456789\nword\n");
    OldMyString line;
    OldMyString word;

    ::getline(input, line);
    input >> word;

    EXPECT_EQ("123456789012345", to_std_string(line));
    EXPECT_EQ("word", to_std_string(word));
}

TEST(MyStringStreamMixTest, ExactlyMaxLengthWordThenGetlineGivesEmptyRemainder) {
    std::istringstream input("123456789012345\nnext\n");
    OldMyString word;
    OldMyString remainder;
    OldMyString next;

    input >> word;
    ::getline(input, remainder);
    ::getline(input, next);

    EXPECT_EQ("123456789012345", to_std_string(word));
    EXPECT_TRUE(remainder.empty());
    EXPECT_EQ("next", to_std_string(next));
}

TEST(MyStringStreamMixTest, ExactlyMaxLengthLineThenExtraction) {
    std::istringstream input("123456789012345\nword\n");
    OldMyString line;
    OldMyString word;

    ::getline(input, line);
    input >> word;

    EXPECT_EQ("123456789012345", to_std_string(line));
    EXPECT_EQ("word", to_std_string(word));
}

// ---------------------------------------------------------------------
// 구분자(delim) 지정
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, CustomDelimiterWithExtraction) {
    std::istringstream input("a,b c,d");
    OldMyString first;
    OldMyString second;
    OldMyString third;

    ::getline(input, first, ',');    // "a"
    input >> second;                 // "b"
    ::getline(input, third, ',');    // " c"

    EXPECT_EQ("a", to_std_string(first));
    EXPECT_EQ("b", to_std_string(second));
    EXPECT_EQ(" c", to_std_string(third));
}

TEST(MyStringStreamMixTest, CustomDelimiterConsumesDelimiterBeforeExtraction) {
    std::istringstream input("key=value other");
    OldMyString key;
    OldMyString value;
    OldMyString other;

    ::getline(input, key, '=');
    input >> value >> other;

    EXPECT_EQ("key", to_std_string(key));
    EXPECT_EQ("value", to_std_string(value));
    EXPECT_EQ("other", to_std_string(other));
}

TEST(MyStringStreamMixTest, NewlineIsPlainCharacterForCustomDelimiter) {
    std::istringstream input("ab\ncd,ef gh");
    OldMyString first;
    OldMyString second;

    ::getline(input, first, ',');
    input >> second;

    EXPECT_EQ("ab\ncd", to_std_string(first));
    EXPECT_EQ("ef", to_std_string(second));
}

// ---------------------------------------------------------------------
// 다른 입력 방식과의 조합
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, IntegerExtractionThenGetline) {
    std::istringstream input("42\nhello world\n");
    int number = 0;
    OldMyString line;

    input >> number;
    input.ignore(1000, '\n');
    ::getline(input, line);

    EXPECT_EQ(42, number);
    EXPECT_EQ("hello world", to_std_string(line));
}

TEST(MyStringStreamMixTest, GetlineThenIntegerExtraction) {
    std::istringstream input("hello world\n42\n");
    OldMyString line;
    int number = 0;

    ::getline(input, line);
    input >> number;

    EXPECT_EQ("hello world", to_std_string(line));
    EXPECT_EQ(42, number);
}

TEST(MyStringStreamMixTest, MyStringAndStdStringCanShareStream) {
    std::istringstream input("mine yours\nrest of line\n");
    OldMyString mine;
    std::string yours;
    OldMyString line;

    input >> mine >> yours;
    input.ignore(1000, '\n');
    ::getline(input, line);

    EXPECT_EQ("mine", to_std_string(mine));
    EXPECT_EQ("yours", yours);
    EXPECT_EQ("rest of line", to_std_string(line));
}

// ---------------------------------------------------------------------
// 반환값 / 체이닝
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, GetlineCanTakeResultOfExtraction) {
    std::istringstream input("word tail\n");
    OldMyString word;
    OldMyString tail;

    ::getline(input >> word, tail);

    EXPECT_EQ("word", to_std_string(word));
    EXPECT_EQ(" tail", to_std_string(tail));
}

TEST(MyStringStreamMixTest, ExtractionCanTakeResultOfGetline) {
    std::istringstream input("line one\nword\n");
    OldMyString line;
    OldMyString word;

    ::getline(input, line) >> word;

    EXPECT_EQ("line one", to_std_string(line));
    EXPECT_EQ("word", to_std_string(word));
}

TEST(MyStringStreamMixTest, BothReturnTheSameStream) {
    std::istringstream input("a b\nc\n");
    OldMyString a;
    OldMyString b;

    std::istream &after_extraction = (input >> a);
    std::istream &after_getline = ::getline(input, b);

    EXPECT_EQ(&input, &after_extraction);
    EXPECT_EQ(&input, &after_getline);
}

// ---------------------------------------------------------------------
// 덮어쓰기 / 반복 읽기
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, SameVariableIsOverwrittenByEachRead) {
    std::istringstream input("first\nsecond line\nthird\n");
    OldMyString value;

    input >> value;
    EXPECT_EQ("first", to_std_string(value));

    input >> value;
    EXPECT_EQ("second", to_std_string(value));

    ::getline(input, value);
    EXPECT_EQ(" line", to_std_string(value));

    ::getline(input, value);
    EXPECT_EQ("third", to_std_string(value));
}

TEST(MyStringStreamMixTest, ReadsEmptyLineThenWordIntoPreviouslyFilledString) {
    std::istringstream input("\nword\n");
    OldMyString value("previous");

    ::getline(input, value);
    EXPECT_TRUE(value.empty());

    input >> value;
    EXPECT_EQ("word", to_std_string(value));
}

// 가정: 읽을 내용이 더 없으면 >> 와 getline 모두 스트림을 실패 상태로 만든다. (std 와 동일)
TEST(MyStringStreamMixTest, ExtractionLoopReadsAllWords) {
    std::istringstream input("one two\nthree\n\n four\n");
    OldMyString word;
    std::vector<std::string> words;

    while (input >> word) {
        words.push_back(to_std_string(word));
    }

    ASSERT_EQ(4u, words.size());
    EXPECT_EQ("one", words[0]);
    EXPECT_EQ("two", words[1]);
    EXPECT_EQ("three", words[2]);
    EXPECT_EQ("four", words[3]);
}

TEST(MyStringStreamMixTest, GetlineLoopReadsAllLines) {
    std::istringstream input("alpha\nbeta gamma\n\ndelta\n");
    OldMyString line;
    std::vector<std::string> lines;

    while (::getline(input, line)) {
        lines.push_back(to_std_string(line));
    }

    ASSERT_EQ(4u, lines.size());
    EXPECT_EQ("alpha", lines[0]);
    EXPECT_EQ("beta gamma", lines[1]);
    EXPECT_EQ("", lines[2]);
    EXPECT_EQ("delta", lines[3]);
}

TEST(MyStringStreamMixTest, FirstWordThenRestOfEachLine) {
    // 각 줄을 "명령어 + 나머지" 로 나누어 읽는 전형적인 사용 예
    std::istringstream input("add apple pie\nremove old\n");
    OldMyString command;
    OldMyString argument;
    std::vector<std::string> commands;
    std::vector<std::string> arguments;

    while (input >> command) {
        ::getline(input, argument);
        commands.push_back(to_std_string(command));
        arguments.push_back(to_std_string(argument));
    }

    ASSERT_EQ(2u, commands.size());
    EXPECT_EQ("add", commands[0]);
    EXPECT_EQ(" apple pie", arguments[0]);
    EXPECT_EQ("remove", commands[1]);
    EXPECT_EQ(" old", arguments[1]);
}

// ---------------------------------------------------------------------
// 읽은 결과의 사용
// ---------------------------------------------------------------------

TEST(MyStringStreamMixTest, ReadValuesCanBeConcatenated) {
    std::istringstream input("hello\nworld\n");
    OldMyString first;
    OldMyString second;

    input >> first;
    input.get();
    ::getline(input, second);

    EXPECT_EQ("hello world", to_std_string(first + " " + second));
}

TEST(MyStringStreamMixTest, ReadValuesCanBeCompared) {
    std::istringstream input("same\nsame\n");
    OldMyString from_extraction;
    OldMyString from_getline;

    input >> from_extraction;
    input.get();
    ::getline(input, from_getline);

    EXPECT_TRUE(from_extraction == from_getline);
}

TEST(MyStringStreamMixTest, ReadValuesCanBePrinted) {
    std::istringstream input("alpha beta gamma\n");
    OldMyString word;
    OldMyString rest;
    std::ostringstream output;

    input >> word;
    ::getline(input, rest);
    output << word << "|" << rest;

    EXPECT_EQ("alpha| beta gamma", output.str());
}

TEST(MyStringStreamMixTest, ReadValuesCanBeSubstringed) {
    std::istringstream input("key: value\n");
    OldMyString key;
    OldMyString rest;

    input >> key;
    ::getline(input, rest);

    EXPECT_EQ("value", to_std_string(rest.substr(1, 5)));
}

TEST(MyStringStreamMixTest, LHMsTest) {
    std::istringstream input("  123 456\n789\n");
    OldMyString str1, str2, str3, str4;

    input >> str1;
    input >> str2;
    getline(input, str3);
    getline(input, str4);

    EXPECT_EQ("123", to_std_string(str1));
    EXPECT_EQ("456", to_std_string(str2));
    EXPECT_EQ("", to_std_string(str3));
    EXPECT_EQ("789", to_std_string(str4));
}
