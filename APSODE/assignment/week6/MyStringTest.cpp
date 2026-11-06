#include <gtest/gtest.h>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#include "MyString.h"

// 기준: 7주차.프로젝트1.MyString.pdf (명시되지 않은 동작은 std::string과 동일)
//       MyString 조건 변경&추가사항.png (PDF와 다르면 이쪽이 우선)
//
// 명세가 모호해서 이 파일에서 정한 해석
//   - erase / replace / insert: pos == length는 std::string처럼 맨 끝, pos > length는 PDF대로 마지막 글자 위치
//   - find / stoi: pos가 범위 밖이면 PDF대로 마지막 글자 위치
//   - capacity: '\0'을 뺀 글자 수 기준이라 8글자는 8에 들어가고, 늘릴 때는 충분해질 때까지 2배씩
//   - 범위 밖 operator[] / at()으로 쓰기: 사용자 책임이라 테스트하지 않음 (읽기는 '\0' 반환)
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

    // 위치마다 글자가 달라서 자기 참조 복사가 어긋나면 결과에 바로 드러나는 문자열
    std::string alphabet(const int length) {
        std::string text;
        for (int index = 0; index < length; ++index) {
            text += static_cast<char>('a' + index % 26);
        }
        return text;
    }

    int doubled_capacity(int capacity, const int required) {
        while (capacity < required) {
            capacity *= 2;
        }
        return capacity;
    }
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

TEST(MyStringCapacity, EightCharactersFitInCapacity8) {
    const MyString constructed("12345678");
    MyString one_by_one, bulk;

    for (int count = 0; count < 8; ++count) {
        one_by_one.append("x");
    }
    bulk.append("12345678");

    EXPECT_EQ(8, constructed.getCapacity());
    EXPECT_EQ(8, one_by_one.getCapacity());
    EXPECT_EQ(8, bulk.getCapacity());
}

TEST(MyStringCapacity, NinthCharacterDoublesTo16) {
    MyString str("12345678");

    str.append("9");

    EXPECT_SAME(str, "123456789");
    EXPECT_EQ(16, str.getCapacity());
}

TEST(MyStringCapacity, AppendWithinCapacityDoesNotGrow) {
    MyString str("abc");

    str.append("defgh");

    EXPECT_SAME(str, "abcdefgh");
    EXPECT_EQ(8, str.getCapacity());
}

TEST(MyStringCapacity, EveryGrowingEditDoublesUntilItFits) {
    for (const int length : {1, 7, 8, 9, 15, 16, 17, 31, 32, 33, 100, 500}) {
        const std::string text(length, 'x');
        const int expected = doubled_capacity(8, length);

        MyString appended, appended_string, plus_assigned, inserted, replaced("y");
        appended.append(text.c_str());
        appended_string.append(MyString(text.c_str()));
        plus_assigned += text.c_str();
        inserted.insert(0, text.c_str());
        replaced.replace(0, 1, text.c_str());

        EXPECT_EQ(expected, appended.getCapacity()) << "append, length: " << length;
        EXPECT_EQ(expected, appended_string.getCapacity()) << "append(MyString), length: " << length;
        EXPECT_EQ(expected, plus_assigned.getCapacity()) << "+=, length: " << length;
        EXPECT_EQ(expected, inserted.getCapacity()) << "insert, length: " << length;
        EXPECT_EQ(expected, replaced.getCapacity()) << "replace, length: " << length;
    }
}

TEST(MyStringCapacity, GrowthDoublesFromCurrentCapacity) {
    const std::string text(100, 'x');
    MyString str(text.c_str());
    const int before = str.getCapacity();
    const std::string extra(before + 1 - str.length(), 'y');

    str.append(extra.c_str());

    EXPECT_SAME(str, text + extra);
    EXPECT_EQ(before * 2, str.getCapacity());
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

// PNG: 예외처리를 아직 안 배웠으므로 at()도 범위 밖이면 '\0' 반환
TEST(MyStringAccess, ConstAtOutOfRangeReturnsNull) {
    const MyString str("hello");

    EXPECT_EQ('\0', str.at(-1));
    EXPECT_EQ('\0', str.at(5));
    EXPECT_EQ('\0', str.at(100));
}

TEST(MyStringAccess, NonConstAtOutOfRangeReturnsNull) {
    MyString str("hello");

    EXPECT_EQ('\0', str.at(-1));
    EXPECT_EQ('\0', str.at(5));
    EXPECT_EQ('\0', str.at(100));
}

TEST(MyStringAccess, AtOnEmptyStringReturnsNull) {
    MyString str;

    EXPECT_EQ('\0', str.at(0));
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

TEST(MyStringInput, FailedReadKeepsPreviousContent) {
    for (const char *input : {"", "   \n  "}) {
        std::istringstream stream(input);
        MyString str("previous value");

        stream >> str;

        EXPECT_TRUE(stream.fail()) << "input: " << testing::PrintToString(std::string(input));
        EXPECT_SAME(str, "previous value") << "input: " << testing::PrintToString(std::string(input));
    }
}

TEST(MyStringInput, ReturnsSameStream) {
    std::istringstream stream("hello");
    MyString str;

    EXPECT_EQ(&stream, &(stream >> str));
}

TEST(MyStringInput, ReadsNonAsciiBytes) {
    std::istringstream stream("한글 입력");
    MyString str;

    stream >> str;

    EXPECT_SAME(str, "한글");
}

TEST(MyStringInput, MatchesStdStringIncludingStreamState) {
    for (const char *input : {"", " ", "\n", "a", "a ", " a", "a b", "  one\ttwo\n\nthree  ", "a\n", "\n\na"}) {
        std::istringstream mine(input), expected_stream(input);
        MyString str("previous");
        std::string expected("previous");

        for (int read = 0; read < 10; ++read) {
            const bool is_read = static_cast<bool>(mine >> str);
            const bool expected_read = static_cast<bool>(expected_stream >> expected);
            const std::string where = "input: " + testing::PrintToString(std::string(input)) + ", read: " + std::to_string(read);

            EXPECT_EQ(expected_read, is_read) << where;
            EXPECT_EQ(expected_stream.eof(), mine.eof()) << where;
            EXPECT_EQ(expected_stream.fail(), mine.fail()) << where;
            EXPECT_SAME(str, expected) << where;

            if (!expected_read || !is_read) {
                break;
            }
        }
    }
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

// >>와 달리 std::getline은 스트림이 정상이면 읽기 전에 문자열을 비운다. 읽을 것이 없어도 비워진다
TEST(MyStringGetline, ReadingAtEndClearsPreviousContent) {
    std::istringstream stream("");
    MyString str("previous value");

    getline(stream, str);

    EXPECT_TRUE(stream.fail());
    EXPECT_SAME(str, "");
}

TEST(MyStringGetline, AlreadyFailedStreamKeepsPreviousContent) {
    std::istringstream stream("next");
    MyString str("previous value");
    stream.setstate(std::ios::failbit);

    getline(stream, str);

    EXPECT_TRUE(stream.fail());
    EXPECT_SAME(str, "previous value");
}

TEST(MyStringGetline, ReturnsSameStream) {
    std::istringstream stream("hello\n");
    MyString str;

    EXPECT_EQ(&stream, &getline(stream, str));
}

// char가 signed인 환경에서 get()의 int 값과 delim을 그대로 비교하면 0x80 이상의 구분자를 찾지 못한다
TEST(MyStringGetline, NonAsciiDelimiter) {
    std::istringstream stream("left\xEBright");
    MyString first, second;

    getline(stream, first, '\xEB');
    getline(stream, second, '\xEB');

    EXPECT_SAME(first, "left");
    EXPECT_SAME(second, "right");
}

TEST(MyStringGetline, MatchesStdStringIncludingStreamState) {
    for (const char *input : {"", "\n", "\n\n", "a", "a\n", "a\nb", "a\n\nb\n", "  a b  \n c"}) {
        std::istringstream mine(input), expected_stream(input);
        MyString str("previous");
        std::string expected("previous");

        for (int read = 0; read < 10; ++read) {
            const bool is_read = static_cast<bool>(getline(mine, str));
            const bool expected_read = static_cast<bool>(std::getline(expected_stream, expected));
            const std::string where = "input: " + testing::PrintToString(std::string(input)) + ", read: " + std::to_string(read);

            EXPECT_EQ(expected_read, is_read) << where;
            EXPECT_EQ(expected_stream.eof(), mine.eof()) << where;
            EXPECT_EQ(expected_stream.fail(), mine.fail()) << where;
            EXPECT_SAME(str, expected) << where;

            if (!expected_read || !is_read) {
                break;
            }
        }
    }
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
    EXPECT_EQ(-1, MyString("").find("a", 5));
    EXPECT_EQ(-1, MyString("").find("a", -5));
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

// PNG: len이 음수이면 빈 문자열
TEST(MyStringSubstr, NegativeLengthIsEmpty) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(7, -1), "");
    EXPECT_SAME(str.substr(0, -1), "");
    EXPECT_SAME(str.substr(-1, -1), "");
    EXPECT_SAME(str.substr(20, -1), "");
}

// PNG: pos가 범위 밖이면 예외 없이 빈 문자열
TEST(MyStringSubstr, PositionOutOfRangeIsEmpty) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(10, 3), "");
    EXPECT_SAME(str.substr(20, 3), "");
    EXPECT_SAME(str.substr(100, 3), "");
    EXPECT_SAME(str.substr(-11, 3), "");
    EXPECT_SAME(str.substr(-15, 3), "");
}

// PNG의 예시를 그대로 옮긴 것
TEST(MyStringSubstr, ExamplesFromConditionChange) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(-1, 3), "789");
    EXPECT_SAME(str.substr(-2, 2), "78");
    EXPECT_SAME(str.substr(-5, 4), "2345");
    EXPECT_SAME(str.substr(-5, 10), "012345");
    EXPECT_SAME(str.substr(-15, 3), "");
    EXPECT_SAME(str.substr(20, 3), "");
    EXPECT_SAME(str, "0123456789");
}

TEST(MyStringSubstr, NegativePositionAtFirstCharacter) {
    const MyString str("0123456789");

    EXPECT_SAME(str.substr(-10, 1), "0");
    EXPECT_SAME(str.substr(-10, 3), "0");
    EXPECT_SAME(str.substr(-1, 10), "0123456789");
    EXPECT_SAME(str.substr(-1, 100), "0123456789");
    EXPECT_SAME(str.substr(-1, 0), "");
}

// 음수 pos: -1이 마지막 글자이고, 그 글자에서 앞쪽으로 len개를 취한다 (PNG 예시와 동일한 해석)
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
    EXPECT_SAME(MyString().substr(1, 3), "");
    EXPECT_SAME(MyString().substr(-1, 3), "");
    EXPECT_SAME(MyString().substr(0, -1), "");
}

TEST(MyStringSubstr, MatchesStdStringForEveryValidRange) {
    const std::string text = "0123456789abcdefghij";
    const MyString str(text.c_str());

    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
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

TEST(MyStringAppend, PointerIntoItself) {
    MyString within("ab"), growing("abcdefgh");

    within.append(&within[1]);
    growing.append(&growing[2]);

    EXPECT_SAME(within, "abb");
    EXPECT_SAME(growing, "abcdefghcdefgh");
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

TEST(MyStringPlusAssign, PointerIntoItself) {
    MyString str("abcdefgh");

    str += &str[2];

    EXPECT_SAME(str, "abcdefghcdefgh");
}

TEST(MyStringAssign, PointerIntoItself) {
    MyString str("abcdefgh");

    str = &str[2];

    EXPECT_SAME(str, "cdefgh");
}

// 자기 버퍼의 어느 위치를 가리키든(끝의 '\0' 포함) 복사본으로 처리되어야 한다
TEST(MyStringAppend, PointerIntoItselfMatchesStdString) {
    for (const int length : {0, 1, 7, 8, 9, 15, 16, 17, 32}) {
        const std::string text = alphabet(length);

        for (int offset = 0; offset <= length; ++offset) {
            MyString mine(text.c_str());
            std::string expected = text;

            mine.append(&mine[offset]);
            expected.append(expected.c_str() + offset);

            EXPECT_SAME(mine, expected) << "length: " << length << ", offset: " << offset;
        }
    }
}

TEST(MyStringAppend, PointerAndAmountIntoItselfMatchesStdString) {
    for (const int length : {1, 7, 8, 9, 16, 17}) {
        const std::string text = alphabet(length);

        for (int offset = 0; offset <= length; ++offset) {
            for (int amount = 0; amount <= length - offset; ++amount) {
                MyString mine(text.c_str());
                std::string expected = text;

                mine.append(&mine[offset], amount);
                expected.append(expected.data() + offset, amount);

                EXPECT_SAME(mine, expected)
                    << "length: " << length << ", offset: " << offset << ", amount: " << amount;
            }
        }
    }
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

// 헤더에 선언된 const char * 오버로드가 그대로 있는지 확인한다.
// 없으면 MyString으로 변환되어 조용히 통과하므로 멤버 함수 포인터로 직접 고른다.
TEST(MyStringOrdering, CStringOverloadsAreDeclared) {
    using CStringComparison = bool (MyString::*)(const char *) const;

    const CStringComparison equal = &MyString::operator==;
    const CStringComparison not_equal = &MyString::operator!=;
    const CStringComparison less = &MyString::operator<;
    const CStringComparison less_equal = &MyString::operator<=;
    const CStringComparison greater = &MyString::operator>;
    const CStringComparison greater_equal = &MyString::operator>=;

    const MyString str("b");

    EXPECT_TRUE((str.*equal)("b"));
    EXPECT_TRUE((str.*not_equal)("a"));
    EXPECT_TRUE((str.*less)("c"));
    EXPECT_TRUE((str.*less_equal)("b"));
    EXPECT_TRUE((str.*greater)("a"));
    EXPECT_TRUE((str.*greater_equal)("b"));
}

TEST(MyStringOrdering, AllOperatorsMatchStdString) {
    const char *const samples[] = {"", " ", "Z", "a", "aa", "ab", "abc", "abd", "a b", "b", "\xEB\x82\x98", "\x7F"};

    for (const char *left : samples) {
        for (const char *right : samples) {
            const std::string left_text(left), right_text(right);
            const MyString mine(left), other(right);

            SCOPED_TRACE(testing::PrintToString(left_text) + " vs " + testing::PrintToString(right_text));

            EXPECT_EQ(left_text == right_text, mine == right);
            EXPECT_EQ(left_text != right_text, mine != right);
            EXPECT_EQ(left_text < right_text, mine < right);
            EXPECT_EQ(left_text <= right_text, mine <= right);
            EXPECT_EQ(left_text > right_text, mine > right);
            EXPECT_EQ(left_text >= right_text, mine >= right);

            EXPECT_EQ(left_text == right_text, mine == other);
            EXPECT_EQ(left_text != right_text, mine != other);
            EXPECT_EQ(left_text < right_text, mine < other);
            EXPECT_EQ(left_text <= right_text, mine <= other);
            EXPECT_EQ(left_text > right_text, mine > other);
            EXPECT_EQ(left_text >= right_text, mine >= other);
        }
    }
}

TEST(MyStringOrdering, AcceptsEveryCStringForm) {
    MyString str("hello");
    const MyString const_str("hello");
    char buffer[] = "hello";
    char *pointer = buffer;
    const char *const_pointer = buffer;

    EXPECT_TRUE(str == buffer);
    EXPECT_TRUE(str == pointer);
    EXPECT_TRUE(str == const_pointer);
    EXPECT_TRUE(const_str == buffer);
    EXPECT_TRUE(const_str == pointer);
    EXPECT_TRUE(const_str == const_pointer);

    EXPECT_TRUE(str <= pointer);
    EXPECT_TRUE(const_str >= const_pointer);
    EXPECT_FALSE(str != buffer);
    EXPECT_FALSE(const_str < buffer);
}

TEST(MyStringOrdering, ComparingWithPointerIntoItself) {
    const MyString str("hello");

    EXPECT_TRUE(str == &str[0]);
    EXPECT_TRUE(str != &str[1]);
    EXPECT_TRUE(str > &str[1]);    // "hello" > "ello"
    EXPECT_TRUE(str > &str[5]);    // 끝의 '\0'을 가리키면 빈 문자열
    EXPECT_SAME(str, "hello");
}

TEST(MyStringOrdering, ComparingKeepsOperands) {
    const char *chars = "banana";
    const MyString str("banana");

    EXPECT_TRUE(str == chars);
    EXPECT_FALSE(str < chars);
    EXPECT_TRUE(str >= MyString(chars));

    EXPECT_SAME(str, "banana");
    EXPECT_STREQ("banana", chars);
}

// compare(const char *, int): 첫 '\0'이 아니라 넘겨받은 length까지만 비교한다
TEST(MyStringCompare, WithLengthUsesOnlyThatLength) {
    const MyString str("banana");

    EXPECT_EQ(0, str.compare("banana split", 6));
    EXPECT_GT(str.compare("banana", 3), 0);
    EXPECT_LT(str.compare("banana split", 7), 0);
    EXPECT_LT(str.compare("cherry", 1), 0);
    EXPECT_GT(str.compare("apple", 1), 0);
}

TEST(MyStringCompare, WithZeroLength) {
    EXPECT_EQ(0, MyString().compare("anything", 0));
    EXPECT_GT(MyString("a").compare("anything", 0), 0);
}

TEST(MyStringCompare, WithLengthReadsPastEmbeddedNull) {
    const MyString str("a\0b", 3);

    EXPECT_EQ(0, str.compare("a\0b", 3));
    EXPECT_LT(str.compare("a\0c", 3), 0);
    EXPECT_GT(str.compare("a\0a", 3), 0);
    EXPECT_GT(str.compare("a", 1), 0);
    EXPECT_LT(MyString("a").compare("a\0b", 3), 0);
}

// 끝에 '\0'이 없는 버퍼도 length를 넘어 읽지 않아야 한다 (AddressSanitizer로 돌리면 더 확실하다)
TEST(MyStringCompare, WithLengthAcceptsBufferWithoutTerminator) {
    const char buffer[] = {'a', 'b', 'c'};

    EXPECT_EQ(0, MyString("abc").compare(buffer, 3));
    EXPECT_EQ(0, MyString("ab").compare(buffer, 2));
    EXPECT_LT(MyString("ab").compare(buffer, 3), 0);
    EXPECT_GT(MyString("abcd").compare(buffer, 3), 0);
}

TEST(MyStringCompare, WithLengthUsesUnsignedCharacterCode) {
    EXPECT_GT(MyString("\xEB").compare("a", 1), 0);
    EXPECT_LT(MyString("a").compare("\xEB", 1), 0);
}

TEST(MyStringCompare, WithLengthAgreesWithPointerIntoItself) {
    const MyString str("hello");

    EXPECT_EQ(0, str.compare(&str[0], 5));
    EXPECT_GT(str.compare(&str[0], 4), 0);
    EXPECT_GT(str.compare(&str[1], 4), 0);    // "hello" > "ello"
    EXPECT_SAME(str, "hello");
}

// append / insert와 같은 인자 검사: 길이가 0이어도 null은 허용하지 않는다
TEST(MyStringCompare, WithLengthRejectsNull) {
    const char *null_chars = nullptr;
    const MyString str("hello");

    EXPECT_THROW(str.compare(null_chars, 3), std::invalid_argument);
    EXPECT_THROW(str.compare(null_chars, 0), std::invalid_argument);
    EXPECT_THROW(MyString().compare(null_chars, 0), std::invalid_argument);
    EXPECT_SAME(str, "hello");
}

TEST(MyStringCompare, WithLengthRejectsNegativeLength) {
    const MyString str("hello");

    EXPECT_THROW(str.compare("hello", -1), std::invalid_argument);
    EXPECT_THROW(MyString().compare("", -1), std::invalid_argument);
    EXPECT_SAME(str, "hello");
}

TEST(MyStringCompare, WithoutLengthRejectsNull) {
    const char *null_chars = nullptr;
    const MyString str("hello");

    EXPECT_THROW(str.compare(null_chars), std::invalid_argument);
    EXPECT_SAME(str, "hello");
}

TEST(MyStringCompare, WithLengthMatchesStdStringSign) {
    const auto sign = [](const int value) {
        return (value > 0) - (value < 0);
    };
    const char *const samples[] = {"", "a", "ab", "abc", "abd", "b", "Z", "\xEB\x82\x98"};

    for (const char *left : samples) {
        const std::string text(left);
        const MyString mine(left);

        for (const char *right : samples) {
            const int right_length = static_cast<int>(std::strlen(right));

            for (int length = 0; length <= right_length; ++length) {
                EXPECT_EQ(sign(text.compare(0, std::string::npos, right, length)), sign(mine.compare(right, length)))
                    << testing::PrintToString(text) << " vs " << testing::PrintToString(std::string(right, length));
            }
        }
    }
}

// MyString(const char *, int)로는 중간에 '\0'이 든 문자열을 만들 수 있다.
// std::string처럼 length까지 비교해야 하며, equals와 결과가 같아야 한다
TEST(MyStringEquality, EmbeddedNullAgreesWithEquals) {
    const MyString str("a\0b", 3), same("a\0b", 3), different("a\0c", 3), prefix("a");

    EXPECT_TRUE(str.equals(same));
    EXPECT_TRUE(str == same);
    EXPECT_FALSE(str != same);
    EXPECT_EQ(0, str.compare(same));

    EXPECT_FALSE(str.equals(different));
    EXPECT_FALSE(str == different);

    EXPECT_FALSE(prefix.equals(str));
    EXPECT_FALSE(prefix == str);
}

TEST(MyStringOrdering, EmbeddedNullMatchesStdString) {
    const std::string left_text("a\0b", 3), right_text("a\0c", 3), prefix_text("a");
    const MyString left("a\0b", 3), right("a\0c", 3), prefix("a");

    EXPECT_EQ(left_text < right_text, left < right);
    EXPECT_EQ(left_text > right_text, left > right);
    EXPECT_EQ(prefix_text < left_text, prefix < left);
    EXPECT_EQ(left_text > prefix_text, left > prefix);
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

TEST(MyStringInsert, PositionAtLengthAppends) {
    MyString str("hello");

    str.insert(5, "XY");

    EXPECT_SAME(str, "helloXY");
}

// PDF: pos가 범위 밖이면 문자열 마지막 위치로 취급
TEST(MyStringInsert, PositionPastLengthIsTreatedAsLastCharacter) {
    for (const int pos : {6, 100}) {
        MyString str("hello"), one("a");

        str.insert(pos, "XY");
        one.insert(pos, "XY");

        EXPECT_SAME(str, "hellXYo") << "pos: " << pos;
        EXPECT_SAME(one, "XYa") << "pos: " << pos;
    }
}

TEST(MyStringInsert, Itself) {
    MyString str("abcd");

    str.insert(2, str);

    EXPECT_SAME(str, "ababcdcd");
}

TEST(MyStringInsert, PointerIntoItself) {
    MyString within("abcd"), growing("abcdefgh");

    within.insert(1, &within[2]);
    growing.insert(3, &growing[5]);

    EXPECT_SAME(within, "acdbcd");
    EXPECT_SAME(growing, "abcfghdefgh");
}

TEST(MyStringInsert, IntoEmpty) {
    for (const int pos : {-1, 0, 5}) {
        MyString str;

        str.insert(pos, "hello");

        EXPECT_SAME(str, "hello") << "pos: " << pos;
    }
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

TEST(MyStringInsert, PointerIntoItselfMatchesStdString) {
    for (const int length : {0, 1, 7, 8, 9, 16, 17}) {
        const std::string text = alphabet(length);

        for (int offset = 0; offset <= length; ++offset) {
            for (int pos = 0; pos <= length; ++pos) {
                MyString mine(text.c_str());
                std::string expected = text;

                mine.insert(pos, &mine[offset]);
                expected.insert(pos, expected.c_str() + offset);

                EXPECT_SAME(mine, expected)
                    << "length: " << length << ", offset: " << offset << ", pos: " << pos;
            }
        }
    }
}

TEST(MyStringInsert, PointerAndAmountIntoItselfMatchesStdString) {
    for (const int length : {1, 7, 8, 9, 16}) {
        const std::string text = alphabet(length);

        for (int offset = 0; offset <= length; ++offset) {
            for (int amount = 0; amount <= length - offset; ++amount) {
                for (int pos = 0; pos <= length; ++pos) {
                    MyString mine(text.c_str());
                    std::string expected = text;

                    mine.insert(pos, &mine[offset], amount);
                    expected.insert(pos, expected.data() + offset, amount);

                    EXPECT_SAME(mine, expected)
                        << "length: " << length << ", offset: " << offset
                        << ", amount: " << amount << ", pos: " << pos;
                }
            }
        }
    }
}

// insert(int, const char &)는 참조를 받으므로 자기 글자를 넘기면 밀리거나 해제된 칸을 읽을 수 있다
TEST(MyStringInsert, ReferenceToOwnCharacter) {
    MyString within("abc"), growing("abcdefgh");

    within.insert(0, within[2]);
    growing.insert(0, growing[7]);

    EXPECT_SAME(within, "cabc");
    EXPECT_SAME(growing, "habcdefgh");
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

TEST(MyStringReplace, PositionAtLengthAppends) {
    for (const int len : {0, 1, 3, -1}) {
        MyString str("hello");

        str.replace(5, len, "X");

        EXPECT_SAME(str, "helloX") << "len: " << len;
    }
}

// PDF: pos가 범위 밖이면 문자열 마지막 위치로 취급
TEST(MyStringReplace, PositionPastLengthIsTreatedAsLastCharacter) {
    for (const int pos : {6, 100}) {
        MyString keeps_last("hello"), one("a");

        keeps_last.replace(pos, 0, "X");
        one.replace(pos, 1, "X");

        EXPECT_SAME(keeps_last, "hellXo") << "pos: " << pos;
        EXPECT_SAME(one, "X") << "pos: " << pos;

        for (const int len : {1, 3, -1}) {
            MyString str("hello");

            str.replace(pos, len, "X");

            EXPECT_SAME(str, "hellX") << "pos: " << pos << ", len: " << len;
        }
    }
}

TEST(MyStringReplace, OnEmptyString) {
    for (const int pos : {-1, 0, 5}) {
        MyString str;

        str.replace(pos, 2, "hello");

        EXPECT_SAME(str, "hello") << "pos: " << pos;
    }
}

TEST(MyStringReplace, Itself) {
    MyString str("abcd");

    str.replace(1, 2, str);

    EXPECT_SAME(str, "aabcdd");
}

TEST(MyStringReplace, PointerIntoItself) {
    MyString str("abcd");

    str.replace(1, 2, &str[2]);

    EXPECT_SAME(str, "acdd");
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
        for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
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

TEST(MyStringErase, PositionAtLengthErasesNothing) {
    for (const int cnt : {0, 1, 3, -1}) {
        MyString str("hello");

        str.erase(5, cnt);

        EXPECT_SAME(str, "hello") << "cnt: " << cnt;
    }
}

// PDF: pos가 범위 밖이면 문자열 마지막 위치로 취급
TEST(MyStringErase, PositionPastLengthIsTreatedAsLastCharacter) {
    for (const int pos : {6, 100}) {
        MyString unchanged("hello"), one("a");

        unchanged.erase(pos, 0);
        one.erase(pos, 1);

        EXPECT_SAME(unchanged, "hello") << "pos: " << pos;
        EXPECT_SAME(one, "") << "pos: " << pos;

        for (const int cnt : {1, 3, -1}) {
            MyString str("hello");

            str.erase(pos, cnt);

            EXPECT_SAME(str, "hell") << "pos: " << pos << ", cnt: " << cnt;
        }
    }
}

TEST(MyStringErase, EverythingLeavesEmptyString) {
    MyString str("hello");

    str.erase(0, 5);

    EXPECT_SAME(str, "");
    EXPECT_TRUE(str.isEmpty());
}

TEST(MyStringErase, OnEmptyString) {
    for (const int pos : {-1, 0, 5}) {
        for (const int cnt : {0, 3, -1}) {
            MyString str;

            str.erase(pos, cnt);

            EXPECT_SAME(str, "") << "pos: " << pos << ", cnt: " << cnt;
            EXPECT_EQ(0, str.length()) << "pos: " << pos << ", cnt: " << cnt;
        }
    }
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

    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
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

TEST(MyStringStoi, NegativePositionIsTreatedAsZero) {
    EXPECT_EQ(123, stoi("123", -1));
    EXPECT_EQ(123, stoi("123", -100));
    EXPECT_EQ(0, stoi("", -1));
}

TEST(MyStringStoi, PositionOutOfRangeIsTreatedAsLastPosition) {
    EXPECT_EQ(3, stoi("123", 100));
    EXPECT_EQ(0, stoi("", 100));
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
