#include <gtest/gtest.h>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "MyString.h"

// 기준: 7주차.프로젝트1.MyString.pdf (명시되지 않은 동작은 std::string과 동일)
//
// 잘못된 메모리 접근이 나면 실행 파일이 통째로 죽으므로 테스트별로 프로세스가 분리되는 ctest를 권장한다.
//   ctest --test-dir cmake-build-debug -R '^MyString' --output-on-failure

namespace {
    std::string to_std_string(const MyString &str) {
        std::ostringstream stream;
        stream << str;
        return stream.str();
    }

    // 내용뿐 아니라 평가 항목의 불변식(length, 끝의 '\0', capacity)까지 함께 검사하고,
    // 실패하면 Expected / Actual 값을 출력한다. EXPECT_SAME, ASSERT_SAME으로 호출한다.
    testing::AssertionResult same_format(
        const char *actual_expression, const char *,
        const MyString &actual, const std::string &expected
    ) {
        const std::string text = to_std_string(actual);
        const int expected_length = static_cast<int>(expected.size());
        const int terminator = static_cast<unsigned char>(actual[actual.length()]);
        const bool valid_capacity = actual.getCapacity() % 8 == 0 && actual.getCapacity() >= actual.length();

        if (text == expected && actual.length() == expected_length && terminator == 0 && valid_capacity) {
            return testing::AssertionSuccess();
        }

        testing::AssertionResult failure = testing::AssertionFailure();
        failure << actual_expression << '\n'
            << "  Expected: " << testing::PrintToString(expected) << " (length " << expected_length << ")\n"
            << "  Actual:   " << testing::PrintToString(text) << " (length " << actual.length() << ")";
        if (terminator != 0) {
            failure << "\n  끝 문자 Expected: 0, Actual: " << terminator;
        }
        if (!valid_capacity) {
            failure << "\n  capacity Expected: 8의 배수이면서 length 이상, Actual: " << actual.getCapacity();
        }
        return failure;
    }

#define EXPECT_SAME(actual, expected) EXPECT_PRED_FORMAT2(same_format, actual, expected)
#define ASSERT_SAME(actual, expected) ASSERT_PRED_FORMAT2(same_format, actual, expected)

    std::string describe(const MyString &str) {
        return testing::PrintToString(to_std_string(str));
    }

    std::string describe(const char *str) {
        return testing::PrintToString(std::string(str));
    }

    // 비교 연산자가 컴파일되지 않아도(상속 모호성 등) 나머지 테스트는 돌 수 있도록,
    // 사용 가능 여부를 먼저 감지하고 불가능하면 컴파일 오류 대신 테스트 실패로 보고한다.
#define DEFINE_COMPARISON(name, op)                                                                     \
    template<typename L, typename R, typename = void>                                                   \
    struct has_##name : std::false_type {};                                                             \
                                                                                                        \
    template<typename L, typename R>                                                                    \
    struct has_##name<L, R, std::void_t<decltype(std::declval<const L &>() op std::declval<const R &>())>> \
        : std::true_type {};                                                                            \
                                                                                                        \
    template<typename L, typename R>                                                                    \
    testing::AssertionResult name(const L &left, const R &right) {                                      \
        if constexpr (has_##name<L, R>::value) {                                                        \
            testing::AssertionResult result(static_cast<bool>(left op right));                          \
            result << describe(left) << " " #op " " << describe(right);                                 \
            return result;                                                                              \
        } else {                                                                                        \
            ADD_FAILURE() << "operator" #op " 를 호출할 수 없음 (컴파일 불가)";                         \
            return testing::AssertionFailure() << "호출 불가";                                          \
        }                                                                                               \
    }

    DEFINE_COMPARISON(eq, ==)
    DEFINE_COMPARISON(ne, !=)
    DEFINE_COMPARISON(lt, <)
    DEFINE_COMPARISON(le, <=)
    DEFINE_COMPARISON(gt, >)
    DEFINE_COMPARISON(ge, >=)

#undef DEFINE_COMPARISON

    template<typename T>
    constexpr bool is_mystring_ref = std::is_same_v<T, MyString &>;
}

// =====================================================================
// 기본 기능: 생성자 / 복사 / 대입
// =====================================================================

TEST(MyStringConstructor, DefaultIsEmptyWithCapacity8) {
    MyString str;

    EXPECT_SAME(str, "");
    EXPECT_TRUE(str.isEmpty());
    EXPECT_EQ(0, str.length());
    EXPECT_EQ(8, str.getCapacity());
}

TEST(MyStringConstructor, FromCString) {
    MyString str("hello");

    EXPECT_SAME(str, "hello");
    EXPECT_FALSE(str.isEmpty());
}

TEST(MyStringConstructor, FromEmptyCString) {
    MyString str("");

    EXPECT_SAME(str, "");
    EXPECT_TRUE(str.isEmpty());
}

TEST(MyStringConstructor, FromCStringIsDeepCopy) {
    char source[] = "hello";
    MyString str(source);

    source[0] = 'J';

    EXPECT_SAME(str, "hello");
}

TEST(MyStringConstructor, FromLongCString) {
    const std::string text(100, 'x');

    EXPECT_SAME(MyString(text.c_str()), text);
}

TEST(MyStringCopy, CopyConstructorCopiesContent) {
    const MyString source("hello");
    MyString copy(source);

    EXPECT_SAME(copy, "hello");
    EXPECT_EQ(source.getCapacity(), copy.getCapacity());
}

TEST(MyStringCopy, CopyConstructorIsDeepCopy) {
    MyString source("hello");
    MyString copy(source);

    copy[0] = 'J';
    copy.append("!!!");

    EXPECT_SAME(source, "hello");
    EXPECT_SAME(copy, "Jello!!!");
}

TEST(MyStringCopy, CopySurvivesSourceDestruction) {
    MyString *source = new MyString("hello");
    MyString copy(*source);

    delete source;

    EXPECT_SAME(copy, "hello");
}

TEST(MyStringAssign, AssignmentIsDeepCopy) {
    MyString source("hello");
    MyString target("previous value");

    target = source;
    target[0] = 'J';

    EXPECT_SAME(source, "hello");
    EXPECT_SAME(target, "Jello");
}

TEST(MyStringAssign, SelfAssignmentKeepsContent) {
    MyString str("hello");
    const MyString &alias = str;

    str = alias;

    EXPECT_SAME(str, "hello");
}

TEST(MyStringAssign, AssignmentCanBeChained) {
    MyString first, second;
    const MyString source("hello");

    first = second = source;

    EXPECT_SAME(first, "hello");
    EXPECT_SAME(second, "hello");
}

TEST(MyStringAssign, FromCString) {
    MyString str("previous value");

    str = "hello";

    EXPECT_SAME(str, "hello");
}

TEST(MyStringAssign, FromCStringIsDeepCopy) {
    char source[] = "hello";
    MyString str;

    str = source;
    source[0] = 'J';

    EXPECT_SAME(str, "hello");
}

TEST(MyStringAssign, FromCStringReturnsMyStringRef) {
    EXPECT_TRUE(is_mystring_ref<decltype(std::declval<MyString &>() = "x")>);
}

TEST(MyStringAssign, FromOwnSubstring) {
    MyString str("hello");

    str = str.substr(1, 3);

    EXPECT_SAME(str, "ell");
}

// =====================================================================
// 기본 기능: capacity
// =====================================================================

TEST(MyStringCapacity, AlwaysMultipleOf8) {
    for (int length = 0; length <= 100; ++length) {
        const std::string text(length, 'x');
        const MyString str(text.c_str());

        EXPECT_EQ(0, str.getCapacity() % 8) << "length: " << length;
        EXPECT_GE(str.getCapacity(), length) << "length: " << length;
    }
}

TEST(MyStringCapacity, GrowsByDoubling) {
    MyString str;
    int capacity = str.getCapacity();

    for (int count = 0; count < 300; ++count) {
        str.append("x");

        if (str.getCapacity() != capacity) {
            EXPECT_EQ(capacity * 2, str.getCapacity()) << "length: " << str.length();
            capacity = str.getCapacity();
        }
    }

    EXPECT_SAME(str, std::string(300, 'x'));
}

TEST(MyStringCapacity, BulkAppendKeepsMultipleOf8) {
    const std::string text(100, 'x');
    MyString str;

    str.append(text.c_str());

    EXPECT_SAME(str, text);
}

// =====================================================================
// 기본 기능: at / operator[]
// =====================================================================

TEST(MyStringAccess, AtReturnsCharacter) {
    const MyString str("hello");

    EXPECT_EQ('h', str.at(0));
    EXPECT_EQ('o', str.at(4));
}

TEST(MyStringAccess, AtReturnsWritableReference) {
    MyString str("hello");

    str.at(0) = 'J';

    EXPECT_SAME(str, "Jello");
}

TEST(MyStringAccess, AtThrowsWhenOutOfRange) {
    MyString str("hello");

    EXPECT_THROW(str.at(-1), std::out_of_range);
    EXPECT_THROW(str.at(5), std::out_of_range);
    EXPECT_THROW(str.at(100), std::out_of_range);
}

TEST(MyStringAccess, SubscriptReturnsCharacter) {
    const MyString str("hello");

    EXPECT_EQ('h', str[0]);
    EXPECT_EQ('o', str[4]);
}

TEST(MyStringAccess, SubscriptReturnsWritableReference) {
    MyString str("hello");

    str[0] = 'J';

    EXPECT_SAME(str, "Jello");
}

TEST(MyStringAccess, SubscriptAtLengthIsNullTerminator) {
    const MyString str("hello");

    EXPECT_EQ('\0', str[5]);
}

TEST(MyStringAccess, ConstSubscriptOutOfRangeReturnsNull) {
    const MyString str("hello");

    EXPECT_EQ('\0', str[-1]);
    EXPECT_EQ('\0', str[100]);
    EXPECT_EQ('\0', str[100000]);
}

TEST(MyStringAccess, NonConstSubscriptOutOfRangeReturnsNull) {
    MyString str("hello");

    EXPECT_EQ('\0', str[-1]);
    EXPECT_EQ('\0', str[100]);
    EXPECT_EQ('\0', str[100000]);
}

// =====================================================================
// 기본 기능: 입출력
// =====================================================================

TEST(MyStringOutput, PrintsContent) {
    std::ostringstream stream;

    stream << MyString("hello") << ' ' << MyString("") << MyString("world");

    EXPECT_EQ("hello world", stream.str());
}

TEST(MyStringInput, ReadsUntilWhitespace) {
    std::istringstream stream("hello world");
    MyString str;

    stream >> str;

    EXPECT_SAME(str, "hello");
    EXPECT_EQ(' ', stream.peek());
}

TEST(MyStringInput, SkipsLeadingWhitespace) {
    std::istringstream stream(" \t\n  hello ");
    MyString str;

    stream >> str;

    EXPECT_SAME(str, "hello");
}

TEST(MyStringInput, ReadsConsecutiveTokens) {
    std::istringstream stream("one two\nthree");
    MyString first, second, third;

    stream >> first >> second >> third;

    EXPECT_SAME(first, "one");
    EXPECT_SAME(second, "two");
    EXPECT_SAME(third, "three");
}

TEST(MyStringInput, ReplacesPreviousContent) {
    std::istringstream stream("new");
    MyString str("previous value");

    stream >> str;

    EXPECT_SAME(str, "new");
}

TEST(MyStringInput, ReadsLongerThanCapacity) {
    const std::string text(100, 'x');
    std::istringstream stream(text + " rest");
    MyString str;

    stream >> str;

    EXPECT_SAME(str, text);
}

TEST(MyStringInput, LastTokenBeforeEofDoesNotFail) {
    std::istringstream stream("hello");
    MyString str;

    stream >> str;

    EXPECT_FALSE(stream.fail());
    EXPECT_TRUE(stream.eof());
    EXPECT_SAME(str, "hello");
}

TEST(MyStringInput, EmptyStreamFails) {
    std::istringstream stream("");
    MyString str;

    stream >> str;

    EXPECT_TRUE(stream.fail());
}

TEST(MyStringInput, WhitespaceOnlyStreamFails) {
    std::istringstream stream("   \n  ");
    MyString str;

    stream >> str;

    EXPECT_TRUE(stream.fail());
}

TEST(MyStringInput, WhileLoopReadsEveryToken) {
    std::istringstream stream("  one two  three\n");
    MyString str;
    int count = 0;

    while (stream >> str) {
        ++count;
    }

    EXPECT_EQ(3, count);
}

TEST(MyStringGetline, ReadsWholeLineWithSpaces) {
    std::istringstream stream("hello big world\nnext");
    MyString str;

    getline(stream, str);

    EXPECT_SAME(str, "hello big world");
}

TEST(MyStringGetline, ConsumesDelimiter) {
    std::istringstream stream("first\nsecond\n");
    MyString first, second;

    getline(stream, first);
    getline(stream, second);

    EXPECT_SAME(first, "first");
    EXPECT_SAME(second, "second");
}

TEST(MyStringGetline, CustomDelimiter) {
    std::istringstream stream("a b,c\nd,e");
    MyString first, second, third;

    getline(stream, first, ',');
    getline(stream, second, ',');
    getline(stream, third, ',');

    EXPECT_SAME(first, "a b");
    EXPECT_SAME(second, "c\nd");
    EXPECT_SAME(third, "e");
}

TEST(MyStringGetline, EmptyLine) {
    std::istringstream stream("\nnext");
    MyString str("previous value");

    getline(stream, str);

    EXPECT_FALSE(stream.fail());
    EXPECT_SAME(str, "");
}

TEST(MyStringGetline, KeepsLeadingWhitespace) {
    std::istringstream stream("  hello\n");
    MyString str;

    getline(stream, str);

    EXPECT_SAME(str, "  hello");
}

TEST(MyStringGetline, ReadsLongerThanCapacity) {
    const std::string text(100, 'x');
    std::istringstream stream(text + "\nnext");
    MyString str, next;

    getline(stream, str);
    getline(stream, next);

    EXPECT_SAME(str, text);
    EXPECT_SAME(next, "next");
}

TEST(MyStringGetline, LineWithoutDelimiterDoesNotFail) {
    std::istringstream stream("hello");
    MyString str;

    getline(stream, str);

    EXPECT_FALSE(stream.fail());
    EXPECT_TRUE(stream.eof());
    EXPECT_SAME(str, "hello");
}

TEST(MyStringGetline, EmptyStreamFails) {
    std::istringstream stream("");
    MyString str;

    getline(stream, str);

    EXPECT_TRUE(stream.fail());
}

TEST(MyStringGetline, WhileLoopReadsEveryLine) {
    std::istringstream stream("one\ntwo\nthree");
    MyString str;
    int count = 0;

    while (getline(stream, str)) {
        ++count;
    }

    EXPECT_EQ(3, count);
}

// =====================================================================
// 필수 기능: operator+
// =====================================================================

TEST(MyStringConcat, WithMyString) {
    const MyString left("hello "), right("world");

    EXPECT_SAME(left + right, "hello world");
    EXPECT_SAME(left, "hello ");
    EXPECT_SAME(right, "world");
}

TEST(MyStringConcat, WithCString) {
    const MyString left("hello ");

    EXPECT_SAME(left + "world", "hello world");
    EXPECT_SAME(left, "hello ");
}

TEST(MyStringConcat, WithEmpty) {
    const MyString str("hello");

    EXPECT_SAME(str + "", "hello");
    EXPECT_SAME(str + MyString(), "hello");
    EXPECT_SAME(MyString() + str, "hello");
}

TEST(MyStringConcat, Chained) {
    const MyString first("a"), second("c");

    EXPECT_SAME(first + "b" + second + "d", "abcd");
}

TEST(MyStringConcat, WithItself) {
    const MyString str("abc");

    EXPECT_SAME(str + str, "abcabc");
}

// =====================================================================
// 필수 기능: == / !=
// =====================================================================

TEST(MyStringEquality, WithMyString) {
    const MyString str("hello");

    EXPECT_TRUE(eq(str, MyString("hello")));
    EXPECT_FALSE(eq(str, MyString("hellp")));
    EXPECT_FALSE(eq(str, MyString("hell")));
    EXPECT_FALSE(eq(str, MyString("hello!")));

    EXPECT_FALSE(ne(str, MyString("hello")));
    EXPECT_TRUE(ne(str, MyString("hellp")));
}

TEST(MyStringEquality, WithCString) {
    const MyString str("hello");

    EXPECT_TRUE(eq(str, "hello"));
    EXPECT_FALSE(eq(str, "hellp"));
    EXPECT_FALSE(eq(str, "hell"));
    EXPECT_FALSE(eq(str, "hello!"));

    EXPECT_FALSE(ne(str, "hello"));
    EXPECT_TRUE(ne(str, "hellp"));
}

TEST(MyStringEquality, Empty) {
    EXPECT_TRUE(eq(MyString(), MyString("")));
    EXPECT_TRUE(eq(MyString(), ""));
    EXPECT_TRUE(ne(MyString(), "x"));
}

// =====================================================================
// 필수 기능: find
// =====================================================================

TEST(MyStringFind, CStringPattern) {
    const MyString str("hello world");

    EXPECT_EQ(0, str.find("hello"));
    EXPECT_EQ(6, str.find("world"));
    EXPECT_EQ(-1, str.find("xyz"));
}

TEST(MyStringFind, MyStringPattern) {
    const MyString str("hello world");

    EXPECT_EQ(0, str.find(MyString("hello")));
    EXPECT_EQ(6, str.find(MyString("world")));
    EXPECT_EQ(-1, str.find(MyString("xyz")));
}

TEST(MyStringFind, FromPosition) {
    const MyString str("abcabc");

    EXPECT_EQ(3, str.find("abc", 1));
    EXPECT_EQ(3, str.find(MyString("abc"), 1));
    EXPECT_EQ(3, str.find("abc", 3));
    EXPECT_EQ(-1, str.find("abc", 4));
}

TEST(MyStringFind, OverlappingPattern) {
    EXPECT_EQ(1, MyString("aaaa").find("aa", 1));
}

TEST(MyStringFind, PatternLongerThanText) {
    EXPECT_EQ(-1, MyString("abc").find("abcd"));
    EXPECT_EQ(-1, MyString("").find("a"));
}

TEST(MyStringFind, NegativePositionIsTreatedAsZero) {
    const MyString str("hello");

    EXPECT_EQ(0, str.find("hello", -3));
    EXPECT_EQ(0, str.find(MyString("hello"), -3));
}

TEST(MyStringFind, PositionOutOfRangeIsTreatedAsLastPosition) {
    const MyString str("hello");

    EXPECT_EQ(4, str.find("o", 100));
    EXPECT_EQ(4, str.find(MyString("o"), 100));
    EXPECT_EQ(-1, str.find("h", 100));
}

TEST(MyStringFind, EmptyPatternMatchesAtPositionLikeStdString) {
    const MyString str("hello");

    EXPECT_EQ(0, str.find(""));
    EXPECT_EQ(2, str.find("", 2));
    EXPECT_EQ(0, MyString("").find(""));
}

TEST(MyStringFind, MatchesStdStringForEveryValidPosition) {
    const std::string text = "the quick brown fox jumps over the lazy dog, the end";
    const MyString str(text.c_str());

    for (const char *pattern : {"the", "e", "dog", "fox jumps", "zzz", "d", "the end"}) {
        for (int pos = 0; pos < static_cast<int>(text.size()); ++pos) {
            const std::size_t found = text.find(pattern, pos);
            const int expected = found == std::string::npos ? -1 : static_cast<int>(found);

            EXPECT_EQ(expected, str.find(pattern, pos)) << "pattern: " << pattern << ", pos: " << pos;
        }
    }
}

// =====================================================================
// 필수 기능: substr
// =====================================================================

TEST(MyStringSubstr, Basic) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(0, 3), "012");
    EXPECT_SAME(str.substr(3, 4), "3456");
    EXPECT_SAME(str.substr(9, 1), "9");
    EXPECT_SAME(str, "0123456789");
}

TEST(MyStringSubstr, ZeroLengthIsEmpty) {
    EXPECT_SAME(MyString("0123456789").substr(3, 0), "");
}

TEST(MyStringSubstr, TooLongLengthIsClamped) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(8, 100), "89");
    EXPECT_SAME(str.substr(0, 100), "0123456789");
}

TEST(MyStringSubstr, NegativeLengthIsTreatedAsMaximum) {
    EXPECT_SAME(MyString("0123456789").substr(7, -1), "789");
}

TEST(MyStringSubstr, PositionOutOfRangeIsTreatedAsLastPosition) {
    EXPECT_SAME(MyString("0123456789").substr(100, 3), "9");
}

// 음수 pos: -1이 마지막 글자이고, 그 글자에서 앞쪽으로 len개를 취한다 (MyString.cpp의 주석과 동일한 해석)
TEST(MyStringSubstr, NegativePositionCountsBackwardFromEnd) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(-1, 1), "9");
    EXPECT_SAME(str.substr(-1, 3), "789");
    EXPECT_SAME(str.substr(-3, 3), "567");
}

TEST(MyStringSubstr, NegativePositionClampsLength) {
    EXPECT_SAME(MyString("0123456789").substr(-8, 100), "012");
}

TEST(MyStringSubstr, OnEmptyString) {
    EXPECT_SAME(MyString().substr(0, 3), "");
}

TEST(MyStringSubstr, MatchesStdStringForEveryValidRange) {
    const std::string text = "0123456789abcdefghij";
    const MyString str(text.c_str());

    for (int pos = 0; pos < static_cast<int>(text.size()); ++pos) {
        for (int len = 0; len <= static_cast<int>(text.size()) + 2; ++len) {
            EXPECT_SAME(str.substr(pos, len), text.substr(pos, len)) << "pos: " << pos << ", len: " << len;
        }
    }
}

// =====================================================================
// 고급 기능: append / +=
// =====================================================================

TEST(MyStringAppend, CString) {
    MyString str("hello");

    str.append(" world");

    EXPECT_SAME(str, "hello world");
}

TEST(MyStringAppend, MyStringArgument) {
    MyString str("hello");
    const MyString other(" world");

    str.append(other);

    EXPECT_SAME(str, "hello world");
    EXPECT_SAME(other, " world");
}

TEST(MyStringAppend, MyStringArgumentReturnsThis) {
    MyString str;

    EXPECT_EQ(&str, &str.append(MyString("x")));
}

TEST(MyStringAppend, CStringArgumentReturnsMyStringRef) {
    EXPECT_TRUE(is_mystring_ref<decltype(std::declval<MyString &>().append("x"))>);
}

TEST(MyStringAppend, Itself) {
    MyString str("abc");

    str.append(str);

    EXPECT_SAME(str, "abcabc");
}

TEST(MyStringAppend, ItselfWhileGrowing) {
    MyString str("0123456789");

    str.append(str);
    str.append(str);

    EXPECT_SAME(str, "0123456789012345678901234567890123456789");
}

TEST(MyStringAppend, Empty) {
    MyString str("hello");

    str.append("");
    str.append(MyString());

    EXPECT_SAME(str, "hello");
}

TEST(MyStringPlusAssign, CString) {
    MyString str("hello");

    str += " world";

    EXPECT_SAME(str, "hello world");
}

TEST(MyStringPlusAssign, MyStringArgument) {
    MyString str("hello");

    str += MyString(" world");

    EXPECT_SAME(str, "hello world");
}

TEST(MyStringPlusAssign, ReturnsThis) {
    MyString str;

    EXPECT_EQ(&str, &(str += "x"));
    EXPECT_EQ(&str, &(str += MyString("y")));
    EXPECT_SAME(str, "xy");
}

TEST(MyStringPlusAssign, Itself) {
    MyString str("abc");

    str += str;

    EXPECT_SAME(str, "abcabc");
}

// =====================================================================
// 고급 기능: < <= > >=
// =====================================================================

TEST(MyStringOrdering, WithMyString) {
    const MyString str("banana");

    EXPECT_TRUE(lt(str, MyString("cherry")));
    EXPECT_FALSE(lt(str, MyString("banana")));
    EXPECT_FALSE(lt(str, MyString("apple")));

    EXPECT_TRUE(le(str, MyString("cherry")));
    EXPECT_TRUE(le(str, MyString("banana")));
    EXPECT_FALSE(le(str, MyString("apple")));

    EXPECT_TRUE(gt(str, MyString("apple")));
    EXPECT_FALSE(gt(str, MyString("banana")));
    EXPECT_FALSE(gt(str, MyString("cherry")));

    EXPECT_TRUE(ge(str, MyString("apple")));
    EXPECT_TRUE(ge(str, MyString("banana")));
    EXPECT_FALSE(ge(str, MyString("cherry")));
}

TEST(MyStringOrdering, WithCString) {
    const MyString str("banana");

    EXPECT_TRUE(lt(str, "cherry"));
    EXPECT_FALSE(lt(str, "banana"));

    EXPECT_TRUE(le(str, "banana"));
    EXPECT_FALSE(le(str, "apple"));

    EXPECT_TRUE(gt(str, "apple"));
    EXPECT_FALSE(gt(str, "banana"));

    EXPECT_TRUE(ge(str, "banana"));
    EXPECT_FALSE(ge(str, "cherry"));
}

TEST(MyStringOrdering, PrefixIsSmaller) {
    EXPECT_TRUE(lt(MyString("abc"), MyString("abcd")));
    EXPECT_TRUE(gt(MyString("abcd"), "abc"));
    EXPECT_TRUE(lt(MyString(), "a"));
}

TEST(MyStringOrdering, UsesCharacterCode) {
    EXPECT_TRUE(lt(MyString("Z"), "a"));
}

TEST(MyStringCompare, ReturnsSign) {
    const MyString str("banana");

    EXPECT_EQ(0, str.compare(MyString("banana")));
    EXPECT_LT(str.compare(MyString("cherry")), 0);
    EXPECT_GT(str.compare(MyString("apple")), 0);

    EXPECT_EQ(0, str.compare("banana"));
    EXPECT_LT(str.compare("banana split"), 0);
    EXPECT_GT(str.compare("ban"), 0);
}

// =====================================================================
// 고급 기능: insert
// =====================================================================

TEST(MyStringInsert, AtFrontMiddleAndEnd) {
    MyString front("world"), middle("held"), end("hello");

    front.insert(0, "hello ");
    middle.insert(2, "llo wor");
    end.insert(5, " world");

    EXPECT_SAME(front, "hello world");
    EXPECT_SAME(middle, "hello world");
    EXPECT_SAME(end, "hello world");
}

TEST(MyStringInsert, MyStringArgument) {
    MyString str("held");
    const MyString other("llo wor");

    str.insert(2, other);

    EXPECT_SAME(str, "hello world");
    EXPECT_SAME(other, "llo wor");
}

TEST(MyStringInsert, MyStringArgumentReturnsThis) {
    MyString str("ab");

    EXPECT_EQ(&str, &str.insert(1, MyString("x")));
}

TEST(MyStringInsert, CStringArgumentReturnsMyStringRef) {
    EXPECT_TRUE(is_mystring_ref<decltype(std::declval<MyString &>().insert(0, "x"))>);
}

TEST(MyStringInsert, NegativePositionIsTreatedAsZero) {
    MyString str("hello");

    str.insert(-3, "XY");

    EXPECT_SAME(str, "XYhello");
}

// "마지막 위치"가 맨 끝 뒤인지 마지막 글자 앞인지는 명세만으로 불분명해서 둘 다 허용한다
TEST(MyStringInsert, PositionOutOfRangeIsTreatedAsLastPosition) {
    MyString str("hello");

    EXPECT_NO_THROW(str.insert(100, "XY"));

    const std::string text = to_std_string(str);
    EXPECT_TRUE(text == "helloXY" || text == "hellXYo") << "결과: " << text;
}

TEST(MyStringInsert, Itself) {
    MyString str("abcd");

    str.insert(2, str);

    EXPECT_SAME(str, "ababcdcd");
}

TEST(MyStringInsert, IntoEmpty) {
    MyString str;

    str.insert(0, "hello");

    EXPECT_SAME(str, "hello");
}

TEST(MyStringInsert, MatchesStdStringForEveryPosition) {
    const std::string text = "0123456789";
    const std::string piece = "abcdefghijklmnopqrstuvwxyz";

    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
        MyString str(text.c_str());
        std::string expected = text;

        str.insert(pos, piece.c_str());
        expected.insert(pos, piece);

        EXPECT_SAME(str, expected) << "pos: " << pos;
    }
}

// =====================================================================
// 고급 기능: replace
// =====================================================================

TEST(MyStringReplace, SameLength) {
    MyString str("hello world");

    str.replace(6, 5, "there");

    EXPECT_SAME(str, "hello there");
}

TEST(MyStringReplace, Longer) {
    MyString str("hello world");

    str.replace(0, 5, "good morning");

    EXPECT_SAME(str, "good morning world");
}

TEST(MyStringReplace, Shorter) {
    MyString str("hello world");

    str.replace(0, 5, "hi");

    EXPECT_SAME(str, "hi world");
}

TEST(MyStringReplace, InTheMiddle) {
    MyString str("abcdefgh");

    str.replace(2, 3, "XY");

    EXPECT_SAME(str, "abXYfgh");
}

TEST(MyStringReplace, WithEmptyActsAsErase) {
    MyString str("hello world");

    str.replace(5, 6, "");

    EXPECT_SAME(str, "hello");
}

TEST(MyStringReplace, ZeroLengthActsAsInsert) {
    MyString str("held");

    str.replace(2, 0, "llo wor");

    EXPECT_SAME(str, "hello world");
}

TEST(MyStringReplace, MyStringArgument) {
    MyString str("hello world");
    const MyString other("there");

    str.replace(6, 5, other);

    EXPECT_SAME(str, "hello there");
    EXPECT_SAME(other, "there");
}

TEST(MyStringReplace, TooLongLengthIsClamped) {
    MyString str("hello");

    str.replace(3, 100, "XY");

    EXPECT_SAME(str, "helXY");
}

TEST(MyStringReplace, NegativeLengthIsTreatedAsMaximum) {
    MyString str("hello");

    str.replace(1, -1, "X");

    EXPECT_SAME(str, "hX");
}

TEST(MyStringReplace, NegativePositionIsTreatedAsZero) {
    MyString str("hello");

    str.replace(-3, 2, "XY");

    EXPECT_SAME(str, "XYllo");
}

TEST(MyStringReplace, PositionOutOfRangeIsTreatedAsLastPosition) {
    MyString str("hello");

    str.replace(100, 1, "X");

    EXPECT_SAME(str, "hellX");
}

TEST(MyStringReplace, OnEmptyString) {
    MyString str;

    str.replace(0, 0, "hello");

    EXPECT_SAME(str, "hello");
}

TEST(MyStringReplace, Itself) {
    MyString str("abcd");

    str.replace(1, 2, str);

    EXPECT_SAME(str, "aabcdd");
}

TEST(MyStringReplace, ReturnsThis) {
    MyString str("hello");

    EXPECT_EQ(&str, &str.replace(0, 1, "J"));
    EXPECT_EQ(&str, &str.replace(0, 1, MyString("C")));
    EXPECT_SAME(str, "Cello");
}

TEST(MyStringReplace, MatchesStdStringForEveryValidRange) {
    const std::string text = "0123456789";

    for (const char *piece : {"", "X", "XYZ", "abcdefghijklmnopqrstuvwxyz"}) {
        for (int pos = 0; pos < static_cast<int>(text.size()); ++pos) {
            for (int len = 0; len <= static_cast<int>(text.size()) + 2; ++len) {
                MyString str(text.c_str());
                std::string expected = text;

                str.replace(pos, len, piece);
                expected.replace(pos, len, piece);

                EXPECT_SAME(str, expected) << "piece: " << piece << ", pos: " << pos << ", len: " << len;
            }
        }
    }
}

// =====================================================================
// 고급 기능: erase
// =====================================================================

TEST(MyStringErase, Middle) {
    MyString str("hello world");

    str.erase(5, 6);

    EXPECT_SAME(str, "hello");
}

TEST(MyStringErase, Front) {
    MyString str("hello world");

    str.erase(0, 6);

    EXPECT_SAME(str, "world");
}

TEST(MyStringErase, ZeroCountDoesNothing) {
    MyString str("hello");

    str.erase(2, 0);

    EXPECT_SAME(str, "hello");
}

TEST(MyStringErase, TooLongCountIsClamped) {
    MyString str("hello");

    str.erase(2, 100);

    EXPECT_SAME(str, "he");
}

TEST(MyStringErase, NegativeCountIsTreatedAsMaximum) {
    MyString str("hello");

    str.erase(2, -1);

    EXPECT_SAME(str, "he");
}

TEST(MyStringErase, NegativePositionIsTreatedAsZero) {
    MyString str("hello");

    str.erase(-3, 2);

    EXPECT_SAME(str, "llo");
}

TEST(MyStringErase, PositionOutOfRangeIsTreatedAsLastPosition) {
    MyString str("hello");

    str.erase(100, 1);

    EXPECT_SAME(str, "hell");
}

TEST(MyStringErase, EverythingLeavesEmptyString) {
    MyString str("hello");

    str.erase(0, 5);

    EXPECT_SAME(str, "");
    EXPECT_TRUE(str.isEmpty());
}

TEST(MyStringErase, OnEmptyString) {
    MyString str;

    str.erase(0, 3);

    EXPECT_SAME(str, "");
}

TEST(MyStringErase, ReturnsThis) {
    MyString str("hello");

    EXPECT_EQ(&str, &str.erase(0, 1));
}

TEST(MyStringErase, UsableAfterErasingLongString) {
    const std::string text(200, 'x');
    MyString str(text.c_str());

    str.erase(0, 200);
    str.append("hello");

    EXPECT_SAME(str, "hello");
}

TEST(MyStringErase, MatchesStdStringForEveryValidRange) {
    const std::string text = "0123456789abcdefghij";

    for (int pos = 0; pos < static_cast<int>(text.size()); ++pos) {
        for (int cnt = 0; cnt <= static_cast<int>(text.size()) + 2; ++cnt) {
            MyString str(text.c_str());
            std::string expected = text;

            str.erase(pos, cnt);
            expected.erase(pos, cnt);

            EXPECT_SAME(str, expected) << "pos: " << pos << ", cnt: " << cnt;
        }
    }
}

// =====================================================================
// 고급 기능: stoi
// =====================================================================

TEST(MyStringStoi, ExamplesFromSpec) {
    EXPECT_EQ(10, stoi("123axyz", 3, 16));
    EXPECT_EQ(171, stoi("123ab", 3, 16));
    EXPECT_EQ(0, stoi("-123defg"));
    EXPECT_EQ(123, stoi("123ab456", 0, 10));
}

TEST(MyStringStoi, DefaultArgumentsAreDecimal) {
    EXPECT_EQ(12345, stoi(MyString("12345")));
    EXPECT_EQ(7, stoi(MyString("7")));
}

TEST(MyStringStoi, LeadingZeros) {
    EXPECT_EQ(42, stoi("00042"));
}

TEST(MyStringStoi, FromPosition) {
    EXPECT_EQ(345, stoi("12345", 2));
}

TEST(MyStringStoi, NoValidDigitReturnsZero) {
    EXPECT_EQ(0, stoi(""));
    EXPECT_EQ(0, stoi("xyz"));
}

TEST(MyStringStoi, Binary) {
    EXPECT_EQ(5, stoi("101", 0, 2));
    EXPECT_EQ(1, stoi("12", 0, 2));
}

TEST(MyStringStoi, Octal) {
    EXPECT_EQ(83, stoi("123", 0, 8));
    EXPECT_EQ(10, stoi("129", 0, 8));
}

TEST(MyStringStoi, Hexadecimal) {
    EXPECT_EQ(255, stoi("ff", 0, 16));
    EXPECT_EQ(255, stoi("FF", 0, 16));
    EXPECT_EQ(15, stoi("fg", 0, 16));
}

TEST(MyStringStoi, Base36) {
    EXPECT_EQ(35, stoi("z", 0, 36));
    EXPECT_EQ(1295, stoi("zz", 0, 36));
    EXPECT_EQ(36, stoi("10", 0, 36));
}

// =====================================================================
// const 정확성 / 메모리
// =====================================================================

TEST(MyStringConst, ReadOnlyFunctionsWorkOnConstObject) {
    const MyString str("hello");

    EXPECT_EQ(5, str.length());
    EXPECT_FALSE(str.isEmpty());
    EXPECT_EQ(0, str.getCapacity() % 8);
    EXPECT_EQ('h', str.at(0));
    EXPECT_EQ('h', str[0]);
    EXPECT_EQ(1, str.find("ell"));
    EXPECT_SAME(str.substr(1, 3), "ell");
    EXPECT_SAME(str + "!", "hello!");
    EXPECT_EQ(0, str.compare("hello"));
}

TEST(MyStringMemory, RepeatedCopyAssignDestroy) {
    const std::string text(50, 'x');
    const MyString source(text.c_str());

    for (int count = 0; count < 1000; ++count) {
        MyString copy(source);
        MyString assigned;
        assigned = copy;
        assigned += "y";
        copy = assigned;

        ASSERT_SAME(copy, text + "y");
    }

    EXPECT_SAME(source, text);
}

TEST(MyStringMemory, MixedEditsMatchStdString) {
    MyString str;
    std::string expected;

    for (int count = 0; count < 200; ++count) {
        str.append("abcdefghijk");
        expected.append("abcdefghijk");

        str.insert(count, "XYZ");
        expected.insert(count, "XYZ");

        str.replace(count, 2, "0123");
        expected.replace(count, 2, "0123");

        str.erase(count + 1, 5);
        expected.erase(count + 1, 5);

        ASSERT_SAME(str, expected) << "count: " << count;
    }
}
