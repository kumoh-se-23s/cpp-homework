#include <gtest/gtest.h>
#include <sstream>
#include <string>

// utils.h가 MyString.h(→ CharSequence.h)를 include한다.
// 헤더에 include guard가 없으므로 MyString.h를 따로 include하면 중복 정의 오류가 난다.
#include "utils.h"

namespace {
    // protected 상수 CAPACITY를 테스트에서 쓰기 위한 접근용 하위 클래스
    class MyStringProbe : public MyString {
        public:
            using MyString::CAPACITY;
    };

    constexpr int MAX_LENGTH = MyStringProbe::CAPACITY;

    // MyString(char inputCharArr[])는 const가 아닌 배열을 받으므로
    // 문자열 리터럴을 수정 가능한 버퍼에 복사한 뒤 생성한다.
    MyString make_string(const char *text) {
        std::string buffer(text);
        return MyString(&buffer[0]);
    }

    // operator<<와 독립적으로 내용을 확인하기 위해 length()와 at()으로 꺼낸다.
    std::string to_std_string(const MyString &str) {
        std::string result;
        for (int index = 0; index < str.length(); ++index) {
            result += str.at(index);
        }
        return result;
    }

    // MyString::find(const MyString&, int)를 편하게 호출하기 위한 도우미
    int find_in(const char *text, const char *pattern, int start) {
        MyString str = make_string(text);
        return str.find(make_string(pattern), start);
    }

    // MyString::find(const char[], int)를 편하게 호출하기 위한 도우미
    int find_char_array_in(const char *text, const char *pattern, int start) {
        MyString str = make_string(text);
        return str.find(pattern, start);
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
    MyString str;

    EXPECT_EQ(0, str.length());
    EXPECT_TRUE(str.empty());
}

TEST(MyStringConstructor, FromCharArray) {
    char text[] = "hello";
    MyString str(text);

    EXPECT_EQ(5, str.length());
    EXPECT_EQ("hello", to_std_string(str));
}

TEST(MyStringConstructor, FromCharArrayDoesNotShareBuffer) {
    char text[] = "hello";
    MyString str(text);

    text[0] = 'j';

    EXPECT_EQ("hello", to_std_string(str));
}

TEST(MyStringConstructor, FromLongCharArrayIsTruncated) {
    MyString str = make_string("1234567890123456789");

    EXPECT_EQ(MAX_LENGTH, str.length());
    EXPECT_EQ("123456789012345", to_std_string(str));
}

TEST(MyStringConstructor, CopyCopiesContent) {
    MyString original = make_string("copy");
    MyString copied(original);

    EXPECT_TRUE(original == copied);
    EXPECT_EQ(4, copied.length());
    EXPECT_EQ("copy", to_std_string(copied));
}

TEST(MyStringConstructor, CopyOfEmptyIsEmpty) {
    MyString original;
    MyString copied(original);

    EXPECT_TRUE(copied.empty());
}

TEST(MyStringConstructor, CopyIsIndependentFromOriginal) {
    MyString original = make_string("abc");
    MyString copied(original);

    copied.setChar('z', 0);

    EXPECT_EQ('a', original.at(0));
    EXPECT_EQ('z', copied.at(0));
}

// =====================================================================
// 상속받은 기능 (CharSequence)
// =====================================================================

TEST(MyStringInherited, AtAndSetCharWork) {
    MyString str = make_string("abc");

    EXPECT_EQ('b', str.at(1));
    str.setChar('x', 1);
    EXPECT_EQ('x', str.at(1));
    EXPECT_EQ(3, str.length());
}

TEST(MyStringInherited, CanBeUsedThroughCharSequenceReference) {
    MyString str = make_string("base");
    const CharSequence &reference = str;

    EXPECT_EQ(4, reference.length());
    EXPECT_FALSE(reference.empty());
    EXPECT_EQ('b', reference.at(0));
}

// =====================================================================
// operator[]
// =====================================================================

TEST(MyStringSubscript, ReturnsEachCharacter) {
    MyString str = make_string("abc");

    EXPECT_EQ('a', str[0]);
    EXPECT_EQ('b', str[1]);
    EXPECT_EQ('c', str[2]);
}

TEST(MyStringSubscript, MatchesAt) {
    MyString str = make_string("hello world");

    for (int index = 0; index < str.length(); ++index) {
        EXPECT_EQ(str.at(index), str[index]) << "index: " << index;
    }
}

TEST(MyStringSubscript, ReflectsSetChar) {
    MyString str = make_string("abc");

    str.setChar('z', 2);

    EXPECT_EQ('z', str[2]);
}

// =====================================================================
// operator==
// =====================================================================

TEST(MyStringEquality, TrueForSameContent) {
    MyString a = make_string("hello");
    MyString b = make_string("hello");

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b == a);
}

TEST(MyStringEquality, FalseForDifferentContent) {
    MyString a = make_string("hello");
    MyString b = make_string("world");

    EXPECT_FALSE(a == b);
}

TEST(MyStringEquality, FalseForDifferentLength) {
    MyString a = make_string("hello");
    MyString b = make_string("hello!");

    EXPECT_FALSE(a == b);
    EXPECT_FALSE(b == a);
}

TEST(MyStringEquality, TrueForSelf) {
    MyString a = make_string("self");

    EXPECT_TRUE(a == a);
}

TEST(MyStringEquality, TrueForTwoEmptyStrings) {
    MyString a;
    MyString b;

    EXPECT_TRUE(a == b);
}

// =====================================================================
// operator!=
// =====================================================================

TEST(MyStringInequality, FalseForSameContent) {
    MyString a = make_string("hello");
    MyString b = make_string("hello");

    EXPECT_FALSE(a != b);
}

TEST(MyStringInequality, TrueForDifferentContent) {
    MyString a = make_string("hello");
    MyString b = make_string("world");

    EXPECT_TRUE(a != b);
    EXPECT_TRUE(b != a);
}

TEST(MyStringInequality, TrueForDifferentLength) {
    MyString a = make_string("hello");
    MyString b = make_string("hello!");

    EXPECT_TRUE(a != b);
}

TEST(MyStringInequality, TrueForEmptyAndNonEmpty) {
    MyString empty;
    MyString text = make_string("a");

    EXPECT_TRUE(empty != text);
    EXPECT_TRUE(text != empty);
}

TEST(MyStringInequality, FalseForTwoEmptyStrings) {
    MyString a;
    MyString b;

    EXPECT_FALSE(a != b);
}

TEST(MyStringInequality, TrueWhenOnlySomeCharactersDiffer) {
    // 첫 글자는 같고 뒤에서 다른 경우
    MyString a = make_string("abc");
    MyString b = make_string("abd");

    EXPECT_TRUE(a != b);
}

TEST(MyStringInequality, DetectsDifferenceAtLastCharacter) {
    MyString a = make_string("abcde");
    MyString b = make_string("abcdf");

    EXPECT_TRUE(a != b);
    EXPECT_FALSE(a == b);
}

TEST(MyStringInequality, AlwaysOppositeOfEquality) {
    const char *samples[] = {"", "a", "ab", "abc", "abd", "hello", "123456789012345"};

    for (const char *left_text: samples) {
        for (const char *right_text: samples) {
            MyString left = make_string(left_text);
            MyString right = make_string(right_text);

            EXPECT_EQ(!(left == right), left != right)
                << "left: " << left_text << ", right: " << right_text;
        }
    }
}

// =====================================================================
// operator= (MyString)
// 헤더상 void를 반환하므로 c = b = a 같은 연쇄 대입은 지원하지 않는다.
// =====================================================================

TEST(MyStringAssignment, CopiesContent) {
    MyString a = make_string("source");
    MyString b = make_string("target");

    b = a;

    EXPECT_TRUE(a == b);
    EXPECT_EQ("source", to_std_string(b));
}

TEST(MyStringAssignment, ShorterSourceReplacesLongerTarget) {
    MyString a = make_string("ab");
    MyString b = make_string("longer text");

    b = a;

    EXPECT_EQ(2, b.length());
    EXPECT_EQ("ab", to_std_string(b));
}

TEST(MyStringAssignment, FromEmptyMakesTargetEmpty) {
    MyString empty;
    MyString target = make_string("something");

    target = empty;

    EXPECT_TRUE(target.empty());
    EXPECT_EQ(0, target.length());
}

TEST(MyStringAssignment, ToSelfKeepsContent) {
    MyString a = make_string("self");
    MyString &reference = a;

    a = reference;

    EXPECT_EQ("self", to_std_string(a));
}

TEST(MyStringAssignment, IsIndependentFromSource) {
    MyString a = make_string("abc");
    MyString b;

    b = a;
    b.setChar('z', 0);

    EXPECT_EQ('a', a.at(0));
    EXPECT_EQ('z', b.at(0));
}

// =====================================================================
// operator= (const char[])
// =====================================================================

TEST(MyStringCharArrayAssignment, ReplacesContent) {
    MyString str = make_string("old");

    str = "new content";

    EXPECT_EQ(11, str.length());
    EXPECT_EQ("new content", to_std_string(str));
}

TEST(MyStringCharArrayAssignment, ShorterInputReplacesLongerContent) {
    MyString str = make_string("long content");

    str = "abc";

    EXPECT_EQ(3, str.length());
    EXPECT_EQ("abc", to_std_string(str));
}

TEST(MyStringCharArrayAssignment, EmptyInputMakesStringEmpty) {
    MyString str = make_string("something");

    str = "";

    EXPECT_TRUE(str.empty());
}

TEST(MyStringCharArrayAssignment, TruncatesLongInput) {
    MyString str;

    str = "1234567890123456789";

    EXPECT_EQ(MAX_LENGTH, str.length());
    EXPECT_EQ("123456789012345", to_std_string(str));
}

TEST(MyStringCharArrayAssignment, ReturnsReferenceToItself) {
    MyString str;

    MyString &returned = (str = "abc");

    EXPECT_EQ(&str, &returned);
}

TEST(MyStringCharArrayAssignment, CanBeChained) {
    MyString a;
    MyString b;

    b = a = "chain";

    EXPECT_EQ("chain", to_std_string(a));
    EXPECT_EQ("chain", to_std_string(b));
}

// =====================================================================
// operator+
// 헤더상 const MyString을 반환하고 operator+/==가 const 멤버 함수가 아니므로,
// (a + b) + "c" 나 (a + b) == c 처럼 결과에 바로 연산자를 이어 쓸 수 없다.
// 결과를 변수에 담은 뒤 검사한다.
// =====================================================================

TEST(MyStringPlus, WithMyString) {
    MyString a = make_string("abc");
    MyString b = make_string("def");

    MyString result = a + b;

    EXPECT_EQ("abcdef", to_std_string(result));
    EXPECT_EQ(6, result.length());
}

TEST(MyStringPlus, WithCharArray) {
    MyString a = make_string("abc");

    MyString result = a + "def";

    EXPECT_EQ("abcdef", to_std_string(result));
    EXPECT_EQ(6, result.length());
}

TEST(MyStringPlus, DoesNotModifyOperands) {
    MyString a = make_string("abc");
    MyString b = make_string("def");

    MyString result = a + b;
    (void) result;

    EXPECT_EQ("abc", to_std_string(a));
    EXPECT_EQ("def", to_std_string(b));
}

TEST(MyStringPlus, CharArrayDoesNotModifyOperand) {
    MyString a = make_string("abc");

    MyString result = a + "def";
    (void) result;

    EXPECT_EQ("abc", to_std_string(a));
    EXPECT_EQ(3, a.length());
}

TEST(MyStringPlus, WithEmptyReturnsSameContent) {
    MyString a = make_string("abc");
    MyString empty;

    MyString right_empty = a + empty;
    MyString left_empty = empty + a;

    EXPECT_TRUE(right_empty == a);
    EXPECT_TRUE(left_empty == a);
}

TEST(MyStringPlus, EmptyCharArray) {
    MyString a = make_string("abc");

    MyString result = a + "";

    EXPECT_EQ("abc", to_std_string(result));
}

TEST(MyStringPlus, TruncatesAtMaxLength) {
    MyString a = make_string("1234567890");
    MyString b = make_string("abcdefghij");

    MyString result = a + b;

    EXPECT_EQ(MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(MyStringPlus, CharArrayTruncatesAtMaxLength) {
    MyString a = make_string("1234567890");

    MyString result = a + "abcdefghij";

    EXPECT_EQ(MAX_LENGTH, result.length());
    EXPECT_EQ("1234567890abcde", to_std_string(result));
}

TEST(MyStringPlus, ExactlyMaxLengthIsNotTruncated) {
    MyString a = make_string("12345678");
    MyString b = make_string("1234567");

    MyString result = a + b;

    EXPECT_EQ(MAX_LENGTH, result.length());
    EXPECT_EQ("123456781234567", to_std_string(result));
}

TEST(MyStringPlus, OneOverMaxLengthIsTruncated) {
    MyString a = make_string("12345678");
    MyString b = make_string("12345678");

    MyString result = a + b;

    EXPECT_EQ(MAX_LENGTH, result.length());
    EXPECT_EQ("123456781234567", to_std_string(result));
}

TEST(MyStringPlus, ToFullStringKeepsContent) {
    MyString a = make_string("123456789012345");

    MyString result = a + "xyz";

    EXPECT_EQ(MAX_LENGTH, result.length());
    EXPECT_EQ("123456789012345", to_std_string(result));
}

TEST(MyStringPlus, SelfConcatenation) {
    MyString a = make_string("ab");

    MyString result = a + a;

    EXPECT_EQ("abab", to_std_string(result));
    EXPECT_EQ("ab", to_std_string(a));
}

TEST(MyStringPlus, StepByStepConcatenation) {
    MyString a = make_string("a");
    MyString b = make_string("b");

    MyString ab = a + b;
    MyString result = ab + "c";

    EXPECT_EQ("abc", to_std_string(result));
}

TEST(MyStringPlus, ResultIsIndependentFromOperands) {
    MyString a = make_string("abc");
    MyString b = make_string("def");

    MyString result = a + b;
    result.setChar('z', 0);

    EXPECT_EQ('a', a.at(0));
    EXPECT_EQ('z', result.at(0));
}

TEST(MyStringPlus, AssignedResultMatchesDirectResult) {
    MyString a = make_string("abc");
    MyString b = make_string("def");
    MyString assigned;

    assigned = a + b;

    EXPECT_TRUE(assigned == (a + b));
}

// =====================================================================
// 길이 경계 (최대 15글자)
// =====================================================================

TEST(MyStringBoundary, ExactlyMaxLengthIsKept) {
    MyString str = make_string("123456789012345");

    EXPECT_EQ(MAX_LENGTH, str.length());
    EXPECT_EQ("123456789012345", to_std_string(str));
}

TEST(MyStringBoundary, OneOverMaxLengthIsTruncated) {
    MyString str = make_string("1234567890123456");

    EXPECT_EQ(MAX_LENGTH, str.length());
    EXPECT_EQ("123456789012345", to_std_string(str));
}

TEST(MyStringBoundary, EmptyCharArrayCreatesEmptyString) {
    MyString str = make_string("");

    EXPECT_EQ(0, str.length());
    EXPECT_TRUE(str.empty());
}

TEST(MyStringBoundary, NonEmptyStringIsNotEmpty) {
    MyString str = make_string("a");

    EXPECT_EQ(1, str.length());
    EXPECT_FALSE(str.empty());
}

TEST(MyStringBoundary, LengthIsContentLengthNotCapacity) {
    // 과제 조건: 전체 용량이 아니라 문자열 자체의 길이를 반환
    MyString str = make_string("abc");

    EXPECT_EQ(3, str.length());
}

// =====================================================================
// operator<< (utils.h)
// =====================================================================

TEST(MyStringOutputOperator, PrintsContent) {
    MyString str = make_string("hello");
    std::ostringstream stream;

    stream << str;

    EXPECT_EQ("hello", stream.str());
}

TEST(MyStringOutputOperator, PrintsNothingForEmptyString) {
    MyString str;
    std::ostringstream stream;

    stream << str;

    EXPECT_EQ("", stream.str());
}

TEST(MyStringOutputOperator, DoesNotAppendNewline) {
    MyString str = make_string("abc");
    std::ostringstream stream;

    stream << str;

    EXPECT_EQ(std::string::npos, stream.str().find('\n'));
}

TEST(MyStringOutputOperator, CanBeChainedWithOtherOutput) {
    MyString a = make_string("abc");
    MyString b = make_string("def");
    std::ostringstream stream;

    stream << a << "," << b << "!";

    EXPECT_EQ("abc,def!", stream.str());
}

TEST(MyStringOutputOperator, PrintsConcatenatedResult) {
    MyString a = make_string("abc");
    std::ostringstream stream;

    stream << (a + "def");

    EXPECT_EQ("abcdef", stream.str());
}

TEST(MyStringOutputOperator, PrintsTruncatedContent) {
    MyString str = make_string("1234567890123456789");
    std::ostringstream stream;

    stream << str;

    EXPECT_EQ("123456789012345", stream.str());
}

// =====================================================================
// operator>> (utils.h)
// =====================================================================

TEST(MyStringInputOperator, ReadsWord) {
    std::istringstream input("hello");
    MyString str;

    input >> str;

    EXPECT_EQ("hello", to_std_string(str));
}

TEST(MyStringInputOperator, StopsAtWhitespace) {
    std::istringstream input("hello world");
    MyString first;
    MyString second;

    input >> first >> second;

    EXPECT_EQ("hello", to_std_string(first));
    EXPECT_EQ("world", to_std_string(second));
}

TEST(MyStringInputOperator, SkipsLeadingWhitespace) {
    std::istringstream input("   \n\t hello");
    MyString str;

    input >> str;

    EXPECT_EQ("hello", to_std_string(str));
}

TEST(MyStringInputOperator, StopsAtNewline) {
    std::istringstream input("first\nsecond");
    MyString a;
    MyString b;

    input >> a >> b;

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ("second", to_std_string(b));
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

TEST(MyStringInputOperator, ReadsExactlyMaxLengthFully) {
    std::istringstream input("123456789012345");
    MyString str;

    input >> str;

    EXPECT_EQ(MAX_LENGTH, str.length());
    EXPECT_EQ("123456789012345", to_std_string(str));
}

TEST(MyStringInputOperator, TruncatesLongInput) {
    std::istringstream input("1234567890123456789");
    MyString str;

    input >> str;

    EXPECT_EQ(MAX_LENGTH, str.length());
    EXPECT_EQ("123456789012345", to_std_string(str));
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

TEST(MyStringInputOperator, OverwritesExistingContent) {
    std::istringstream input("new");
    MyString str = make_string("previous");

    input >> str;

    EXPECT_EQ("new", to_std_string(str));
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
// getline (utils.h)
// =====================================================================

TEST(MyStringGetLine, ReadsWholeLineIncludingSpaces) {
    std::istringstream input("hello world\n");
    MyString str;

    getline(input, str);

    EXPECT_EQ("hello world", to_std_string(str));
}

TEST(MyStringGetLine, DoesNotIncludeDelimiter) {
    std::istringstream input("abc\n");
    MyString str;

    getline(input, str);

    EXPECT_EQ(3, str.length());
    EXPECT_EQ(std::string::npos, to_std_string(str).find('\n'));
}

TEST(MyStringGetLine, ConsumesDelimiterSoNextLineIsRead) {
    std::istringstream input("first\nsecond\n");
    MyString a;
    MyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_EQ("first", to_std_string(a));
    EXPECT_EQ("second", to_std_string(b));
}

TEST(MyStringGetLine, KeepsLeadingWhitespace) {
    std::istringstream input("  hi\n");
    MyString str;

    getline(input, str);

    EXPECT_EQ("  hi", to_std_string(str));
}

TEST(MyStringGetLine, EmptyLineGivesEmptyString) {
    std::istringstream input("\nnext\n");
    MyString a;
    MyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_TRUE(a.empty());
    EXPECT_EQ("next", to_std_string(b));
}

TEST(MyStringGetLine, LastLineWithoutDelimiterIsRead) {
    std::istringstream input("only line");
    MyString str;

    getline(input, str);

    EXPECT_EQ("only line", to_std_string(str));
}

TEST(MyStringGetLine, CustomDelimiter) {
    std::istringstream input("a,b,c");
    MyString a;
    MyString b;
    MyString c;

    getline(input, a, ',');
    getline(input, b, ',');
    getline(input, c, ',');

    EXPECT_EQ("a", to_std_string(a));
    EXPECT_EQ("b", to_std_string(b));
    EXPECT_EQ("c", to_std_string(c));
}

TEST(MyStringGetLine, NewlineIsNotDelimiterWhenCustomDelimiterGiven) {
    std::istringstream input("ab\ncd,ef");
    MyString str;

    getline(input, str, ',');

    EXPECT_EQ("ab\ncd", to_std_string(str));
}

TEST(MyStringGetLine, ExactlyMaxLengthIsKept) {
    std::istringstream input("123456789012345\nnext\n");
    MyString a;
    MyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_EQ("123456789012345", to_std_string(a));
    EXPECT_EQ("next", to_std_string(b));
}

// 과제 조건: delim까지 모든 글자를 읽되 16번째 글자부터는 버린다.
TEST(MyStringGetLine, DiscardsCharactersBeyondLimit) {
    std::istringstream input("1234567890123456789\nnext\n");
    MyString a;
    MyString b;

    getline(input, a);
    getline(input, b);

    EXPECT_EQ(MAX_LENGTH, a.length());
    EXPECT_EQ("123456789012345", to_std_string(a));
    EXPECT_EQ("next", to_std_string(b));
}

TEST(MyStringGetLine, OverwritesExistingContent) {
    std::istringstream input("new\n");
    MyString str = make_string("previous");

    getline(input, str);

    EXPECT_EQ("new", to_std_string(str));
}

TEST(MyStringGetLine, ReturnsStreamForChaining) {
    std::istringstream input("a\nb\n");
    MyString first;
    MyString second;

    std::istream &returned = getline(input, first);
    getline(returned, second);

    EXPECT_EQ("a", to_std_string(first));
    EXPECT_EQ("b", to_std_string(second));
}

TEST(MyStringGetLine, MixesWithExtractionOperator) {
    // >>로 단어를 읽은 뒤 같은 스트림에서 getline이 다음 줄을 읽을 수 있어야 함
    std::istringstream input("word\nrest of line\n");
    MyString word;
    MyString line;

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
    MyString str = make_string("hello world");

    EXPECT_EQ("hello", to_std_string(str.substr(0, 5)));
}

TEST(MyStringSubstr, FromMiddle) {
    MyString str = make_string("hello world");

    EXPECT_EQ("world", to_std_string(str.substr(6, 5)));
}

TEST(MyStringSubstr, SingleCharacter) {
    MyString str = make_string("abc");

    EXPECT_EQ("b", to_std_string(str.substr(1, 1)));
}

TEST(MyStringSubstr, WholeString) {
    MyString str = make_string("abcde");

    EXPECT_EQ("abcde", to_std_string(str.substr(0, 5)));
}

TEST(MyStringSubstr, LastCharacter) {
    MyString str = make_string("abcde");

    EXPECT_EQ("e", to_std_string(str.substr(4, 1)));
}

TEST(MyStringSubstr, ZeroLengthGivesEmpty) {
    MyString str = make_string("hello");

    EXPECT_TRUE(str.substr(2, 0).empty());
}

// 과제 조건: len이 맞지 않으면 std::string::substr처럼 끝까지만 반환
TEST(MyStringSubstr, LengthBeyondEndStopsAtEnd) {
    MyString str = make_string("hello world");

    EXPECT_EQ("world", to_std_string(str.substr(6, 100)));
}

TEST(MyStringSubstr, LengthOneBeyondEndStopsAtEnd) {
    MyString str = make_string("abc");

    EXPECT_EQ("bc", to_std_string(str.substr(1, 3)));
}

// 과제 조건: pos가 음수이면 0으로 취급
TEST(MyStringSubstr, NegativePositionIsTreatedAsZero) {
    MyString str = make_string("hello");

    EXPECT_EQ("he", to_std_string(str.substr(-3, 2)));
}

TEST(MyStringSubstr, VeryNegativePositionIsTreatedAsZero) {
    MyString str = make_string("hello");

    EXPECT_EQ("hello", to_std_string(str.substr(-100, 5)));
}

// 과제 조건: 문자열 범위 밖이면 빈 문자열
TEST(MyStringSubstr, PositionBeyondEndGivesEmpty) {
    MyString str = make_string("hello");

    EXPECT_TRUE(str.substr(10, 2).empty());
}

TEST(MyStringSubstr, PositionAtEndGivesEmpty) {
    // std::string::substr(size(), n)도 빈 문자열을 반환
    MyString str = make_string("hello");

    EXPECT_TRUE(str.substr(5, 3).empty());
}

TEST(MyStringSubstr, EmptySourceGivesEmpty) {
    MyString empty;

    EXPECT_TRUE(empty.substr(0, 3).empty());
}

TEST(MyStringSubstr, DoesNotModifyOriginal) {
    MyString str = make_string("hello");

    MyString result = str.substr(1, 3);
    (void) result;

    EXPECT_EQ("hello", to_std_string(str));
}

TEST(MyStringSubstr, ResultIsIndependentFromOriginal) {
    MyString str = make_string("hello");

    MyString result = str.substr(0, 3);
    result.setChar('z', 0);

    EXPECT_EQ('h', str.at(0));
    EXPECT_EQ('z', result.at(0));
}

TEST(MyStringSubstr, ResultLengthMatchesContent) {
    MyString str = make_string("hello world");

    EXPECT_EQ(5, str.substr(0, 5).length());
    EXPECT_EQ(5, str.substr(6, 100).length());
    EXPECT_EQ(0, str.substr(20, 3).length());
}

TEST(MyStringSubstr, FromMaxLengthString) {
    MyString str = make_string("123456789012345");

    EXPECT_EQ("12345", to_std_string(str.substr(10, 5)));
    EXPECT_EQ("5", to_std_string(str.substr(14, 1)));
}

TEST(MyStringSubstr, MatchesStdStringForManyRanges) {
    const std::string text = "hello world";
    MyString str = make_string(text.c_str());

    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
        for (int len = 0; len <= 15; ++len) {
            EXPECT_EQ(text.substr(static_cast<std::size_t>(pos), static_cast<std::size_t>(len)),
                      to_std_string(str.substr(pos, len)))
                << "pos: " << pos << ", len: " << len;
        }
    }
}

TEST(MyStringSubstr, CanBeConcatenated) {
    MyString str = make_string("hello world");

    MyString result = str.substr(0, 5) + str.substr(5, 6);

    EXPECT_EQ("hello world", to_std_string(result));
}

// =====================================================================
// find(const MyString&, int)
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

TEST(MyStringFind, DefaultStartIsZero) {
    MyString str = make_string("abcabc");

    EXPECT_EQ(0, str.find(make_string("abc")));
    EXPECT_EQ(1, str.find("bca"));
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
    MyString str = make_string("hello");

    str.find(make_string("ll"), 0);
    str.find("ll", 0);

    EXPECT_EQ("hello", to_std_string(str));
    EXPECT_EQ(5, str.length());
}

TEST(MyStringFind, DoesNotModifyPattern) {
    MyString str = make_string("hello");
    MyString pattern = make_string("ll");

    str.find(pattern, 0);

    EXPECT_EQ("ll", to_std_string(pattern));
}

TEST(MyStringFind, StringCanBeUsedAsItsOwnPattern) {
    MyString str = make_string("hello");

    EXPECT_EQ(0, str.find(str, 0));
}

TEST(MyStringFind, WorksOnConcatenatedResult) {
    MyString a = make_string("hello ");
    MyString b = make_string("world");

    MyString joined = a + b;

    EXPECT_EQ(6, joined.find(make_string("world"), 0));
    EXPECT_EQ(4, joined.find(make_string("o w"), 0));
}

TEST(MyStringFind, WorksOnSubstrResult) {
    MyString str = make_string("hello world");

    MyString part = str.substr(6, 5);

    EXPECT_EQ(2, part.find(make_string("rl"), 0));
    EXPECT_EQ(-1, part.find(make_string("hello"), 0));
}

TEST(MyStringFind, WorksOnAssignedString) {
    MyString source = make_string("find me");
    MyString target = make_string("other");

    target = source;

    EXPECT_EQ(5, target.find(make_string("me"), 0));
}

TEST(MyStringFind, WorksOnCharArrayAssignedString) {
    MyString str;

    str = "find me";

    EXPECT_EQ(5, str.find("me", 0));
}

TEST(MyStringFind, WorksOnInputString) {
    std::istringstream input("needle haystack");
    MyString first;
    MyString second;

    input >> first >> second;

    EXPECT_EQ(3, second.find(make_string("st"), 0));
    EXPECT_EQ(-1, first.find(make_string("st"), 0));
}

TEST(MyStringFind, WorksOnGetLineString) {
    std::istringstream input("find in line\n");
    MyString line;

    getline(input, line);

    EXPECT_EQ(1, line.find(make_string("in"), 0));
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
// find(const char[], int)
// =====================================================================

TEST(MyStringFindCharArray, FindsSubstring) {
    EXPECT_EQ(0, find_char_array_in("hello world", "hello", 0));
    EXPECT_EQ(3, find_char_array_in("hello world", "lo w", 0));
    EXPECT_EQ(6, find_char_array_in("hello world", "world", 0));
}

TEST(MyStringFindCharArray, ReturnsMinusOneWhenNotFound) {
    EXPECT_EQ(-1, find_char_array_in("hello", "xyz", 0));
    EXPECT_EQ(-1, find_char_array_in("abc", "abcd", 0));
    EXPECT_EQ(-1, find_char_array_in("", "a", 0));
}

TEST(MyStringFindCharArray, StartRules) {
    EXPECT_EQ(0, find_char_array_in("abcabc", "abc", -5));
    EXPECT_EQ(3, find_char_array_in("abcabc", "abc", 1));
    EXPECT_EQ(-1, find_char_array_in("abcabc", "abc", 10));
}

TEST(MyStringFindCharArray, AgreesWithMyStringOverload) {
    const char *texts[] = {"", "a", "abc", "abcabc", "aaaa", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "b", "abc", "aa", "ll", "o w", "345", "zzz", "123456789012345"};

    for (const char *text: texts) {
        for (const char *pattern: patterns) {
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(find_in(text, pattern, start), find_char_array_in(text, pattern, start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}

// =====================================================================
// CharSequence 참조를 통한 find 호출
// find(const char[], int)는 virtual이므로 MyString의 구현이 호출되고,
// find(const CharSequence&, int)는 MyString에서 가려지므로 기반 참조로만 호출할 수 있다.
// =====================================================================

TEST(MyStringInheritedFind, CharArrayOverloadThroughBaseReference) {
    MyString str = make_string("hello world");
    const CharSequence &base = str;

    EXPECT_EQ(2, base.find("ll", 0));
    EXPECT_EQ(-1, base.find("xyz", 0));
}

TEST(MyStringInheritedFind, CharSequenceOverloadThroughBaseReference) {
    MyString str = make_string("hello world");
    MyString world = make_string("world");
    MyString missing = make_string("xyz");
    const CharSequence &base = str;

    EXPECT_EQ(6, base.find(static_cast<const CharSequence &>(world), 0));
    EXPECT_EQ(-1, base.find(static_cast<const CharSequence &>(missing), 0));
}

TEST(MyStringInheritedFind, StartRulesApplyThroughBaseReference) {
    MyString str = make_string("abcabc");
    const CharSequence &base = str;

    EXPECT_EQ(0, base.find("abc", -5));
    EXPECT_EQ(3, base.find("abc", 1));
    EXPECT_EQ(-1, base.find("abc", 10));
}

TEST(MyStringInheritedFind, BaseAndDerivedFindAgree) {
    const char *texts[] = {"", "a", "abcabc", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "abc", "ll", "o w", "345", "zzz"};

    for (const char *text: texts) {
        MyString str = make_string(text);
        const CharSequence &base = str;
        for (const char *pattern: patterns) {
            MyString pattern_string = make_string(pattern);
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(base.find(static_cast<const CharSequence &>(pattern_string), start),
                          str.find(pattern_string, start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}
