#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ManageCircleApp.h"

namespace {
    const char *SPEC_FILE_BEFORE =
        "1998 2024\n"
        "9 3\n"
        "20220987 G1 Harley Kim\n"
        "20231555 G1 William Jin\n"
        "20221004 G2 Thomas Lee\n"
        "19980123 G1 Jenny Anh\n"
        "19980909 G3 Liam Son\n"
        "19990131 G2 Owen Park\n"
        "19981475 - Aria Lee\n"
        "20230554 G2 Oliver Kim\n"
        "20240178 G1 Nora Kang\n";

    const char *BASIC_INPUT =
        "L\n"
        "I\n"
        "20250907 G3 Rosa Jang\n"
        "D\n"
        "19981475\n"
        "L\n"
        "Q\n";

    const char *BASIC_TRANSCRIPT =
        "L\n"
        "[Year:1998~2024] [Student Count:9] [Group Count: 3]\n"
        "19980123 Jenny Anh ([G1] 20220987 Harley Kim - 20231555 William Jin - 20240178 Nora Kang)\n"
        "19980909 Liam Son ([G3])\n"
        "19981475 Aria Lee ()\n"
        "19990131 Owen Park ([G2] 20221004 Thomas Lee - 20230554 Oliver Kim)\n"
        "20220987 Harley Kim ([G1] 20231555 William Jin - 20240178 Nora Kang - 19980123 Jenny Anh)\n"
        "20221004 Thomas Lee ([G2] 20230554 Oliver Kim - 19990131 Owen Park)\n"
        "20230554 Oliver Kim ([G2] 19990131 Owen Park - 20221004 Thomas Lee)\n"
        "20231555 William Jin ([G1] 20240178 Nora Kang - 19980123 Jenny Anh - 20220987 Harley Kim)\n"
        "20240178 Nora Kang ([G1] 19980123 Jenny Anh - 20220987 Harley Kim - 20231555 William Jin)\n"
        "I\n"
        "20250907 G3 Rosa Jang\n"
        "D\n"
        "19981475\n"
        "L\n"
        "[Year:1998~2025] [Student Count:9] [Group Count: 3]\n"
        "19980123 Jenny Anh ([G1] 20220987 Harley Kim - 20231555 William Jin - 20240178 Nora Kang)\n"
        "19980909 Liam Son ([G3] 20250907 Rosa Jang)\n"
        "19990131 Owen Park ([G2] 20221004 Thomas Lee - 20230554 Oliver Kim)\n"
        "20220987 Harley Kim ([G1] 20231555 William Jin - 20240178 Nora Kang - 19980123 Jenny Anh)\n"
        "20221004 Thomas Lee ([G2] 20230554 Oliver Kim - 19990131 Owen Park)\n"
        "20230554 Oliver Kim ([G2] 19990131 Owen Park - 20221004 Thomas Lee)\n"
        "20231555 William Jin ([G1] 20240178 Nora Kang - 19980123 Jenny Anh - 20220987 Harley Kim)\n"
        "20240178 Nora Kang ([G1] 19980123 Jenny Anh - 20220987 Harley Kim - 20231555 William Jin)\n"
        "20250907 Rosa Jang ([G3] 19980909 Liam Son)\n"
        "Q\n";

    const char *BASIC_FILE_AFTER =
        "1998 2025\n"
        "9 3\n"
        "19980123 G1 Jenny Anh\n"
        "19980909 G3 Liam Son\n"
        "19990131 G2 Owen Park\n"
        "20220987 G1 Harley Kim\n"
        "20221004 G2 Thomas Lee\n"
        "20230554 G2 Oliver Kim\n"
        "20231555 G1 William Jin\n"
        "20240178 G1 Nora Kang\n"
        "20250907 G3 Rosa Jang\n";

    const char *BASIC_RELOAD_TRANSCRIPT =
        "L\n"
        "[Year:1998~2025] [Student Count:9] [Group Count: 3]\n"
        "19980123 Jenny Anh ([G1] 20220987 Harley Kim - 20231555 William Jin - 20240178 Nora Kang)\n"
        "19980909 Liam Son ([G3] 20250907 Rosa Jang)\n"
        "19990131 Owen Park ([G2] 20221004 Thomas Lee - 20230554 Oliver Kim)\n"
        "20220987 Harley Kim ([G1] 20231555 William Jin - 20240178 Nora Kang - 19980123 Jenny Anh)\n"
        "20221004 Thomas Lee ([G2] 20230554 Oliver Kim - 19990131 Owen Park)\n"
        "20230554 Oliver Kim ([G2] 19990131 Owen Park - 20221004 Thomas Lee)\n"
        "20231555 William Jin ([G1] 20240178 Nora Kang - 19980123 Jenny Anh - 20220987 Harley Kim)\n"
        "20240178 Nora Kang ([G1] 19980123 Jenny Anh - 20220987 Harley Kim - 20231555 William Jin)\n"
        "20250907 Rosa Jang ([G3] 19980909 Liam Son)\n"
        "Q\n";

    const char *EDGE_INPUT =
        "X\n"
        "LL\n"
        "I\n"
        "20220987 G1 Dup Person\n"
        "D\n"
        "99999999\n"
        "D\n"
        "19980909\n"
        "I\n"
        "19950101 G4 Mary Jane Choi\n"
        "I\n"
        "20260315 - Solo Park\n"
        "D\n"
        "19981475\n"
        "D\n"
        "19981475\n"
        "L\n"
        "Q\n";

    const char *EDGE_TRANSCRIPT =
        "X\n"
        "Unknown command: X\n"
        "LL\n"
        "Unknown command: LL\n"
        "I\n"
        "20220987 G1 Dup Person\n"
        "Already exists: 20220987\n"
        "D\n"
        "99999999\n"
        "Not found: 99999999\n"
        "D\n"
        "19980909\n"
        "I\n"
        "19950101 G4 Mary Jane Choi\n"
        "I\n"
        "20260315 - Solo Park\n"
        "D\n"
        "19981475\n"
        "D\n"
        "19981475\n"
        "Not found: 19981475\n"
        "L\n"
        "[Year:1995~2026] [Student Count:9] [Group Count: 3]\n"
        "19950101 Mary Jane Choi ([G4])\n"
        "19980123 Jenny Anh ([G1] 20220987 Harley Kim - 20231555 William Jin - 20240178 Nora Kang)\n"
        "19990131 Owen Park ([G2] 20221004 Thomas Lee - 20230554 Oliver Kim)\n"
        "20220987 Harley Kim ([G1] 20231555 William Jin - 20240178 Nora Kang - 19980123 Jenny Anh)\n"
        "20221004 Thomas Lee ([G2] 20230554 Oliver Kim - 19990131 Owen Park)\n"
        "20230554 Oliver Kim ([G2] 19990131 Owen Park - 20221004 Thomas Lee)\n"
        "20231555 William Jin ([G1] 20240178 Nora Kang - 19980123 Jenny Anh - 20220987 Harley Kim)\n"
        "20240178 Nora Kang ([G1] 19980123 Jenny Anh - 20220987 Harley Kim - 20231555 William Jin)\n"
        "20260315 Solo Park ()\n"
        "Q\n";

    const char *EDGE_FILE_AFTER =
        "1995 2026\n"
        "9 3\n"
        "19950101 G4 Mary Jane Choi\n"
        "19980123 G1 Jenny Anh\n"
        "19990131 G2 Owen Park\n"
        "20220987 G1 Harley Kim\n"
        "20221004 G2 Thomas Lee\n"
        "20230554 G2 Oliver Kim\n"
        "20231555 G1 William Jin\n"
        "20240178 G1 Nora Kang\n"
        "20260315 - Solo Park\n";

    const char *EDGE_RELOAD_TRANSCRIPT =
        "L\n"
        "[Year:1995~2026] [Student Count:9] [Group Count: 3]\n"
        "19950101 Mary Jane Choi ([G4])\n"
        "19980123 Jenny Anh ([G1] 20220987 Harley Kim - 20231555 William Jin - 20240178 Nora Kang)\n"
        "19990131 Owen Park ([G2] 20221004 Thomas Lee - 20230554 Oliver Kim)\n"
        "20220987 Harley Kim ([G1] 20231555 William Jin - 20240178 Nora Kang - 19980123 Jenny Anh)\n"
        "20221004 Thomas Lee ([G2] 20230554 Oliver Kim - 19990131 Owen Park)\n"
        "20230554 Oliver Kim ([G2] 19990131 Owen Park - 20221004 Thomas Lee)\n"
        "20231555 William Jin ([G1] 20240178 Nora Kang - 19980123 Jenny Anh - 20220987 Harley Kim)\n"
        "20240178 Nora Kang ([G1] 19980123 Jenny Anh - 20220987 Harley Kim - 20231555 William Jin)\n"
        "20260315 Solo Park ()\n"
        "Q\n";

    class EchoInputBuffer : public std::streambuf {
        std::istringstream m_source;
        std::ostream &m_echo;
        std::string m_line;

        public:
            EchoInputBuffer(const std::string &input, std::ostream &echo)
                : m_source(input), m_echo(echo) {
            }

        protected:
            int_type underflow() override {
                if (!std::getline(this->m_source, this->m_line)) {
                    return traits_type::eof();
                }

                this->m_line += '\n';
                this->m_echo << this->m_line;
                this->setg(this->m_line.data(), this->m_line.data(), this->m_line.data() + this->m_line.size());

                return traits_type::to_int_type(this->m_line[0]);
            }
    };

    struct RunResult {
        bool is_success;
        std::string transcript;
    };

    class ManageCircleAppTest : public ::testing::Test {
        protected:
            std::filesystem::path m_path;

            void SetUp() override {
                const auto *info = ::testing::UnitTest::GetInstance()->current_test_info();
                this->m_path = std::filesystem::temp_directory_path() / (std::string("week7_") + info->name() + ".txt");

                std::ofstream output_stream(this->m_path);
                output_stream << SPEC_FILE_BEFORE;
            }

            void TearDown() override {
                std::filesystem::remove(this->m_path);
            }

            std::string read_file() const {
                std::ifstream input_stream(this->m_path);
                std::stringstream buffer;
                buffer << input_stream.rdbuf();
                return buffer.str();
            }

            static RunResult run_app(const std::filesystem::path &path, const std::string &input) {
                std::ostringstream screen;
                EchoInputBuffer keyboard(input, screen);
                std::streambuf *old_input = std::cin.rdbuf(&keyboard);
                std::streambuf *old_output = std::cout.rdbuf(screen.rdbuf());
                std::streambuf *old_error = std::cerr.rdbuf(screen.rdbuf());

                bool is_success;
                {
                    std::string path_string = path.string();
                    ManageCircleApp app(path_string.c_str());
                    is_success = app.run();
                }

                std::cin.rdbuf(old_input);
                std::cout.rdbuf(old_output);
                std::cerr.rdbuf(old_error);

                return RunResult{is_success, screen.str()};
            }

            RunResult run_app(const std::string &input) const {
                return run_app(this->m_path, input);
            }
    };
}

TEST_F(ManageCircleAppTest, BasicTranscriptMatchesSpecExample) {
    RunResult result = this->run_app(BASIC_INPUT);

    EXPECT_TRUE(result.is_success);
    EXPECT_EQ(result.transcript, BASIC_TRANSCRIPT);
}

TEST_F(ManageCircleAppTest, BasicSavedFileMatchesSpecExample) {
    this->run_app(BASIC_INPUT);

    EXPECT_EQ(this->read_file(), BASIC_FILE_AFTER);
}

TEST_F(ManageCircleAppTest, BasicSavedFileRunsAgain) {
    this->run_app(BASIC_INPUT);
    RunResult result = this->run_app("L\nQ\n");

    EXPECT_EQ(result.transcript, BASIC_RELOAD_TRANSCRIPT);
    EXPECT_EQ(this->read_file(), BASIC_FILE_AFTER);
}

TEST_F(ManageCircleAppTest, EdgeTranscript) {
    RunResult result = this->run_app(EDGE_INPUT);

    EXPECT_TRUE(result.is_success);
    EXPECT_EQ(result.transcript, EDGE_TRANSCRIPT);
}

TEST_F(ManageCircleAppTest, EdgeSavedFile) {
    this->run_app(EDGE_INPUT);

    EXPECT_EQ(this->read_file(), EDGE_FILE_AFTER);
}

TEST_F(ManageCircleAppTest, EdgeSavedFileRunsAgain) {
    this->run_app(EDGE_INPUT);
    RunResult result = this->run_app("L\nQ\n");

    EXPECT_EQ(result.transcript, EDGE_RELOAD_TRANSCRIPT);
    EXPECT_EQ(this->read_file(), EDGE_FILE_AFTER);
}

TEST_F(ManageCircleAppTest, EndOfInputWithoutQuitDoesNotSave) {
    RunResult result = this->run_app("D\n19981475\n");

    EXPECT_EQ(result.transcript, "D\n19981475\n");
    EXPECT_EQ(this->read_file(), SPEC_FILE_BEFORE);
}

TEST_F(ManageCircleAppTest, MissingFileFailsToRun) {
    std::filesystem::path missing = this->m_path.string() + ".missing";
    RunResult result = run_app(missing, "L\nQ\n");

    EXPECT_FALSE(result.is_success);
    EXPECT_EQ(result.transcript, "Cannot load file: " + missing.string() + "\n");
    EXPECT_FALSE(std::filesystem::exists(missing));
}
