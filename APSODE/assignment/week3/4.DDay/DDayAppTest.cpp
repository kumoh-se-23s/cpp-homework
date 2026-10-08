//
// Created by leegu on 26. 10. 8..
//
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "DDayApp.h"

namespace {
    // std::cin의 버퍼를 문자열로 바꿔치기하고, 소멸 시 원래대로 되돌림
    class CinRedirect {
        public:
            explicit CinRedirect(const std::string &text)
                : stream(text), original(std::cin.rdbuf(stream.rdbuf())) {
            }

            ~CinRedirect() {
                std::cin.rdbuf(original);
                std::cin.clear();
            }

        private:
            std::istringstream stream;
            std::streambuf *original;
    };

    // 입력을 그대로 넣고 실행 (q가 없으면 무한 반복하므로 호출 시 주의)
    std::string run_app_raw(const std::string &input) {
        CinRedirect redirect(input);
        testing::internal::CaptureStdout();
        DDayApp app;
        app.run();
        std::cout.flush();
        return testing::internal::GetCapturedStdout();
    }

    // 명령 뒤에 항상 q를 붙여서 실행
    std::string run_app(const std::string &commands) {
        return run_app_raw(commands + "\nq\n");
    }

    // 출력에서 "<<"로 시작하는 결과 줄만 뽑아냄 (프롬프트와 같은 줄에 있어도 동작)
    std::vector<std::string> extract_results(const std::string &output) {
        std::vector<std::string> results;
        std::size_t pos = output.find("<<");
        while (pos != std::string::npos) {
            std::size_t end = output.find('\n', pos);
            if (end == std::string::npos) end = output.size();

            std::string line = output.substr(pos, end - pos);
            while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) {
                line.pop_back();
            }
            results.push_back(line);
            pos = output.find("<<", end);
        }
        return results;
    }

    std::vector<std::string> results_of(const std::string &commands) {
        return extract_results(run_app(commands));
    }

    int count_occurrences(const std::string &text, const std::string &target) {
        int count = 0;
        for (std::size_t pos = text.find(target); pos != std::string::npos;
             pos = text.find(target, pos + target.size())) {
            ++count;
        }
        return count;
    }

    const std::string ERROR_TEXT = "*** ERROR";
    const std::string END_TEXT = "=== END ===";
    const std::string PROMPT_TEXT =
            ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";
}

// ---------- 시작 / 종료 ----------

TEST(DDayAppStartTest, PrintsInitialStateBeforeFirstCommand) {
    const auto results = extract_results(run_app_raw("q\n"));

    ASSERT_GE(results.size(), 1u);
    EXPECT_EQ(results[0], "<< 2026/10/01 [D-day:+0] 2026/10/01");
}

TEST(DDayAppStartTest, PrintsPromptBeforeEachCommand) {
    const std::string output = run_app("t\ny");

    // 명령 2개 + 종료용 q 1개
    EXPECT_EQ(count_occurrences(output, PROMPT_TEXT), 3);
}

TEST(DDayAppQuitTest, LowerCaseQuitPrintsEndBanner) {
    const std::string output = run_app_raw("q\n");
    EXPECT_NE(output.find(END_TEXT), std::string::npos);
}

TEST(DDayAppQuitTest, UpperCaseQuitPrintsEndBanner) {
    const std::string output = run_app_raw("Q\n");
    EXPECT_NE(output.find(END_TEXT), std::string::npos);
}

TEST(DDayAppQuitTest, QuitDoesNotPrintResultOrError) {
    const std::string output = run_app_raw("q\n");

    // 결과 줄은 시작 시 출력되는 1개뿐이어야 함
    EXPECT_EQ(extract_results(output).size(), 1u);
    EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 0);
}

TEST(DDayAppQuitTest, StopsProcessingAfterQuit) {
    const std::string output = run_app_raw("t\nq\nt\nt\n");

    // 시작 1줄 + t 1줄, q 이후의 t는 처리되지 않음
    EXPECT_EQ(extract_results(output).size(), 2u);
}

TEST(DDayAppQuitTest, EndBannerIsPrintedOnlyOnce) {
    const std::string output = run_app("t\ny\nt");
    EXPECT_EQ(count_occurrences(output, END_TEXT), 1);
}

// ---------- Tomorrow / Yesterday ----------

TEST(DDayAppTomorrowTest, LowerCaseMovesToNextDay) {
    const auto results = results_of("t");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/10/02 [D-day:+0] 2026/10/02");
}

TEST(DDayAppTomorrowTest, UpperCaseMovesToNextDay) {
    const auto results = results_of("T");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/10/02 [D-day:+0] 2026/10/02");
}

TEST(DDayAppTomorrowTest, RepeatedCallsAccumulate) {
    const auto results = results_of("t\nt\nt");

    ASSERT_EQ(results.size(), 4u);
    EXPECT_EQ(results[3], "<< 2026/10/04 [D-day:+0] 2026/10/04");
}

TEST(DDayAppTomorrowTest, CrossesMonthBoundary) {
    // 2026/10/01 -> 31번 이동하면 11/01
    std::string commands;
    for (int i = 0; i < 31; ++i) commands += "t\n";

    const auto results = results_of(commands);

    ASSERT_EQ(results.size(), 32u);
    EXPECT_EQ(results[31], "<< 2026/11/01 [D-day:+0] 2026/11/01");
}

TEST(DDayAppYesterdayTest, LowerCaseMovesToPreviousDay) {
    const auto results = results_of("y");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/09/30 [D-day:+0] 2026/09/30");
}

TEST(DDayAppYesterdayTest, UpperCaseMovesToPreviousDay) {
    const auto results = results_of("Y");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/09/30 [D-day:+0] 2026/09/30");
}

TEST(DDayAppYesterdayTest, RepeatedCallsAccumulate) {
    const auto results = results_of("y\ny\ny");

    ASSERT_EQ(results.size(), 4u);
    EXPECT_EQ(results[3], "<< 2026/09/28 [D-day:+0] 2026/09/28");
}

TEST(DDayAppMoveTest, YesterdayThenTomorrowReturnsOriginal) {
    const auto results = results_of("y\nT");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[1], "<< 2026/09/30 [D-day:+0] 2026/09/30");
    EXPECT_EQ(results[2], "<< 2026/10/01 [D-day:+0] 2026/10/01");
}

// ---------- D-day 설정 (+/-int) ----------

TEST(DDayAppSetDDayTest, PositiveValue) {
    const auto results = results_of("+1004");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/10/01 [D-day:+1004] 2029/07/01");
}

TEST(DDayAppSetDDayTest, SmallPositiveValue) {
    const auto results = results_of("+30");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/10/01 [D-day:+30] 2026/10/31");
}

TEST(DDayAppSetDDayTest, NegativeValue) {
    const auto results = results_of("-1");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/10/01 [D-day:-1] 2026/09/30");
}

TEST(DDayAppSetDDayTest, ZeroResetsToPlusZero) {
    const auto results = results_of("+30\n+0");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2026/10/01 [D-day:+0] 2026/10/01");
}

TEST(DDayAppSetDDayTest, NewValueOverwritesPreviousValue) {
    const auto results = results_of("+30\n+5");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2026/10/01 [D-day:+5] 2026/10/06");
}

TEST(DDayAppSetDDayTest, KeepsBaseDate) {
    // D-day를 설정해도 기준 날짜(앞쪽)는 바뀌지 않음
    const auto results = results_of("+100");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1].substr(0, 13), "<< 2026/10/01");
}

TEST(DDayAppSetDDayTest, DDayIsKeptWhenMovingBaseDate) {
    const auto results = results_of("+1004\nY\nt");

    ASSERT_EQ(results.size(), 4u);
    EXPECT_EQ(results[2], "<< 2026/09/30 [D-day:+1004] 2029/06/30");
    EXPECT_EQ(results[3], "<< 2026/10/01 [D-day:+1004] 2029/07/01");
}

TEST(DDayAppSetDDayTest, RepeatedPrintDoesNotAccumulate) {
    // 결과를 여러 번 출력해도 계산 날짜가 누적되어 밀리면 안 됨
    const auto results = results_of("+10\nt\ny");

    ASSERT_EQ(results.size(), 4u);
    EXPECT_EQ(results[1], "<< 2026/10/01 [D-day:+10] 2026/10/11");
    EXPECT_EQ(results[2], "<< 2026/10/02 [D-day:+10] 2026/10/12");
    EXPECT_EQ(results[3], "<< 2026/10/01 [D-day:+10] 2026/10/11");
}

// ---------- 새 날짜 설정 (yyyymmdd) ----------

TEST(DDayAppNewDayTest, ValidDateReplacesBaseDate) {
    const auto results = results_of("20261225");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/12/25 [D-day:+0] 2026/12/25");
}

TEST(DDayAppNewDayTest, LeapDayIsAcceptedInLeapYear) {
    const auto results = results_of("20240229");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2024/02/29 [D-day:+0] 2024/02/29");
}

TEST(DDayAppNewDayTest, Year2000LeapDayIsAccepted) {
    const auto results = results_of("20000229");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2000/02/29 [D-day:+0] 2000/02/29");
}

TEST(DDayAppNewDayTest, KeepsDDayWhenBaseDateChanges) {
    const auto results = results_of("+1004\n20240229");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2024/02/29 [D-day:+1004] 2026/11/29");
}

TEST(DDayAppNewDayTest, ValidDateDoesNotPrintError) {
    const std::string output = run_app("20261225");
    EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 0);
}

TEST(DDayAppNewDayTest, TomorrowAndYesterdayWorkFromNewDate) {
    const auto results = results_of("20240229\nt\ny");

    ASSERT_EQ(results.size(), 4u);
    EXPECT_EQ(results[2], "<< 2024/03/01 [D-day:+0] 2024/03/01");
    EXPECT_EQ(results[3], "<< 2024/02/29 [D-day:+0] 2024/02/29");
}

TEST(DDayAppNewDayTest, LeapDayMovesThroughNewDate) {
    const auto results = results_of("20240228\nt\nt");

    ASSERT_EQ(results.size(), 4u);
    EXPECT_EQ(results[2], "<< 2024/02/29 [D-day:+0] 2024/02/29");
    EXPECT_EQ(results[3], "<< 2024/03/01 [D-day:+0] 2024/03/01");
}

// ---------- 잘못된 날짜 ----------

TEST(DDayAppInvalidDayTest, Feb29InCommonYearIsRejected) {
    const std::string output = run_app("20300229");
    EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 1);
}

TEST(DDayAppInvalidDayTest, RejectsNonexistentDates) {
    const std::vector<std::string> invalid_dates = {
        "20300229", // 평년의 2월 29일
        "20230229", // 평년의 2월 29일
        "21000229", // 100의 배수(400의 배수 아님)는 평년
        "20261301", // 13월
        "20260431", // 4월은 30일까지
        "20260100", // 0일
        "20260001", // 0월
    };

    for (const std::string &date: invalid_dates) {
        SCOPED_TRACE(date);
        const std::string output = run_app(date);

        EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 1);
    }
}

TEST(DDayAppInvalidDayTest, StateIsUnchangedAfterError) {
    const auto results = results_of("+1004\n20300229");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2026/10/01 [D-day:+1004] 2029/07/01");
}

TEST(DDayAppInvalidDayTest, ResultIsPrintedAfterError) {
    // 사진처럼 오류 메시지 다음에 현재 상태를 다시 출력함
    const std::string output = run_app("20300229");
    const std::size_t error_pos = output.find(ERROR_TEXT);
    const std::size_t result_pos = output.find("<<", error_pos);

    ASSERT_NE(error_pos, std::string::npos);
    EXPECT_NE(result_pos, std::string::npos);
}

TEST(DDayAppInvalidDayTest, CanContinueAfterError) {
    const auto results = results_of("20300229\nt");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2026/10/02 [D-day:+0] 2026/10/02");
}

// ---------- 알 수 없는 명령 ----------

TEST(DDayAppUnknownCommandTest, LetterCommandPrintsError) {
    const std::string output = run_app("a");
    EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 1);
}

TEST(DDayAppUnknownCommandTest, RejectsVariousUnknownCommands) {
    const std::vector<std::string> unknown = {"a", "x", "z", "?", "!"};

    for (const std::string &command: unknown) {
        SCOPED_TRACE(command);
        const std::string output = run_app(command);

        EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 1);
    }
}

TEST(DDayAppUnknownCommandTest, StateIsUnchangedAfterError) {
    const auto results = results_of("+10\na");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2026/10/01 [D-day:+10] 2026/10/11");
}

TEST(DDayAppUnknownCommandTest, ResultIsPrintedAfterError) {
    const auto results = results_of("a");

    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[1], "<< 2026/10/01 [D-day:+0] 2026/10/01");
}

TEST(DDayAppUnknownCommandTest, CanContinueAfterError) {
    const auto results = results_of("a\ny");

    ASSERT_EQ(results.size(), 3u);
    EXPECT_EQ(results[2], "<< 2026/09/30 [D-day:+0] 2026/09/30");
}

TEST(DDayAppUnknownCommandTest, ValidCommandsDoNotPrintError) {
    const std::string output = run_app("t\ny\n+5\n20261225");
    EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 0);
}

// ---------- 사진의 전체 시나리오 ----------

TEST(DDayAppScenarioTest, MatchesReferenceSession) {
    const std::string output = run_app("y\nT\n+1004\nY\nt\n20300229\n20240229\n-8826\nt\ny\na");
    const auto results = extract_results(output);

    const std::vector<std::string> expected = {
        "<< 2026/10/01 [D-day:+0] 2026/10/01",       // 시작
        "<< 2026/09/30 [D-day:+0] 2026/09/30",       // y
        "<< 2026/10/01 [D-day:+0] 2026/10/01",       // T
        "<< 2026/10/01 [D-day:+1004] 2029/07/01",    // +1004
        "<< 2026/09/30 [D-day:+1004] 2029/06/30",    // Y
        "<< 2026/10/01 [D-day:+1004] 2029/07/01",    // t
        "<< 2026/10/01 [D-day:+1004] 2029/07/01",    // 20300229 (오류)
        "<< 2024/02/29 [D-day:+1004] 2026/11/29",    // 20240229
        "<< 2024/02/29 [D-day:-8826] 1999/12/31",    // -8826
        "<< 2024/03/01 [D-day:-8826] 2000/01/01",    // t
        "<< 2024/02/29 [D-day:-8826] 1999/12/31",    // y
        "<< 2024/02/29 [D-day:-8826] 1999/12/31",    // a (오류)
    };

    ASSERT_EQ(results.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(results[i], expected[i]) << "결과 줄 " << i;
    }

    EXPECT_EQ(count_occurrences(output, ERROR_TEXT), 2);
    EXPECT_EQ(count_occurrences(output, END_TEXT), 1);
}

TEST(DDayAppScenarioTest, ErrorsAppearAtTheRightPositions) {
    const std::string output = run_app("20300229\nt\na");

    // 순서: 시작 결과 -> 오류 -> 결과 -> t 결과 -> 오류 -> 결과 -> 종료
    const std::size_t first_error = output.find(ERROR_TEXT);
    const std::size_t second_error = output.find(ERROR_TEXT, first_error + 1);
    const std::size_t end_pos = output.find(END_TEXT);

    ASSERT_NE(first_error, std::string::npos);
    ASSERT_NE(second_error, std::string::npos);
    ASSERT_NE(end_pos, std::string::npos);
    EXPECT_LT(first_error, second_error);
    EXPECT_LT(second_error, end_pos);
}