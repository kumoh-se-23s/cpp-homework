// =====================================================================
// mainTest.cpp  -  assign8 엣지케이스 테스트
//
//  * assert() 대신 자체 CHECK 매크로 사용 -> NDEBUG(Release) 빌드에서도 동작하고,
//    하나가 실패해도 멈추지 않고 전부 실행해서 한 번에 결과를 보여줍니다.
//  * CHECK : 반드시 지켜져야 하는 동작 (실패 시 FAIL, 종료코드 1)
//  * WARN  : 스펙에 따라 해석이 갈리거나 "견고성 개선 권장" 항목 (종료코드 영향 없음)
//  * RUN_RISKY_TESTS : 현재 구현에서 크래시/UB를 일으키는 입력들. 수정 후 켜서 확인.
//
//  빌드 예 (main.cpp 는 제외):
//    g++ -std=c++17 -g -fsanitize=address,undefined mainTest.cpp App.cpp AdStudent.cpp
//        EmployEduStudent.cpp FileIO.cpp GiftedStudent.cpp LifeLongStudent.cpp
//        MyString.cpp ShortTermEduStudent.cpp -o mainTest   (한 줄로 입력) 후 ./mainTest
//    위험 테스트까지:  -DRUN_RISKY_TESTS
//
//  주의: EmployEdu/ShortTerm 의 nextID 는 static 이라 프로세스 내에서 줄어들지 않습니다.
//        그래서 (1) testBasicScenario 가 가장 먼저, (2) testAutoID 가 그 다음에 실행되고
//        이후 테스트는 학번 값을 하드코딩하지 않고 상대 비교만 합니다.
// =====================================================================
#include "App.h"
#include "FileIO.h"
#include "AdStudent.h"
#include "GiftedStudent.h"
#include "LifeLongStudent.h"
#include "EmployEduStudent.h"
#include "ShortTermEduStudent.h"
#include "SinglyLinkedList.hpp"

#include <climits>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------
// 미니 테스트 프레임워크
// ---------------------------------------------------------------------
static int g_pass = 0, g_fail = 0, g_warn = 0;

static void report(bool ok, const char* expr, int line) {
    if (ok) { ++g_pass; return; }
    ++g_fail;
    cout << "  [FAIL] line " << line << ": " << expr << "\n";
}
#define CHECK(c) report((c), #c, __LINE__)

static void reportMsg(bool ok, const string& msg, int line) {
    if (ok) { ++g_pass; return; }
    ++g_fail;
    cout << "  [FAIL] line " << line << ": " << msg << "\n";
}
#define CHECK_MSG(c, msg) reportMsg((c), (msg), __LINE__)

static void reportStr(const MyString& got, const char* want, int line) {
    if (got == want) { ++g_pass; return; }
    ++g_fail;
    cout << "  [FAIL] line " << line << ": expected \"" << want << "\" but got \"" << got << "\"\n";
}
#define CHECK_STR(got, want) reportStr((got), (want), __LINE__)

static void reportInt(long long got, long long want, const char* expr, int line) {
    if (got == want) { ++g_pass; return; }
    ++g_fail;
    cout << "  [FAIL] line " << line << ": " << expr << " expected " << want << " but got " << got << "\n";
}
#define CHECK_EQ(got, want) reportInt((got), (want), #got, __LINE__)

static void reportWarn(bool ok, const char* msg, int line) {
    if (ok) { ++g_pass; return; }
    ++g_warn;
    cout << "  [WARN] line " << line << ": " << msg << "\n";
}
#define WARN(c, msg) reportWarn((c), (msg), __LINE__)

static void runTest(const char* name, void (*fn)()) {
    int f0 = g_fail, w0 = g_warn;
    fn();
    int f = g_fail - f0, w = g_warn - w0;
    cout << (f ? "[FAIL] " : "[PASS] ") << name;
    if (f) cout << "  (" << f << " failed)";
    if (w) cout << "  (" << w << " warn)";
    cout << "\n";
}

// ---------------------------------------------------------------------
// 헬퍼
// ---------------------------------------------------------------------
static string readAll(const string& f) {
    ifstream in(f.c_str(), ios::binary);
    stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}
static void writeAll(const string& f, const string& s) {
    ofstream o(f.c_str(), ios::binary);
    o << s;
}
struct TempFile {
    string name;
    explicit TempFile(const string& n, const string& content = "") : name(n) { writeAll(name, content); }
    ~TempFile() { remove(name.c_str()); }
};

static bool has(const string& s, const string& sub) { return s.find(sub) != string::npos; }
static int countOf(const string& s, const string& sub) {
    int c = 0;
    for (size_t p = s.find(sub); p != string::npos; p = s.find(sub, p + sub.size())) ++c;
    return c;
}
static void checkHas(const string& out, const string& sub, int line) {
    if (has(out, sub)) { ++g_pass; return; }
    ++g_fail;
    cout << "  [FAIL] line " << line << ": output does not contain: " << sub << "\n";
}
#define CHECK_HAS(out, sub) checkHas((out), (sub), __LINE__)
static void checkNotHas(const string& out, const string& sub, int line) {
    if (!has(out, sub)) { ++g_pass; return; }
    ++g_fail;
    cout << "  [FAIL] line " << line << ": output unexpectedly contains: " << sub << "\n";
}
#define CHECK_NOT_HAS(out, sub) checkNotHas((out), (sub), __LINE__)

// cin/cout 를 가로채서 App 을 실행. cin 상태(fail/eof)가 다음 테스트로 새지 않도록 clear.
static string runApp(const string& file, const string& input) {
    istringstream iss(input);
    ostringstream oss;
    auto* cinB = cin.rdbuf(iss.rdbuf());
    auto* coutB = cout.rdbuf(oss.rdbuf());
    cin.clear();
    { App app(file.c_str()); app.run(); }
    cin.clear();
    cin.rdbuf(cinB);
    cout.rdbuf(coutB);
    return oss.str();
}

static bool loadList(const string& f, SinglyLinkedList<AdStudent>& l) { return FileIO::loadFile(f.c_str(), l); }

// 파일을 읽어 목록 순서대로 학번을 "A,B,C" 로 반환
static string orderOf(const string& f) {
    SinglyLinkedList<AdStudent> l;
    if (!loadList(f, l)) return "<load-failed>";
    string r;
    for (SinglyNode<AdStudent>* n = l.getHead(); n; n = n->getNext()) {
        if (!r.empty()) r += ",";
        r += n->getData()->getID().getStr();
    }
    return r;
}

// 출력 중 marker 이후 ~ 다음 메뉴 프롬프트 직전까지
static string afterFirst(const string& out, const string& marker) {
    size_t p = out.find(marker);
    if (p == string::npos) return "";
    p += marker.size();
    size_t e = out.find("[1] View all", p);
    return out.substr(p, e == string::npos ? string::npos : e - p);
}

// UTF-8 한글 (소스 인코딩에 영향받지 않도록 hex escape)
static const char* KO_GA = "\xEA\xB0\x80";  // 가
static const char* KO_NA = "\xEB\x82\x98";  // 나
static const char* KO_DA = "\xEB\x8B\xA4";  // 다

// 메모리 해제 추적용 Mock
class TrackedStudent : public AdStudent {
    MyString id;
public:
    static int aliveCount;
    TrackedStudent(const char* idStr, const char* nameStr, const char* phoneStr) : id(idStr) {
        studentName = nameStr;
        course = "Test";
        phoneNumber = phoneStr;
        ++aliveCount;
    }
    ~TrackedStudent() override { --aliveCount; }
    const MyString& getID() const override { return id; }
    void input() override {}
    void modify() override {}
    MyString toString() const override { return id; }
    MyString toFileFormatString() const override { return id; }
};
int TrackedStudent::aliveCount = 0;

static_assert(!is_copy_constructible<Node<AdStudent>>::value, "Node must be non-copyable (double free)");
static_assert(!is_copy_assignable<Node<AdStudent>>::value, "Node must be non-assignable (double free)");

// =====================================================================
// A. MyString
// =====================================================================
static void testMyStringConstruction() {
    MyString e;
    CHECK_EQ(e.length(), 0);
    CHECK(e.isEmpty());
    CHECK_EQ(e.getCapacity(), MyString::DEFAULT_CAPACITY);
    CHECK(strcmp(e.getStr(), "") == 0);

    MyString z("");
    CHECK(z.isEmpty());
    CHECK(z == e);

    // 경계 길이(7/8/15/16/17)에서도 널문자 자리가 항상 확보돼야 함
    const char* lens[] = {"1234567", "12345678", "123456789012345", "1234567890123456", "12345678901234567"};
    for (const char* c : lens) {
        MyString s(c);
        CHECK_EQ(s.length(), (int)strlen(c));
        CHECK(s.getCapacity() > s.length());
        CHECK(strcmp(s.getStr(), c) == 0);
    }

    MyString big(100);
    CHECK(big.isEmpty());
    CHECK(big.getCapacity() > 100);
    MyString negCap(-5);          // 음수 capacity 요청에도 안전해야 함
    CHECK(negCap.isEmpty());
    CHECK(negCap.getCapacity() > 0);
    MyString zeroCap(0);
    CHECK(zeroCap.isEmpty());
}

static void testMyStringAtAndIndex() {
    MyString s("abc");
    CHECK(s.at(0) == 'a' && s.at(2) == 'c');
    CHECK(s[1] == 'b');
    CHECK(s.at(3) == '\0');       // 범위 밖은 크래시 없이 '\0'
    CHECK(s.at(-1) == '\0');
    CHECK(s.at(1000) == '\0');
    const MyString& cs = s;
    CHECK(cs[-7] == '\0');
    CHECK(cs[99] == '\0');
    s[0] = 'X';
    CHECK_STR(s, "Xbc");
    MyString e;
    CHECK(e.at(0) == '\0');
}

static void testMyStringCopyAssign() {
    MyString a("hello");
    MyString b(a);
    b[0] = 'J';
    CHECK_STR(a, "hello");        // 깊은 복사
    CHECK_STR(b, "Jello");

    MyString emptyCopy((MyString()));
    CHECK(emptyCopy.isEmpty());

    MyString longStr("this string is definitely longer than the default capacity of eight");
    MyString longCopy(longStr);
    CHECK(longCopy == longStr);
    CHECK_EQ(longCopy.getCapacity(), longStr.getCapacity());

    // 짧은 -> 긴 -> 짧은 -> 빈 문자열
    MyString x("short");
    x = longStr;
    CHECK(x == longStr);
    x = "tiny";
    CHECK_STR(x, "tiny");
    x = "";
    CHECK(x.isEmpty());
    CHECK(strcmp(x.getStr(), "") == 0);

    // 자기 대입 (참조/c-string 둘 다)
    MyString y("self");
    MyString& ry = y;
    y = ry;
    CHECK_STR(y, "self");
    y = y.getStr();
    CHECK_STR(y, "self");

    // 연쇄 대입
    MyString p, q, r("chain");
    p = q = r;
    CHECK_STR(p, "chain");
    CHECK_STR(q, "chain");
}

static void testMyStringConcat() {
    MyString a("foo"), b("bar"), e;
    CHECK_STR(a + b, "foobar");
    CHECK_STR(a + "baz", "foobaz");
    CHECK_STR(a + e, "foo");
    CHECK_STR(e + a, "foo");
    CHECK((e + e).isEmpty());
    CHECK_STR(a + "", "foo");
    CHECK_STR(a, "foo");           // 피연산자 불변
    CHECK_STR(b, "bar");
    CHECK_STR(a + " " + b + "!" + MyString("?"), "foo bar!?");

    MyString seven("1234567");     // 7 + 1 = 8 : capacity 경계
    MyString eight = seven + "8";
    CHECK_STR(eight, "12345678");
    CHECK(strlen(eight.getStr()) == 8);

    MyString res = a + b;          // 결과는 독립적
    res[0] = 'X';
    CHECK_STR(a, "foo");
}

static void testMyStringCompare() {
    MyString abc("abc"), abd("abd"), ab("ab"), e;
    CHECK(abc == "abc");
    CHECK(abc == MyString("abc"));
    CHECK(abc != abd);
    CHECK(abc != ab);              // 접두사는 같지 않음
    CHECK(!(ab == abc));
    CHECK(e == "");
    CHECK(MyString("A") != MyString("a"));   // 대소문자 구분

    CHECK(abc < abd);
    CHECK(ab < abc);               // 짧은 접두사가 먼저
    CHECK(e < ab);
    CHECK(!(e < e));
    CHECK(abd > abc);
    CHECK(abc > ab);
    CHECK(abc <= abc);
    CHECK(abc >= abc);
    CHECK(!(abc < abc));
    CHECK(!(abc > abc));
    CHECK(MyString("Z") < MyString("a"));    // ASCII: 대문자 < 소문자
    CHECK(MyString("9") < MyString("A"));
    CHECK(abc < "abd");
    CHECK(abd > "abc");
    CHECK(abc <= "abc");
    CHECK(abc >= "abc");
    CHECK(abc != "ab");

    // 삼분법: 정확히 하나만 참
    const char* vals[] = {"", "a", "ab", "abc", "b", "B", "10", "9"};
    for (const char* x : vals) {
        for (const char* y : vals) {
            MyString mx(x), my(y);
            int n = (mx < my) + (mx == my) + (mx > my);
            CHECK_MSG(n == 1, string("trichotomy broken for '") + x + "' vs '" + y + "'");
            CHECK((mx <= my) == (mx < my || mx == my));
            CHECK((mx >= my) == (mx > my || mx == my));
        }
    }
}

static void testMyStringFind() {
    MyString s("abcabcabc");
    CHECK_EQ(s.find("abc"), 0);
    CHECK_EQ(s.find("abc", 1), 3);
    CHECK_EQ(s.find("abc", 4), 6);
    CHECK_EQ(s.find("abc", 7), -1);
    CHECK_EQ(s.find("c"), 2);
    CHECK_EQ(s.find("cab"), 2);
    CHECK_EQ(s.find("xyz"), -1);
    CHECK_EQ(s.find("abcabcabcd"), -1);   // 대상보다 긴 부분문자열
    CHECK_EQ(s.find("abc", 100), -1);     // pos 범위 밖
    CHECK_EQ(s.find("abc", -3), 0);       // 음수 pos
    CHECK_EQ(s.find(MyString("bca")), 1);
    CHECK_EQ(s.find("c", 8), 8);          // 마지막 글자
    CHECK_EQ(MyString("aaa").find("aa", 1), 1);   // 겹치는 매칭
    CHECK_EQ(MyString().find("a"), -1);           // 빈 문자열에서 검색
    CHECK_EQ(MyString("a").find("a"), 0);         // 길이 1
}

static void testMyStringSubstr() {
    MyString s("abcdef");
    CHECK_STR(s.substr(0, 3), "abc");
    CHECK_STR(s.substr(2, 2), "cd");
    CHECK_STR(s.substr(4, 100), "ef");     // 길이 초과는 끝까지
    CHECK_STR(s.substr(0, 6), "abcdef");
    CHECK_STR(s.substr(5, 1), "f");
    CHECK(s.substr(6, 1).isEmpty());       // pos == size
    CHECK(s.substr(100, 1).isEmpty());     // pos > size
    CHECK(s.substr(0, 0).isEmpty());
    CHECK(s.substr(0, -1).isEmpty());
    CHECK(MyString().substr(0, 3).isEmpty());
    CHECK_STR(s, "abcdef");                // 원본 불변
    MyString t = s.substr(1, 3);
    t[0] = 'X';
    CHECK_STR(s, "abcdef");                // 결과는 독립
}

static void testMyStringInsert() {
    MyString s("abc");
    s.insert(0, "X");               CHECK_STR(s, "Xabc");
    s.insert(2, "YY");              CHECK_STR(s, "XaYYbc");
    s.insert(s.length(), "Z");      CHECK_STR(s, "XaYYbcZ");
    s.insert(100, "!");             CHECK_STR(s, "XaYYbcZ!");     // pos > size -> 끝
    s.insert(-5, "^");              CHECK_STR(s, "^XaYYbcZ!");    // pos < 0 -> 앞
    s.insert(1, "");                CHECK_STR(s, "^XaYYbcZ!");
    s.insert(1, MyString());        CHECK_STR(s, "^XaYYbcZ!");

    MyString g("1234567");          // capacity 경계에서 재할당
    g.insert(3, "abcdefghij");
    CHECK_STR(g, "123abcdefghij4567");
    CHECK(strlen(g.getStr()) == 17);

    MyString e;
    e.insert(0, "hello");           CHECK_STR(e, "hello");
    MyString e2;
    e2.insert(5, "hi");             CHECK_STR(e2, "hi");

    // 자기 자신 삽입 (non-resize / resize 경로)
    MyString a("abc");
    a.insert(1, a);                 CHECK_STR(a, "aabcbc");
    MyString b("0123456");
    b.insert(2, b);                 CHECK_STR(b, "01012345623456");
    MyString c("abc");
    c.insert(0, c.getStr());        CHECK_STR(c, "abcabc");
}

static void testMyStringReplace() {
    MyString s("Hello World");
    s.replace(6, 5, "There");       CHECK_STR(s, "Hello There");
    s.replace(0, 5, "Hi");          CHECK_STR(s, "Hi There");
    s.replace(0, 2, "Greetings");   CHECK_STR(s, "Greetings There");
    MyString t("Hello World");
    t.replace(5, 100, "!");         CHECK_STR(t, "Hello!");       // 길이 초과
    MyString u("Hello");
    u.replace(5, 0, "?");           CHECK_STR(u, "Hello?");       // 끝 위치 = append
    u.replace(50, 2, "Z");          CHECK_STR(u, "Hello?Z");      // pos > size -> 끝
    MyString v("Hello World");
    v.replace(2, 3, "");            CHECK_STR(v, "He World");     // 빈 문자열 = erase
    MyString w("abc");
    w.replace(0, 3, "");            CHECK(w.isEmpty());
    w.replace(0, 0, "xyz");         CHECK_STR(w, "xyz");
    MyString x("same");
    x.replace(0, 4, "SAME");        CHECK_STR(x, "SAME");         // 같은 길이

    // 자기 메모리를 가리키는 인자 (aliasing): non-resize / resize
    MyString a("abcdef");
    a.replace(0, 2, a.getStr() + 3);          CHECK_STR(a, "defcdef");
    MyString b("abcdefg");
    b.replace(0, 1, b.getStr() + 1);          CHECK_STR(b, "bcdefgbcdefg");
    MyString c("abcdef");
    c.replace(1, 2, c);                       CHECK_STR(c, "aabcdefdef");
}

static void testMyStringErase() {
    MyString s("abcdef");
    s.erase(2, 2);                  CHECK_STR(s, "abef");
    s.erase(2, 100);                CHECK_STR(s, "ab");           // 길이 초과
    s.erase(0, 0);                  CHECK_STR(s, "ab");
    s.erase(10, 2);                 CHECK_STR(s, "ab");           // pos 범위 밖 -> no-op
    s.erase(2, 1);                  CHECK_STR(s, "ab");           // pos == size
    s.erase(0, 2);                  CHECK(s.isEmpty());
    s.erase(0, 5);                  CHECK(s.isEmpty());           // 빈 문자열 erase
    CHECK(strcmp(s.getStr(), "") == 0);

    MyString t("abcdef");
    t.erase(0, 100);                CHECK(t.isEmpty());
    MyString u("abcdef");
    u.erase(5, 1);                  CHECK_STR(u, "abcde");        // 마지막 글자
    MyString v("abcdef");
    v.erase(0, 1);                  CHECK_STR(v, "bcdef");        // 첫 글자
}

static void testMyStringAppend() {
    MyString s;
    s.append('a').append("bc").append(MyString("de"));
    CHECK_STR(s, "abcde");
    s += "f";
    s += MyString("g");
    CHECK_STR(s, "abcdefg");
    s.append("");
    s.append(MyString());
    s += "";
    CHECK_STR(s, "abcdefg");

    // 자기 자신 append (non-resize / resize)
    MyString a("ab");
    a.append(a);                    CHECK_STR(a, "abab");
    a += a;                         CHECK_STR(a, "abababab");
    MyString b("0123456789012345678");
    MyString copy(b);
    b.append(b);
    CHECK_STR(b, "01234567890123456780123456789012345678");
    CHECK_EQ(b.length(), copy.length() * 2);

    // 길이 0..300 까지 한 글자씩: 길이/널종료/용량 불변식
    MyString g;
    for (int i = 1; i <= 300; ++i) {
        g.append('x');
        if (g.length() != i || (int)strlen(g.getStr()) != i || g.getCapacity() <= i) {
            CHECK_MSG(false, "append(char) invariant broken at length " + to_string(i));
            break;
        }
    }
    MyString h;
    for (int i = 1; i <= 100; ++i) {
        h.append("ab");
        if (h.length() != 2 * i || (int)strlen(h.getStr()) != 2 * i) {
            CHECK_MSG(false, "append(cstr) invariant broken at iteration " + to_string(i));
            break;
        }
    }
    CHECK(true);
}

static void testMyStringTrim() {
    MyString a("  hi  ");          a.trim(); CHECK_STR(a, "hi");
    MyString b("\t\r\nhi\r\n\t");  b.trim(); CHECK_STR(b, "hi");
    MyString c("   \t\r\n  ");     c.trim(); CHECK(c.isEmpty());
    MyString d;                    d.trim(); CHECK(d.isEmpty());
    MyString e("hi");              e.trim(); CHECK_STR(e, "hi");
    MyString f(" a b  c ");        f.trim(); CHECK_STR(f, "a b  c");    // 내부 공백 보존
    MyString g("x");               g.trim(); CHECK_STR(g, "x");
    MyString h(" ");               h.trim(); CHECK(h.isEmpty());
    MyString i("  lead");          i.trim(); CHECK_STR(i, "lead");
    MyString j("trail  ");         j.trim(); CHECK_STR(j, "trail");
    CHECK_STR(MyString(" x ").trim(), "x");      // 체이닝
}

static void testStoi() {
    CHECK_EQ(stoi(MyString("123")), 123);
    CHECK_EQ(stoi(MyString("0")), 0);
    CHECK_EQ(stoi(MyString("007")), 7);
    CHECK_EQ(stoi(MyString("12abc")), 12);
    CHECK_EQ(stoi(MyString("abc")), 0);
    CHECK_EQ(stoi(MyString("")), 0);
    CHECK_EQ(stoi(MyString("2147483647")), INT_MAX);
    CHECK_EQ(stoi(MyString("E00042"), 1), 42);        // 학번 파싱 용도
    CHECK_EQ(stoi(MyString("E00042"), 0), 0);         // 'E' 는 10진수 아님
    CHECK_EQ(stoi(MyString("E00042"), -5), 0);        // 음수 pos -> 0
    CHECK_EQ(stoi(MyString("E00042"), 100), 0);       // 범위 밖 pos
    CHECK_EQ(stoi(MyString("E"), 1), 0);
    CHECK_EQ(stoi(MyString("ff"), 0, 16), 255);
    CHECK_EQ(stoi(MyString("FF"), 0, 16), 255);
    CHECK_EQ(stoi(MyString("1010"), 0, 2), 10);
    CHECK_EQ(stoi(MyString("z"), 0, 36), 35);
    CHECK_EQ(stoi(MyString("129"), 0, 8), 10);        // '9' 에서 중단 -> "12"(8진) = 10
    CHECK_EQ(stoi(MyString("123"), 0, 1), 0);         // 잘못된 base
    CHECK_EQ(stoi(MyString("123"), 0, 37), 0);
    CHECK_EQ(stoi(MyString("123"), 0, 0), 0);
    CHECK_EQ(stoi(MyString("12-3")), 12);             // 중간 기호에서 중단
}

static void testToString() {
    CHECK_STR(MyString::toString(0), "0");
    CHECK_STR(MyString::toString(7), "7");
    CHECK_STR(MyString::toString(10), "10");
    CHECK_STR(MyString::toString(100000), "100000");
    CHECK_STR(MyString::toString(-1), "-1");
    CHECK_STR(MyString::toString(-500), "-500");
    CHECK_STR(MyString::toString(INT_MAX), "2147483647");
    CHECK_STR(MyString::toString(INT_MIN), "-2147483648");   // -INT_MIN 오버플로 주의
    // 왕복
    int vals[] = {0, 1, 9, 10, 99, 100, 12345, 2147483647};
    for (int v : vals) CHECK_EQ(stoi(MyString::toString(v)), v);
}

static void testMyStringIO() {
    // operator>> : 공백/탭/개행/CR 구분, 앞쪽 공백 스킵
    {
        istringstream in("  hello\t world\r\n  last");
        MyString a, b, c;
        in >> a >> b >> c;
        CHECK_STR(a, "hello");
        CHECK_STR(b, "world");
        CHECK_STR(c, "last");
    }
    // 공백만 있는 입력 -> 실패하고 기존 값 유지
    {
        istringstream in("   \n\t  ");
        MyString s("keep");
        bool ok = static_cast<bool>(in >> s);
        CHECK(!ok);
        CHECK_STR(s, "keep");
    }
    // 빈 스트림
    {
        istringstream in("");
        MyString s("keep");
        CHECK(!(in >> s));
        CHECK_STR(s, "keep");
    }
    // 이전 값이 있어도 새 토큰으로 덮어쓰기
    {
        istringstream in("new");
        MyString s("a very long old value that must be replaced");
        in >> s;
        CHECK_STR(s, "new");
    }
    // 아주 긴 토큰
    {
        string big(10000, 'q');
        istringstream in(big + " x");
        MyString s;
        in >> s;
        CHECK_EQ(s.length(), 10000);
    }
    // 마지막 토큰 뒤에 공백이 없을 때 (표준 스트림은 eofbit만 세우고 fail은 아님)
    {
        istringstream in("abc");
        MyString s;
        bool ok = static_cast<bool>(in >> s);
        CHECK_STR(s, "abc");
        WARN(ok, "마지막 토큰이 EOF로 끝나면 operator>> 가 failbit까지 세움 -> if (cin >> s) 가 거짓이 됨 (표준은 eofbit만)");
    }
    // UTF-8 한글 토큰은 바이트 그대로 보존
    {
        istringstream in(string(KO_GA) + KO_NA + " " + KO_DA);
        MyString a, b;
        in >> a >> b;
        CHECK_EQ(a.length(), 6);
        CHECK_EQ(b.length(), 3);
    }
    // getline
    {
        istringstream in("line one\n\nlast line no newline");
        MyString s;
        ::getline(in, s);  CHECK_STR(s, "line one");
        ::getline(in, s);  CHECK(s.isEmpty());          // 빈 줄
        ::getline(in, s);  CHECK_STR(s, "last line no newline");
        CHECK(!::getline(in, s));                       // 더 이상 없음
        CHECK(s.isEmpty());
    }
    {
        istringstream in("a,b,,c");
        MyString s;
        ::getline(in, s, ',');  CHECK_STR(s, "a");
        ::getline(in, s, ',');  CHECK_STR(s, "b");
        ::getline(in, s, ',');  CHECK(s.isEmpty());
        ::getline(in, s, ',');  CHECK_STR(s, "c");
    }
    {
        istringstream in("crlf\r\nnext");
        MyString s;
        ::getline(in, s);
        CHECK_EQ(s.length(), 5);                        // 표준과 동일하게 '\r' 남음
    }
    // operator<<
    {
        ostringstream os;
        os << MyString("ab") << MyString() << MyString("cd");
        CHECK(os.str() == "abcd");
    }
}

// =====================================================================
// B. SinglyLinkedList / Node
// =====================================================================
static void testNodeOwnership() {
    TrackedStudent::aliveCount = 0;
    {
        SinglyNode<AdStudent> n(new TrackedStudent("N1", "A", "010-1"));
        CHECK_EQ(TrackedStudent::aliveCount, 1);
        CHECK(n.getNext() == nullptr);
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);           // 노드 소멸 시 데이터 해제

    {
        SinglyNode<AdStudent> n(new TrackedStudent("N2", "A", "010-1"));
        AdStudent* d = n.getData();
        n.setData(nullptr);                            // 소유권 분리
        CHECK_EQ(TrackedStudent::aliveCount, 1);
        delete d;
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);

    { SinglyNode<AdStudent> n; CHECK(n.getData() == nullptr); }   // 빈 노드 소멸 안전
}

static void testListInsertAndGet() {
    TrackedStudent::aliveCount = 0;
    {
        SinglyLinkedList<AdStudent> l;
        CHECK(l.isEmpty());
        CHECK_EQ(l.getSize(), 0);
        CHECK(l.getHead() == nullptr);

        l.add(new TrackedStudent("A", "a", "1"));
        l.add(new TrackedStudent("B", "b", "2"));      // add 는 head 삽입
        CHECK(l.get(0)->getID() == "B");
        CHECK(l.get(1)->getID() == "A");

        l.insert(0, new TrackedStudent("C", "c", "3"));     // 맨 앞
        l.insert(l.getSize(), new TrackedStudent("D", "d", "4"));  // 맨 뒤 (index == size)
        l.insert(2, new TrackedStudent("E", "e", "5"));     // 중간
        CHECK_EQ(l.getSize(), 5);
        const char* want[] = {"C", "B", "E", "A", "D"};
        for (int i = 0; i < 5; ++i) CHECK(l.get(i)->getID() == want[i]);

        // 잘못된 index 는 거부하고 소유권은 호출자에게 남음 (list 가 delete 하면 안 됨)
        TrackedStudent* rej1 = new TrackedStudent("X", "x", "9");
        TrackedStudent* rej2 = new TrackedStudent("Y", "y", "9");
        l.insert(-1, rej1);
        l.insert(l.getSize() + 1, rej2);
        CHECK_EQ(l.getSize(), 5);
        CHECK_EQ(TrackedStudent::aliveCount, 7);
        delete rej1;
        delete rej2;

        CHECK(l.get(-1) == nullptr);
        CHECK(l.get(5) == nullptr);
        CHECK(l.get(INT_MAX) == nullptr);
        CHECK(l.get(INT_MIN) == nullptr);
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);
}

static void testListRemoveAndFind() {
    TrackedStudent::aliveCount = 0;
    {
        SinglyLinkedList<AdStudent> l;
        CHECK(l.find(MyString("A")) == nullptr);           // 빈 목록
        CHECK(l.remove(MyString("A")) == nullptr);
        CHECK(l.removeFirst() == nullptr);

        l.insertAscend(new TrackedStudent("A", "Amy", "010-1"));
        l.insertAscend(new TrackedStudent("B", "Bob", "010-2"));
        l.insertAscend(new TrackedStudent("C", "Cat", "010-3"));
        l.insertAscend(new TrackedStudent("D", "Dan", "010-4"));
        CHECK_EQ(l.getSize(), 4);

        CHECK(l.find(MyString("B")) != nullptr);
        CHECK(l.find(MyString("010-3")) != nullptr);       // 전화번호로도 조회
        CHECK(l.find(MyString("Bob")) == nullptr);         // 이름은 키가 아님
        CHECK(l.find(MyString("010")) == nullptr);         // 부분 일치 불가
        CHECK(l.find(MyString("b")) == nullptr);           // 대소문자 구분
        CHECK(l.find(MyString("")) == nullptr);

        AdStudent* mid = l.remove(MyString("C"));          // 중간
        CHECK(mid && mid->getID() == "C");
        delete mid;
        AdStudent* tail = l.remove(MyString("010-4"));     // 꼬리 (전화번호로)
        CHECK(tail && tail->getID() == "D");
        delete tail;
        AdStudent* head = l.remove(MyString("A"));         // 머리
        CHECK(head && head->getID() == "A");
        delete head;
        CHECK_EQ(l.getSize(), 1);
        CHECK(l.remove(MyString("A")) == nullptr);         // 이미 삭제됨
        CHECK(l.remove(MyString("ZZZ")) == nullptr);
        CHECK_EQ(l.getSize(), 1);

        AdStudent* last = l.remove(MyString("B"));         // 마지막 하나
        CHECK(last != nullptr);
        delete last;
        CHECK(l.isEmpty());
        CHECK(l.getHead() == nullptr);

        // 비운 뒤 재사용
        l.insertAscend(new TrackedStudent("R", "Re", "010-9"));
        CHECK_EQ(l.getSize(), 1);

        // remove(int index)
        l.insertAscend(new TrackedStudent("S", "Se", "010-8"));
        l.insertAscend(new TrackedStudent("T", "Te", "010-7"));
        CHECK_EQ(l.getSize(), 3);                          // Re, Se, Te
        AdStudent* byIdx = l.remove(1);
        CHECK(byIdx && byIdx->getID() == "S");
        delete byIdx;
        byIdx = l.remove(1);                               // 이제 마지막
        CHECK(byIdx && byIdx->getID() == "T");
        delete byIdx;
        CHECK(l.remove(1) == nullptr);
        byIdx = l.remove(0);
        CHECK(byIdx && byIdx->getID() == "R");
        delete byIdx;
        CHECK(l.isEmpty());
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);
}

static void testListInsertAscendOrdering() {
    TrackedStudent::aliveCount = 0;
    {
        SinglyLinkedList<AdStudent> l;
        // 머리/중간/꼬리/동명이인(전화번호 순)/대소문자/접두사
        l.insertAscend(new TrackedStudent("1", "M", "010-5"));
        l.insertAscend(new TrackedStudent("2", "A", "010-1"));     // 새 head
        l.insertAscend(new TrackedStudent("3", "Z", "010-9"));     // 새 tail
        l.insertAscend(new TrackedStudent("4", "M", "010-3"));     // 동명이인, 전화 더 작음
        l.insertAscend(new TrackedStudent("5", "M", "010-7"));     // 동명이인, 전화 더 큼
        l.insertAscend(new TrackedStudent("6", "Ma", "010-0"));    // 접두사 M < Ma
        l.insertAscend(new TrackedStudent("7", "a", "010-0"));     // 소문자는 대문자 뒤
        const char* want[] = {"2", "4", "1", "5", "6", "3", "7"};
        CHECK_EQ(l.getSize(), 7);
        for (int i = 0; i < 7; ++i) CHECK_MSG(l.get(i)->getID() == want[i], string("order mismatch at index ") + to_string(i));

        // 이름+전화가 완전히 같아도 둘 다 보관되어야 함 (유실/덮어쓰기 금지)
        l.insertAscend(new TrackedStudent("8", "M", "010-5"));
        CHECK_EQ(l.getSize(), 8);
        CHECK(l.find(MyString("1")) != nullptr);
        CHECK(l.find(MyString("8")) != nullptr);
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);
}

static void testListMoveFromAndClear() {
    TrackedStudent::aliveCount = 0;
    {
        SinglyLinkedList<AdStudent> a, b;
        a.insertAscend(new TrackedStudent("A1", "A", "1"));
        a.insertAscend(new TrackedStudent("A2", "B", "2"));
        b.moveFrom(a);
        CHECK(a.isEmpty());
        CHECK(a.getHead() == nullptr);
        CHECK_EQ(b.getSize(), 2);
        CHECK_EQ(TrackedStudent::aliveCount, 2);
        a.clear();                                      // 비어있는 원본 clear
        b.clear();
        b.clear();                                      // 중복 clear
        CHECK_EQ(TrackedStudent::aliveCount, 0);
        SinglyLinkedList<AdStudent> empty;
        empty.clear();
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);

}

static void testListLargeAndModelBased() {
    // (1) 대량 add 후 소멸: clear 가 재귀가 아니어야 스택이 안 터짐
    TrackedStudent::aliveCount = 0;
    {
        SinglyLinkedList<AdStudent> l;
        for (int i = 0; i < 100000; ++i) l.add(new TrackedStudent("L", "n", "p"));
        CHECK_EQ(l.getSize(), 100000);
        CHECK_EQ(TrackedStudent::aliveCount, 100000);
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);

    // (2) 모델 기반 랜덤 테스트: insertAscend/remove/find 를 set 과 비교
    {
        unsigned seed = 20241125u;
        auto rnd = [&](unsigned m) { seed = seed * 1103515245u + 12345u; return (seed >> 16) % m; };

        SinglyLinkedList<AdStudent> list;
        set<string> ids;
        int nextId = 0;
        bool ok = true;
        string why;

        for (int step = 0; step < 4000 && ok; ++step) {
            unsigned op = rnd(10);
            if (op < 5) {
                string id = "R" + to_string(nextId++);
                string name = "N" + to_string(rnd(15));            // 동명이인 많이
                string phone = "010-" + to_string(1000 + rnd(50)); // 전화번호 중복도 많이
                list.insertAscend(new TrackedStudent(id.c_str(), name.c_str(), phone.c_str()));
                ids.insert(id);
            } else if (op < 8 && !ids.empty()) {
                auto it = ids.begin();
                advance(it, rnd((unsigned)ids.size()));
                AdStudent* r = list.remove(MyString(it->c_str()));
                if (r == nullptr) { ok = false; why = "remove(existing) returned null at step " + to_string(step); break; }
                delete r;
                ids.erase(it);
            } else if (op == 8) {
                if (list.remove(MyString("NOPE")) != nullptr) { ok = false; why = "remove(missing) non-null"; break; }
            } else if (!ids.empty()) {
                auto it = ids.begin();
                advance(it, rnd((unsigned)ids.size()));
                if (list.find(MyString(it->c_str())) == nullptr) { ok = false; why = "find(existing) null at step " + to_string(step); break; }
            }

            if (list.getSize() != (int)ids.size()) { ok = false; why = "size mismatch at step " + to_string(step); break; }
            if (TrackedStudent::aliveCount != list.getSize()) { ok = false; why = "alive/size mismatch at step " + to_string(step); break; }
            int cnt = 0;
            for (SinglyNode<AdStudent>* n = list.getHead(); n && ok; n = n->getNext(), ++cnt) {
                SinglyNode<AdStudent>* nx = n->getNext();
                if (nx && *(nx->getData()) < *(n->getData())) { ok = false; why = "not sorted at step " + to_string(step); }
            }
            if (ok && cnt != list.getSize()) { ok = false; why = "node count != size at step " + to_string(step); }
        }
        CHECK_MSG(ok, why);
    }
    CHECK_EQ(TrackedStudent::aliveCount, 0);
}

// =====================================================================
// C. AdStudent 계열
// =====================================================================
static void testStudentOperators() {
    LifeLongStudent a("L1", "Kim", "Yoga", 100, "010-0000-0001");
    LifeLongStudent b("L2", "Kim", "Yoga", 100, "010-0000-0002");
    LifeLongStudent c("L3", "Lee", "Yoga", 100, "010-0000-0000");

    CHECK(a < b);                       // 동명이인: 전화번호 순
    CHECK(!(b < a));
    CHECK(a <= b);
    CHECK(!(b <= a));
    CHECK(a <= a);
    CHECK(!(a < a));
    CHECK(a < c);                       // 이름이 우선 (전화번호는 c가 더 작아도)
    CHECK(!(c < a));
    CHECK(a <= c);

    CHECK(a == MyString("L1"));         // 학번으로
    CHECK(a == MyString("010-0000-0001"));   // 전화번호로
    CHECK(!(a == MyString("Kim")));     // 이름은 키 아님
    CHECK(!(a == MyString("l1")));      // 대소문자 구분
    CHECK(!(a == MyString("L")));
    CHECK(!(a == MyString("L10")));
    CHECK(!(a == MyString("")));

    CHECK_STR(a.getName(), "Kim");
    CHECK_STR(a.getCourse(), "Yoga");
    CHECK_STR(a.getPhoneNumber(), "010-0000-0001");
    CHECK_STR(a.getID(), "L1");

    // 한글 이름끼리 정렬 (UTF-8 바이트 순 = 가나다 순)
    LifeLongStudent ga("A", KO_GA, "c", 0, "1"), na("B", KO_NA, "c", 0, "1"), da("C", KO_DA, "c", 0, "1");
    CHECK(ga < na);
    CHECK(na < da);
    CHECK(ga < da);
    CHECK(!(da < ga));

    // 한글 vs ASCII: 표준(unsigned 바이트) 순서라면 ASCII 가 먼저
    WARN(!(MyString(KO_GA) < MyString("Adam")),
         "UTF-8 한글이 ASCII 보다 먼저 정렬됨: compareTo 가 signed char 로 비교함 (unsigned char 로 비교 권장)");
}

static void testStudentFormats() {
    GiftedStudent g("230602", "Cathy", "SangjuMid", "Info", "010-9999-8888", "Jeny", "010-9898-8888");
    CHECK_STR(g.toString(), "[Gifted] Cathy (230602) SangjuMid Info 010-9999-8888 Jeny (010-9898-8888)");
    CHECK_STR(g.toFileFormatString(), "Gifted 230602 Cathy SangjuMid Info 010-9999-8888 Jeny 010-9898-8888");

    LifeLongStudent l("Y1", "Dan", "Yoga", 160000, "010-1");
    CHECK_STR(l.toString(), "[LifeLong] Dan (Y1) Yoga 160000 010-1");
    CHECK_STR(l.toFileFormatString(), "LifeLong Y1 Dan Yoga 160000 010-1");

    LifeLongStudent neg("Y2", "Neg", "Yoga", -500, "010-2");
    CHECK_STR(neg.toString(), "[LifeLong] Neg (Y2) Yoga -500 010-2");
    LifeLongStudent mx("Y3", "Max", "Yoga", INT_MAX, "010-3");
    CHECK_STR(mx.toFileFormatString(), "LifeLong Y3 Max Yoga 2147483647 010-3");
    LifeLongStudent mn("Y4", "Min", "Yoga", INT_MIN, "010-4");
    CHECK_STR(mn.toFileFormatString(), "LifeLong Y4 Min Yoga -2147483648 010-4");

    EmployEduStudent e("E00001", "Ben", "Hexagon", "Staff", "SuperCom", "010-5");
    CHECK_STR(e.toString(), "[EmployeeEdu] Ben (E00001) Hexagon Staff SuperCom 010-5");
    CHECK_STR(e.toFileFormatString(), "EmployeeEdu E00001 Ben Hexagon Staff SuperCom 010-5");

    ShortTermEduStudent s("S00001", "Adam", "SuperCom", "CS", "010-6");
    CHECK_STR(s.toString(), "[ShortTermEdu] Adam (S00001) SuperCom CS 010-6");
    CHECK_STR(s.toFileFormatString(), "ShortTermEdu S00001 Adam SuperCom CS 010-6");

    // 기본 생성자 객체도 출력 시 크래시 없이 동작
    GiftedStudent g0; LifeLongStudent l0; EmployEduStudent e0; ShortTermEduStudent s0;
    CHECK(g0.toString().length() > 0);
    CHECK(l0.toString().length() > 0);
    CHECK(e0.toString().length() > 0);
    CHECK(s0.toString().length() > 0);
    CHECK_STR(l0.getName(), "");
    CHECK_STR(l0.toFileFormatString(), "LifeLong    0 ");    // 빈 필드 + fee 기본값 0

    // 한글 이름 보존
    LifeLongStudent k("K1", KO_GA, "Yoga", 1, "010-7");
    CHECK(has(k.toString().getStr(), KO_GA));

    // 기본 클래스 포인터로 delete 시 파생 소멸자 실행 (가상 소멸자)
    TrackedStudent::aliveCount = 0;
    AdStudent* p = new TrackedStudent("V", "v", "0");
    CHECK_EQ(TrackedStudent::aliveCount, 1);
    delete p;
    CHECK_EQ(TrackedStudent::aliveCount, 0);
}

// =====================================================================
// D. FileIO
// =====================================================================
static void testFileLoadSaveRoundTrip() {
    TempFile f("tmp_mt_rt.txt",
        "ShortTermEdu S00004 Adam SuperCom CS 010-3738-5465\n"
        "EmployeeEdu E00002 Alex ZigTop Director CMS 010-7895-6587\n"
        "Gifted 230602 Cathy SangjuMid Info 010-9999-8888 Jeny 010-9898-8888\n"
        "LifeLong Y240134 Daniel Yoga 160000 010-9876-4545\n");
    SinglyLinkedList<AdStudent> l;
    CHECK(loadList(f.name, l));
    CHECK_EQ(l.getSize(), 4);
    CHECK(l.find(MyString("S00004")) != nullptr);
    CHECK(l.find(MyString("010-7895-6587")) != nullptr);

    TempFile out("tmp_mt_rt_out.txt");
    CHECK(FileIO::saveFile(out.name.c_str(), l));
    CHECK(readAll(out.name) ==
        "ShortTermEdu S00004 Adam SuperCom CS 010-3738-5465\n"
        "EmployeeEdu E00002 Alex ZigTop Director CMS 010-7895-6587\n"
        "Gifted 230602 Cathy SangjuMid Info 010-9999-8888 Jeny 010-9898-8888\n"
        "LifeLong Y240134 Daniel Yoga 160000 010-9876-4545\n");

    // 멱등성: 저장 -> 로드 -> 저장 결과가 동일
    SinglyLinkedList<AdStudent> l2;
    CHECK(loadList(out.name, l2));
    TempFile out2("tmp_mt_rt_out2.txt");
    CHECK(FileIO::saveFile(out2.name.c_str(), l2));
    CHECK(readAll(out.name) == readAll(out2.name));
}

static void testFileSortsOnSave() {
    TempFile f("tmp_mt_sortsave.txt",
        "LifeLong L3 Zed Yoga 1 010-3\n"
        "LifeLong L2 amy Yoga 1 010-2\n"
        "LifeLong L1 Bob Yoga 1 010-1\n"
        "LifeLong L4 Amy Yoga 1 010-4\n");
    CHECK(orderOf(f.name) == "L4,L1,L3,L2");     // Amy < Bob < Zed < amy (ASCII)
    SinglyLinkedList<AdStudent> l;
    loadList(f.name, l);
    TempFile out("tmp_mt_sortsave_out.txt");
    CHECK(FileIO::saveFile(out.name.c_str(), l));
    CHECK(orderOf(out.name) == "L4,L1,L3,L2");
}

static void testFileEdgeFormats() {
    // 마지막 줄에 개행 없음
    {
        TempFile f("tmp_mt_nonl.txt", "LifeLong L1 A Yoga 1 010-1\nLifeLong L2 B Yoga 2 010-2");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK_EQ(l.getSize(), 2);
        CHECK(l.find(MyString("010-2")) != nullptr);
    }
    // 탭/다중 공백 구분
    {
        TempFile f("tmp_mt_tab.txt", "LifeLong\tL1\t\tA   Yoga\t5\t010-1\n");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK_EQ(l.getSize(), 1);
        CHECK_STR(l.get(0)->getCourse(), "Yoga");
    }
    // CRLF: 마지막 필드에 '\r' 이 붙으면 안 됨 (전화번호 조회가 깨짐)
    {
        TempFile f("tmp_mt_crlf.txt",
            "LifeLong L1 A Yoga 1 010-1111-1111\r\n"
            "Gifted G1 B Mid Math 010-2222-2222 Mom 010-3333-3333\r\n");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK_EQ(l.getSize(), 2);
        CHECK_STR(l.get(0)->getPhoneNumber(), "010-1111-1111");
        CHECK(l.find(MyString("010-1111-1111")) != nullptr);
        CHECK(l.find(MyString("G1")) != nullptr);
        TempFile out("tmp_mt_crlf_out.txt");
        CHECK(FileIO::saveFile(out.name.c_str(), l));
        CHECK(!has(readAll(out.name), "\r"));       // 저장 시에는 '\r' 없음
    }
    // 빈 파일 / 공백만 있는 파일
    {
        TempFile f("tmp_mt_empty.txt", "");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK(l.isEmpty());
        TempFile g("tmp_mt_ws.txt", "\n\n   \r\n\t\n");
        SinglyLinkedList<AdStudent> l2;
        CHECK(loadList(g.name, l2));
        CHECK(l2.isEmpty());
    }
    // 알 수 없는 타입/소문자 타입 줄은 건너뛰고 나머지는 정상 로드
    {
        TempFile f("tmp_mt_unknown.txt",
            "Alien X1 Foo Bar\n"
            "gifted G9 Low Mid Math 010-9 Mom 010-8\n"
            "LifeLong L1 A Yoga 1 010-1\n");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK(l.find(MyString("L1")) != nullptr);
        CHECK_EQ(l.getSize(), 1);                           // 알 수 없는 줄은 건너뜀
    }
    // 같은 학번이 파일에 두 번: 로더는 둘 다 읽고, 삭제는 하나씩만
    {
        TempFile f("tmp_mt_dupid.txt",
            "LifeLong L1 A Yoga 1 010-1\n"
            "LifeLong L1 B Yoga 2 010-2\n");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK_EQ(l.getSize(), 2);
        AdStudent* r = l.remove(MyString("L1"));
        CHECK(r != nullptr);
        delete r;
        CHECK_EQ(l.getSize(), 1);
    }
    // 매우 긴 필드 왕복
    {
        string longName(5000, 'n');
        TempFile f("tmp_mt_long.txt", "LifeLong L1 " + longName + " Yoga 1 010-1\n");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK_EQ(l.get(0)->getName().length(), 5000);
        TempFile out("tmp_mt_long_out.txt");
        CHECK(FileIO::saveFile(out.name.c_str(), l));
        CHECK(readAll(out.name) == "LifeLong L1 " + longName + " Yoga 1 010-1\n");
    }
    // 한글 이름 왕복 + 가나다 정렬
    {
        TempFile f("tmp_mt_ko.txt",
            string("LifeLong L3 ") + KO_DA + " Yoga 1 010-3\n"
            + "LifeLong L1 " + KO_GA + " Yoga 1 010-1\n"
            + "LifeLong L2 " + KO_NA + " Yoga 1 010-2\n");
        CHECK(orderOf(f.name) == "L1,L2,L3");
        SinglyLinkedList<AdStudent> l;
        loadList(f.name, l);
        TempFile out("tmp_mt_ko_out.txt");
        CHECK(FileIO::saveFile(out.name.c_str(), l));
        CHECK(has(readAll(out.name), string("LifeLong L1 ") + KO_GA + " Yoga"));
    }
}

static void testFileFailureModes() {
    SinglyLinkedList<AdStudent> l;
    CHECK(!loadList("tmp_mt_definitely_missing_file.txt", l));
    CHECK(l.isEmpty());

    l.insertAscend(new LifeLongStudent("L1", "A", "Yoga", 1, "010-1"));
    CHECK(!FileIO::saveFile("tmp_mt_no_such_dir_xyz/out.txt", l));      // 쓸 수 없는 경로

    // 빈 목록 저장 -> 0바이트 파일, 기존 내용은 덮어써짐
    TempFile f("tmp_mt_overwrite.txt", "LifeLong L9 Old Yoga 1 010-9\nLifeLong L8 Old2 Yoga 1 010-8\n");
    SinglyLinkedList<AdStudent> empty;
    CHECK(FileIO::saveFile(f.name.c_str(), empty));
    CHECK_EQ(readAll(f.name).size(), 0);

    // 짧은 내용으로 덮어쓰기 시 이전 내용이 남지 않음
    CHECK(FileIO::saveFile(f.name.c_str(), l));
    CHECK(readAll(f.name) == "LifeLong L1 A Yoga 1 010-1\n");
}

static void testFileRobustnessWarnings() {
    // UTF-8 BOM (윈도우 메모장 저장 시 흔함): 첫 레코드가 조용히 사라지면 안 됨
    {
        TempFile f("tmp_mt_bom.txt",
            string("\xEF\xBB\xBF") + "LifeLong L1 A Yoga 1 010-1\nLifeLong L2 B Yoga 2 010-2\n");
        SinglyLinkedList<AdStudent> l;
        loadList(f.name, l);
        WARN(l.getSize() == 2, "UTF-8 BOM 이 있는 파일의 첫 레코드가 로드되지 않음 (BOM 3바이트 제거 필요)");
    }
    // 마지막 레코드가 잘린 경우: 쓰레기 값의 반쪽짜리 학생이 만들어지면 안 됨
    {
        TempFile f("tmp_mt_trunc.txt", "LifeLong L1 A Yoga 1 010-1\nLifeLong L2 Kim");
        SinglyLinkedList<AdStudent> l;
        loadList(f.name, l);
        WARN(l.getSize() == 1,
             "필드가 모자란 레코드(LifeLong L2 Kim)가 부분 객체로 로드됨. 또한 int fee 가 초기화되지 않은 채 사용됨 (int fee = 0; + 스트림 상태 검사)");
    }
    // 수강료 int 오버플로: 이후 레코드까지 전부 유실되면 안 됨
    {
        TempFile f("tmp_mt_feeovf.txt",
            "LifeLong L1 A Yoga 99999999999 010-1\nLifeLong L2 B Yoga 2 010-2\n");
        SinglyLinkedList<AdStudent> l;
        loadList(f.name, l);
        WARN(l.find(MyString("L2")) != nullptr, "수강료 파싱 실패(failbit) 이후 나머지 레코드가 전부 무시됨");
    }
}

// =====================================================================
// E. App 시나리오 (cin/cout 스크립트)
// =====================================================================

// E-1. 기본 시나리오 (메뉴 1~6, 출력 양식, 자동 학번, 중복 학번, 수정, 삭제, 저장)
//      !! static nextID 때문에 반드시 가장 먼저 실행 !!
static void testBasicScenario() {
    TempFile f("tmp_mt_basic.txt",
        "ShortTermEdu S00004 Adam SuperCom CS 010-3738-5465\n"
        "EmployeeEdu E00002 Alex ZigTop Director CMS 010-7895-6587\n"
        "EmployeeEdu E00001 Ben Hexagon Staff SuperCom 010-1234-5678\n"
        "Gifted 230602 Cathy SangjuMid Info 010-9999-8888 Jeny 010-9898-8888\n"
        "LifeLong Y240134 Daniel Yoga 160000 010-9876-4545\n");

    string out = runApp(f.name,
        "2\nSuperCom\n"
        "3\nEmployeeEdu Chris Naver Dev AI 010-1111-2222\n"
        "3\nShortTermEdu Eve Python SW 010-2222-3333\n"
        "3\nGifted 230602 Dup Mid Info 010-0000-0000 Mom 010-0000-0001\n"
        "4\n999999\n"
        "4\n010-9876-4545\nPilates 200000 010-9876-9999\n"
        "5\nE00001\nN\n"
        "5\nE00001\nY\n"
        "1\n"
        "6\nW\n");

    CHECK_HAS(out, "*********************************************");
    CHECK_HAS(out, "--------- AdStudent List ---------");
    CHECK_HAS(out, "[ShortTermEdu] Adam (S00004) SuperCom CS 010-3738-5465");
    CHECK_HAS(out, "[EmployeeEdu] Alex (E00002) ZigTop Director CMS 010-7895-6587");
    CHECK_HAS(out, "[Gifted] Cathy (230602) SangjuMid Info 010-9999-8888 Jeny (010-9898-8888)");
    CHECK_HAS(out, "[LifeLong] Daniel (Y240134) Yoga 160000 010-9876-4545");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] Chris (E00003) Naver Dev AI 010-1111-2222");
    CHECK_HAS(out, "<INSERT> [ShortTermEdu] Eve (S00005) Python SW 010-2222-3333");
    CHECK_HAS(out, "<ERROR> The same ID already exists.");
    CHECK_HAS(out, "<ERROR> ID or Phone Number is invalid");
    CHECK_HAS(out, "<UPDATE> [LifeLong] Daniel (Y240134) Pilates 200000 010-9876-9999");
    CHECK_HAS(out, "<DELETE> [EmployeeEdu] Ben (E00001) Hexagon Staff SuperCom 010-1234-5678");
    CHECK_HAS(out, "Save changes and Exit");
    CHECK_EQ(countOf(out, "<DELETE>"), 1);                 // 'N' 은 취소

    SinglyLinkedList<AdStudent> re;
    CHECK(loadList(f.name, re));
    CHECK_EQ(re.getSize(), 6);
    CHECK(re.find(MyString("E00001")) == nullptr);
    CHECK(re.find(MyString("E00003")) != nullptr);
    CHECK(re.find(MyString("S00005")) != nullptr);
}

// E-2. 자동 학번 경계 (static 카운터는 줄어들지 않으므로 값이 커지는 순서로 진행)
static void testAutoID() {
    TempFile f("tmp_mt_autoid.txt");

    // 1) 건너뛴 번호 뒤에서 이어짐
    writeAll(f.name, "EmployeeEdu E00010 Skip Co Dev AI 010-1111-1111\n");
    string out = runApp(f.name, "3\nEmployeeEdu Next Co Dev AI 010-2222-2222\n6\nC\n");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] Next (E00011)");

    // 2) E / S 카운터는 서로 독립, 연속 생성 시 1씩 증가, 5자리 0패딩
    writeAll(f.name,
        "EmployeeEdu E00020 EOld Co Dev AI 010-3000-0001\n"
        "ShortTermEdu S00500 SOld Py SW 010-3000-0002\n");
    out = runApp(f.name,
        "3\nEmployeeEdu E1 Co Dev AI 010-3000-0003\n"
        "3\nShortTermEdu S1 Py SW 010-3000-0004\n"
        "3\nEmployeeEdu E2 Co Dev AI 010-3000-0005\n"
        "3\nShortTermEdu S2 Py SW 010-3000-0006\n"
        "6\nC\n");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] E1 (E00021)");
    CHECK_HAS(out, "<INSERT> [ShortTermEdu] S1 (S00501)");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] E2 (E00022)");
    CHECK_HAS(out, "<INSERT> [ShortTermEdu] S2 (S00502)");

    // 3) 5자리 초과 (E99999 -> E100000), 삭제 후에도 번호 재사용 없음
    writeAll(f.name, "EmployeeEdu E99999 Max Co Dev AI 010-4444-0001\n");
    out = runApp(f.name,
        "3\nEmployeeEdu Over Co Dev AI 010-4444-0002\n"
        "5\nE100000\nY\n"
        "3\nEmployeeEdu AfterDel Co Dev AI 010-4444-0003\n"
        "6\nC\n");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] Over (E100000)");
    CHECK_HAS(out, "<DELETE> [EmployeeEdu] Over (E100000)");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] AfterDel (E100001)");

    // 4) 낮은 번호를 로드해도 카운터가 뒤로 가지 않음 (중복 학번 방지)
    writeAll(f.name, "EmployeeEdu E00001 Low Co Dev AI 010-5555-0001\n");
    out = runApp(f.name, "3\nEmployeeEdu Fresh Co Dev AI 010-5555-0002\n6\nC\n");
    CHECK_HAS(out, "<INSERT> [EmployeeEdu] Fresh (E100002)");

    // 5) 자동 학번이 파일 내 기존 학번과 절대 충돌하지 않음
    writeAll(f.name, "ShortTermEdu S00700 Hi Py SW 010-6666-0001\n");
    out = runApp(f.name,
        "3\nShortTermEdu A1 Py SW 010-6666-0002\n"
        "3\nShortTermEdu A2 Py SW 010-6666-0003\n"
        "3\nShortTermEdu A3 Py SW 010-6666-0004\n6\nC\n");
    CHECK_EQ(countOf(out, "<ERROR>"), 0);
    CHECK_HAS(out, "(S00701)");
    CHECK_HAS(out, "(S00703)");
}

// E-3. 메뉴 루프: 잘못된 숫자, 빈 목록, 조회 순서
static void testMenuLoopAndViews() {
    // 빈 목록
    {
        TempFile f("tmp_mt_menu_empty.txt");
        string out = runApp(f.name, "1\n2\nYoga\n7\n0\n-1\n100\n4\nL1\n5\nL1\n6\nC\n");
        CHECK_EQ(countOf(out, "[1] View all"), 9);        // 메뉴 명령 9개(1,2,7,0,-1,100,4,5,6) = 프롬프트 9번
        CHECK_NOT_HAS(out, "[LifeLong]");
        CHECK_EQ(countOf(out, "<ERROR> ID or Phone Number is invalid"), 2);   // 수정/삭제
        CHECK_HAS(out, "Exit without Saving");
    }
    // 정렬되지 않은 파일은 로드 시 정렬되어 출력
    {
        TempFile f("tmp_mt_menu_sorted.txt",
            "LifeLong L1 Zed Yoga 1 010-1\n"
            "LifeLong L2 amy Yoga 1 010-2\n"
            "LifeLong L3 Bob Yoga 1 010-3\n"
            "LifeLong L4 Amy Yoga 1 010-4\n");
        string out = runApp(f.name, "6\nC\n");
        size_t pAmy = out.find("Amy (L4)"), pBob = out.find("Bob (L3)"),
               pZed = out.find("Zed (L1)"), pamy = out.find("amy (L2)");
        CHECK(pAmy < pBob && pBob < pZed && pZed < pamy);
    }
    // 메뉴 1 은 몇 번이고 반복 출력
    {
        TempFile f("tmp_mt_menu_repeat.txt", "LifeLong L1 A Yoga 1 010-1\n");
        string out = runApp(f.name, "1\n1\n1\n6\nC\n");
        CHECK_EQ(countOf(out, "[LifeLong] A (L1)"), 4);   // 시작 1 + 메뉴 3
    }
    // EOF 로 끝나면 종료 메시지/저장 없이 조용히 종료, 파일은 그대로
    {
        string content = "LifeLong L1 A Yoga 1 010-1\n";
        TempFile f("tmp_mt_menu_eof.txt", content);
        string out = runApp(f.name, "5\nL1\nY\n1\n");      // 삭제는 했지만 저장 없이 EOF
        CHECK_NOT_HAS(out, "Save changes");
        CHECK_NOT_HAS(out, "Exit without Saving");
        CHECK(readAll(f.name) == content);
    }
    // 숫자가 아닌 입력 / int 오버플로 입력 -> 무한 루프 없이 종료
    {
        TempFile f("tmp_mt_menu_bad.txt", "LifeLong L1 A Yoga 1 010-1\n");
        runApp(f.name, "abc\n");
        runApp(f.name, "99999999999999999999\n");
        runApp(f.name, "1abc\n6\nC\n");
        runApp(f.name, "\n\n\n");
        CHECK(true);   // 여기까지 도달 = 행(hang) 없음
    }
    // 존재하지 않는 파일
    {
        string out = runApp("tmp_mt_missing_999.txt", "1\n6\nW\n");
        CHECK_HAS(out, "Error loading file.");
        CHECK_NOT_HAS(out, "[1] View all");
    }
}

// E-4. 검색(과정)
static void testSearch() {
    TempFile f("tmp_mt_search.txt",
        "LifeLong L1 Alice Yoga 100 010-1\n"
        "EmployeeEdu E00001 Bob Co Dev Yoga 010-2\n"
        "Gifted G1 Cara Mid Yoga 010-3 Mom 010-4\n"
        "ShortTermEdu S00001 Dan Yoga Dept 010-5\n"
        "LifeLong L2 Eve Swim 100 010-6\n");
    auto search = [&](const string& course) {
        string out = runApp(f.name, "2\n" + course + "\n6\nC\n");
        return afterFirst(out, "Input Course : ");
    };

    string yoga = search("Yoga");                       // 4개 타입 모두, 이름 순
    CHECK_HAS(yoga, "[LifeLong] Alice (L1)");
    CHECK_HAS(yoga, "[EmployeeEdu] Bob (E00001)");
    CHECK_HAS(yoga, "[Gifted] Cara (G1)");
    CHECK_HAS(yoga, "[ShortTermEdu] Dan (S00001)");
    CHECK_NOT_HAS(yoga, "Eve");
    CHECK(yoga.find("Alice") < yoga.find("Bob") && yoga.find("Bob") < yoga.find("Cara") && yoga.find("Cara") < yoga.find("Dan"));

    string swim = search("Swim");
    CHECK_HAS(swim, "[LifeLong] Eve (L2)");
    CHECK_NOT_HAS(swim, "Alice");

    // 결과가 없어야 하는 검색들
    CHECK_EQ(search("yoga").size(), 0);                 // 대소문자 구분
    CHECK_EQ(search("Yog").size(), 0);                  // 접두사 불가
    CHECK_EQ(search("Yogaa").size(), 0);                // 더 긴 문자열 불가
    CHECK_EQ(search("Dept").size(), 0);                 // 과정이 아닌 다른 필드 값
    CHECK_EQ(search("Alice").size(), 0);                // 이름
    CHECK_EQ(search("NoSuchCourse").size(), 0);

    // 검색은 조회 전용: 파일/목록이 변하지 않음
    runApp(f.name, "2\nYoga\n2\nYoga\n6\nW\n");
    SinglyLinkedList<AdStudent> l;
    CHECK(loadList(f.name, l));
    CHECK_EQ(l.getSize(), 5);
}

// E-5. 수정
static void testModify() {
    TempFile f("tmp_mt_modify.txt",
        "Gifted G1 Amy Mid Math 010-1000-0001 Mom 010-2000-0001\n"
        "EmployeeEdu E00001 Bob Co Dev AI 010-1000-0002\n"
        "ShortTermEdu S00001 Cara Py SW 010-1000-0003\n"
        "LifeLong L1 Dan Yoga 100 010-1000-0004\n");
    string out = runApp(f.name,
        "4\nG1\nNewMid NewCourse 010-9999-0001 NewMom 010-8888-0001\n"
        "4\n010-1000-0002\nNewCo Boss ML 010-9999-0002\n"
        "4\nS00001\nJava HR 010-9999-0003\n"
        "4\nL1\nSwim 0 010-9999-0004\n"
        "4\nNOPE\n"
        "4\nAmy\n"                  // 이름으로는 수정 불가
        "4\n010-1000-0001\n"       // 이미 바뀐 옛 전화번호
        "4\n010\n"                 // 부분 번호
        "6\nW\n");
    CHECK_HAS(out, "Current: [Gifted] Amy (G1) Mid Math 010-1000-0001 Mom (010-2000-0001)");
    CHECK_HAS(out, "<UPDATE> [Gifted] Amy (G1) NewMid NewCourse 010-9999-0001 NewMom (010-8888-0001)");
    CHECK_HAS(out, "<UPDATE> [EmployeeEdu] Bob (E00001) NewCo Boss ML 010-9999-0002");
    CHECK_HAS(out, "<UPDATE> [ShortTermEdu] Cara (S00001) Java HR 010-9999-0003");
    CHECK_HAS(out, "<UPDATE> [LifeLong] Dan (L1) Swim 0 010-9999-0004");
    CHECK_EQ(countOf(out, "<ERROR> ID or Phone Number is invalid"), 4);
    CHECK_EQ(countOf(out, "<UPDATE>"), 4);
    CHECK(readAll(f.name) ==
        "Gifted G1 Amy NewMid NewCourse 010-9999-0001 NewMom 010-8888-0001\n"
        "EmployeeEdu E00001 Bob NewCo Boss ML 010-9999-0002\n"
        "ShortTermEdu S00001 Cara Java HR 010-9999-0003\n"
        "LifeLong L1 Dan Swim 0 010-9999-0004\n");     // 학번/이름 불변, 개수 유지(누수·유실 없음)
}

static void testModifyReorderAndDuplicatePhone() {
    // 동명이인: 전화번호를 바꾸면 순서가 재정렬되어야 함
    {
        TempFile f("tmp_mt_mod_reorder.txt",
            "LifeLong L1 Kim Yoga 1 010-1111-1111\n"
            "LifeLong L2 Kim Yoga 1 010-2222-2222\n"
            "LifeLong L3 Kim Yoga 1 010-3333-3333\n");
        CHECK(orderOf(f.name) == "L1,L2,L3");
        runApp(f.name, "4\nL1\nYoga 1 010-9999-9999\n6\nW\n");        // 맨 앞 -> 맨 뒤
        CHECK(orderOf(f.name) == "L2,L3,L1");
        runApp(f.name, "4\nL1\nYoga 1 010-0000-0000\n6\nW\n");        // 맨 뒤 -> 맨 앞
        CHECK(orderOf(f.name) == "L1,L2,L3");
        runApp(f.name, "4\nL2\nYoga 1 010-2222-2222\n6\nW\n");        // 값 그대로 수정 (제자리)
        CHECK(orderOf(f.name) == "L1,L2,L3");
    }
    // 전화번호가 같은 두 명: 수정/삭제는 정확히 한 명에게만
    {
        TempFile f("tmp_mt_dupphone.txt",
            "LifeLong L1 Alice Yoga 100 010-7777-7777\n"
            "LifeLong L2 Bob Swim 200 010-7777-7777\n");
        runApp(f.name, "4\n010-7777-7777\nArt 5 010-1234-1234\n6\nW\n");
        string s = readAll(f.name);
        CHECK_EQ(countOf(s, "\n"), 2);                               // 둘 다 보존
        CHECK(has(s, "Alice Art 5 010-1234-1234") ^ has(s, "Bob Art 5 010-1234-1234"));   // 정확히 한 명만 수정
        runApp(f.name, "5\n010-7777-7777\nY\n6\nW\n");
        CHECK_EQ(countOf(readAll(f.name), "\n"), 1);
    }
    // 수정 후 다시 조회 가능 (새 전화번호/같은 학번)
    {
        TempFile f("tmp_mt_mod_find.txt", "LifeLong L1 A Yoga 1 010-1\n");
        string out = runApp(f.name, "4\nL1\nSwim 2 010-2\n4\n010-2\nArt 3 010-3\n4\n010-1\n6\nC\n");
        CHECK_EQ(countOf(out, "<UPDATE>"), 2);
        CHECK_EQ(countOf(out, "<ERROR> ID or Phone Number is invalid"), 1);
    }
}

// E-6. 삭제
static void testDelete() {
    TempFile f("tmp_mt_delete.txt",
        "LifeLong L1 A Yoga 1 010-1\n"
        "LifeLong L2 B Yoga 1 010-2\n"
        "LifeLong L3 C Yoga 1 010-3\n"
        "LifeLong L4 D Yoga 1 010-4\n");
    string out = runApp(f.name,
        "5\nL1\nY\n"            // 머리
        "5\nL4\ny\n"            // 꼬리, 소문자 y
        "5\n010-3\nN\n"         // 취소 (대문자)
        "5\nL2\nn\n"            // 취소 (소문자)
        "5\nL2\nX\n"            // 이상한 확인 키 -> 취소
        "5\nL1\n"               // 이미 삭제된 학번 (확인 질문 없이 에러)
        "6\nW\n");
    CHECK_EQ(countOf(out, "<DELETE>"), 2);
    CHECK_EQ(countOf(out, "<ERROR> ID or Phone Number is invalid"), 1);
    CHECK_EQ(countOf(out, "will be deleted. OK? [Y/N]"), 5);
    CHECK(orderOf(f.name) == "L2,L3");

    // 전화번호로 중간 삭제, 이후 전부 삭제 -> 빈 목록 -> 재삽입
    out = runApp(f.name,
        "5\n010-3\nY\n"
        "5\nL2\nY\n"
        "1\n"
        "3\nLifeLong L9 Z Yoga 9 010-9\n"
        "6\nW\n");
    CHECK_EQ(countOf(out, "<DELETE>"), 2);
    CHECK(orderOf(f.name) == "L9");

    // 삭제한 학번을 다시 삽입할 수 있어야 함
    out = runApp(f.name, "5\nL9\nY\n3\nLifeLong L9 Z2 Yoga 9 010-9\n6\nW\n");
    CHECK_HAS(out, "<INSERT> [LifeLong] Z2 (L9)");
    CHECK_NOT_HAS(out, "<ERROR>");
}

// E-7. 삽입
static void testInsert() {
    TempFile f("tmp_mt_insert.txt", "LifeLong L0 Mmm Yoga 1 010-0\n");
    string out = runApp(f.name,
        "3\nGifted G1 Aaa Mid Math 010-1 Mom 010-2\n"           // 맨 앞
        "3\nLifeLong L1 Zzz Yoga 0 010-3\n"                      // 맨 뒤, 수강료 0
        "3\nLifeLong L2 Neg Yoga -500 010-4\n"                   // 음수 수강료
        "3\nLifeLong L3 Big Yoga 2147483647 010-5\n"             // INT_MAX
        "3\nGifted L0 Dup Mid Math 010-6 Mom 010-7\n"            // 다른 타입이라도 학번 중복 -> 거부
        "3\nLifeLong G1 Dup2 Yoga 1 010-8\n"                     // 마찬가지
        "6\nW\n");
    CHECK_HAS(out, "<INSERT> [Gifted] Aaa (G1) Mid Math 010-1 Mom (010-2)");
    CHECK_HAS(out, "<INSERT> [LifeLong] Zzz (L1) Yoga 0 010-3");
    CHECK_HAS(out, "<INSERT> [LifeLong] Neg (L2) Yoga -500 010-4");
    CHECK_HAS(out, "<INSERT> [LifeLong] Big (L3) Yoga 2147483647 010-5");
    CHECK_EQ(countOf(out, "<ERROR> The same ID already exists."), 2);
    CHECK(orderOf(f.name) == "G1,L3,L0,L2,L1");               // Aaa, Big, Mmm, Neg, Zzz

    // 한글 이름 입력/저장
    out = runApp(f.name, string("3\nLifeLong K1 ") + KO_GA + " Yoga 5 010-9\n6\nW\n");
    CHECK_HAS(out, string("<INSERT> [LifeLong] ") + KO_GA + " (K1) Yoga 5 010-9");
    CHECK(has(readAll(f.name), string("LifeLong K1 ") + KO_GA + " Yoga 5 010-9\n"));

    // 알 수 없는 타입 (대소문자 다름 포함): 아무것도 삽입되지 않음
    {
        TempFile g("tmp_mt_insert_bad.txt", "LifeLong L0 Mmm Yoga 1 010-0\n");
        out = runApp(g.name, "3\nWrongType\n3\ngifted\n3\nLIFELONG\n1\n6\nW\n");
        CHECK_NOT_HAS(out, "<INSERT>");
        CHECK(orderOf(g.name) == "L0");
    }

    // 전화번호가 같은 학생 추가는 허용 (학번만 유일해야 함)
    {
        TempFile g("tmp_mt_insert_phone.txt", "LifeLong L0 Mmm Yoga 1 010-0\n");
        out = runApp(g.name, "3\nLifeLong L1 Nnn Yoga 1 010-0\n6\nW\n");
        CHECK_HAS(out, "<INSERT>");
        CHECK(orderOf(g.name) == "L0,L1");
    }

    // 대량 삽입 후 정렬 유지 + 저장/재로드 일치
    {
        TempFile g("tmp_mt_insert_many.txt");
        string in;
        for (int i = 0; i < 60; ++i) {
            in += "3\nLifeLong L" + to_string(i) + " N" + to_string((i * 37) % 11) + " Yoga " + to_string(i) + " 010-" + to_string(i) + "\n";
        }
        in += "6\nW\n";
        runApp(g.name, in);
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(g.name, l));
        CHECK_EQ(l.getSize(), 60);
        bool sorted = true;
        for (SinglyNode<AdStudent>* n = l.getHead(); n && n->getNext(); n = n->getNext())
            if (*(n->getNext()->getData()) < *(n->getData())) sorted = false;
        CHECK(sorted);
    }
}

// E-8. 종료 / 저장
static void testExitAndPersistence() {
    string original = "LifeLong L1 A Yoga 1 010-1\n";
    TempFile f("tmp_mt_exit.txt", original);

    // C/c/그 외 -> 저장 안 함
    const char* noSave[] = {"C", "c", "X", "N", "0"};
    for (const char* k : noSave) {
        string out = runApp(f.name, string("5\nL1\nY\n6\n") + k + "\n");
        CHECK_HAS(out, "Exit without Saving");
        CHECK(readAll(f.name) == original);
    }
    // W/w -> 저장
    string out = runApp(f.name, "5\nL1\nY\n6\nw\n");
    CHECK_HAS(out, "Save changes and Exit");
    CHECK(readAll(f.name) == "");

    // 세션 간 영속성: 저장 -> 재실행 시 보임 -> 삭제 후 저장 -> 사라짐
    writeAll(f.name, original);
    runApp(f.name, "3\nLifeLong L2 B Swim 2 010-2\n6\nW\n");
    out = runApp(f.name, "6\nC\n");
    CHECK_HAS(out, "[LifeLong] B (L2) Swim 2 010-2");
    runApp(f.name, "5\nL2\nY\n6\nW\n");
    out = runApp(f.name, "6\nC\n");
    CHECK_NOT_HAS(out, "(L2)");
    CHECK_HAS(out, "(L1)");

    // 프롬프트에 파일명 표시
    out = runApp(f.name, "6\nC\n");
    CHECK_HAS(out, "W/w to save changes to tmp_mt_exit.txt, C/c otherwise: ");
}

// E-9. 입력 견고성: 아래 항목들은 현재 구현에서 "조용히 오동작" 하는 지점 (WARN)
static void testInputRobustnessWarnings() {
    // (a) 삽입 도중 입력이 끊김 -> 반쪽짜리 레코드가 삽입되면 안 됨
    {
        TempFile f("tmp_mt_rob_a.txt", "LifeLong L0 Zed Yoga 1 010-0\n");
        string out = runApp(f.name, "3\nGifted G1 Amy");
        WARN(!has(out, "<INSERT>"), "입력이 중간에 끊겼는데 빈 필드의 Gifted 가 삽입됨 (input() 후 cin 상태 검사 필요)");
    }
    // (b) 수강료에 숫자가 아닌 값 -> 반쪽 레코드 삽입 + cin 고장으로 앱이 조용히 종료됨
    {
        TempFile f("tmp_mt_rob_b.txt", "LifeLong L0 Zed Yoga 1 010-0\n");
        string out = runApp(f.name, "3\nLifeLong L1 Kim Yoga abc 010-1\n1\n6\nC\n");
        WARN(!has(out, "<INSERT>"), "수강료가 숫자가 아닌데 삽입됨 (fee=0, phone 비어있음)");
        WARN(has(out, "Exit without Saving"), "잘못된 수강료 한 번에 앱이 종료 절차 없이 죽음 (cin.clear()/ignore() 필요)");
    }
    // (c) 알 수 없는 타입 뒤에 필드가 이어서 입력되면 그 필드들이 메뉴 번호로 해석되어 앱이 종료됨
    {
        TempFile f("tmp_mt_rob_c.txt", "LifeLong L0 Zed Yoga 1 010-0\n");
        string out = runApp(f.name, "3\nWrongType Name Co Dev AI 010-1\n6\nC\n");
        WARN(has(out, "Exit without Saving"), "잘못된 타입 입력 뒤 남은 토큰 때문에 이후 메뉴가 동작하지 않음 (입력 줄 버리기 필요)");
    }
    // (d) 삭제 확인에 "yes" 같은 여러 글자 입력 -> 'es' 가 남아 메뉴가 깨짐
    {
        TempFile f("tmp_mt_rob_d.txt", "LifeLong L0 Zed Yoga 1 010-0\n");
        string out = runApp(f.name, "5\nL0\nyes\n6\nC\n");
        WARN(has(out, "Exit without Saving"), "확인 입력 'yes' 의 나머지 글자가 남아 이후 메뉴가 동작하지 않음");
    }
    // (e) 수정 도중 입력 끊김 -> 레코드가 사라지거나 반쪽이 되면 안 됨
    {
        TempFile f("tmp_mt_rob_e.txt", "LifeLong L1 A Yoga 7 010-1\n");
        runApp(f.name, "4\nL1\nSwim");              // 크래시/누수 없이 끝나야 함
        SinglyLinkedList<AdStudent> l;
        loadList(f.name, l);
        CHECK(l.getSize() == 1);                     // 저장 전이라 파일은 그대로
    }
}

// E-10. 메모리/소유권: App 경로에서 누수·이중해제가 없는지 (ASan/LSan 으로 확인)
static void testAppMemoryPaths() {
    TempFile f("tmp_mt_mem.txt",
        "LifeLong L1 A Yoga 1 010-1\n"
        "Gifted G1 B Mid Math 010-2 Mom 010-3\n");
    string in;
    for (int i = 0; i < 40; ++i) {
        in += "3\nLifeLong L1 Dup Yoga 1 010-9\n";                          // 중복 -> delete student
        in += "4\nL1\nYoga " + to_string(i) + " 010-1\n";                  // 수정 -> remove + reinsert
        in += "5\nG1\nN\n";                                                // 취소
        in += "5\nZZZ\n";                                                  // 없는 키
        in += "3\nLifeLong T" + to_string(i) + " Tmp Yoga 1 010-" + to_string(100 + i) + "\n";
        in += "5\nT" + to_string(i) + "\nY\n";                             // 삽입 후 삭제 (delete removed)
    }
    in += "6\nC\n";
    string out = runApp(f.name, in);
    CHECK_EQ(countOf(out, "<ERROR> The same ID already exists."), 40);
    CHECK_EQ(countOf(out, "<DELETE>"), 40);
    CHECK_HAS(out, "Exit without Saving");
}

// =====================================================================
// F. 위험 입력 (현재 구현에서 크래시/UB/UAF 발생) - 수정 후 -DRUN_RISKY_TESTS 로 확인
// =====================================================================
#ifdef RUN_RISKY_TESTS
static void testRiskyInputs() {
    // 비어있지 않은 대상에 moveFrom -> 기존 노드가 통째로 누수 (노드 자체라 테스트에서 회수 불가)
    {
        TrackedStudent::aliveCount = 0;
        {
            SinglyLinkedList<AdStudent> dst, src;
            dst.insertAscend(new TrackedStudent("D1", "D", "1"));
            src.insertAscend(new TrackedStudent("S1", "S", "2"));
            dst.moveFrom(src);
            CHECK_EQ(dst.getSize(), 1);
        }
        CHECK_EQ(TrackedStudent::aliveCount, 0);       // D1 이 해제되어야 함 (this->clear() 선행)
    }
    // 자기 자신에게 moveFrom -> 전체가 사라지면 안 됨
    {
        TrackedStudent::aliveCount = 0;
        {
            SinglyLinkedList<AdStudent> l;
            l.insertAscend(new TrackedStudent("M1", "M", "1"));
            l.moveFrom(l);
            CHECK_EQ(l.getSize(), 1);
        }
        CHECK_EQ(TrackedStudent::aliveCount, 0);
    }
    // pos + len 정수 오버플로 -> 음수가 되어 잘못된 분기 / SEGV
    {
        MyString s("abcdef");
        s.erase(1, INT_MAX);
        CHECK_STR(s, "a");
    }
    {
        MyString s("abcdef");
        s.replace(1, INT_MAX, "Z");
        CHECK_STR(s, "aZ");
    }
    {
        MyString s("abcdef");
        CHECK_STR(s.substr(1, INT_MAX), "bcdef");      // 오버플로 + calcCapacity 무한루프
    }
    // 자기 메모리의 일부를 c-string 으로 대입 -> delete[] 후 읽기 (use-after-free)
    {
        MyString s("abcdefghijklmnopqrstuvwxyz");
        s = s.getStr() + 20;
        CHECK_STR(s, "uvwxyz");
    }
    // stoi int 오버플로 (UB). 최소한 UB 없이 포화/0 처리
    {
        int v = stoi(MyString("99999999999"));
        CHECK(v == INT_MAX || v == 0);
    }
    // 학번 숫자가 int 를 넘는 파일: 생성자의 stoi 오버플로로 nextID 가 깨질 수 있음
    {
        TempFile f("tmp_mt_risky_id.txt", "EmployeeEdu E99999999999 Huge Co Dev AI 010-1\n");
        SinglyLinkedList<AdStudent> l;
        CHECK(loadList(f.name, l));
        CHECK_EQ(l.getSize(), 1);
    }
}
#endif

int main() {
    cout.setf(ios::unitbuf);
    // 순서 중요: E/S 학번 카운터는 static 이라 되돌릴 수 없음
    runTest("App: basic scenario (menu 1~6, format, auto ID, save)  [must run first]", testBasicScenario);
    runTest("App: auto ID boundaries (skip, E/S independence, 6 digits, no decrease)", testAutoID);

    runTest("MyString: construction & capacity boundaries", testMyStringConstruction);
    runTest("MyString: at / operator[] out of range", testMyStringAtAndIndex);
    runTest("MyString: copy / assign / self-assign / chain", testMyStringCopyAssign);
    runTest("MyString: operator+", testMyStringConcat);
    runTest("MyString: ==, !=, <, >, <=, >= (+ trichotomy)", testMyStringCompare);
    runTest("MyString: find", testMyStringFind);
    runTest("MyString: substr", testMyStringSubstr);
    runTest("MyString: insert (incl. self-insert)", testMyStringInsert);
    runTest("MyString: replace (incl. aliasing)", testMyStringReplace);
    runTest("MyString: erase", testMyStringErase);
    runTest("MyString: append / += (incl. self-append, 0..300 sweep)", testMyStringAppend);
    runTest("MyString: trim", testMyStringTrim);
    runTest("MyString: stoi", testStoi);
    runTest("MyString: toString", testToString);
    runTest("MyString: operator>>, getline, operator<<", testMyStringIO);

    runTest("Node: ownership / non-copyable", testNodeOwnership);
    runTest("List: add / insert / get / invalid index", testListInsertAndGet);
    runTest("List: remove / find / removeFirst", testListRemoveAndFind);
    runTest("List: insertAscend ordering & duplicates", testListInsertAscendOrdering);
    runTest("List: moveFrom / clear", testListMoveFromAndClear);
    runTest("List: 100k nodes + randomized model-based test", testListLargeAndModelBased);

    runTest("Student: operators (<, <=, == by ID/phone) & Korean order", testStudentOperators);
    runTest("Student: toString / toFileFormatString / defaults", testStudentFormats);

    runTest("FileIO: round trip & idempotent save", testFileLoadSaveRoundTrip);
    runTest("FileIO: sorted on save", testFileSortsOnSave);
    runTest("FileIO: CRLF, tabs, no trailing newline, unknown types, dup IDs, long, Korean", testFileEdgeFormats);
    runTest("FileIO: missing file / unwritable path / overwrite", testFileFailureModes);
    runTest("FileIO: robustness (BOM, truncated record, fee overflow)", testFileRobustnessWarnings);

    runTest("App: menu loop, empty list, sorting, EOF, bad numbers", testMenuLoopAndViews);
    runTest("App: search", testSearch);
    runTest("App: modify (all 4 types, errors)", testModify);
    runTest("App: modify re-sort & duplicate phones", testModifyReorderAndDuplicatePhone);
    runTest("App: delete (confirm keys, head/tail/middle, re-insert)", testDelete);
    runTest("App: insert (cross-type dup ID, fee bounds, Korean, bulk)", testInsert);
    runTest("App: exit / save / persistence across sessions", testExitAndPersistence);
    runTest("App: input robustness (warnings)", testInputRobustnessWarnings);
    runTest("App: memory paths (run with ASan/LSan)", testAppMemoryPaths);
#ifdef RUN_RISKY_TESTS
    runTest("RISKY: overflow / use-after-free inputs", testRiskyInputs);
#endif

    cout << "\n=========================================\n"
         << " checks passed : " << g_pass << "\n"
         << " checks FAILED : " << g_fail << "\n"
         << " warnings      : " << g_warn << "  (견고성 개선 권장 - 실패 아님)\n"
         << "=========================================\n";
    cout << (g_fail == 0 ? "ALL REQUIRED TESTS PASSED!\n" : "SOME TESTS FAILED\n");
    return g_fail == 0 ? 0 : 1;
}