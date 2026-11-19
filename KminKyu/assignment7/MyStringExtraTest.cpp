#include <gtest/gtest.h>
#include <climits>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "MyString.h"

// =====================================================================
// MyStringTest.cpp 에서 다루지 않은 부분을 보충하는 테스트
//
//  - integerToString (0 이상) / trim (' ' 공백) / stoi / erase 경계값 / 비 ASCII(한글) 비교
//  - moved-from(이동 당한) 객체의 안전한 재할당·소멸·추가
//  - nullptr 방어 코드가 있는 메서드(append, insert, replace, find, compare)
//  - 자기 자신을 인자로 넘기는 경우 (s.insert(0, s) 등)
//  - std::string 과 무작위 연산을 비교하는 차분(differential) 테스트
//
//  전제 조건 (위반 시 UB, 테스트 제외):
//   - MyString(nullptr) 생성 및 operator=(nullptr) 대입
//   - moved-from 상태(charArray == nullptr)에서의 읽기/복사/erase
//   - 범위 밖 인덱스(operator[]/at)에 값을 대입하는 쓰기
//   - integerToString / stoi 의 음수 처리
//
//  메모리 오류(예: erase 버퍼 초과 읽기, trim 의 charArray[-1] 접근)는
//  반드시 AddressSanitizer(-fsanitize=address,undefined) 와 함께 실행해서 확인할 것.
// =====================================================================

namespace {
    std::string to_std_string(const MyString &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    void expect_same(const MyString &actual, const std::string &expected) {
        EXPECT_EQ(expected, to_std_string(actual));
        EXPECT_EQ(static_cast<int>(expected.size()), actual.length());
        EXPECT_EQ(expected.empty(), actual.isEmpty());
        EXPECT_EQ('\0', actual[actual.length()]);
        EXPECT_GE(actual.getCapacity(), actual.length());
        EXPECT_EQ(0, actual.getCapacity() % 8);
    }

    int clamp_pos(const std::string &text, const int pos) {
        if (pos < 0) {
            return 0;
        }
        const int size = static_cast<int>(text.size());
        return pos > size ? size : pos;
    }

    std::size_t to_len(const int len) {
        return len < 0 ? std::string::npos : static_cast<std::size_t>(len);
    }

    std::string make_text(const int length, const char start = 'a') {
        std::string text;
        for (int index = 0; index < length; ++index) {
            text += static_cast<char>(start + index % 26);
        }
        return text;
    }
}

// ---------------------------------------------------------------------
// integerToString (0 이상 정수)
// ---------------------------------------------------------------------

TEST(MyStringIntegerToString, SingleDigit) {
    for (int value = 0; value <= 9; ++value) {
        expect_same(MyString::integerToString(value), std::to_string(value));
    }
}

TEST(MyStringIntegerToString, MultipleDigits) {
    const int values[] = {10, 11, 19, 20, 99, 100, 101, 123, 999, 1000, 1024, 19980123, 20250907, 1234567890, INT_MAX};
    for (const int value : values) {
        expect_same(MyString::integerToString(value), std::to_string(value));
    }
}

TEST(MyStringIntegerToString, PowersOfTen) {
    int value = 1;
    for (int count = 0; count < 10; ++count) {
        expect_same(MyString::integerToString(value), std::to_string(value));
        if (count < 9) {
            value *= 10;
        }
    }
}

// ---------------------------------------------------------------------
// trim (' ' 앞뒤 공백 제거)
// ---------------------------------------------------------------------

TEST(MyStringTrim, RemovesLeadingAndTrailingSpaces) {
    const std::pair<const char *, const char *> cases[] = {
        {"abc", "abc"},
        {"  abc", "abc"},
        {"abc  ", "abc"},
        {"  abc  ", "abc"},
        {" a", "a"},
        {"a ", "a"},
        {"a", "a"},
        {"  Jenny Anh  ", "Jenny Anh"},
        {"a   b", "a   b"},
    };
    for (const auto &[input, expected] : cases) {
        MyString string(input);
        string.trim();
        expect_same(string, expected);
    }
}

TEST(MyStringTrim, EmptyAndOnlySpaces) {
    const char *cases[] = {"", " ", "   ", "        ", "                "};
    for (const char *input : cases) {
        MyString string(input);
        string.trim();
        expect_same(string, "");
    }
}

TEST(MyStringTrim, ReturnsReferenceToItself) {
    MyString string("  abc  ");
    EXPECT_EQ(&string, &string.trim());
}

TEST(MyStringTrim, LongString) {
    const std::string body = make_text(50);
    MyString string(("     " + body + "      ").c_str());
    string.trim();
    expect_same(string, body);
}

// ---------------------------------------------------------------------
// erase 경계값 (ASan 과 함께 실행해야 버퍼 초과 읽기를 확실히 잡는다)
// ---------------------------------------------------------------------

TEST(MyStringEraseBoundary, EveryLengthEveryPositionEveryCount) {
    for (int length = 0; length <= 40; ++length) {
        const std::string base = make_text(length);
        for (int pos = -1; pos <= length + 1; ++pos) {
            const int counts[] = {-1, 0, 1, 2, length - pos, length, length + 5};
            for (const int count : counts) {
                MyString actual(base.c_str());
                std::string expected = base;
                expected.erase(static_cast<std::size_t>(clamp_pos(base, pos)), to_len(count));
                actual.erase(pos, count);
                expect_same(actual, expected);
                if (HasFailure()) {
                    FAIL() << "length: " << length << ", pos: " << pos << ", count: " << count;
                }
            }
        }
    }
}

TEST(MyStringEraseBoundary, WholeStringWhenSizeIsCapacityMinusOne) {
    const int lengths[] = {7, 15, 31, 63};
    for (const int length : lengths) {
        const std::string base = make_text(length);
        MyString actual(base.c_str());
        actual.erase(0, length);
        expect_same(actual, "");
    }
}

TEST(MyStringEraseBoundary, EraseFromMiddleThenAppend) {
    MyString string("abcdefghij");
    string.erase(2, 3);
    string.append("XYZ");
    expect_same(string, "abfghijXYZ");
}

// ---------------------------------------------------------------------
// 비교: 한글(UTF-8) 등 128 이상의 바이트
// ---------------------------------------------------------------------

TEST(MyStringCompareNonAscii, MatchesStdString) {
    const char *words[] = {"", "a", "a가", "가", "나", "가나다", "한글", "한글입니다", "abc", "abd", "é", "e", "z", "ñ"};
    for (const char *left : words) {
        for (const char *right : words) {
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

            EXPECT_EQ(sa < sb, a < right) << trace;
            EXPECT_EQ(sa <= sb, a <= right) << trace;
            EXPECT_EQ(sa > sb, a > right) << trace;
            EXPECT_EQ(sa >= sb, a >= right) << trace;
        }
    }
}

TEST(MyStringCompareNonAscii, PrefixIsSmaller) {
    EXPECT_TRUE(MyString("a") < MyString("a가"));
    EXPECT_TRUE(MyString("한글") < MyString("한글입니다"));
    EXPECT_FALSE(MyString("한글입니다") < MyString("한글"));
}

// ---------------------------------------------------------------------
// moved-from(이동 당한) 객체: 소멸·재대입·append·clear 가 안전해야 한다
// ---------------------------------------------------------------------

TEST(MyStringMovedFrom, MoveConstructorTransfersValueAndEmptiesSource) {
    MyString source("hello world, this is long enough to grow");
    MyString target(std::move(source));
    expect_same(target, "hello world, this is long enough to grow");
    EXPECT_EQ(0, source.length());
    EXPECT_TRUE(source.isEmpty());
}

TEST(MyStringMovedFrom, MoveAssignmentTransfersValueAndEmptiesSource) {
    MyString source("hello");
    MyString target("something else");
    target = std::move(source);
    expect_same(target, "hello");
    EXPECT_EQ(0, source.length());
    EXPECT_TRUE(source.isEmpty());
}

TEST(MyStringMovedFrom, CopyAssignToMovedFrom) {
    MyString source("abc");
    MyString moved(std::move(source));
    source = moved;
    expect_same(source, "abc");
}

TEST(MyStringMovedFrom, AssignCStringToMovedFrom) {
    MyString source("abc");
    MyString moved(std::move(source));
    source = "reassigned";
    expect_same(source, "reassigned");
}

TEST(MyStringMovedFrom, AppendAndModifyMovedFrom) {
    {
        MyString source("abc");
        MyString moved(std::move(source));
        source.append('x');
        source += "yz";
        source.append(MyString("!"));
        expect_same(source, "xyz!");
    }
    {
        MyString source("abc");
        MyString moved(std::move(source));
        source.insert(0, "ins");
        expect_same(source, "ins");
    }
    {
        MyString source("abc");
        MyString moved(std::move(source));
        source.replace(0, 1, "rep");
        expect_same(source, "rep");
    }
    {
        MyString source("abc");
        MyString moved(std::move(source));
        source.clear();
        EXPECT_EQ(0, source.length());
        EXPECT_TRUE(source.isEmpty());
    }
}

TEST(MyStringMovedFrom, MovedTwice) {
    MyString first("abc");
    MyString second(std::move(first));
    MyString third(std::move(first));
    EXPECT_EQ(0, first.length());
    EXPECT_EQ(0, third.length());
    expect_same(second, "abc");
}

TEST(MyStringMovedFrom, SelfMoveAssignmentKeepsValue) {
    MyString string("abc");
    MyString &alias = string;
    string = std::move(alias);
    expect_same(string, "abc");
}

// ---------------------------------------------------------------------
// nullptr 방어 코드가 있는 메서드 검증
// ---------------------------------------------------------------------

TEST(MyStringNullptr, ModifyingWithNullptrChangesNothing) {
    const char *null = nullptr;
    MyString string("abc");
    string.append(null);
    string += null;
    string.insert(1, null);
    string.replace(0, 2, null);
    expect_same(string, "abc");
}

TEST(MyStringNullptr, FindAndCompareWithNullptr) {
    const char *null = nullptr;
    const MyString string("abc");
    EXPECT_EQ(-1, string.find(null));
    EXPECT_FALSE(string == null);
    EXPECT_TRUE(string != null);
    EXPECT_TRUE(string > null);
    EXPECT_TRUE(string >= null);
    EXPECT_FALSE(string < null);
    EXPECT_FALSE(string <= null);
}

// ---------------------------------------------------------------------
// 범위 밖 읽기 / 범위 내 쓰기
// ---------------------------------------------------------------------

TEST(MyStringRangeAccess, ConstOutOfRangeReadReturnsNullChar) {
    const MyString string("abc");
    EXPECT_EQ('\0', string[3]);
    EXPECT_EQ('\0', string[100]);
    EXPECT_EQ('\0', string[-1]);
    EXPECT_EQ('\0', string.at(1000));
    EXPECT_EQ('\0', string.at(-1000));
}

TEST(MyStringRangeAccess, InRangeWriteWorks) {
    MyString string("abc");
    string[1] = 'X';
    string.at(2) = 'Y';
    expect_same(string, "aXY");
}

// ---------------------------------------------------------------------
// 자기 자신을 인자로 넘기는 경우
// ---------------------------------------------------------------------

TEST(MyStringSelfReference, InsertSelf) {
    for (int length = 0; length <= 20; ++length) {
        for (int pos = 0; pos <= length; ++pos) {
            const std::string base = make_text(length);
            MyString actual(base.c_str());
            std::string expected = base;
            expected.insert(static_cast<std::size_t>(pos), std::string(base));
            actual.insert(pos, actual);
            expect_same(actual, expected);
        }
    }
}

TEST(MyStringSelfReference, ReplaceSelf) {
    for (int length = 1; length <= 20; ++length) {
        for (int pos = 0; pos <= length; ++pos) {
            for (int count = 0; count <= length; count += 3) {
                const std::string base = make_text(length);
                MyString actual(base.c_str());
                std::string expected = base;
                expected.replace(static_cast<std::size_t>(pos), static_cast<std::size_t>(count), std::string(base));
                actual.replace(pos, count, actual);
                expect_same(actual, expected);
            }
        }
    }
}

TEST(MyStringSelfReference, AppendAndPlusEqualsSelf) {
    for (int length = 0; length <= 40; ++length) {
        const std::string base = make_text(length);
        {
            MyString actual(base.c_str());
            actual.append(actual);
            expect_same(actual, base + base);
        }
        {
            MyString actual(base.c_str());
            actual += actual;
            expect_same(actual, base + base);
        }
    }
}

TEST(MyStringSelfReference, SelfCopyAssignment) {
    MyString string("abc");
    MyString &alias = string;
    string = alias;
    expect_same(string, "abc");
}

TEST(MyStringSelfReference, PlusDoesNotModifyOperands) {
    const MyString left("abc");
    const MyString right("def");
    const MyString result = left + right;
    expect_same(result, "abcdef");
    expect_same(left, "abc");
    expect_same(right, "def");
}

// ---------------------------------------------------------------------
// stoi / changeAlphabetToInt
// ---------------------------------------------------------------------

TEST(MyStringStoi, DecimalDigits) {
    EXPECT_EQ(123, stoi(MyString("123")));
    EXPECT_EQ(0, stoi(MyString("0")));
    EXPECT_EQ(12, stoi(MyString("0012")));
    EXPECT_EQ(19980123, stoi(MyString("19980123")));
    EXPECT_EQ(2147483647, stoi(MyString("2147483647")));
}

TEST(MyStringStoi, StopsAtFirstInvalidCharacter) {
    EXPECT_EQ(12, stoi(MyString("12abc")));
    EXPECT_EQ(12, stoi(MyString("12 34")));
    EXPECT_EQ(0, stoi(MyString("abc")));
    EXPECT_EQ(0, stoi(MyString("x12")));
    EXPECT_EQ(0, stoi(MyString("")));
}

TEST(MyStringStoi, StartPosition) {
    EXPECT_EQ(123, stoi(MyString("ab123"), 2));
    EXPECT_EQ(23, stoi(MyString("123"), 1));
    EXPECT_EQ(0, stoi(MyString("123"), 3));
    EXPECT_EQ(0, stoi(MyString("123"), 10));
    EXPECT_EQ(0, stoi(MyString("123"), 1000000));
    EXPECT_EQ(123, stoi(MyString("123"), -5));
}

TEST(MyStringStoi, OtherBases) {
    EXPECT_EQ(255, stoi(MyString("ff"), 0, 16));
    EXPECT_EQ(255, stoi(MyString("FF"), 0, 16));
    EXPECT_EQ(26, stoi(MyString("1a"), 0, 16));
    EXPECT_EQ(11, stoi(MyString("1011"), 0, 2));
    EXPECT_EQ(10, stoi(MyString("12"), 0, 8));
    EXPECT_EQ(35, stoi(MyString("z"), 0, 36));
    EXPECT_EQ(35, stoi(MyString("Z"), 0, 36));
}

TEST(MyStringStoi, DigitNotBelowBaseStopsParsing) {
    EXPECT_EQ(10, stoi(MyString("129"), 0, 8));
    EXPECT_EQ(1, stoi(MyString("12"), 0, 2));
}

TEST(MyStringStoi, BaseIsClamped) {
    EXPECT_EQ(5, stoi(MyString("101"), 0, 1));
    EXPECT_EQ(5, stoi(MyString("101"), 0, -7));
    EXPECT_EQ(35, stoi(MyString("z"), 0, 100));
}

TEST(MyStringStoi, ChangeAlphabetToInt) {
    EXPECT_EQ(0, changeAlphabetToInt('0'));
    EXPECT_EQ(9, changeAlphabetToInt('9'));
    EXPECT_EQ(10, changeAlphabetToInt('a'));
    EXPECT_EQ(10, changeAlphabetToInt('A'));
    EXPECT_EQ(35, changeAlphabetToInt('z'));
    EXPECT_EQ(35, changeAlphabetToInt('Z'));
    EXPECT_EQ(-1, changeAlphabetToInt('-'));
    EXPECT_EQ(-1, changeAlphabetToInt(' '));
    EXPECT_EQ(-1, changeAlphabetToInt('/'));
    EXPECT_EQ(-1, changeAlphabetToInt(':'));
    EXPECT_EQ(-1, changeAlphabetToInt('@'));
    EXPECT_EQ(-1, changeAlphabetToInt('['));
}

// ---------------------------------------------------------------------
// std::string 과 무작위 연산 비교 (차분 테스트)
// ---------------------------------------------------------------------

TEST(MyStringDifferential, RandomOperationsMatchStdString) {
    std::mt19937 generator(20261119);
    auto random_text = [&generator](const int maxLength) {
        std::string text;
        const int length = static_cast<int>(generator() % (maxLength + 1));
        for (int index = 0; index < length; ++index) {
            text += static_cast<char>('a' + generator() % 3);
        }
        return text;
    };

    for (int round = 0; round < 20000; ++round) {
        const std::string original = random_text(24);
        const int op = static_cast<int>(generator() % 8);
        const int pos = static_cast<int>(generator() % 40) - 6;
        const int len = static_cast<int>(generator() % 40) - 4;
        const std::string sub = random_text(14);

        std::ostringstream trace;
        trace << "round: " << round << ", op: " << op << ", text: \"" << original << "\", sub: \"" << sub
              << "\", pos: " << pos << ", len: " << len;
        SCOPED_TRACE(trace.str());

        MyString actual(original.c_str());
        std::string expected = original;

        switch (op) {
            case 0:
                expected.insert(static_cast<std::size_t>(clamp_pos(original, pos)), sub);
                actual.insert(pos, sub.c_str());
                break;
            case 1:
                expected.insert(static_cast<std::size_t>(clamp_pos(original, pos)), sub);
                actual.insert(pos, MyString(sub.c_str()));
                break;
            case 2:
                expected.replace(static_cast<std::size_t>(clamp_pos(original, pos)), to_len(len), sub);
                actual.replace(pos, len, sub.c_str());
                break;
            case 3:
                expected.replace(static_cast<std::size_t>(clamp_pos(original, pos)), to_len(len), sub);
                actual.replace(pos, len, MyString(sub.c_str()));
                break;
            case 4:
                expected.erase(static_cast<std::size_t>(clamp_pos(original, pos)), to_len(len));
                actual.erase(pos, len);
                break;
            case 5:
                expected += sub;
                actual += sub.c_str();
                break;
            case 6:
                expected += original;
                actual += actual;
                break;
            default: {
                const std::size_t found = original.find(sub, static_cast<std::size_t>(clamp_pos(original, pos)));
                const int expectedIndex = found == std::string::npos ? -1 : static_cast<int>(found);
                EXPECT_EQ(expectedIndex, actual.find(sub.c_str(), pos));
                EXPECT_EQ(expectedIndex, actual.find(MyString(sub.c_str()), pos));
                break;
            }
        }
        expect_same(actual, expected);
        if (HasFailure()) {
            return;
        }
    }
}

TEST(MyStringDifferential, RepeatedAppendGrowsCapacityCorrectly) {
    MyString actual;
    std::string expected;
    for (int index = 0; index < 500; ++index) {
        const char ch = static_cast<char>('a' + index % 26);
        actual.append(ch);
        expected += ch;
        if (index % 7 == 0) {
            actual += "xy";
            expected += "xy";
        }
    }
    expect_same(actual, expected);
}