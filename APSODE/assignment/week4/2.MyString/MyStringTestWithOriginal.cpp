#include <gtest/gtest.h>
#include <iomanip>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "MyString.h"

// =====================================================================
// 검증 방식
//
//  "우리가 생각한 값"을 직접 적지 않고, 같은 동작을 std::string 에 똑같이 수행시켜
//  그 결과와 MyString 의 결과가 같은지 비교한다.
//
//  std::string 에 없는 규칙은 아래 도우미로만 반영한다.
//   - limit()      : MyString 은 최대 15글자까지만 저장한다.
//   - read_word()  : operator>> 는 최대 15글자만 읽고 나머지는 스트림에 남긴다.
//                    (std::setw(15) 와 동일한 동작)
//   - read_line()  : getline 은 delim 까지 전부 읽고 16번째 글자부터는 버린다.
//   - expected_find / std_substr : 음수 위치는 0 으로 취급한다. (과제 조건)
//   - std_set      : 범위를 벗어난 위치는 std::string::at 의 예외 대신 false 로 취급한다.
// =====================================================================

namespace {
    const std::size_t MAX_LEN = CharSequence::MAX_LENGTH;

    std::string to_std_string(const CharSequence &sequence) {
        std::ostringstream stream;
        stream << sequence;
        return stream.str();
    }

    // MyString 의 최대 길이(15글자) 제한을 std::string 에 반영
    std::string limit(std::string text) {
        if (text.size() > MAX_LEN) {
            text.resize(MAX_LEN);
        }
        return text;
    }

    // MyString 과 std::string 의 내용/길이/비어있음 여부가 모두 같은지 확인
    void expect_same(const CharSequence &actual, const std::string &expected) {
        EXPECT_EQ(expected, to_std_string(actual));
        EXPECT_EQ(static_cast<int>(expected.size()), actual.length());
        EXPECT_EQ(expected.empty(), actual.empty());
    }

    // std::string::at 로 set 을 흉내 낸다. (범위를 벗어나면 예외 -> false)
    bool std_set(std::string &text, const char chr, const int pos) {
        try {
            text.at(static_cast<std::size_t>(pos)) = chr;
            return true;
        } catch (const std::out_of_range &) {
            return false;
        }
    }

    // std::string::find 결과를 int 로 바꾼 기대값 (npos -> -1, 음수 start -> 0)
    int expected_find(const std::string &text, const std::string &pattern, int start) {
        if (start < 0) {
            start = 0;
        }
        const std::size_t found = text.find(pattern, static_cast<std::size_t>(start));
        return found == std::string::npos ? -1 : static_cast<int>(found);
    }

    // std::string::substr 기대값 (음수 pos -> 0, 범위 밖 pos -> 빈 문자열)
    std::string std_substr(const std::string &text, int pos, const int len) {
        if (pos < 0) {
            pos = 0;
        }
        if (pos > static_cast<int>(text.size())) {
            return "";
        }
        return text.substr(static_cast<std::size_t>(pos), static_cast<std::size_t>(len));
    }

    // MyString 과 std::string 으로 같은 위치에서 find 했을 때 결과가 같은지 확인 (세 오버로드 모두)
    void expect_find_same(const char *text, const char *pattern, const int start) {
        const int expected = expected_find(limit(text), pattern, start);
        const MyString string(text);

        EXPECT_EQ(expected, string.find(MyString(pattern), start))
            << "[MyString] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
        EXPECT_EQ(expected, string.find(CharSequence(pattern), start))
            << "[CharSequence] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
        EXPECT_EQ(expected, string.find(pattern, start))
            << "[char array] text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
    }

    // ==, != 결과가 std::string 과 같은지 확인
    void expect_compare_same(const char *left, const char *right) {
        const MyString a(left);
        const MyString b(right);
        const std::string sa = limit(left);
        const std::string sb = limit(right);

        EXPECT_EQ(sa == sb, a == b) << "left: \"" << left << "\", right: \"" << right << "\"";
        EXPECT_EQ(sa != sb, a != b) << "left: \"" << left << "\", right: \"" << right << "\"";
    }

    // ---------- 스트림 도우미 ----------

    // operator>> 와 같은 규칙으로 std::string 에 읽기
    std::string read_word(std::istream &stream) {
        std::string word;
        stream >> std::setw(static_cast<int>(MAX_LEN)) >> word;
        return word;
    }

    // getline 과 같은 규칙으로 std::string 에 읽기
    std::string read_line(std::istream &stream, const char delim = '\n') {
        std::string line;
        std::getline(stream, line, delim);
        return limit(line);
    }

    // 스트림이 어디까지 읽었는지 (eof 상태여도 위치를 얻기 위해 잠시 상태를 지움)
    std::streamoff position_of(std::istream &stream) {
        if (stream.fail()) {
            return -1;
        }
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

            // operator>> : 읽은 값과 스트림 상태/위치가 std 쪽과 같은지 확인
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

            // getline : 읽은 값과 스트림 상태/위치가 std 쪽과 같은지 확인
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

            void expect_same_stream() {
                EXPECT_EQ(ref.fail(), mine.fail());
                EXPECT_EQ(position_of(ref), position_of(mine));
            }

            std::istringstream mine;
            std::istringstream ref;
    };

    std::vector<std::string> collect_words_of_mystring(const std::string &text) {
        std::istringstream stream(text);
        MyString word;
        std::vector<std::string> words;
        while (stream >> word) {
            words.push_back(to_std_string(word));
        }
        return words;
    }

    std::vector<std::string> collect_words_of_std(const std::string &text) {
        std::istringstream stream(text);
        std::vector<std::string> words;
        std::string word;
        while (stream >> std::setw(static_cast<int>(MAX_LEN)) >> word) {
            words.push_back(word);
        }
        return words;
    }

    std::vector<std::string> collect_lines_of_mystring(const std::string &text) {
        std::istringstream stream(text);
        MyString line;
        std::vector<std::string> lines;
        while (::getline(stream, line)) {
            lines.push_back(to_std_string(line));
        }
        return lines;
    }

    std::vector<std::string> collect_lines_of_std(const std::string &text) {
        std::istringstream stream(text);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(limit(line));
        }
        return lines;
    }
}

// =====================================================================
// 생성자
// =====================================================================

TEST(MyStringConstructor, DefaultCreatesEmptyString) {
    const MyString string;
    const std::string expected;

    expect_same(string, expected);
}

TEST(MyStringConstructor, FromCharArray) {
    const char *text = "hello";

    expect_same(MyString(text), limit(text));
}

TEST(MyStringConstructor, FromLongCharArrayIsTruncated) {
    const char *text = "1234567890123456789";

    expect_same(MyString(text), limit(text));
}

TEST(MyStringConstructor, FromCharSequence) {
    const CharSequence sequence("sequence");
    const MyString string(sequence);

    expect_same(string, to_std_string(sequence));
    EXPECT_TRUE(string.equals(sequence));
}

TEST(MyStringConstructor, CopyCopiesContent) {
    const MyString original("copy");
    const MyString copied(original);
    const std::string ref_original = "copy";
    const std::string ref_copied(ref_original);

    EXPECT_EQ(ref_original == ref_copied, original == copied);
    expect_same(copied, ref_copied);
}

TEST(MyStringConstructor, CopyIsIndependentFromOriginal) {
    MyString original("abc");
    MyString copied(original);
    std::string ref_original = "abc";
    std::string ref_copied(ref_original);

    copied.set('z', 0);
    ref_copied[0] = 'z';

    expect_same(original, ref_original);
    expect_same(copied, ref_copied);
}

// =====================================================================
// 상속받은 기능 (CharSequence)
// =====================================================================

TEST(MyStringInherited, AtMatchesStdString) {
    const MyString string("abc");
    const std::string ref = "abc";

    for (std::size_t index = 0; index < ref.size(); ++index) {
        EXPECT_EQ(ref.at(index), string.at(static_cast<int>(index))) << "index: " << index;
    }
}

TEST(MyStringInherited, SetMatchesStdString) {
    const int positions[] = {-1, 0, 1, 2, 3, 10};

    for (const int pos: positions) {
        MyString string("abc");
        std::string ref = "abc";

        const bool expected = std_set(ref, 'x', pos);

        EXPECT_EQ(expected, string.set('x', pos)) << "pos: " << pos;
        expect_same(string, ref);
    }
}

TEST(MyStringInherited, SetOnEmptyStringMatchesStdString) {
    MyString string;
    std::string ref;

    EXPECT_EQ(std_set(ref, 'x', 0), string.set('x', 0));
    expect_same(string, ref);
}

TEST(MyStringInherited, CanBeUsedThroughCharSequenceReference) {
    const MyString string("base");
    const CharSequence &reference = string;
    const std::string ref = "base";

    EXPECT_EQ(static_cast<int>(ref.size()), reference.length());
    EXPECT_EQ(ref.at(0), reference.at(0));
    EXPECT_EQ(ref.empty(), reference.empty());
}

TEST(MyStringInherited, PlusResultCanBeStoredAsCharSequence) {
    const MyString string("1234567890");
    const std::string ref = limit(std::string("1234567890") + "abcdefghij");

    const CharSequence result = string + "abcdefghij";

    EXPECT_EQ(ref, to_std_string(result));
}

// =====================================================================
// operator==
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

TEST(MyStringEquality, Self) {
    const MyString a("self");
    const std::string ref = "self";

    EXPECT_EQ(ref == ref, a == a);
}

TEST(MyStringEquality, TwoEmptyStrings) {
    const MyString a;
    const MyString b;
    const std::string ra;
    const std::string rb;

    EXPECT_EQ(ra == rb, a == b);
}

// =====================================================================
// operator=
// =====================================================================

TEST(MyStringAssignment, CopiesContent) {
    MyString a("source");
    MyString b("target");
    std::string ra = "source";
    std::string rb = "target";

    b = a;
    rb = ra;

    expect_same(b, rb);
    expect_same(a, ra);
}

TEST(MyStringAssignment, FromEmptyMakesTargetEmpty) {
    MyString empty;
    MyString target("something");
    std::string ref_empty;
    std::string ref_target = "something";

    target = empty;
    ref_target = ref_empty;

    expect_same(target, ref_target);
}

TEST(MyStringAssignment, ToSelfKeepsContent) {
    MyString a("self");
    MyString &reference = a;
    std::string ra = "self";
    std::string &ref_reference = ra;

    a = reference;
    ra = ref_reference;

    expect_same(a, ra);
}

TEST(MyStringAssignment, ReturnsReferenceForChaining) {
    MyString a("chain");
    MyString b;
    MyString c;
    std::string ra = "chain";
    std::string rb;
    std::string rc;

    c = b = a;
    rc = rb = ra;

    expect_same(a, ra);
    expect_same(b, rb);
    expect_same(c, rc);
}

TEST(MyStringAssignment, IsIndependentFromSource) {
    MyString a("abc");
    MyString b;
    std::string ra = "abc";
    std::string rb;

    b = a;
    b.set('z', 0);
    rb = ra;
    rb[0] = 'z';

    expect_same(a, ra);
    expect_same(b, rb);
}

// =====================================================================
// operator+
// =====================================================================

TEST(MyStringPlus, WithMyString) {
    const MyString a("abc");
    const MyString b("def");

    expect_same(a + b, limit(std::string("abc") + std::string("def")));
}

TEST(MyStringPlus, WithCharSequence) {
    const MyString a("abc");
    const CharSequence b("def");

    expect_same(a + b, limit(std::string("abc") + to_std_string(b)));
}

TEST(MyStringPlus, WithCharArray) {
    const MyString a("abc");

    expect_same(a + "def", limit(std::string("abc") + "def"));
}

TEST(MyStringPlus, DoesNotModifyOperands) {
    const MyString a("abc");
    const MyString b("def");
    const std::string ra = "abc";
    const std::string rb = "def";

    const MyString result = a + b;
    const std::string ref_result = ra + rb;
    (void) result;
    (void) ref_result;

    expect_same(a, ra);
    expect_same(b, rb);
}

TEST(MyStringPlus, WithEmpty) {
    const MyString a("abc");
    const MyString empty;
    const std::string ra = "abc";
    const std::string re;

    expect_same(a + empty, ra + re);
    expect_same(empty + a, re + ra);
}

TEST(MyStringPlus, TruncatesAtMaxLength) {
    const MyString a("1234567890");
    const MyString b("abcdefghij");

    expect_same(a + b, limit(std::string("1234567890") + "abcdefghij"));
}

TEST(MyStringPlus, CharArrayTruncatesAtMaxLength) {
    const MyString a("1234567890");

    expect_same(a + "abcdefghij", limit(std::string("1234567890") + "abcdefghij"));
}

TEST(MyStringPlus, CanBeChained) {
    const MyString a("a");
    const MyString b("b");

    expect_same(a + b + "c", limit(std::string("a") + "b" + "c"));
}

TEST(MyStringPlus, ExactlyMaxLengthIsNotTruncated) {
    const MyString a("12345678");
    const MyString b("1234567");

    expect_same(a + b, limit(std::string("12345678") + "1234567"));
}

TEST(MyStringPlus, OneOverMaxLengthIsTruncated) {
    const MyString a("12345678");
    const MyString b("12345678");

    expect_same(a + b, limit(std::string("12345678") + "12345678"));
}

TEST(MyStringPlus, SelfConcatenation) {
    const MyString a("ab");
    const std::string ra = "ab";

    expect_same(a + a, ra + ra);
    expect_same(a, ra);
}

TEST(MyStringPlus, EmptyCharArray) {
    const MyString a("abc");

    expect_same(a + "", std::string("abc") + "");
}

TEST(MyStringPlus, ResultIsIndependentFromOperands) {
    const MyString a("abc");
    const MyString b("def");
    const std::string ra = "abc";
    const std::string rb = "def";

    MyString result = a + b;
    std::string ref_result = ra + rb;
    result.set('z', 0);
    ref_result[0] = 'z';

    expect_same(a, ra);
    expect_same(result, ref_result);
}

TEST(MyStringPlus, AssignedResultMatchesDirectResult) {
    const MyString a("abc");
    const MyString b("def");
    MyString assigned;
    std::string ref_assigned;

    assigned = a + b;
    ref_assigned = std::string("abc") + "def";

    expect_same(assigned, ref_assigned);
    EXPECT_TRUE(assigned == (a + b));
}

TEST(MyStringPlus, MatchesStdStringForManyCombinations) {
    const char *samples[] = {"", "a", "abc", "hello", "12345678", "123456789012345"};

    for (const char *left: samples) {
        for (const char *right: samples) {
            const MyString a(left);
            const MyString b(right);
            const std::string expected = limit(std::string(left) + right);

            expect_same(a + b, expected);
            expect_same(a + CharSequence(right), expected);
            expect_same(a + right, expected);
        }
    }
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

TEST(MyStringInputOperator, TruncatesLongInput) {
    StreamPair in("1234567890123456789");

    in.word();
}

TEST(MyStringInputOperator, OverwritesExistingContent) {
    StreamPair in("new");
    MyString string("previous");

    in.word(string);
}

TEST(MyStringInputOperator, ReturnsStreamForChaining) {
    std::istringstream input("a b");
    std::istringstream ref("a b");
    MyString first;
    MyString second;

    std::istream &returned = (input >> first);
    returned >> second;
    const std::string ref_first = read_word(ref);
    const std::string ref_second = read_word(ref);

    EXPECT_EQ(&input, &returned);
    expect_same(first, ref_first);
    expect_same(second, ref_second);
}

TEST(MyStringInputOperator, SkipsLeadingWhitespace) {
    StreamPair in("   \n\t hello");

    in.word();
}

TEST(MyStringInputOperator, ReadsExactlyMaxLengthFully) {
    StreamPair in("123456789012345");

    in.word();
}

// 넘친 글자는 스트림에 남아 다음 읽기에서 이어서 읽힌다. (std::setw(15) 와 동일)
TEST(MyStringInputOperator, CharactersBeyondLimitStayInStream) {
    StreamPair in("1234567890123456789");

    in.word();
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

TEST(MyStringInputOperator, FailsWhenNothingLeftToRead) {
    StreamPair in("   ");

    in.word();
    EXPECT_TRUE(in.ref.fail());
    EXPECT_TRUE(in.mine.fail());
}

TEST(MyStringInputOperator, MatchesStdStringForVariousInputs) {
    const char *inputs[] = {
        "", " ", "a", "a b", "  lead", "trail  ", "tab\tseparated", "line\nbreak",
        "123456789012345", "1234567890123456", "12345678901234567890 next", "x 123456789012345678 y"
    };

    for (const char *input: inputs) {
        SCOPED_TRACE(std::string("input: \"") + input + "\"");
        EXPECT_EQ(collect_words_of_std(input), collect_words_of_mystring(input));
    }
}

// =====================================================================
// 길이 경계 (최대 15글자)
// =====================================================================

TEST(MyStringBoundary, ExactlyMaxLengthIsKept) {
    const char *text = "123456789012345";

    expect_same(MyString(text), limit(text));
    EXPECT_EQ(static_cast<int>(MAX_LEN), MyString(text).length());
}

TEST(MyStringBoundary, OneOverMaxLengthIsTruncated) {
    const char *text = "1234567890123456";

    expect_same(MyString(text), limit(text));
}

TEST(MyStringBoundary, EmptyCharArrayCreatesEmptyString) {
    expect_same(MyString(""), std::string(""));
}

TEST(MyStringBoundary, SingleCharacter) {
    expect_same(MyString("a"), std::string("a"));
}

TEST(MyStringBoundary, LengthIsContentLengthNotCapacity) {
    // 과제 조건: 전체 용량이 아니라 문자열 자체의 길이를 반환
    const std::string ref = "abc";

    EXPECT_EQ(static_cast<int>(ref.size()), MyString("abc").length());
}

TEST(MyStringBoundary, EveryLengthFromZeroToTwentyMatches) {
    std::string text;

    for (int length = 0; length <= 20; ++length) {
        SCOPED_TRACE("length: " + std::to_string(length));
        expect_same(MyString(text.c_str()), limit(text));
        text += static_cast<char>('a' + length);
    }
}

// =====================================================================
// operator!=
// =====================================================================

TEST(MyStringInequality, SameContent) {
    expect_compare_same("hello", "hello");
}

TEST(MyStringInequality, DifferentContent) {
    expect_compare_same("hello", "world");
    expect_compare_same("world", "hello");
}

TEST(MyStringInequality, DifferentLength) {
    expect_compare_same("hello", "hello!");
}

TEST(MyStringInequality, EmptyAndNonEmpty) {
    expect_compare_same("", "a");
    expect_compare_same("a", "");
}

TEST(MyStringInequality, TwoEmptyStrings) {
    expect_compare_same("", "");
}

TEST(MyStringInequality, DifferenceAtLastCharacter) {
    expect_compare_same("abcde", "abcdf");
}

TEST(MyStringInequality, MatchesStdStringForManyCombinations) {
    const char *samples[] = {"", "a", "ab", "abc", "abd", "hello", "hellp", "123456789012345", "123456789012346"};

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
        ref << limit(text);

        EXPECT_EQ(ref.str(), mine.str()) << "text: \"" << text << "\"";
    }
}

TEST(MyStringOutputOperator, DoesNotAppendNewline) {
    std::ostringstream mine;
    std::ostringstream ref;

    mine << MyString("abc");
    ref << std::string("abc");

    EXPECT_EQ(ref.str().find('\n'), mine.str().find('\n'));
}

TEST(MyStringOutputOperator, CanBeChainedWithOtherOutput) {
    const MyString a("abc");
    const MyString b("def");
    const std::string ra = "abc";
    const std::string rb = "def";
    std::ostringstream mine;
    std::ostringstream ref;

    mine << a << "," << b << "!";
    ref << ra << "," << rb << "!";

    EXPECT_EQ(ref.str(), mine.str());
}

TEST(MyStringOutputOperator, PrintsConcatenatedResult) {
    const MyString a("abc");
    std::ostringstream mine;
    std::ostringstream ref;

    mine << (a + "def");
    ref << (std::string("abc") + "def");

    EXPECT_EQ(ref.str(), mine.str());
}

TEST(MyStringOutputOperator, PrintsTruncatedContent) {
    std::ostringstream mine;
    std::ostringstream ref;

    mine << MyString("1234567890123456789");
    ref << limit("1234567890123456789");

    EXPECT_EQ(ref.str(), mine.str());
}

TEST(MyStringOutputOperator, ReturnsStreamForChaining) {
    std::ostringstream stream;

    std::ostream &returned = (stream << MyString("a"));

    EXPECT_EQ(&stream, &returned);
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

TEST(MyStringGetLine, ExactlyMaxLengthIsKept) {
    StreamPair in("123456789012345\nnext\n");

    in.line();
    in.line();
}

// 16번째 글자부터는 버리고 delim 까지 소비한다.
TEST(MyStringGetLine, DiscardsCharactersBeyondLimit) {
    StreamPair in("1234567890123456789\nnext\n");

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
    std::istringstream ref("a\nb\n");
    MyString first;
    MyString second;

    std::istream &returned = ::getline(input, first);
    ::getline(returned, second);
    const std::string ref_first = read_line(ref);
    const std::string ref_second = read_line(ref);

    EXPECT_EQ(&input, &returned);
    expect_same(first, ref_first);
    expect_same(second, ref_second);
}

TEST(MyStringGetLine, FailsWhenNothingLeftToRead) {
    StreamPair in("");

    in.line();
    EXPECT_TRUE(in.ref.fail());
    EXPECT_TRUE(in.mine.fail());
}

TEST(MyStringGetLine, MatchesStdStringForVariousInputs) {
    const char *inputs[] = {
        "", "\n", "a", "a\n", "a\nb", "a\n\nb\n", "  spaced  \n", "123456789012345\n",
        "1234567890123456\nx\n", "12345678901234567890123\nshort\n\n", "no newline at end"
    };

    for (const char *input: inputs) {
        SCOPED_TRACE(std::string("input: \"") + input + "\"");
        EXPECT_EQ(collect_lines_of_std(input), collect_lines_of_mystring(input));
    }
}

// =====================================================================
// substr
// =====================================================================

TEST(MyStringSubstr, FromStart) {
    const MyString string("hello world");

    expect_same(string.substr(0, 5), std_substr("hello world", 0, 5));
}

TEST(MyStringSubstr, FromMiddle) {
    const MyString string("hello world");

    expect_same(string.substr(6, 5), std_substr("hello world", 6, 5));
}

TEST(MyStringSubstr, SingleCharacter) {
    const MyString string("abc");

    expect_same(string.substr(1, 1), std_substr("abc", 1, 1));
}

TEST(MyStringSubstr, WholeString) {
    const MyString string("abcde");

    expect_same(string.substr(0, 5), std_substr("abcde", 0, 5));
}

TEST(MyStringSubstr, LastCharacter) {
    const MyString string("abcde");

    expect_same(string.substr(4, 1), std_substr("abcde", 4, 1));
}

TEST(MyStringSubstr, ZeroLength) {
    const MyString string("hello");

    expect_same(string.substr(2, 0), std_substr("hello", 2, 0));
}

// len 이 남은 길이보다 길면 끝까지만 반환 (std::string::substr 과 동일)
TEST(MyStringSubstr, LengthBeyondEndStopsAtEnd) {
    const MyString string("hello world");

    expect_same(string.substr(6, 100), std_substr("hello world", 6, 100));
}

TEST(MyStringSubstr, LengthOneBeyondEndStopsAtEnd) {
    const MyString string("abc");

    expect_same(string.substr(1, 3), std_substr("abc", 1, 3));
}

// 음수 pos 는 0 으로 취급
TEST(MyStringSubstr, NegativePositionIsTreatedAsZero) {
    const MyString string("hello");

    expect_same(string.substr(-3, 2), std_substr("hello", -3, 2));
}

TEST(MyStringSubstr, VeryNegativePositionIsTreatedAsZero) {
    const MyString string("hello");

    expect_same(string.substr(-100, 5), std_substr("hello", -100, 5));
}

// 문자열 범위 밖이면 빈 문자열
TEST(MyStringSubstr, PositionBeyondEndGivesEmpty) {
    const MyString string("hello");

    expect_same(string.substr(10, 2), std_substr("hello", 10, 2));
}

TEST(MyStringSubstr, PositionAtEndGivesEmpty) {
    // std::string::substr(size(), n) 도 빈 문자열을 반환
    const MyString string("hello");

    expect_same(string.substr(5, 3), std::string("hello").substr(5, 3));
}

TEST(MyStringSubstr, EmptySourceGivesEmpty) {
    const MyString empty;

    expect_same(empty.substr(0, 3), std::string().substr(0, 3));
}

TEST(MyStringSubstr, DoesNotModifyOriginal) {
    const MyString string("hello");
    const std::string ref = "hello";

    const MyString result = string.substr(1, 3);
    const std::string ref_result = ref.substr(1, 3);
    (void) result;
    (void) ref_result;

    expect_same(string, ref);
}

TEST(MyStringSubstr, ResultIsIndependentFromOriginal) {
    const MyString string("hello");
    const std::string ref = "hello";

    MyString result = string.substr(0, 3);
    std::string ref_result = ref.substr(0, 3);
    result.set('z', 0);
    ref_result[0] = 'z';

    expect_same(string, ref);
    expect_same(result, ref_result);
}

TEST(MyStringSubstr, FromMaxLengthString) {
    const MyString string("123456789012345");

    expect_same(string.substr(10, 5), std_substr("123456789012345", 10, 5));
    expect_same(string.substr(14, 1), std_substr("123456789012345", 14, 1));
}

TEST(MyStringSubstr, MatchesStdStringForManyRanges) {
    const char *texts[] = {"", "a", "hello world", "123456789012345"};

    for (const char *text: texts) {
        const MyString string(text);
        const std::string ref = text;

        for (int pos = -3; pos <= static_cast<int>(ref.size()) + 3; ++pos) {
            for (int len = 0; len <= 20; ++len) {
                SCOPED_TRACE("text: \"" + ref + "\", pos: " + std::to_string(pos) + ", len: " + std::to_string(len));
                expect_same(string.substr(pos, len), std_substr(ref, pos, len));
            }
        }
    }
}

TEST(MyStringSubstr, CanBeConcatenated) {
    const MyString string("hello world");
    const std::string ref = "hello world";

    expect_same(string.substr(0, 5) + string.substr(5, 6), ref.substr(0, 5) + ref.substr(5, 6));
}

// =====================================================================
// find (std::string::find 와 비교)
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

// 음수 start 는 0 으로 취급 (과제 조건)
TEST(MyStringFind, NegativeStart) {
    expect_find_same("hello", "he", -1);
    expect_find_same("hello", "he", -100);
    expect_find_same("hello", "lo", -3);
}

TEST(MyStringFind, StartBeyondLength) {
    expect_find_same("hello", "h", 6);
    expect_find_same("hello", "h", 100);
}

TEST(MyStringFind, StartAtLength) {
    expect_find_same("hello", "o", 5);
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
    // 해시 값이 큰 문자들로 구성된 경우에도 std::string 과 같아야 한다.
    expect_find_same("~}|{zyxwvutsrqp", "~}|", 0);
    expect_find_same("~}|{zyxwvutsrqp", "rqp", 0);
    expect_find_same("~}|{zyxwvutsrqp", "}~", 0);
}

TEST(MyStringFind, MaxLengthString) {
    expect_find_same("123456789012345", "5", 0);
    expect_find_same("123456789012345", "5", 5);
    expect_find_same("123456789012345", "12345", 0);
    expect_find_same("123456789012345", "12345", 1);
}

TEST(MyStringFind, TruncatedString) {
    // 15글자로 잘린 뒤의 내용만 검색 대상이 된다. (기대값도 잘린 문자열 기준)
    expect_find_same("123456789012345678", "345", 0);
    expect_find_same("123456789012345678", "678", 0);
    expect_find_same("123456789012345678", "456789012345678", 0);
}

TEST(MyStringFind, PatternAtEveryPosition) {
    const std::string text = "abcdefghijklmno";

    for (std::size_t index = 0; index < text.size(); ++index) {
        const std::string pattern = text.substr(index, 1);
        expect_find_same(text.c_str(), pattern.c_str(), 0);
    }
}

TEST(MyStringFind, DoesNotModifyString) {
    const MyString string("hello");
    const std::string ref = "hello";

    string.find(MyString("ll"), 0);
    (void) ref.find("ll", 0);

    expect_same(string, ref);
}

TEST(MyStringFind, DoesNotModifyPattern) {
    const MyString string("hello");
    const MyString pattern("ll");
    const std::string ref_pattern = "ll";

    string.find(pattern, 0);

    expect_same(pattern, ref_pattern);
}

TEST(MyStringFind, StringCanBeUsedAsItsOwnPattern) {
    const MyString string("hello");
    const std::string ref = "hello";

    EXPECT_EQ(static_cast<int>(ref.find(ref, 0)), string.find(string, 0));
}

TEST(MyStringFind, OnConcatenatedResult) {
    const MyString a("hello ");
    const MyString b("world");
    const std::string joined = std::string("hello ") + "world";

    const MyString result = a + b;

    EXPECT_EQ(expected_find(joined, "world", 0), result.find(MyString("world"), 0));
    EXPECT_EQ(expected_find(joined, "o w", 0), result.find(MyString("o w"), 0));
}

TEST(MyStringFind, OnSubstrResult) {
    const MyString string("hello world");
    const std::string ref = std_substr("hello world", 6, 5);

    const MyString part = string.substr(6, 5);

    EXPECT_EQ(expected_find(ref, "rl", 0), part.find(MyString("rl"), 0));
    EXPECT_EQ(expected_find(ref, "hello", 0), part.find(MyString("hello"), 0));
}

TEST(MyStringFind, OnAssignedString) {
    const MyString source("find me");
    MyString target("other");
    std::string ref = "other";

    target = source;
    ref = "find me";

    EXPECT_EQ(expected_find(ref, "me", 0), target.find(MyString("me"), 0));
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

TEST(MyStringFind, MatchesStdStringForManyInputs) {
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
// 기반 클래스(CharSequence) 참조를 통한 find
// =====================================================================

TEST(MyStringInheritedFind, CharArrayOverloadThroughBaseReference) {
    const MyString string("hello world");
    const CharSequence &base = string;

    EXPECT_EQ(expected_find("hello world", "ll", 0), base.find("ll", 0));
    EXPECT_EQ(expected_find("hello world", "xyz", 0), base.find("xyz", 0));
}

TEST(MyStringInheritedFind, CharSequenceOverloadThroughBaseReference) {
    const MyString string("hello world");
    const CharSequence &base = string;

    EXPECT_EQ(expected_find("hello world", "world", 0), base.find(CharSequence("world"), 0));
    EXPECT_EQ(expected_find("hello world", "xyz", 0), base.find(CharSequence("xyz"), 0));
}

TEST(MyStringInheritedFind, StartRulesThroughBaseReference) {
    const MyString string("abcabc");
    const CharSequence &base = string;

    EXPECT_EQ(expected_find("abcabc", "abc", -5), base.find("abc", -5));
    EXPECT_EQ(expected_find("abcabc", "abc", 1), base.find("abc", 1));
    EXPECT_EQ(expected_find("abcabc", "abc", 10), base.find("abc", 10));
}

TEST(MyStringInheritedFind, BaseAndDerivedFindAgree) {
    const char *texts[] = {"", "a", "abcabc", "hello world", "123456789012345"};
    const char *patterns[] = {"a", "abc", "ll", "o w", "345", "zzz"};

    for (const char *text: texts) {
        const MyString string(text);
        const CharSequence &base = string;
        for (const char *pattern: patterns) {
            for (int start = -2; start <= 17; ++start) {
                EXPECT_EQ(base.find(CharSequence(pattern), start), string.find(MyString(pattern), start))
                    << "text: \"" << text << "\", pattern: \"" << pattern << "\", start: " << start;
            }
        }
    }
}

// =====================================================================
// operator>> 와 getline 을 하나의 스트림에서 함께 사용
//
//  같은 입력을 std::istringstream(MyString 용)과 또 하나의 std::istringstream(std::string 용)에
//  똑같은 순서로 읽혀서, 읽은 값과 스트림 상태/위치가 같은지 비교한다.
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

// ---------- 길이 제한이 걸린 경우의 조합 ----------

// >> 가 남긴 넘친 글자를 getline 이 이어서 읽는다.
TEST(MyStringStreamMixTest, OverflowLeftByExtractionIsReadByGetline) {
    StreamPair in("1234567890123456789\nnext\n");

    in.word();
    in.line();
    in.line();
}

TEST(MyStringStreamMixTest, OverflowLeftByExtractionIsReadByNextExtraction) {
    StreamPair in("1234567890123456789 word\n");

    in.word();
    in.word();
    in.word();
}

// getline 은 16번째 글자부터 버리고 delim 까지 소비한다.
TEST(MyStringStreamMixTest, GetlineDiscardsOverflowThenExtractionReadsNextLine) {
    StreamPair in("1234567890123456789\nword\n");

    in.line();
    in.word();
}

TEST(MyStringStreamMixTest, ExactlyMaxLengthWordThenGetline) {
    StreamPair in("123456789012345\nnext\n");

    in.word();
    in.line();
    in.line();
}

TEST(MyStringStreamMixTest, ExactlyMaxLengthLineThenExtraction) {
    StreamPair in("123456789012345\nword\n");

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
    std::istringstream ref("mine yours\nrest of line\n");
    MyString mine;
    MyString line;
    std::string yours;
    std::string ref_yours;

    input >> mine >> yours;
    input.ignore(1000, '\n');
    ::getline(input, line);
    const std::string ref_mine = read_word(ref);
    ref >> ref_yours;
    ref.ignore(1000, '\n');
    const std::string ref_line = read_line(ref);

    expect_same(mine, ref_mine);
    EXPECT_EQ(ref_yours, yours);
    expect_same(line, ref_line);
}

// ---------- 반환값 / 체이닝 ----------

TEST(MyStringStreamMixTest, GetlineCanTakeResultOfExtraction) {
    std::istringstream input("word tail\n");
    std::istringstream ref("word tail\n");
    MyString word;
    MyString tail;
    std::string ref_word;

    ::getline(input >> word, tail);
    ref >> std::setw(static_cast<int>(MAX_LEN)) >> ref_word;
    const std::string ref_tail = read_line(ref);

    expect_same(word, ref_word);
    expect_same(tail, ref_tail);
}

TEST(MyStringStreamMixTest, ExtractionCanTakeResultOfGetline) {
    std::istringstream input("line one\nword\n");
    std::istringstream ref("line one\nword\n");
    MyString line;
    MyString word;

    ::getline(input, line) >> word;
    const std::string ref_line = read_line(ref);
    const std::string ref_word = read_word(ref);

    expect_same(line, ref_line);
    expect_same(word, ref_word);
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

    EXPECT_EQ(collect_words_of_std(text), collect_words_of_mystring(text));
}

TEST(MyStringStreamMixTest, GetlineLoopReadsAllLines) {
    const std::string text = "alpha\nbeta gamma\n\ndelta\n";

    EXPECT_EQ(collect_lines_of_std(text), collect_lines_of_mystring(text));
}

TEST(MyStringStreamMixTest, FirstWordThenRestOfEachLine) {
    // 각 줄을 "명령어 + 나머지" 로 나누어 읽는 전형적인 사용 예
    StreamPair in("add apple pie\nremove old\n");

    for (int index = 0; index < 2; ++index) {
        in.word();
        in.line();
    }
}

// ---------- 읽은 결과의 사용 ----------

TEST(MyStringStreamMixTest, ReadValuesCanBeConcatenated) {
    StreamPair in("hello\nworld\n");
    MyString first;
    MyString second;

    const std::string ref_first = in.word(first);
    in.get();
    const std::string ref_second = in.line(second);

    expect_same(first + " " + second, limit(ref_first + " " + ref_second));
}

TEST(MyStringStreamMixTest, ReadValuesCanBeCompared) {
    StreamPair in("same\nsame\n");
    MyString from_extraction;
    MyString from_getline;

    const std::string ref_extraction = in.word(from_extraction);
    in.get();
    const std::string ref_getline = in.line(from_getline);

    EXPECT_EQ(ref_extraction == ref_getline, from_extraction == from_getline);
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

    expect_same(rest.substr(1, 5), std_substr(ref_rest, 1, 5));
}

TEST(MyStringStreamMixTest, LHMsTest) {
    StreamPair in("  123 456\n789\n");
    MyString str1, str2, str3, str4;

    in.word(str1);
    in.word(str2);
    in.line(str3);
    in.line(str4);
}
