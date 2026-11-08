#include <gtest/gtest.h>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "MyString.h"

// =====================================================================
// 검증 방식
//
//  "우리가 생각한 값"을 직접 적지 않고, 같은 동작을 std::string 에 똑같이 수행시켜
//  그 결과와 MyString 의 결과가 같은지 비교한다.
//
//  MyString 은 길이 제한이 없다. (capacity 가 8 -> 16 -> 32 ... 로 2배씩 늘어난다.)
//
//  과제 PDF 에서 std::string 과 다르게 정한 규칙은 아래 도우미로만 반영한다.
//   - clamp_pos()       : pos 가 음수이면 0, 문자열 범위 밖(pos >= size)이면 마지막 위치(size-1)
//                         (find / insert / replace / erase 에 적용)
//   - expected_substr() : 교수님 예시 기준 (음수 pos 는 뒤에서 앞으로 len 글자, 범위 밖 pos 는 "")
//   - at() / operator[] : 범위를 벗어나면 '\0' return (예외를 던지지 않는다)
//
//  [교수님께 확인이 필요한 부분]
//   - pos >= size 를 "마지막 위치(size-1)"로 취급하는 해석 (insert(size, ...) 도 size-1 로 취급)
//   - substr 의 범위 밖 pos 는 교수님 예시에서 "" 였는데, 그렇다면 find/insert/replace/erase 의
//     "마지막 위치" 규칙도 같은 뜻인지 (여기서는 PDF 문구 그대로 size-1 로 해석)
//   - EOF / 스트림 상태 테스트 (과제 PDF 에 직접 언급은 없고 "std 와 동일하게" 조항에 근거)
//
//  스트림의 상태 플래그는 EOF 관련 테스트에서만 직접 검사하고,
//  나머지는 읽은 값과 "어디까지 읽었는지(읽은 위치)"만 std 쪽과 비교한다.
// =====================================================================

namespace {
    std::string to_std_string(const MyString &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    // MyString 과 std::string 의 내용/길이/비어있음 여부가 모두 같은지,
    // 그리고 '\0' 종료와 capacity 규칙(8의 배수, 길이 이상. 배열은 capacity + 1 칸)이 지켜지는지 확인
    void expect_same(const MyString &actual, const std::string &expected) {
        EXPECT_EQ(expected, to_std_string(actual));
        EXPECT_EQ(static_cast<int>(expected.size()), actual.length());
        EXPECT_EQ(expected.empty(), actual.isEmpty());
        EXPECT_EQ('\0', actual[actual.length()]);        // const operator[] : 문자열 끝의 '\0'
        EXPECT_GE(actual.getCapacity(), actual.length()); // 배열은 capacity + 1 칸이라 '\0' 자리는 따로 있다.
        EXPECT_EQ(0, actual.getCapacity() % 8);
    }

    // pos 규칙: 음수 -> 0, 범위 밖(pos >= size) -> 마지막 위치
    int clamp_pos(const std::string &text, const int pos) {
        if (pos < 0) {
            return 0;
        }
        const int size = static_cast<int>(text.size());
        if (pos > size) {
            return size;
        }
        return pos;
    }

    int expected_find(const std::string &text, const std::string &pattern, const int start) {
        const std::size_t found = text.find(pattern, static_cast<std::size_t>(clamp_pos(text, start)));
        return found == std::string::npos ? -1 : static_cast<int>(found);
    }

    // substr 규칙 (교수님 예시 기준, "0123456789" 에서)
    //  - pos >= 0 : pos 에서 뒤로 len 글자 (넘치면 문자열 끝까지). pos >= size 이면 빈 문자열
    //  - pos <  0 : -1 이 제일 뒷글자. 그 글자에서 "앞(왼쪽)으로" len 글자 (넘치면 문자열 처음까지)
    //               size + pos < 0 (불가능한 위치)이면 빈 문자열
    std::string expected_substr(const std::string &text, const int pos, const int len) {
        const int size = static_cast<int>(text.size());
        if (len <= 0) {
            return "";
        }
        if (pos >= 0) {
            return pos >= size ? "" : text.substr(static_cast<std::size_t>(pos), static_cast<std::size_t>(len));
        }
        const int last = size + pos;                               // 포함되는 마지막 글자의 index
        if (last < 0) {
            return "";
        }
        const int first = (last - len + 1 < 0) ? 0 : last - len + 1;  // 포함되는 첫 글자의 index
        return text.substr(static_cast<std::size_t>(first), static_cast<std::size_t>(last - first + 1));
    }

    std::string expected_insert(std::string text, const int pos, const std::string &sub) {
        text.insert(static_cast<std::size_t>(clamp_pos(text, pos)), sub);
        return text;
    }

    std::string expected_replace(std::string text, const int pos, const int len, const std::string &sub) {
        text.replace(static_cast<std::size_t>(clamp_pos(text, pos)), static_cast<std::size_t>(len), sub);
        return text;
    }

    std::string expected_erase(std::string text, const int pos, const int cnt) {
        text.erase(static_cast<std::size_t>(clamp_pos(text, pos)), static_cast<std::size_t>(cnt));
        return text;
    }

    // find 의 두 오버로드(MyString / const char*)가 모두 기대값과 같은지 확인
    void expect_find_same(const char *text, const char *pattern, const int start) {
        const int expected = expected_find(text, pattern, start);
        const MyString string(text);

        EXPECT_EQ(expected, string.find(MyString(pattern), start))
            << "[MyString] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
        EXPECT_EQ(expected, string.find(pattern, start))
            << "[char array] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
    }

    // ==, !=, <, <=, >, >= 가 std::string 과 같은지 확인 (MyString 끼리 / const char* 모두)
    void expect_compare_same(const char *left, const char *right) {
        const MyString a(left);
        const MyString b(right);
        const std::string sa = left;
        const std::string sb = right;
        const std::string trace = std::string("left: \"") + left + "\", right: \"" + right + "\"";

        EXPECT_EQ(sa == sb, a == b) << trace;
        EXPECT_EQ(sa != sb, a != b) << trace;
        EXPECT_EQ(sa < sb, a < b) << trace;
        EXPECT_EQ(sa <= sb, a <= b) << trace;
        EXPECT_EQ(sa > sb, a > b) << trace;
        EXPECT_EQ(sa >= sb, a >= b) << trace;

        EXPECT_EQ(sa == sb, a == right) << trace;
        EXPECT_EQ(sa != sb, a != right) << trace;
        EXPECT_EQ(sa < sb, a < right) << trace;
        EXPECT_EQ(sa <= sb, a <= right) << trace;
        EXPECT_EQ(sa > sb, a > right) << trace;
        EXPECT_EQ(sa >= sb, a >= right) << trace;
    }

    // ---------- 스트림 도우미 ----------

    // operator>> 와 같은 규칙으로 std::string 에 읽기
    std::string read_word(std::istream &stream) {
        std::string word;
        stream >> word;
        return word;
    }

    // getline 과 같은 규칙으로 std::string 에 읽기
    std::string read_line(std::istream &stream, const char delim = '\n') {
        std::string line;
        std::getline(stream, line, delim);
        return line;
    }

    // 스트림이 어디까지 읽었는지 (상태 플래그와 무관하게 위치를 얻기 위해 잠시 상태를 지우고 되돌림)
    std::streamoff position_of(std::istream &stream) {
        const std::ios_base::iostate saved = stream.rdstate();
        stream.clear();
        const std::streamoff position = stream.tellg();
        stream.clear(saved);
        return position;
    }

    // 같은 입력을 MyString 쪽 스트림과 std::string 쪽 스트림에 나란히 읽혀서 비교한다.
    class StreamPair {
        public:
            explicit StreamPair(const std::string &text)
                : mine(text), ref(text) {
            }

            // operator>> : 읽은 값과 스트림 위치가 std 쪽과 같은지 확인
            std::string word(MyString &target) {
                mine >> target;
                const std::string expected = read_word(ref);
                if (!ref.fail()) {
                    expect_same(target, expected);
                }
                expect_same_stream();
                return expected;
            }

            std::string word() {
                MyString target;
                return word(target);
            }

            // getline : 읽은 값과 스트림 위치가 std 쪽과 같은지 확인
            std::string line(MyString &target, const char delim = '\n') {
                ::getline(mine, target, delim);
                const std::string expected = read_line(ref, delim);
                if (!ref.fail()) {
                    expect_same(target, expected);
                }
                expect_same_stream();
                return expected;
            }

            std::string line(const char delim = '\n') {
                MyString target;
                return line(target, delim);
            }

            // 두 스트림에 같은 동작 적용
            void get() {
                mine.get();
                ref.get();
            }

            void ignore_line() {
                mine.ignore(1000, '\n');
                ref.ignore(1000, '\n');
            }

            // 읽은 위치만 비교한다. (std 쪽이 읽기에 실패한 경우는 위치 비교를 하지 않는다.)
            void expect_same_stream() {
                if (!ref.fail()) {
                    EXPECT_EQ(position_of(ref), position_of(mine));
                }
            }

            std::istringstream mine;
            std::istringstream ref;
    };

    // 스트림 상태를 확인하지 않고 지정한 횟수만큼 읽는다.
    std::vector<std::string> collect_words_of_mystring(const std::string &text, const std::size_t count) {
        std::istringstream stream(text);
        MyString word;
        std::vector<std::string> words;
        for (std::size_t index = 0; index < count; ++index) {
            stream >> word;
            words.push_back(to_std_string(word));
        }
        return words;
    }

    std::vector<std::string> collect_words_of_std(const std::string &text) {
        std::istringstream stream(text);
        std::vector<std::string> words;
        std::string word;
        while (stream >> word) {
            words.push_back(word);
        }
        return words;
    }

    std::vector<std::string> collect_lines_of_mystring(const std::string &text, const std::size_t count) {
        std::istringstream stream(text);
        MyString line;
        std::vector<std::string> lines;
        for (std::size_t index = 0; index < count; ++index) {
            ::getline(stream, line);
            lines.push_back(to_std_string(line));
        }
        return lines;
    }

    std::vector<std::string> collect_lines_of_std(const std::string &text) {
        std::istringstream stream(text);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    const char *const SAMPLES[] = {"", "a", "abc", "hello", "1234567", "12345678", "abcdefghijklmnop"};
    const char *const SUBS[] = {"", "X", "XYZ", "0123456789abcdef"};
}

// =====================================================================
// 생성자 / 소멸자 / capacity
// =====================================================================

TEST(MyStringConstructor, DefaultCreatesEmptyString) {
    const MyString string;

    expect_same(string, "");
    EXPECT_EQ(8, string.getCapacity());
}

TEST(MyStringConstructor, FromCharArray) {
    expect_same(MyString("hello"), "hello");
}

TEST(MyStringConstructor, FromEmptyCharArray) {
    expect_same(MyString(""), "");
}

TEST(MyStringConstructor, FromLongCharArrayKeepsEverything) {
    const std::string text(1000, 'x');

    expect_same(MyString(text.c_str()), text);
}

TEST(MyStringConstructor, CopyCopiesContent) {
    const MyString original("copy");
    const MyString copied(original);

    expect_same(copied, "copy");
    expect_same(original, "copy");
}

TEST(MyStringConstructor, CopyOfLongStringKeepsEverything) {
    const std::string text(100, 'q');
    const MyString original(text.c_str());
    const MyString copied(original);

    expect_same(copied, text);
}

TEST(MyStringConstructor, CopyIsDeepCopy) {
    MyString original("abc");
    MyString copied(original);

    copied[0] = 'X';

    expect_same(original, "abc");
    expect_same(copied, "Xbc");
}

TEST(MyStringConstructor, ImplicitConversionFromCharArray) {
    const MyString string = "implicit";

    expect_same(string, "implicit");
}

TEST(MyStringCapacity, DefaultCapacityIsEight) {
    EXPECT_EQ(8, MyString().getCapacity());
}

TEST(MyStringCapacity, AlwaysMultipleOfEightAndAtLeastLength) {
    std::string text;

    for (int length = 0; length <= 100; ++length) {
        SCOPED_TRACE("length: " + std::to_string(length));
        const MyString string(text.c_str());
        expect_same(string, text);
        text += static_cast<char>('a' + length % 26);
    }
}

TEST(MyStringCapacity, LengthSevenFitsInEight) {
    EXPECT_EQ(8, MyString("1234567").getCapacity());
}

TEST(MyStringCapacity, LengthEightStillFitsInEight) {
    // 배열은 capacity + 1 칸이므로 8글자도 capacity 8 에 들어간다.
    EXPECT_EQ(8, MyString("12345678").getCapacity());
}

TEST(MyStringCapacity, LengthNineDoublesToSixteen) {
    EXPECT_EQ(16, MyString("123456789").getCapacity());
}

TEST(MyStringCapacity, GrowsByDoublingOnlyWhenAppending) {
    MyString string;
    std::string ref;
    int previous = string.getCapacity();

    for (int index = 0; index < 300; ++index) {
        string.append('a');
        ref += 'a';
        const int current = string.getCapacity();
        if (current != previous) {
            EXPECT_EQ(previous * 2, current) << "length: " << string.length();
            previous = current;
        }
    }
    expect_same(string, ref);
}

TEST(MyStringCapacity, ClearKeepsCapacityAndEmptiesString) {
    MyString string("this is a fairly long string");
    const int capacity = string.getCapacity();

    string.clear();

    expect_same(string, "");
    EXPECT_EQ(capacity, string.getCapacity());
}

TEST(MyStringCapacity, CanBeReusedAfterClear) {
    MyString string("something");

    string.clear();
    string.append("again");

    expect_same(string, "again");
}

// =====================================================================
// length / empty
// =====================================================================

TEST(MyStringLength, IsContentLengthNotCapacity) {
    EXPECT_EQ(3, MyString("abc").length());
    EXPECT_NE(MyString("abc").getCapacity(), MyString("abc").length());
}

TEST(MyStringLength, EveryLengthFromZeroToThirtyMatches) {
    std::string text;

    for (int length = 0; length <= 30; ++length) {
        SCOPED_TRACE("length: " + std::to_string(length));
        expect_same(MyString(text.c_str()), text);
        text += static_cast<char>('a' + length % 26);
    }
}

TEST(MyStringEmpty, DefaultIsEmpty) {
    EXPECT_TRUE(MyString().isEmpty());
}

TEST(MyStringEmpty, NonEmptyIsNotEmpty) {
    EXPECT_FALSE(MyString("a").isEmpty());
}

TEST(MyStringEmpty, ErasingEverythingMakesItEmpty) {
    MyString string("abc");

    string.erase(0, 3);

    EXPECT_TRUE(string.isEmpty());
}

// =====================================================================
// operator=
// =====================================================================

TEST(MyStringAssignment, CopiesContent) {
    MyString a("source");
    MyString b("target");

    b = a;

    expect_same(b, "source");
    expect_same(a, "source");
}

TEST(MyStringAssignment, FromEmptyMakesTargetEmpty) {
    MyString empty;
    MyString target("something");

    target = empty;

    expect_same(target, "");
}

TEST(MyStringAssignment, LongerSourceThanTargetCapacity) {
    const std::string text(100, 'z');
    MyString source(text.c_str());
    MyString target("ab");

    target = source;

    expect_same(target, text);
    expect_same(source, text);
}

TEST(MyStringAssignment, ShorterSourceThanTarget) {
    MyString source("ab");
    MyString target("a much longer target string");

    target = source;

    expect_same(target, "ab");
}

TEST(MyStringAssignment, ToSelfKeepsContent) {
    MyString a("self");
    MyString &reference = a;

    a = reference;

    expect_same(a, "self");
}

TEST(MyStringAssignment, IsDeepCopy) {
    MyString a("abc");
    MyString b;

    b = a;
    b[1] = 'X';

    expect_same(a, "abc");
    expect_same(b, "aXc");
}

TEST(MyStringAssignment, ReturnsReferenceForChaining) {
    MyString a("chain");
    MyString b;
    MyString c;

    c = b = a;

    expect_same(a, "chain");
    expect_same(b, "chain");
    expect_same(c, "chain");
}

TEST(MyStringAssignment, FromCharArray) {
    MyString string("old");

    string = "new value";

    expect_same(string, "new value");
}

TEST(MyStringAssignment, FromEmptyCharArray) {
    MyString string("old");

    string = "";

    expect_same(string, "");
}

TEST(MyStringAssignment, FromLongCharArray) {
    const std::string text(200, 'k');
    MyString string("old");

    string = text.c_str();

    expect_same(string, text);
}

TEST(MyStringAssignment, CharArrayAssignmentReturnsReference) {
    MyString string;

    MyString &returned = (string = "abc");

    EXPECT_EQ(&string, &returned);
}

// =====================================================================
// 이동 생성자 / 이동 대입
// =====================================================================

TEST(MyStringMove, ConstructorTakesContent) {
    MyString source("hello");

    const MyString destination(std::move(source));

    expect_same(destination, "hello");
    EXPECT_EQ(0, source.length());
}

TEST(MyStringMove, ConstructorWithLongString) {
    const std::string text(100, 'm');
    MyString source(text.c_str());

    const MyString destination(std::move(source));

    expect_same(destination, text);
}

TEST(MyStringMove, AssignmentTakesContent) {
    MyString source("hello");
    MyString destination("old content that is long");

    destination = std::move(source);

    expect_same(destination, "hello");
    EXPECT_EQ(0, source.length());
}

TEST(MyStringMove, MovedFromObjectCanBeReused) {
    MyString source("hello");
    MyString destination(std::move(source));

    source = "again";
    source.append("!");

    expect_same(source, "again!");
    expect_same(destination, "hello");
}

// =====================================================================
// at / operator[]
// =====================================================================

TEST(MyStringAt, MatchesStdString) {
    MyString string("abc");
    const std::string ref = "abc";

    for (std::size_t index = 0; index < ref.size(); ++index) {
        EXPECT_EQ(ref.at(index), string.at(static_cast<int>(index))) << "index: " << index;
    }
}

TEST(MyStringAt, CanModifyThroughReference) {
    MyString string("abc");

    string.at(1) = 'X';

    expect_same(string, "aXc");
}

// at 도 범위가 맞지 않으면 예외를 던지지 않고 '\0' 을 return 한다.
TEST(MyStringAt, OutOfRangeReturnsNullCharacter) {
    MyString string("abc");

    EXPECT_EQ('\0', string.at(3));
    EXPECT_EQ('\0', string.at(100));
    EXPECT_EQ('\0', string.at(-1));
}

TEST(MyStringAt, EmptyStringReturnsNullCharacter) {
    MyString string;

    EXPECT_EQ('\0', string.at(0));
}

TEST(MyStringAt, OutOfRangeDoesNotChangeString) {
    MyString string("abc");

    string.at(10);

    expect_same(string, "abc");
}

TEST(MyStringIndex, MatchesStdString) {
    MyString string("hello");
    const std::string ref = "hello";

    for (std::size_t index = 0; index < ref.size(); ++index) {
        EXPECT_EQ(ref[index], string[static_cast<int>(index)]) << "index: " << index;
    }
}

TEST(MyStringIndex, CanModifyThroughReference) {
    MyString string("abc");

    string[0] = 'Z';
    string[2] = 'Y';

    expect_same(string, "ZbY");
}

TEST(MyStringIndex, ConstVersionReads) {
    const MyString string("abc");

    EXPECT_EQ('a', string[0]);
    EXPECT_EQ('c', string[2]);
}

// 과제 조건: operator[] 는 범위가 맞지 않으면 '\0' return
TEST(MyStringIndex, OutOfRangeReturnsNullCharacter) {
    MyString string("abc");

    EXPECT_EQ('\0', string[3]);
    EXPECT_EQ('\0', string[100]);
    EXPECT_EQ('\0', string[-1]);
}

TEST(MyStringIndex, ConstOutOfRangeReturnsNullCharacter) {
    const MyString string("abc");

    EXPECT_EQ('\0', string[3]);
    EXPECT_EQ('\0', string[100]);
    EXPECT_EQ('\0', string[-1]);
}

TEST(MyStringIndex, EmptyStringReturnsNullCharacter) {
    const MyString string;

    EXPECT_EQ('\0', string[0]);
}

// =====================================================================
// operator+
// =====================================================================

TEST(MyStringPlus, WithMyString) {
    const MyString a("abc");
    const MyString b("def");

    expect_same(a + b, "abcdef");
}

TEST(MyStringPlus, WithCharArray) {
    const MyString a("abc");

    expect_same(a + "def", "abcdef");
}

TEST(MyStringPlus, DoesNotModifyOperands) {
    const MyString a("abc");
    const MyString b("def");

    const MyString result = a + b;
    (void) result;

    expect_same(a, "abc");
    expect_same(b, "def");
}

TEST(MyStringPlus, WithEmpty) {
    const MyString a("abc");
    const MyString empty;

    expect_same(a + empty, "abc");
    expect_same(empty + a, "abc");
    expect_same(empty + empty, "");
}

TEST(MyStringPlus, EmptyCharArray) {
    const MyString a("abc");

    expect_same(a + "", "abc");
}

TEST(MyStringPlus, CanBeChained) {
    const MyString a("a");
    const MyString b("b");

    expect_same(a + b + "c" + a, "abca");
}

TEST(MyStringPlus, SelfConcatenation) {
    const MyString a("ab");

    expect_same(a + a, "abab");
    expect_same(a, "ab");
}

TEST(MyStringPlus, GrowsBeyondInitialCapacity) {
    const MyString a("1234567890");
    const MyString b("abcdefghij");

    expect_same(a + b, "1234567890abcdefghij");
    expect_same(a + "abcdefghij", "1234567890abcdefghij");
}

TEST(MyStringPlus, VeryLongResult) {
    const std::string left(300, 'l');
    const std::string right(500, 'r');

    expect_same(MyString(left.c_str()) + MyString(right.c_str()), left + right);
}

TEST(MyStringPlus, AssignedResultMatchesDirectResult) {
    const MyString a("abc");
    const MyString b("def");
    MyString assigned;

    assigned = a + b;

    expect_same(assigned, "abcdef");
    EXPECT_TRUE(assigned == (a + b));
}

TEST(MyStringPlus, MatchesStdStringForManyCombinations) {
    for (const char *left: SAMPLES) {
        for (const char *right: SAMPLES) {
            SCOPED_TRACE(std::string("left: \"") + left + "\", right: \"" + right + "\"");
            const MyString a(left);
            const MyString b(right);
            const std::string expected = std::string(left) + right;

            expect_same(a + b, expected);
            expect_same(a + right, expected);
        }
    }
}

// =====================================================================
// operator+= / append
// =====================================================================

TEST(MyStringPlusAssign, WithMyString) {
    MyString a("abc");

    a += MyString("def");

    expect_same(a, "abcdef");
}

TEST(MyStringPlusAssign, WithCharArray) {
    MyString a("abc");

    a += "def";

    expect_same(a, "abcdef");
}

TEST(MyStringPlusAssign, ReturnsSelfForChaining) {
    MyString a("a");

    MyString &returned = (a += "b");
    returned += "c";

    EXPECT_EQ(&a, &returned);
    expect_same(a, "abc");
}

TEST(MyStringPlusAssign, SelfAppend) {
    MyString a("ab");

    a += a;

    expect_same(a, "abab");
}

TEST(MyStringPlusAssign, RepeatedAppendGrowsCorrectly) {
    MyString a;
    std::string ref;

    for (int index = 0; index < 50; ++index) {
        a += "xyz";
        ref += "xyz";
    }

    expect_same(a, ref);
}

TEST(MyStringPlusAssign, EmptyOperands) {
    MyString a("abc");
    MyString empty;

    a += empty;
    a += "";
    empty += "";

    expect_same(a, "abc");
    expect_same(empty, "");
}

TEST(MyStringAppend, CharArray) {
    MyString a("abc");

    a.append("def");

    expect_same(a, "abcdef");
}

TEST(MyStringAppend, MyString) {
    MyString a("abc");

    a.append(MyString("def"));

    expect_same(a, "abcdef");
}

TEST(MyStringAppend, SingleCharacter) {
    MyString a("abc");

    a.append('d');

    expect_same(a, "abcd");
}

TEST(MyStringAppend, ReturnsSelfForChaining) {
    MyString a;

    MyString &returned = a.append("a").append(MyString("b")).append('c');

    EXPECT_EQ(&a, &returned);
    expect_same(a, "abc");
}

TEST(MyStringAppend, SelfAppendNeedsReallocation) {
    // 길이 8 -> capacity 를 넘어서면서 자기 자신을 붙인다. (std::string 도 "12345678" * 2 가 된다.)
    MyString a("12345678");

    a.append(a);

    expect_same(a, "1234567812345678");
}

TEST(MyStringAppend, SelfAppendOfShortString) {
    MyString a("ab");

    a.append(a);

    expect_same(a, "abab");
}

TEST(MyStringAppend, FillsExactlyUpToCapacityBoundary) {
    MyString a;
    std::string ref;

    for (int index = 0; index < 40; ++index) {
        a.append(static_cast<char>('a' + index % 26));
        ref += static_cast<char>('a' + index % 26);
        SCOPED_TRACE("length: " + std::to_string(index + 1));
        expect_same(a, ref);
    }
}

// =====================================================================
// operator== / !=
// =====================================================================

TEST(MyStringEquality, SameContent) {
    expect_compare_same("hello", "hello");
}

TEST(MyStringEquality, DifferentContent) {
    expect_compare_same("hello", "world");
    expect_compare_same("world", "hello");
}

TEST(MyStringEquality, DifferentLength) {
    expect_compare_same("hello", "hello!");
    expect_compare_same("hello!", "hello");
}

TEST(MyStringEquality, PrefixIsNotEqual) {
    expect_compare_same("abc", "abcd");
    expect_compare_same("abcd", "abc");
}

TEST(MyStringEquality, EmptyAndNonEmpty) {
    expect_compare_same("", "a");
    expect_compare_same("a", "");
}

TEST(MyStringEquality, TwoEmptyStrings) {
    expect_compare_same("", "");
}

TEST(MyStringEquality, DifferenceAtLastCharacter) {
    expect_compare_same("abcde", "abcdf");
}

TEST(MyStringEquality, Self) {
    const MyString a("self");

    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a != a);
}

TEST(MyStringEquality, LongStringsThatDifferOnlyAtTheEnd) {
    const std::string base(100, 'a');
    std::string other = base;
    other.back() = 'b';

    expect_compare_same(base.c_str(), base.c_str());
    expect_compare_same(base.c_str(), other.c_str());
}

TEST(MyStringEquality, MatchesStdStringForManyCombinations) {
    const char *samples[] = {"", "a", "ab", "abc", "abd", "hello", "hellp", "123456789012345", "1234567890123456"};

    for (const char *left: samples) {
        for (const char *right: samples) {
            expect_compare_same(left, right);
        }
    }
}

// =====================================================================
// <, <=, >, >=
// =====================================================================

TEST(MyStringOrdering, BasicOrder) {
    expect_compare_same("apple", "banana");
    expect_compare_same("banana", "apple");
}

TEST(MyStringOrdering, PrefixIsSmaller) {
    expect_compare_same("app", "apple");
    expect_compare_same("apple", "app");
}

TEST(MyStringOrdering, EmptyIsSmallest) {
    expect_compare_same("", "a");
    expect_compare_same("a", "");
    expect_compare_same("", "");
}

TEST(MyStringOrdering, DecidedByFirstDifference) {
    expect_compare_same("abcz", "abd");
    expect_compare_same("abd", "abcz");
}

TEST(MyStringOrdering, UppercaseComesBeforeLowercase) {
    expect_compare_same("Zebra", "apple");
    expect_compare_same("apple", "Zebra");
}

TEST(MyStringOrdering, DigitsAndSymbols) {
    expect_compare_same("10", "9");
    expect_compare_same("a-b", "a_b");
}

TEST(MyStringOrdering, SameStringIsLessEqualAndGreaterEqual) {
    const MyString a("same");
    const MyString b("same");

    EXPECT_FALSE(a < b);
    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(a > b);
    EXPECT_TRUE(a >= b);
}

TEST(MyStringOrdering, MatchesStdStringForManyCombinations) {
    const char *samples[] = {"", "a", "A", "ab", "abc", "abd", "b", "hello", "hellp", "Hello", "12345678", "123456789"};

    for (const char *left: samples) {
        for (const char *right: samples) {
            expect_compare_same(left, right);
        }
    }
}

// =====================================================================
// operator<< (출력)
// =====================================================================

TEST(MyStringOutputOperator, PrintsSameAsStdString) {
    const char *samples[] = {"", "a", "hello", "hello world", "123456789012345", "1234567890123456789"};

    for (const char *text: samples) {
        std::ostringstream mine;
        std::ostringstream ref;

        mine << MyString(text);
        ref << std::string(text);

        EXPECT_EQ(ref.str(), mine.str()) << "text: \"" << text << "\"";
    }
}

TEST(MyStringOutputOperator, PrintsLongString) {
    const std::string text(500, 'p');
    std::ostringstream mine;

    mine << MyString(text.c_str());

    EXPECT_EQ(text, mine.str());
}

TEST(MyStringOutputOperator, DoesNotAppendNewline) {
    std::ostringstream mine;

    mine << MyString("abc");

    EXPECT_EQ(std::string::npos, mine.str().find('\n'));
}

TEST(MyStringOutputOperator, CanBeChainedWithOtherOutput) {
    const MyString a("abc");
    const MyString b("def");
    std::ostringstream mine;
    std::ostringstream ref;

    mine << a << "," << b << "!" << 42;
    ref << "abc" << "," << "def" << "!" << 42;

    EXPECT_EQ(ref.str(), mine.str());
}

TEST(MyStringOutputOperator, PrintsConcatenatedResult) {
    const MyString a("abc");
    std::ostringstream mine;

    mine << (a + "def");

    EXPECT_EQ("abcdef", mine.str());
}

TEST(MyStringOutputOperator, ReturnsStreamForChaining) {
    std::ostringstream stream;

    std::ostream &returned = (stream << MyString("a"));

    EXPECT_EQ(&stream, &returned);
}

// =====================================================================
// operator>>
// =====================================================================

TEST(MyStringInputOperator, ReadsWord) {
    StreamPair in("hello");

    in.word();
}

TEST(MyStringInputOperator, StopsAtWhitespace) {
    StreamPair in("hello world");

    in.word();
    in.word();
}

TEST(MyStringInputOperator, ReadsLongWordFully) {
    StreamPair in(std::string(100, 'x') + " tail");

    in.word();
    in.word();
}

TEST(MyStringInputOperator, ReadsVeryLongWordFully) {
    StreamPair in(std::string(2000, 'y'));

    in.word();
}

TEST(MyStringInputOperator, OverwritesExistingContent) {
    StreamPair in("new");
    MyString string("previous");

    in.word(string);
}

TEST(MyStringInputOperator, ReturnsStreamForChaining) {
    std::istringstream input("a b");
    MyString first;
    MyString second;

    std::istream &returned = (input >> first);
    returned >> second;

    EXPECT_EQ(&input, &returned);
    expect_same(first, "a");
    expect_same(second, "b");
}

TEST(MyStringInputOperator, SkipsLeadingWhitespace) {
    StreamPair in("   \n\t hello");

    in.word();
}

TEST(MyStringInputOperator, ReadsThreeWords) {
    StreamPair in("one two three");

    in.word();
    in.word();
    in.word();
}

TEST(MyStringInputOperator, StopsAtNewline) {
    StreamPair in("first\nsecond");

    in.word();
    in.word();
}

TEST(MyStringInputOperator, LeavesTrailingWhitespaceInStream) {
    StreamPair in("word   next");

    in.word();    // 공백을 소비하지 않고 남겨두는지도 위치 비교로 확인된다.
    in.word();
}

TEST(MyStringInputOperator, MatchesStdStringForVariousInputs) {
    const char *inputs[] = {
        "", " ", "a", "a b", "  lead", "trail  ", "tab\tseparated", "line\nbreak",
        "123456789012345", "1234567890123456", "12345678901234567890 next", "x 123456789012345678 y"
    };

    for (const char *input: inputs) {
        SCOPED_TRACE(std::string("input: \"") + input + "\"");
        const std::vector<std::string> expected = collect_words_of_std(input);
        EXPECT_EQ(expected, collect_words_of_mystring(input, expected.size()));
    }
}

// =====================================================================
// getline
// =====================================================================

TEST(MyStringGetLine, ReadsWholeLineIncludingSpaces) {
    StreamPair in("hello world\n");

    in.line();
}

TEST(MyStringGetLine, DoesNotIncludeDelimiter) {
    StreamPair in("abc\n");
    MyString string;

    in.line(string);

    EXPECT_EQ(std::string::npos, to_std_string(string).find('\n'));
}

TEST(MyStringGetLine, ConsumesDelimiterSoNextLineIsRead) {
    StreamPair in("first\nsecond\n");

    in.line();
    in.line();
}

TEST(MyStringGetLine, KeepsLeadingWhitespace) {
    StreamPair in("  hi\n");

    in.line();
}

TEST(MyStringGetLine, EmptyLineGivesEmptyString) {
    StreamPair in("\nnext\n");

    in.line();
    in.line();
}

TEST(MyStringGetLine, LastLineWithoutDelimiterIsRead) {
    StreamPair in("only line");

    in.line();
}

TEST(MyStringGetLine, CustomDelimiter) {
    StreamPair in("a,b,c");

    in.line(',');
    in.line(',');
    in.line(',');
}

TEST(MyStringGetLine, NewlineIsNotDelimiterWhenCustomDelimiterGiven) {
    StreamPair in("ab\ncd,ef");

    in.line(',');
}

TEST(MyStringGetLine, ReadsLongLineFully) {
    StreamPair in(std::string(300, 'g') + "\nnext\n");

    in.line();
    in.line();
}

TEST(MyStringGetLine, OverwritesExistingContent) {
    StreamPair in("new\n");
    MyString string("previous");

    in.line(string);
}

TEST(MyStringGetLine, ReturnsStreamForChaining) {
    std::istringstream input("a\nb\n");
    MyString first;
    MyString second;

    std::istream &returned = ::getline(input, first);
    ::getline(returned, second);

    EXPECT_EQ(&input, &returned);
    expect_same(first, "a");
    expect_same(second, "b");
}

TEST(MyStringGetLine, MatchesStdStringForVariousInputs) {
    const char *inputs[] = {
        "", "\n", "a", "a\n", "a\nb", "a\n\nb\n", "  spaced  \n", "123456789012345\n",
        "1234567890123456\nx\n", "12345678901234567890123\nshort\n\n", "no newline at end"
    };

    for (const char *input: inputs) {
        SCOPED_TRACE(std::string("input: \"") + input + "\"");
        const std::vector<std::string> expected = collect_lines_of_std(input);
        EXPECT_EQ(expected, collect_lines_of_mystring(input, expected.size()));
    }
}

// =====================================================================
// substr (교수님 예시 기준)
//  - pos >= 0 : pos 에서 len 글자. len 이 넘치면 가능한 최대 길이
//  - pos <  0 : -1 이 제일 뒷글자, 거기서 앞으로 len 글자. 넘치면 문자열 처음까지
//  - 불가능한 위치(pos < -size)와 범위 밖 pos(pos >= size)는 예외 없이 빈 문자열
// =====================================================================

TEST(MyStringSubstr, FromStart) {
    const MyString string("hello world");

    expect_same(string.substr(0, 5), "hello");
}

TEST(MyStringSubstr, FromMiddle) {
    const MyString string("hello world");

    expect_same(string.substr(6, 5), "world");
}

TEST(MyStringSubstr, SingleCharacter) {
    const MyString string("abc");

    expect_same(string.substr(1, 1), "b");
}

TEST(MyStringSubstr, WholeString) {
    const MyString string("abcde");

    expect_same(string.substr(0, 5), "abcde");
}

TEST(MyStringSubstr, LastCharacter) {
    const MyString string("abcde");

    expect_same(string.substr(4, 1), "e");
}

TEST(MyStringSubstr, ZeroLength) {
    const MyString string("hello");

    expect_same(string.substr(2, 0), "");
}

TEST(MyStringSubstr, LengthBeyondEndStopsAtEnd) {
    const MyString string("hello world");

    expect_same(string.substr(6, 100), "world");
}

TEST(MyStringSubstr, LengthOneBeyondEndStopsAtEnd) {
    const MyString string("abc");

    expect_same(string.substr(1, 3), "bc");
}

// 교수님이 주신 예시 그대로
TEST(MyStringSubstr, ProfessorExamples) {
    const MyString str("0123456789");

    expect_same(str.substr(-1, 3), "789");      // 제일 뒷글자부터 3글자
    expect_same(str.substr(-2, 2), "78");       // 뒤 두번째 글자 '8' 부터 앞으로 두 글자
    expect_same(str.substr(-5, 4), "2345");     // 뒤에서 다섯번째 '5' 부터 앞으로 4글자
    expect_same(str.substr(-5, 10), "012345");  // 범위를 넘어서면 문자열 처음까지
    expect_same(str.substr(-15, 3), "");        // 불가능한 위치
    expect_same(str.substr(20, 3), "");         // 예외처리 하지 않고 빈 문자열
}

TEST(MyStringSubstr, NegativePositionLastCharacterOnly) {
    const MyString str("0123456789");

    expect_same(str.substr(-1, 1), "9");
    expect_same(str.substr(-1, 100), "0123456789");
}

TEST(MyStringSubstr, NegativePositionAtFirstCharacter) {
    const MyString str("0123456789");

    expect_same(str.substr(-10, 1), "0");
    expect_same(str.substr(-10, 5), "0");       // 처음 글자에서 더 앞으로 갈 수 없다.
}

TEST(MyStringSubstr, NegativePositionJustOutOfRangeGivesEmpty) {
    const MyString str("0123456789");

    expect_same(str.substr(-11, 3), "");
    expect_same(str.substr(-100, 3), "");
}

TEST(MyStringSubstr, NegativePositionWithZeroLength) {
    const MyString str("0123456789");

    expect_same(str.substr(-3, 0), "");
}

TEST(MyStringSubstr, PositionAtOrBeyondEndGivesEmpty) {
    const MyString str("0123456789");

    expect_same(str.substr(10, 3), "");
    expect_same(str.substr(11, 3), "");
    expect_same(str.substr(20, 3), "");
    expect_same(str.substr(1000, 1), "");
}

TEST(MyStringSubstr, NegativePositionOnShortStrings) {
    expect_same(MyString("a").substr(-1, 1), "a");
    expect_same(MyString("a").substr(-2, 1), "");
    expect_same(MyString("ab").substr(-1, 2), "ab");
}

TEST(MyStringSubstr, EmptySourceGivesEmpty) {
    const MyString empty;

    expect_same(empty.substr(0, 3), "");
}

TEST(MyStringSubstr, DoesNotModifyOriginal) {
    const MyString string("hello");

    const MyString result = string.substr(1, 3);
    (void) result;

    expect_same(string, "hello");
}

TEST(MyStringSubstr, ResultIsIndependentCopy) {
    MyString string("hello");

    MyString part = string.substr(1, 3);
    part[0] = 'X';

    expect_same(string, "hello");
    expect_same(part, "Xll");
}

TEST(MyStringSubstr, FromLongString) {
    const std::string text = "0123456789abcdefghijklmnopqrstuvwxyz";
    const MyString string(text.c_str());

    expect_same(string.substr(10, 20), text.substr(10, 20));
    expect_same(string.substr(0, 36), text);
}

TEST(MyStringSubstr, MatchesExpectedForManyRanges) {
    const char *texts[] = {"", "a", "hello world", "123456789012345", "this is a rather long string of text"};

    for (const char *text: texts) {
        const MyString string(text);
        const std::string ref = text;

        for (int pos = -static_cast<int>(ref.size()) - 3; pos <= static_cast<int>(ref.size()) + 3; ++pos) {
            for (int len = 0; len <= 40; ++len) {
                SCOPED_TRACE("text: \"" + ref + "\", pos: " + std::to_string(pos) + ", len: " + std::to_string(len));
                expect_same(string.substr(pos, len), expected_substr(ref, pos, len));
            }
        }
    }
}

TEST(MyStringSubstr, CanBeConcatenated) {
    const MyString string("hello world");

    expect_same(string.substr(0, 5) + string.substr(5, 6), "hello world");
}

// =====================================================================
// find
//  - pos 가 음수이면 0, 문자열 범위 밖이면 마지막 위치로 취급
//  - 없으면 -1
// =====================================================================

TEST(MyStringFind, FindsAtStart) {
    expect_find_same("hello world", "hello", 0);
}

TEST(MyStringFind, FindsInMiddle) {
    expect_find_same("hello world", "lo w", 0);
}

TEST(MyStringFind, FindsAtEnd) {
    expect_find_same("hello world", "world", 0);
}

TEST(MyStringFind, FindsSingleCharacter) {
    expect_find_same("hello", "l", 0);
}

TEST(MyStringFind, FindsWholeString) {
    expect_find_same("hello", "hello", 0);
}

TEST(MyStringFind, NotFound) {
    expect_find_same("hello", "xyz", 0);
}

TEST(MyStringFind, ReturnsMinusOneWhenNotFound) {
    EXPECT_EQ(-1, MyString("hello").find("xyz"));
    EXPECT_EQ(-1, MyString("hello").find(MyString("xyz")));
}

TEST(MyStringFind, DefaultStartIsZero) {
    EXPECT_EQ(2, MyString("hello").find("ll"));
    EXPECT_EQ(2, MyString("hello").find(MyString("ll")));
}

TEST(MyStringFind, PatternLongerThanText) {
    expect_find_same("abc", "abcd", 0);
}

TEST(MyStringFind, EmptyText) {
    expect_find_same("", "a", 0);
}

TEST(MyStringFind, FirstOccurrence) {
    expect_find_same("abcabcabc", "abc", 0);
    expect_find_same("xabcabc", "abc", 0);
}

TEST(MyStringFind, StartPositionSkipsEarlierMatch) {
    expect_find_same("abcabcabc", "abc", 1);
    expect_find_same("abcabcabc", "abc", 3);
    expect_find_same("abcabcabc", "abc", 4);
}

TEST(MyStringFind, StartPositionAfterLastMatch) {
    expect_find_same("abcabc", "abc", 4);
}

TEST(MyStringFind, NegativeStartIsTreatedAsZero) {
    expect_find_same("hello", "he", -1);
    expect_find_same("hello", "he", -100);
    expect_find_same("hello", "lo", -3);
}

TEST(MyStringFind, StartBeyondLengthIsTreatedAsLastPosition) {
    char *pattern = new char[2];
    pattern[0] = 'o';
    pattern[1] = '\0';
    expect_find_same("hello", pattern, 6);
    expect_find_same("hello", pattern, 100);
    pattern[0] = 'h';
    expect_find_same("hello", pattern, 6);     // 마지막 위치부터 찾으므로 -1
    pattern[0] = 'o';
    expect_find_same("hello", pattern, 5);
}

TEST(MyStringFind, StartAtLastCharacter) {
    expect_find_same("hello", "o", 4);
}

TEST(MyStringFind, RepeatedCharacters) {
    expect_find_same("aaaaaaaaaa", "aaa", 0);
    expect_find_same("aaaaaaaaaa", "aaa", 7);
    expect_find_same("aaaaaaaaaa", "aaa", 8);
}

TEST(MyStringFind, PartialMatchBeforeRealMatch) {
    expect_find_same("aaaaab", "aab", 0);
    expect_find_same("abababc", "abc", 0);
}

TEST(MyStringFind, SamePrefixDifferentTail) {
    expect_find_same("abcabd", "abd", 0);
    expect_find_same("abcabd", "abe", 0);
}

TEST(MyStringFind, SameCharactersInDifferentOrder) {
    expect_find_same("abcdef", "cba", 0);
    expect_find_same("abcdef", "fed", 0);
}

TEST(MyStringFind, IsCaseSensitive) {
    expect_find_same("Hello", "hello", 0);
    expect_find_same("Hello", "Hello", 0);
}

TEST(MyStringFind, SpacesAndSymbols) {
    expect_find_same("a b-c_d", " ", 0);
    expect_find_same("a b-c_d", "-c_", 0);
}

TEST(MyStringFind, Digits) {
    expect_find_same("2026/10/12", "10", 0);
    expect_find_same("2026/10/12", "10", 6);
}

TEST(MyStringFind, HighAsciiCharacters) {
    expect_find_same("~}|{zyxwvutsrqp", "~}|", 0);
    expect_find_same("~}|{zyxwvutsrqp", "rqp", 0);
    expect_find_same("~}|{zyxwvutsrqp", "}~", 0);
}

TEST(MyStringFind, LongText) {
    std::string text;
    for (int index = 0; index < 200; ++index) {
        text += static_cast<char>('a' + index % 26);
    }
    text += "NEEDLE";
    for (int index = 0; index < 100; ++index) {
        text += static_cast<char>('a' + index % 26);
    }

    expect_find_same(text.c_str(), "NEEDLE", 0);
    expect_find_same(text.c_str(), "NEEDLE", 150);
    expect_find_same(text.c_str(), "NEEDLE", 206);
    expect_find_same(text.c_str(), "xyz", 0);
    expect_find_same(text.c_str(), "xyz", 250);
}

TEST(MyStringFind, PatternAtEveryPosition) {
    const std::string text = "abcdefghijklmnopqrstuvwxyz";

    for (std::size_t index = 0; index < text.size(); ++index) {
        const std::string pattern = text.substr(index, 3);
        expect_find_same(text.c_str(), pattern.c_str(), 0);
    }
}

TEST(MyStringFind, DoesNotModifyString) {
    const MyString string("hello");

    string.find(MyString("ll"), 0);

    expect_same(string, "hello");
}

TEST(MyStringFind, DoesNotModifyPattern) {
    const MyString string("hello");
    const MyString pattern("ll");

    string.find(pattern, 0);

    expect_same(pattern, "ll");
}

TEST(MyStringFind, StringCanBeUsedAsItsOwnPattern) {
    const MyString string("hello");

    EXPECT_EQ(0, string.find(string, 0));
}

TEST(MyStringFind, OnConcatenatedResult) {
    const MyString a("hello ");
    const MyString b("world");

    const MyString result = a + b;

    EXPECT_EQ(expected_find("hello world", "world", 0), result.find(MyString("world"), 0));
    EXPECT_EQ(expected_find("hello world", "o w", 0), result.find(MyString("o w"), 0));
}

TEST(MyStringFind, OnSubstrResult) {
    const MyString string("hello world");

    const MyString part = string.substr(6, 5);

    EXPECT_EQ(expected_find("world", "rl", 0), part.find(MyString("rl"), 0));
    EXPECT_EQ(expected_find("world", "hello", 0), part.find(MyString("hello"), 0));
}

TEST(MyStringFind, OnInputString) {
    StreamPair in("needle haystack");
    MyString first;
    MyString second;

    const std::string ref_first = in.word(first);
    const std::string ref_second = in.word(second);

    EXPECT_EQ(expected_find(ref_second, "st", 0), second.find(MyString("st"), 0));
    EXPECT_EQ(expected_find(ref_first, "st", 0), first.find(MyString("st"), 0));
}

TEST(MyStringFind, OnGetLineString) {
    StreamPair in("find in line\n");
    MyString line;

    const std::string ref_line = in.line(line);

    EXPECT_EQ(expected_find(ref_line, "in", 0), line.find(MyString("in"), 0));
}

TEST(MyStringFind, MatchesExpectedForManyInputs) {
    const char *texts[] = {"abcabcabc", "aaaaaaaaaa", "hello world", "2026/10/12", "abababababababa", "123456789012345"};
    const char *patterns[] = {"a", "ab", "aba", "abc", "aa", "o", "lo w", "10", "/", "5", "345", "zz", "ba"};

    for (const char *text: texts) {
        for (const char *pattern: patterns) {
            for (int start = -3; start <= 17; ++start) {
                expect_find_same(text, pattern, start);
            }
        }
    }
}

// =====================================================================
// insert
//  - pos 가 음수이면 0, 문자열 범위 밖(pos >= size)이면 마지막 위치로 취급
// =====================================================================

TEST(MyStringInsert, AtStart) {
    MyString string("world");

    string.insert(0, "hello ");

    expect_same(string, "hello world");
}

TEST(MyStringInsert, InMiddle) {
    MyString string("held");

    string.insert(2, "llo wor");

    expect_same(string, "hello world");
}

TEST(MyStringInsert, MyStringOverload) {
    MyString string("ac");

    string.insert(1, MyString("b"));

    expect_same(string, "abc");
}

TEST(MyStringInsert, NegativePositionIsTreatedAsZero) {
    MyString string("bc");

    string.insert(-5, "a");

    expect_same(string, "abc");
}

TEST(MyStringInsert, PositionBeyondEndIsTreatedAsLastPosition) {
    MyString string("abc");

    string.insert(100, "X");

    expect_same(string, "abcX");
}

TEST(MyStringInsert, EmptySubstringChangesNothing) {
    MyString string("abc");

    string.insert(1, "");

    expect_same(string, "abc");
}

TEST(MyStringInsert, IntoEmptyString) {
    MyString string;

    string.insert(0, "abc");

    expect_same(string, "abc");
}

TEST(MyStringInsert, GrowsBeyondCapacity) {
    MyString string("1234567");

    string.insert(3, "abcdefghijklmnopqrstuvwxyz");

    expect_same(string, "123abcdefghijklmnopqrstuvwxyz4567");
}

TEST(MyStringInsert, ReturnsSelfForChaining) {
    MyString string("ad");

    MyString &returned = string.insert(1, "b").insert(2, "c");

    EXPECT_EQ(&string, &returned);
    expect_same(string, "abcd");
}

TEST(MyStringInsert, MatchesExpectedForManyCombinations) {
    for (const char *text: SAMPLES) {
        for (const char *sub: SUBS) {
            for (int pos = -2; pos <= 20; ++pos) {
                SCOPED_TRACE(std::string("text: \"") + text + "\", sub: \"" + sub + "\", pos: " + std::to_string(pos));
                MyString by_array(text);
                MyString by_string(text);
                const std::string expected = expected_insert(text, pos, sub);

                expect_same(by_array.insert(pos, sub), expected);
                expect_same(by_string.insert(pos, MyString(sub)), expected);
            }
        }
    }
}

// =====================================================================
// replace
//  - str[pos] 에서 str[pos+len-1] 까지를 subStr 로 교체
//  - len 이 맞지 않으면 가능한 최대 길이로 취급
// =====================================================================

TEST(MyStringReplace, SameLengthReplacement) {
    MyString string("abcdef");

    string.replace(2, 2, "XY");

    expect_same(string, "abXYef");
}

// 질문했던 예: abcdef 에서 replace(2, 2, "a") -> c, d 두 글자가 a 한 글자로 바뀐다.
TEST(MyStringReplace, ShorterReplacement) {
    MyString string("abcdef");

    string.replace(2, 2, "a");

    expect_same(string, "abaef");
}

TEST(MyStringReplace, LongerReplacement) {
    MyString string("abcdef");

    string.replace(2, 2, "XYZW");

    expect_same(string, "abXYZWef");
}

TEST(MyStringReplace, EmptyReplacementActsLikeErase) {
    MyString string("abcdef");

    string.replace(2, 2, "");

    expect_same(string, "abef");
}

TEST(MyStringReplace, ZeroLengthActsLikeInsert) {
    MyString string("abcdef");

    string.replace(2, 0, "XY");

    expect_same(string, "abXYcdef");
}

TEST(MyStringReplace, WholeString) {
    MyString string("abcdef");

    string.replace(0, 6, "XYZ");

    expect_same(string, "XYZ");
}

TEST(MyStringReplace, MyStringOverload) {
    MyString string("abcdef");

    string.replace(1, 3, MyString("-"));

    expect_same(string, "a-ef");
}

TEST(MyStringReplace, LengthBeyondEndStopsAtEnd) {
    MyString string("abcdef");

    string.replace(3, 100, "X");

    expect_same(string, "abcX");
}

TEST(MyStringReplace, NegativePositionIsTreatedAsZero) {
    MyString string("abcdef");

    string.replace(-4, 2, "X");

    expect_same(string, "Xcdef");
}

TEST(MyStringReplace, PositionBeyondEndIsTreatedAsLastPosition) {
    MyString string("abcdef");

    string.replace(100, 3, "XY");

    expect_same(string, "abcdefXY");
}

TEST(MyStringReplace, LastCharacter) {
    MyString string("abcdef");

    string.replace(5, 1, "XYZ");

    expect_same(string, "abcdeXYZ");
}

TEST(MyStringReplace, GrowsBeyondCapacity) {
    MyString string("abcdef");

    string.replace(1, 1, "0123456789012345678901234567890");

    expect_same(string, "a0123456789012345678901234567890cdef");
}

TEST(MyStringReplace, ReturnsSelfForChaining) {
    MyString string("abcdef");

    MyString &returned = string.replace(0, 1, "X").replace(5, 1, "Y");

    EXPECT_EQ(&string, &returned);
    expect_same(string, "XbcdeY");
}

TEST(MyStringReplace, MatchesExpectedForManyCombinations) {
    for (const char *text: SAMPLES) {
        for (const char *sub: SUBS) {
            for (int pos = -2; pos <= 18; ++pos) {
                for (int len = 0; len <= 18; ++len) {
                    SCOPED_TRACE(std::string("text: \"") + text + "\", sub: \"" + sub + "\", pos: " + std::to_string(pos)
                                 + ", len: " + std::to_string(len));
                    MyString by_array(text);
                    MyString by_string(text);
                    const std::string expected = expected_replace(text, pos, len, sub);

                    expect_same(by_array.replace(pos, len, sub), expected);
                    expect_same(by_string.replace(pos, len, MyString(sub)), expected);
                }
            }
        }
    }
}

// =====================================================================
// erase
// =====================================================================

TEST(MyStringErase, FromStart) {
    MyString string("abcdef");

    string.erase(0, 2);

    expect_same(string, "cdef");
}

TEST(MyStringErase, InMiddle) {
    MyString string("abcdef");

    string.erase(2, 2);

    expect_same(string, "abef");
}

TEST(MyStringErase, AtEnd) {
    MyString string("abcdef");

    string.erase(4, 2);

    expect_same(string, "abcd");
}

TEST(MyStringErase, WholeString) {
    MyString string("abcdef");

    string.erase(0, 6);

    expect_same(string, "");
}

TEST(MyStringErase, ZeroCountChangesNothing) {
    MyString string("abcdef");

    string.erase(2, 0);

    expect_same(string, "abcdef");
}

TEST(MyStringErase, CountBeyondEndStopsAtEnd) {
    MyString string("abcdef");

    string.erase(2, 100);

    expect_same(string, "ab");
}

TEST(MyStringErase, NegativePositionIsTreatedAsZero) {
    MyString string("abcdef");

    string.erase(-3, 2);

    expect_same(string, "cdef");
}

TEST(MyStringErase, PositionBeyondEndIsTreatedAsLastPosition) {
    MyString string("abcdef");

    string.erase(100, 3);

    expect_same(string, "abcdef");
}

TEST(MyStringErase, FromEmptyStringStaysEmpty) {
    MyString string;

    string.erase(0, 3);

    expect_same(string, "");
}

TEST(MyStringErase, ReturnsSelfForChaining) {
    MyString string("abcdef");

    MyString &returned = string.erase(0, 1).erase(0, 1);

    EXPECT_EQ(&string, &returned);
    expect_same(string, "cdef");
}

TEST(MyStringErase, LongString) {
    std::string text;
    for (int index = 0; index < 100; ++index) {
        text += static_cast<char>('a' + index % 26);
    }
    MyString string(text.c_str());

    string.erase(10, 50);
    text.erase(10, 50);

    expect_same(string, text);
}

TEST(MyStringErase, MatchesExpectedForManyCombinations) {
    for (const char *text: SAMPLES) {
        for (int pos = -2; pos <= 18; ++pos) {
            for (int cnt = 0; cnt <= 18; ++cnt) {
                SCOPED_TRACE(std::string("text: \"") + text + "\", pos: " + std::to_string(pos) + ", cnt: " + std::to_string(cnt));
                MyString string(text);

                expect_same(string.erase(pos, cnt), expected_erase(text, pos, cnt));
            }
        }
    }
}

// =====================================================================
// stoi (과제 PDF 의 예시 포함)
// =====================================================================

TEST(MyStringStoi, PdfExamples) {
    EXPECT_EQ(10, ::stoi(MyString("123axyz"), 3, 16));
    EXPECT_EQ(171, ::stoi(MyString("123ab"), 3, 16));
    EXPECT_EQ(0, ::stoi(MyString("-123defg")));
    EXPECT_EQ(123, ::stoi(MyString("123ab456"), 0, 10));
}

TEST(MyStringStoi, DefaultArguments) {
    EXPECT_EQ(42, ::stoi(MyString("42")));
}

TEST(MyStringStoi, StopsAtFirstInvalidCharacter) {
    EXPECT_EQ(12, ::stoi(MyString("12abc")));
    EXPECT_EQ(12, ::stoi(MyString("12 34")));
}

TEST(MyStringStoi, NoDigitsGivesZero) {
    EXPECT_EQ(0, ::stoi(MyString("abc")));
    EXPECT_EQ(0, ::stoi(MyString("")));
    EXPECT_EQ(0, ::stoi(MyString(" 12")));
}

TEST(MyStringStoi, StartPosition) {
    EXPECT_EQ(456, ::stoi(MyString("123456"), 3));
    EXPECT_EQ(6, ::stoi(MyString("123456"), 5));
}

TEST(MyStringStoi, StartPositionAtOrBeyondEndGivesZero) {
    EXPECT_EQ(0, ::stoi(MyString("123"), 3));
    EXPECT_EQ(0, ::stoi(MyString("123"), 10));
}

TEST(MyStringStoi, Binary) {
    EXPECT_EQ(5, ::stoi(MyString("101"), 0, 2));
    EXPECT_EQ(2, ::stoi(MyString("102"), 0, 2));   // '2' 는 2진수에 없다.
}

TEST(MyStringStoi, Octal) {
    EXPECT_EQ(64, ::stoi(MyString("100"), 0, 8));
    EXPECT_EQ(7, ::stoi(MyString("789"), 0, 8));
}

TEST(MyStringStoi, Hexadecimal) {
    EXPECT_EQ(255, ::stoi(MyString("ff"), 0, 16));
    EXPECT_EQ(0xabc, ::stoi(MyString("abc"), 0, 16));
}

TEST(MyStringStoi, Base36) {
    EXPECT_EQ(35, ::stoi(MyString("z"), 0, 36));
    EXPECT_EQ(35 * 36 + 35, ::stoi(MyString("zz"), 0, 36));
    EXPECT_EQ(36, ::stoi(MyString("10"), 0, 36));
}

TEST(MyStringStoi, WorksOnResultOfOtherOperations) {
    const MyString string("id=1234&x");

    EXPECT_EQ(1234, ::stoi(string, string.find("=") + 1));
    EXPECT_EQ(1234, ::stoi(string.substr(3, 4)));
}

TEST(MyStringStoi, ReadFromStream) {
    std::istringstream in("77 rest");
    MyString word;

    in >> word;

    EXPECT_EQ(77, ::stoi(word));
}

// =====================================================================
// operator>> 와 getline 을 하나의 스트림에서 함께 사용
//
//  같은 입력을 std::istringstream(MyString 용)과 또 하나의 std::istringstream(std::string 용)에
//  똑같은 순서로 읽혀서, 읽은 값과 스트림 위치가 같은지 비교한다.
//  (operator>> 는 뒤따르는 공백을 소비하지 않고, getline 은 delim 을 소비한다.)
// =====================================================================

// ---------- >> 다음에 getline ----------

TEST(MyStringStreamMixTest, GetlineAfterExtractionReadsRestOfSameLine) {
    StreamPair in("word rest of line\nnext\n");

    in.word();
    in.line();
}

TEST(MyStringStreamMixTest, GetlineRightAfterExtractionAtLineEnd) {
    StreamPair in("word\nrest\n");

    in.word();
    in.line();    // 개행이 남아 있으므로 빈 줄
    in.line();
}

TEST(MyStringStreamMixTest, GetlineAfterExtractionAndManualNewlineSkip) {
    StreamPair in("word\nrest of line\n");

    in.word();
    in.get();
    in.line();
}

TEST(MyStringStreamMixTest, GetlineAfterExtractionAndIgnore) {
    StreamPair in("word\nrest of line\n");

    in.word();
    in.ignore_line();
    in.line();
}

TEST(MyStringStreamMixTest, ExtractionThenGetlineThenExtraction) {
    StreamPair in("first second\nthird fourth\n");

    in.word();
    in.line();
    in.word();
}

// ---------- getline 다음에 >> ----------

TEST(MyStringStreamMixTest, ExtractionAfterGetlineReadsNextLineWord) {
    StreamPair in("whole line here\nword other\n");

    in.line();
    in.word();
}

TEST(MyStringStreamMixTest, ExtractionSkipsEmptyLinesLeftByGetline) {
    StreamPair in("line\n\n\n   word\n");

    in.line();
    in.word();
}

TEST(MyStringStreamMixTest, AlternatesBetweenGetlineAndExtraction) {
    StreamPair in("title line\nkeyword extra\nsecond title\nkeyword2\n");

    in.line();
    in.word();
    in.ignore_line();
    in.line();
    in.word();
}

// ---------- 긴 입력의 조합 ----------

TEST(MyStringStreamMixTest, LongWordThenGetline) {
    StreamPair in(std::string(100, 'a') + " tail of line\nnext\n");

    in.word();
    in.line();
    in.line();
}

TEST(MyStringStreamMixTest, LongLineThenExtraction) {
    StreamPair in(std::string(100, 'b') + "\nword\n");

    in.line();
    in.word();
}

// ---------- 구분자(delim) 지정 ----------

TEST(MyStringStreamMixTest, CustomDelimiterWithExtraction) {
    StreamPair in("a,b c,d");

    in.line(',');
    in.word();
    in.line(',');
}

TEST(MyStringStreamMixTest, CustomDelimiterConsumesDelimiterBeforeExtraction) {
    StreamPair in("key=value other");

    in.line('=');
    in.word();
    in.word();
}

TEST(MyStringStreamMixTest, NewlineIsPlainCharacterForCustomDelimiter) {
    StreamPair in("ab\ncd,ef gh");

    in.line(',');
    in.word();
}

// ---------- 다른 입력 방식과의 조합 ----------

TEST(MyStringStreamMixTest, IntegerExtractionThenGetline) {
    StreamPair in("42\nhello world\n");
    int mine_number = 0;
    int ref_number = 0;

    in.mine >> mine_number;
    in.ref >> ref_number;
    in.ignore_line();
    in.line();

    EXPECT_EQ(ref_number, mine_number);
}

TEST(MyStringStreamMixTest, GetlineThenIntegerExtraction) {
    StreamPair in("hello world\n42\n");
    int mine_number = 0;
    int ref_number = 0;

    in.line();
    in.mine >> mine_number;
    in.ref >> ref_number;

    EXPECT_EQ(ref_number, mine_number);
    in.expect_same_stream();
}

TEST(MyStringStreamMixTest, MyStringAndStdStringCanShareStream) {
    std::istringstream input("mine yours\nrest of line\n");
    MyString mine;
    MyString line;
    std::string yours;

    input >> mine >> yours;
    input.ignore(1000, '\n');
    ::getline(input, line);

    expect_same(mine, "mine");
    EXPECT_EQ("yours", yours);
    expect_same(line, "rest of line");
}

// ---------- 반환값 / 체이닝 ----------

TEST(MyStringStreamMixTest, GetlineCanTakeResultOfExtraction) {
    std::istringstream input("word tail\n");
    MyString word;
    MyString tail;

    ::getline(input >> word, tail);

    expect_same(word, "word");
    expect_same(tail, " tail");
}

TEST(MyStringStreamMixTest, ExtractionCanTakeResultOfGetline) {
    std::istringstream input("line one\nword\n");
    MyString line;
    MyString word;

    ::getline(input, line) >> word;

    expect_same(line, "line one");
    expect_same(word, "word");
}

TEST(MyStringStreamMixTest, BothReturnTheSameStream) {
    std::istringstream input("a b\nc\n");
    MyString a;
    MyString b;

    std::istream &after_extraction = (input >> a);
    std::istream &after_getline = ::getline(input, b);

    EXPECT_EQ(&input, &after_extraction);
    EXPECT_EQ(&input, &after_getline);
}

// ---------- 덮어쓰기 / 반복 읽기 ----------

TEST(MyStringStreamMixTest, SameVariableIsOverwrittenByEachRead) {
    StreamPair in("first\nsecond line\nthird\n");
    MyString value;

    in.word(value);
    in.word(value);
    in.line(value);
    in.line(value);
}

TEST(MyStringStreamMixTest, ReadsEmptyLineThenWordIntoPreviouslyFilledString) {
    StreamPair in("\nword\n");
    MyString value("previous");

    in.line(value);
    in.word(value);
}

TEST(MyStringStreamMixTest, ExtractionLoopReadsAllWords) {
    const std::string text = "one two\nthree\n\n four\n";

    const std::vector<std::string> expected = collect_words_of_std(text);

    EXPECT_EQ(expected, collect_words_of_mystring(text, expected.size()));
}

TEST(MyStringStreamMixTest, GetlineLoopReadsAllLines) {
    const std::string text = "alpha\nbeta gamma\n\ndelta\n";

    const std::vector<std::string> expected = collect_lines_of_std(text);

    EXPECT_EQ(expected, collect_lines_of_mystring(text, expected.size()));
}

TEST(MyStringStreamMixTest, FirstWordThenRestOfEachLine) {
    // 각 줄을 "명령어 + 나머지" 로 나누어 읽는 전형적인 사용 예
    StreamPair in("add apple pie\nremove old\n");

    for (int index = 0; index < 2; ++index) {
        in.word();
        in.line();
    }
}

TEST(MyStringStreamMixTest, NumbersThenLines) {
    StreamPair in("  123 456\n789\n");
    MyString str1, str2, str3, str4;

    in.word(str1);
    in.word(str2);
    in.line(str3);
    in.line(str4);
}

// ---------- 읽은 결과의 사용 ----------

TEST(MyStringStreamMixTest, ReadValuesCanBeConcatenated) {
    StreamPair in("hello\nworld\n");
    MyString first;
    MyString second;

    const std::string ref_first = in.word(first);
    in.get();
    const std::string ref_second = in.line(second);

    expect_same(first + " " + second, ref_first + " " + ref_second);
}

TEST(MyStringStreamMixTest, ReadValuesCanBeCompared) {
    StreamPair in("same\nsame\n");
    MyString from_extraction;
    MyString from_getline;

    in.word(from_extraction);
    in.get();
    in.line(from_getline);

    EXPECT_TRUE(from_extraction == from_getline);
}

TEST(MyStringStreamMixTest, ReadValuesCanBePrinted) {
    StreamPair in("alpha beta gamma\n");
    MyString word;
    MyString rest;
    std::ostringstream mine;
    std::ostringstream ref;

    const std::string ref_word = in.word(word);
    const std::string ref_rest = in.line(rest);
    mine << word << "|" << rest;
    ref << ref_word << "|" << ref_rest;

    EXPECT_EQ(ref.str(), mine.str());
}

TEST(MyStringStreamMixTest, ReadValuesCanBeSubstringed) {
    StreamPair in("key: value\n");
    MyString key;
    MyString rest;

    in.word(key);
    const std::string ref_rest = in.line(rest);

    expect_same(rest.substr(1, 5), expected_substr(ref_rest, 1, 5));
}

// ---------- operator>> : EOF / 스트림 상태 ----------

TEST(MyStringInputEofTest, LastWordWithoutTrailingWhitespaceSucceeds) {
    std::istringstream in("abc");              // 끝에 공백/개행 없음
    MyString a;
    ASSERT_TRUE(static_cast<bool>(in >> a));
    expect_same(a, "abc");
}

TEST(MyStringInputEofTest, WhileLoopReadsEveryWord) {
    std::istringstream in("one two three");    // 마지막 단어 뒤 개행 없음
    MyString w;
    int count = 0;
    while (in >> w) ++count;
    EXPECT_EQ(count, 3);
}

TEST(MyStringInputEofTest, TrailingNewlineDoesNotAddExtraRead) {
    std::istringstream in("one two\n");
    MyString w;
    int count = 0;
    while (in >> w) ++count;
    EXPECT_EQ(count, 2);
}

TEST(MyStringInputEofTest, ReadAfterLastWordFails) {
    std::istringstream in("abc");
    MyString a, b;
    in >> a;
    EXPECT_FALSE(static_cast<bool>(in >> b));
    expect_same(b, "");                          // 실패해도 clear된 상태
}

TEST(MyStringInputEofTest, EmptyStreamFails) {
    std::istringstream in("");
    MyString a("old");
    EXPECT_FALSE(static_cast<bool>(in >> a));
    expect_same(a, "");
}

TEST(MyStringInputEofTest, WhitespaceOnlyFails) {
    std::istringstream in("   \n\t  ");
    MyString a;
    EXPECT_FALSE(static_cast<bool>(in >> a));
}

TEST(MyStringInputEofTest, AlreadyFailedStreamStaysFailed) {
    std::istringstream in("abc");
    in.setstate(std::ios::failbit);
    MyString a;
    in >> a;
    EXPECT_TRUE(in.fail());
    // 초기화 안 된 char 를 putback 하면 UB -> sanitizer/valgrind 에서만 잡힘
}

TEST(MyStringInputEofTest, NonAsciiBytesDoNotBreakIsspace) {
    std::istringstream in("안녕 world");        // UTF-8 바이트는 signed char 에서 음수
    MyString a, b;
    in >> a >> b;                              // MSVC 디버그: isspace assertion
    expect_same(a, "안녕");
    expect_same(b, "world");
}

// ---------- getline : EOF / 스트림 상태 ----------

TEST(MyStringGetlineEofTest, LastLineWithoutDelimiterSucceeds) {
    std::istringstream in("last line");
    MyString s;
    ASSERT_TRUE(static_cast<bool>(getline(in, s)));
    expect_same(s, "last line");
}

TEST(MyStringGetlineEofTest, WhileLoopReadsEveryLine) {
    std::istringstream in("line1\nline2");     // 마지막 줄 개행 없음
    MyString s;
    int count = 0;
    while (getline(in, s)) ++count;
    EXPECT_EQ(count, 2);
}

TEST(MyStringGetlineEofTest, TrailingNewlineDoesNotAddExtraLine) {
    std::istringstream in("a\nb\n");
    MyString s;
    int count = 0;
    while (getline(in, s)) ++count;
    EXPECT_EQ(count, 2);                       // 3이 되면 안 됨
}

TEST(MyStringGetlineEofTest, EmptyStreamFails) {
    std::istringstream in("");
    MyString s("old");
    EXPECT_FALSE(static_cast<bool>(getline(in, s)));
    expect_same(s, "");
}

TEST(MyStringGetlineEofTest, LoneDelimiterIsOneEmptyLine) {
    std::istringstream in("\n");
    MyString s("old");
    ASSERT_TRUE(static_cast<bool>(getline(in, s)));
    expect_same(s, "");
    EXPECT_FALSE(static_cast<bool>(getline(in, s)));
}

TEST(MyStringGetlineEofTest, CustomDelimiterLastTokenWithoutDelimiter) {
    std::istringstream in("abc,def");
    MyString s;
    int count = 0;
    while (getline(in, s, ',')) ++count;
    EXPECT_EQ(count, 2);
}