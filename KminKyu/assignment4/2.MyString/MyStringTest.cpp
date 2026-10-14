#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "MyString.h"

namespace {
    // MyString 내용이 expected와 완전히 같은지 (길이 + 각 글자) 확인
    void expectStr(const MyString &actual, const std::string &expected) {
        ASSERT_EQ(actual.length(), static_cast<int>(expected.size()));
        for (int i = 0; i < actual.length(); ++i) {
            EXPECT_EQ(actual.at(i), expected[i]) << "index " << i;
        }
        EXPECT_EQ(actual.empty(), expected.empty());
    }

    std::string str(const MyString &s) {
        std::ostringstream oss;
        oss << s;
        return oss.str();
    }
}

// ---------- 생성자 ----------

TEST(MyStringConstructorTest, DefaultIsEmpty) {
    MyString s;
    expectStr(s, "");
}

TEST(MyStringConstructorTest, FromCString) {
    expectStr(MyString("hello"), "hello");
}

TEST(MyStringConstructorTest, FromEmptyCString) {
    expectStr(MyString(""), "");
}

TEST(MyStringConstructorTest, FromCStringExactly15) {
    expectStr(MyString("123456789012345"), "123456789012345");
}

TEST(MyStringConstructorTest, FromCStringOver15IsTruncated) {
    expectStr(MyString("abcdefghijklmnopqrst"), "abcdefghijklmno");
}

TEST(MyStringConstructorTest, CopyConstructor) {
    MyString a("hello");
    MyString b(a);
    expectStr(b, "hello");
}

TEST(MyStringConstructorTest, CopyIsIndependent) {
    MyString a("hello");
    MyString b(a);
    b.append('!');
    expectStr(a, "hello");
    expectStr(b, "hello!");
}

// ---------- operator= ----------

TEST(MyStringAssignTest, FromMyString) {
    MyString a, b("world");
    a = b;
    expectStr(a, "world");
}

TEST(MyStringAssignTest, FromCString) {
    MyString a("old");
    a = "new value";
    expectStr(a, "new value");
}

TEST(MyStringAssignTest, ShorterOverwritesLonger) {
    MyString a("long string");
    a = "ab";
    expectStr(a, "ab");
}

TEST(MyStringAssignTest, SelfAssignment) {
    MyString a("hello");
    a = a;
    expectStr(a, "hello");
}

TEST(MyStringAssignTest, ChainedAssignment) {
    MyString a, b, c;
    a = b = c = "chain";
    expectStr(a, "chain");
    expectStr(b, "chain");
    expectStr(c, "chain");
}

TEST(MyStringAssignTest, CStringOver15IsTruncated) {
    MyString a;
    a = "abcdefghijklmnopqrst";
    expectStr(a, "abcdefghijklmno");
}

TEST(MyStringAssignTest, EmptyCStringClears) {
    MyString a("hello");
    a = "";
    expectStr(a, "");
}

// ---------- length / empty ----------

TEST(MyStringLengthTest, LengthIsStringLengthNotCapacity) {
    EXPECT_EQ(MyString().length(), 0);
    EXPECT_EQ(MyString("a").length(), 1);
    EXPECT_EQ(MyString("hello").length(), 5);
}

TEST(MyStringEmptyTest, EmptyAndNonEmpty) {
    EXPECT_TRUE(MyString().empty());
    EXPECT_TRUE(MyString("").empty());
    EXPECT_FALSE(MyString("a").empty());
}

// ---------- at / operator[] ----------

TEST(MyStringAtTest, ValidPositions) {
    MyString s("hello");
    EXPECT_EQ(s.at(0), 'h');
    EXPECT_EQ(s.at(4), 'o');
}

TEST(MyStringAtTest, OutOfRangeReturnsNul) {
    MyString s("hello");
    EXPECT_EQ(s.at(5), '\0');
    EXPECT_EQ(s.at(100), '\0');
    EXPECT_EQ(s.at(-1), '\0');
}

TEST(MyStringAtTest, EmptyStringAlwaysNul) {
    MyString s;
    EXPECT_EQ(s.at(0), '\0');
}

TEST(MyStringIndexTest, SameAsAt) {
    MyString s("hello");
    for (int i = -2; i <= 7; ++i) {
        EXPECT_EQ(s[i], s.at(i)) << "index " << i;
    }
}

// ---------- operator+ ----------

TEST(MyStringPlusTest, WithCString) {
    expectStr(MyString("hello") + " world", "hello world");
}

TEST(MyStringPlusTest, WithMyString) {
    expectStr(MyString("hello") + MyString("!!!"), "hello!!!");
}

TEST(MyStringPlusTest, WithEmpty) {
    expectStr(MyString("abc") + "", "abc");
    expectStr(MyString() + "abc", "abc");
    expectStr(MyString() + MyString(), "");
}

TEST(MyStringPlusTest, ExactlyFifteen) {
    expectStr(MyString("1234567890") + "abcde", "1234567890abcde");
}

TEST(MyStringPlusTest, Over15IsTruncated) {
    expectStr(MyString("1234567890") + "abcdefghij", "1234567890abcde");
    expectStr(MyString("1234567890") + MyString("abcdefghij"), "1234567890abcde");
}

TEST(MyStringPlusTest, DoesNotModifyOriginal) {
    MyString a("hello");
    MyString b = a + " world";
    expectStr(a, "hello");
    expectStr(b, "hello world");
}

TEST(MyStringPlusTest, Chained) {
    expectStr(MyString("a") + "b" + "c" + MyString("d"), "abcd");
}

// ---------- == / != ----------

TEST(MyStringCompareTest, EqualStrings) {
    EXPECT_TRUE(MyString("hello") == MyString("hello"));
    EXPECT_FALSE(MyString("hello") != MyString("hello"));
}

TEST(MyStringCompareTest, DifferentContentSameLength) {
    EXPECT_FALSE(MyString("hello") == MyString("hellp"));
    EXPECT_TRUE(MyString("hello") != MyString("hellp"));
    EXPECT_TRUE(MyString("abc") != MyString("xbc"));
}

TEST(MyStringCompareTest, DifferentLength) {
    EXPECT_FALSE(MyString("hello") == MyString("hell"));
    EXPECT_TRUE(MyString("hello") != MyString("hello!"));
}

TEST(MyStringCompareTest, EmptyStrings) {
    EXPECT_TRUE(MyString() == MyString(""));
    EXPECT_TRUE(MyString() != MyString("a"));
}

TEST(MyStringCompareTest, SameObject) {
    MyString a("hello");
    EXPECT_TRUE(a == a);
}

TEST(MyStringCompareTest, AfterClearEqualsEmpty) {
    MyString a("hello");
    a.clear();
    EXPECT_TRUE(a == MyString());
}

TEST(MyStringCompareTest, TruncatedEqualsFirst15) {
    EXPECT_TRUE(MyString("abcdefghijklmnopqrst") == MyString("abcdefghijklmno"));
}

// ---------- find ----------

TEST(MyStringFindTest, FoundAtStart) {
    EXPECT_EQ(MyString("hello world").find("hello"), 0);
}

TEST(MyStringFindTest, FoundInMiddle) {
    EXPECT_EQ(MyString("hello world").find("lo w"), 3);
}

TEST(MyStringFindTest, FoundAtEnd) {
    EXPECT_EQ(MyString("hello world").find("world"), 6);
    EXPECT_EQ(MyString("hello").find("o"), 4);
}

TEST(MyStringFindTest, NotFound) {
    EXPECT_EQ(MyString("hello").find("xyz"), -1);
    EXPECT_EQ(MyString("hello").find("helloo"), -1);   // 더 긴 부분문자열
    EXPECT_EQ(MyString("hello").find("hellp"), -1);    // 거의 일치
}

TEST(MyStringFindTest, ReturnsFirstOccurrence) {
    EXPECT_EQ(MyString("abcabcabc").find("abc"), 0);
    EXPECT_EQ(MyString("abcabcabc").find("cab"), 2);
}

TEST(MyStringFindTest, WithPos) {
    MyString s("abcabcabc");
    EXPECT_EQ(s.find("abc", 1), 3);
    EXPECT_EQ(s.find("abc", 3), 3);
    EXPECT_EQ(s.find("abc", 4), 6);
    EXPECT_EQ(s.find("abc", 7), -1);
}

TEST(MyStringFindTest, NegativePosTreatedAsZero) {
    EXPECT_EQ(MyString("hello").find("h", -5), 0);
    EXPECT_EQ(MyString("hello").find("l", -1), 2);
}

TEST(MyStringFindTest, PosOutOfRangeReturnsMinusOne) {
    EXPECT_EQ(MyString("hello").find("o", 5), -1);
    EXPECT_EQ(MyString("hello").find("h", 100), -1);
}

TEST(MyStringFindTest, SingleCharacter) {
    EXPECT_EQ(MyString("hello").find("l"), 2);
    EXPECT_EQ(MyString("hello").find("l", 3), 3);
}

TEST(MyStringFindTest, WholeStringMatch) {
    EXPECT_EQ(MyString("hello").find("hello"), 0);
}

TEST(MyStringFindTest, OverlappingPatterns) {
    // KMP 실패 함수 확인용: 접두사/접미사가 겹치는 패턴
    EXPECT_EQ(MyString("aaaaab").find("aaab"), 2);
    EXPECT_EQ(MyString("ababababc").find("ababc"), 4);
    EXPECT_EQ(MyString("abababab").find("abac"), -1);
    EXPECT_EQ(MyString("aabaabaaa").find("aabaaa"), 3);
}

TEST(MyStringFindTest, MyStringOverloadMatchesCStringOverload) {
    MyString s("abcabcabc");
    EXPECT_EQ(s.find(MyString("cab")), 2);
    EXPECT_EQ(s.find(MyString("abc"), 4), 6);
    EXPECT_EQ(s.find(MyString("zzz")), -1);
    EXPECT_EQ(s.find(MyString("a"), -3), 0);
}

TEST(MyStringFindTest, OnEmptyString) {
    EXPECT_EQ(MyString().find("a"), -1);
}

TEST(MyStringFindTest, ConsistentWithBruteForce) {
    // 모든 (pos, 패턴) 조합을 std::string과 비교
    const std::string text = "abababcabcabab";
    MyString s(text.c_str());
    const char *patterns[] = {"a", "ab", "aba", "abab", "abc", "bca", "cab", "bab", "x", "abababc"};
    for (const char *p : patterns) {
        for (int pos = 0; pos < static_cast<int>(text.size()); ++pos) {
            size_t expected = text.find(p, pos);
            int want = expected == std::string::npos ? -1 : static_cast<int>(expected);
            EXPECT_EQ(s.find(p, pos), want) << "pattern=" << p << " pos=" << pos;
        }
    }
}

// ---------- substr ----------

TEST(MyStringSubstrTest, Basic) {
    MyString s("hello world");
    expectStr(s.substr(0, 5), "hello");
    expectStr(s.substr(6, 5), "world");
    expectStr(s.substr(2, 3), "llo");
}

TEST(MyStringSubstrTest, LenLongerThanRemainingCopiesToEnd) {
    MyString s("hello");
    expectStr(s.substr(2, 100), "llo");
    expectStr(s.substr(0, 100), "hello");
}

TEST(MyStringSubstrTest, NegativePosTreatedAsZero) {
    MyString s("hello");
    expectStr(s.substr(-3, 2), "he");
}

TEST(MyStringSubstrTest, PosAtOrBeyondLengthReturnsEmpty) {
    MyString s("hello");
    expectStr(s.substr(5, 3), "");
    expectStr(s.substr(10, 3), "");
}

TEST(MyStringSubstrTest, ZeroLenReturnsEmpty) {
    expectStr(MyString("hello").substr(1, 0), "");
}

TEST(MyStringSubstrTest, OnEmptyString) {
    expectStr(MyString().substr(0, 3), "");
}

TEST(MyStringSubstrTest, DoesNotModifyOriginal) {
    MyString s("hello");
    MyString t = s.substr(1, 3);
    expectStr(s, "hello");
    expectStr(t, "ell");
}

TEST(MyStringSubstrTest, MatchesStdStringBehavior) {
    const std::string text = "hello world";
    MyString s(text.c_str());
    for (int pos = 0; pos <= static_cast<int>(text.size()); ++pos) {
        for (int len = 0; len <= 15; ++len) {
            std::string expected = text.substr(pos, len);
            SCOPED_TRACE("pos=" + std::to_string(pos) + " len=" + std::to_string(len));
            expectStr(s.substr(pos, len), expected);
        }
    }
}

// ---------- append / clear ----------

TEST(MyStringAppendTest, AppendChar) {
    MyString s;
    s.append('a');
    s.append('b');
    expectStr(s, "ab");
}

TEST(MyStringAppendTest, AppendCString) {
    MyString s("hello");
    s.append(" world");
    expectStr(s, "hello world");
}

TEST(MyStringAppendTest, AppendCharIgnoredWhenFull) {
    MyString s("123456789012345");
    s.append('x');
    expectStr(s, "123456789012345");
}

TEST(MyStringAppendTest, AppendCStringTruncatesAt15) {
    MyString s("1234567890");
    s.append("abcdefghij");
    expectStr(s, "1234567890abcde");
}

TEST(MyStringClearTest, ClearMakesEmpty) {
    MyString s("hello");
    s.clear();
    expectStr(s, "");
}

TEST(MyStringClearTest, UsableAfterClear) {
    MyString s("hello");
    s.clear();
    s.append("abc");
    expectStr(s, "abc");
    EXPECT_TRUE(s == MyString("abc"));
}

// ---------- operator<< ----------

TEST(MyStringOutputTest, PrintsContent) {
    EXPECT_EQ(str(MyString("hello")), "hello");
}

TEST(MyStringOutputTest, EmptyPrintsNothing) {
    EXPECT_EQ(str(MyString()), "");
}

TEST(MyStringOutputTest, Chained) {
    std::ostringstream oss;
    oss << MyString("a") << "-" << MyString("b");
    EXPECT_EQ(oss.str(), "a-b");
}

TEST(MyStringOutputTest, TruncatedStringPrints15Chars) {
    EXPECT_EQ(str(MyString("abcdefghijklmnopqrst")), "abcdefghijklmno");
}

// ---------- operator>> ----------

TEST(MyStringInputTest, ReadsUntilWhitespace) {
    std::istringstream in("hello world");
    MyString a, b;
    in >> a >> b;
    expectStr(a, "hello");
    expectStr(b, "world");
}

TEST(MyStringInputTest, StopsAtNewline) {
    std::istringstream in("abc\ndef");
    MyString a, b;
    in >> a >> b;
    expectStr(a, "abc");
    expectStr(b, "def");
}

TEST(MyStringInputTest, StopsAtTab) {
    std::istringstream in("abc\tdef");
    MyString a, b;
    in >> a >> b;
    expectStr(a, "abc");
    expectStr(b, "def");
}

TEST(MyStringInputTest, OverwritesPreviousContent) {
    std::istringstream in("new");
    MyString a("old content");
    in >> a;
    expectStr(a, "new");
}

TEST(MyStringInputTest, Exactly15Chars) {
    std::istringstream in("123456789012345 next");
    MyString a;
    in >> a;
    expectStr(a, "123456789012345");
}

TEST(MyStringInputTest, Over15CharsLeavesRestInBuffer) {
    std::istringstream in("abcdefghijklmnopqrst");   // 20글자
    MyString a, b;
    in >> a;
    expectStr(a, "abcdefghijklmno");
    in >> b;
    expectStr(b, "pqrst");                           // 넘친 글자가 버퍼에 남아 있어야 함
}

TEST(MyStringInputTest, Over30CharsSplitsIntoChunks) {
    std::istringstream in("abcdefghijklmnopqrstuvwxyzABCDE");   // 31글자
    MyString a, b, c;
    in >> a >> b >> c;
    expectStr(a, "abcdefghijklmno");
    expectStr(b, "pqrstuvwxyzABCD");
    expectStr(c, "E");
}

// ---------- getline ----------

TEST(MyStringGetlineTest, ReadsUntilNewline) {
    std::istringstream in("hello world\nsecond");
    MyString s;
    getline(in, s);
    expectStr(s, "hello world");
}

TEST(MyStringGetlineTest, ConsumesDelimiter) {
    std::istringstream in("first\nsecond\n");
    MyString a, b;
    getline(in, a);
    getline(in, b);
    expectStr(a, "first");
    expectStr(b, "second");
}

TEST(MyStringGetlineTest, KeepsWhitespaceInside) {
    std::istringstream in("  a b  c \n");
    MyString s;
    getline(in, s);
    expectStr(s, "  a b  c ");
}

TEST(MyStringGetlineTest, CustomDelimiter) {
    std::istringstream in("abc,def,ghi");
    MyString a, b, c;
    getline(in, a, ',');
    getline(in, b, ',');
    getline(in, c, ',');
    expectStr(a, "abc");
    expectStr(b, "def");
    expectStr(c, "ghi");
}

TEST(MyStringGetlineTest, EmptyLine) {
    std::istringstream in("\nabc\n");
    MyString a, b;
    getline(in, a);
    getline(in, b);
    expectStr(a, "");
    expectStr(b, "abc");
}

TEST(MyStringGetlineTest, OverwritesPreviousContent) {
    std::istringstream in("new\n");
    MyString s("old content");
    getline(in, s);
    expectStr(s, "new");
}

TEST(MyStringGetlineTest, Over15CharsDiscardsRestOfLine) {
    std::istringstream in("abcdefghijklmnopqrst\nnext\n");
    MyString a, b;
    getline(in, a);
    getline(in, b);
    expectStr(a, "abcdefghijklmno");   // 16번째부터 버림
    expectStr(b, "next");              // 다음 줄은 정상
}

TEST(MyStringGetlineTest, Exactly15Chars) {
    std::istringstream in("123456789012345\nnext\n");
    MyString a, b;
    getline(in, a);
    getline(in, b);
    expectStr(a, "123456789012345");
    expectStr(b, "next");
}

TEST(MyStringGetlineTest, ReturnsStream) {
    std::istringstream in("a\nb\n");
    MyString a, b;
    getline(getline(in, a), b);        // 반환값이 istream&인지 확인
    expectStr(a, "a");
    expectStr(b, "b");
}

TEST(MyStringGetlineTest, EofWithoutDelimiterTerminates) {
    std::istringstream in("last line");
    MyString s;
    getline(in, s);                    // 무한 루프에 빠지지 않아야 함
    expectStr(s, "last line");
}

TEST(MyStringGetlineTest, MixedWithExtraction) {
    std::istringstream in("word rest of line\nnext\n");
    MyString w, line;
    in >> w;
    getline(in, line);
    expectStr(w, "word");
    expectStr(line, " rest of line");
}

TEST(MyStringFindCollisionTest, CollisionIsRejectedByDirectComparison) {
    EXPECT_EQ(MyString("ba").find("ah"), -1);   // 해시는 같지만 다른 문자열
    EXPECT_EQ(MyString("ah").find("ba"), -1);
}

TEST(MyStringFindCollisionTest, CollisionThenRealMatchLater) {
    // index 1의 "ba"가 먼저 충돌하고, 진짜 매칭은 index 3
    EXPECT_EQ(MyString("xbaah").find("ah"), 3);
}

TEST(MyStringFindCollisionTest, ThreeCharCollision) {
    // index 0의 "baz"가 충돌, 진짜 매칭은 index 3
    EXPECT_EQ(MyString("bazahz").find("ahz"), 3);
    EXPECT_EQ(MyString("baz").find("ahz"), -1);
}

TEST(MyStringFindCollisionTest, CollisionWithPos) {
    EXPECT_EQ(MyString("baah").find("ah", 1), 2);
    EXPECT_EQ(MyString("ba").find("ah", 0), -1);
}


// ---------- operator>> : EOF / 스트림 상태 ----------

TEST(MyStringInputEofTest, LastWordWithoutTrailingWhitespaceSucceeds) {
    std::istringstream in("abc");              // 끝에 공백/개행 없음
    MyString a;
    ASSERT_TRUE(static_cast<bool>(in >> a));   // 원래 코드: 여기서 실패
    expectStr(a, "abc");
}

TEST(MyStringInputEofTest, WhileLoopReadsEveryWord) {
    std::istringstream in("one two three");    // 마지막 단어 뒤 개행 없음
    MyString w;
    int count = 0;
    while (in >> w) ++count;
    EXPECT_EQ(count, 3);                       // 원래 코드: 2
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
    expectStr(b, "");                          // 실패해도 clear된 상태
}

TEST(MyStringInputEofTest, EmptyStreamFails) {
    std::istringstream in("");
    MyString a("old");
    EXPECT_FALSE(static_cast<bool>(in >> a));
    expectStr(a, "");
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
    // 원래 코드는 초기화 안 된 char를 putback함 (UB) -> sanitizer/valgrind에서만 잡힘
}

TEST(MyStringInputEofTest, NonAsciiBytesDoNotBreakIsspace) {
    std::istringstream in("안녕 world");        // UTF-8 바이트는 signed char에서 음수
    MyString a, b;
    in >> a >> b;                              // MSVC 디버그: isspace assertion
    expectStr(a, "안녕");
    expectStr(b, "world");
}

// ---------- getline : EOF / 스트림 상태 ----------

TEST(MyStringGetlineEofTest, LastLineWithoutDelimiterSucceeds) {
    std::istringstream in("last line");
    MyString s;
    ASSERT_TRUE(static_cast<bool>(getline(in, s)));   // 원래 코드: 여기서 실패
    expectStr(s, "last line");
}

TEST(MyStringGetlineEofTest, WhileLoopReadsEveryLine) {
    std::istringstream in("line1\nline2");     // 마지막 줄 개행 없음
    MyString s;
    int count = 0;
    while (getline(in, s)) ++count;
    EXPECT_EQ(count, 2);                       // 원래 코드: 1
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
    expectStr(s, "");
}

TEST(MyStringGetlineEofTest, LoneDelimiterIsOneEmptyLine) {
    std::istringstream in("\n");
    MyString s("old");
    ASSERT_TRUE(static_cast<bool>(getline(in, s)));
    expectStr(s, "");
    EXPECT_FALSE(static_cast<bool>(getline(in, s)));
}

TEST(MyStringGetlineEofTest, CustomDelimiterLastTokenWithoutDelimiter) {
    std::istringstream in("abc,def");
    MyString s;
    int count = 0;
    while (getline(in, s, ',')) ++count;
    EXPECT_EQ(count, 2);                       // 원래 코드: 1
}