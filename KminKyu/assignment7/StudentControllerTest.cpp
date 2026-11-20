#include <gtest/gtest.h>
#include <algorithm>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "MyString.h"
#include "StudentController.h"

// =====================================================================
// StudentController 통합 및 차분(differential) 테스트
//
//  - 빈 컨트롤러 / 단일 삽입 / 중복 학번 무시 / 학번(연도·번호) 정렬 출력
//  - 헤더 출력([Year:min~max] [Student Count:N] [Group Count: M]) 및 toString() 파일 포맷
//  - 그룹원 원형 연결 출력 (1명 단독 그룹([G]), 다수 그룹([G] ...), "-" 무소속(()))
//  - 삭제 경계값 (빈 상태, 없는 연도/학번, 그룹 head/중간/tail/마지막, "-" 삭제, 재삽입)
//  - LookUpTable 용량 확장(DEFAULT_CAPACITY 초과) 및 커스텀 생성자
//  - std::map 기반 참조 모델과 무작위 삽입/삭제를 비교하는 차분(differential) 테스트
//
//  전제 조건 (위반 시 테스트 제외):
//   - 음수 학번
//
//  메모리 오류(Use-After-Free, Double Free)는
//  AddressSanitizer(-fsanitize=address,undefined) 와 함께 실행해서 확인할 것.
// =====================================================================

namespace {
    struct StudentRecord {
        int id;
        std::string group;
        std::string name;
    };

    class ReferenceController {
    public:
        void insertStudent(const int id, const std::string &group, const std::string &name) {
            if (students_.find(id) != students_.end()) {
                return;
            }
            students_[id] = StudentRecord{id, group, name};
        }

        void deleteStudent(const int id) {
            students_.erase(id);
        }

        bool contains(const int id) const {
            return students_.find(id) != students_.end();
        }

        std::size_t size() const {
            return students_.size();
        }

        std::string expectedOutput() const {
            std::map<std::string, std::vector<const StudentRecord *>> groups;
            for (const auto &[id, record] : students_) {
                if (record.group != "-") {
                    groups[record.group].push_back(&record);
                }
            }

            std::ostringstream oss;
            const int minYear = students_.empty() ? 0 : (students_.begin()->first / 10000);
            const int maxYear = students_.empty() ? 0 : (students_.rbegin()->first / 10000);
            oss << "[Year:" << minYear << '~' << maxYear
                << "] [Student Count:" << students_.size()
                << "] [Group Count: " << groups.size() << "]\n";

            for (const auto &[id, record] : students_) {
                oss << record.id << ' ' << record.name << '(';
                if (record.group != "-") {
                    oss << '[' << record.group << ']';
                    const auto &members = groups[record.group];
                    if (members.size() > 1) {
                        std::size_t selfIdx = 0;
                        for (std::size_t i = 0; i < members.size(); ++i) {
                            if (members[i]->id == record.id) {
                                selfIdx = i;
                                break;
                            }
                        }
                        for (std::size_t offset = 1; offset < members.size(); ++offset) {
                            const StudentRecord *member = members[(selfIdx + offset) % members.size()];
                            oss << ' ' << member->id << ' ' << member->name;
                            if (offset + 1 < members.size()) {
                                oss << " -";
                            }
                        }
                    }
                }
                oss << ")\n";
            }
            return oss.str();
        }

        std::string expectedToString() const {
            std::set<std::string> groupNames;
            for (const auto &[id, record] : students_) {
                if (record.group != "-") {
                    groupNames.insert(record.group);
                }
            }

            std::ostringstream oss;
            const int minYear = students_.empty() ? 0 : (students_.begin()->first / 10000);
            const int maxYear = students_.empty() ? 0 : (students_.rbegin()->first / 10000);
            oss << minYear << ' ' << maxYear << '\n'
                << students_.size() << ' ' << groupNames.size() << '\n';
            for (const auto &[id, record] : students_) {
                oss << record.id << ' ' << record.group << ' ' << record.name << '\n';
            }
            return oss.str();
        }

    private:
        std::map<int, StudentRecord> students_;
    };

    std::string stream_output(const StudentController &controller) {
        std::ostringstream os;
        os << controller;
        return os.str();
    }

    std::string to_std_string(const MyString &sequence) {
        std::ostringstream os;
        os << sequence;
        return os.str();
    }

    void expect_same(const StudentController &actual, const ReferenceController &expected) {
        EXPECT_EQ(expected.expectedOutput(), stream_output(actual));
        EXPECT_EQ(expected.expectedToString(), to_std_string(actual.toString()));
    }

    void insert_both(StudentController &actual, ReferenceController &expected,
                     const int id, const std::string &group, const std::string &name) {
        actual.insertStudent(id, MyString(group.c_str()), MyString(name.c_str()));
        expected.insertStudent(id, group, name);
    }

    void delete_both(StudentController &actual, ReferenceController &expected, const int id) {
        actual.deleteStudent(id);
        expected.deleteStudent(id);
    }
}

// ---------------------------------------------------------------------
// 기본 삽입 및 정렬 출력
// ---------------------------------------------------------------------

TEST(StudentControllerBasic, EmptyControllerOutputsHeaderOnly) {
    const StudentController controller;
    const ReferenceController expected;
    expect_same(controller, expected);
}
TEST(StudentControllerBasic, InsertSingleStudent) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20220001, "G1", "Alice");
    expect_same(controller, expected);
}

TEST(StudentControllerBasic, DuplicateStudentIdIsIgnored) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20220001, "G1", "Alice");
    insert_both(controller, expected, 20220001, "G2", "Duplicate");
    expect_same(controller, expected);
}

TEST(StudentControllerBasic, StudentsOrderedByYearAndStudentId) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20240002, "G1", "Nora");
    insert_both(controller, expected, 19980005, "G2", "Jenny");
    insert_both(controller, expected, 20220010, "G3", "Harley");
    insert_both(controller, expected, 20220001, "G4", "Liam");
    insert_both(controller, expected, 20220005, "-", "NoGroup");

    expect_same(controller, expected);
}

TEST(StudentControllerBasic, CustomCapacityConstructorWorks) {
    StudentController controller(2020, 2025, 10, 4);
    ReferenceController expected;

    insert_both(controller, expected, 20230001, "Alpha", "Kim");
    insert_both(controller, expected, 20210002, "Alpha", "Lee");
    insert_both(controller, expected, 20250003, "-", "Park");

    expect_same(controller, expected);
}

// ---------------------------------------------------------------------
// 그룹 원형 연결 및 "-" 무소속 처리
// ---------------------------------------------------------------------

TEST(StudentControllerGroup, SameGroupMembersAppearInCircularOrder) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20230001, "Alpha", "Kim");
    insert_both(controller, expected, 20230002, "Alpha", "Lee");
    insert_both(controller, expected, 20230003, "Alpha", "Park");
    insert_both(controller, expected, 20240001, "Beta", "Choi");
    insert_both(controller, expected, 20240002, "-", "Jung");

    expect_same(controller, expected);
}

TEST(StudentControllerGroup, MultipleNoGroupStudentsDoNotLinkTogether) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20220001, "-", "Alice");
    insert_both(controller, expected, 20220002, "-", "Bob");
    insert_both(controller, expected, 20230001, "-", "Charlie");

    expect_same(controller, expected);
}

// ---------------------------------------------------------------------
// 삭제 경계값
// ---------------------------------------------------------------------

TEST(StudentControllerDelete, DeleteOnEmptyOrMissingIdDoesNotCrash) {
    StudentController controller;
    ReferenceController expected;

    delete_both(controller, expected, 20220001);
    expect_same(controller, expected);

    insert_both(controller, expected, 20220001, "G1", "Alice");
    delete_both(controller, expected, 99999999);  // 없는 연도
    delete_both(controller, expected, 20229999);  // 같은 연도, 없는 학번
    expect_same(controller, expected);
}

TEST(StudentControllerDelete, DeleteHeadMiddleTailAndLastFromGroup) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20230001, "Alpha", "Kim");
    insert_both(controller, expected, 20230002, "Alpha", "Lee");
    insert_both(controller, expected, 20230003, "Alpha", "Park");
    insert_both(controller, expected, 20230004, "Alpha", "Choi");
    expect_same(controller, expected);

    // 중간 삭제
    delete_both(controller, expected, 20230002);
    expect_same(controller, expected);

    // head 삭제
    delete_both(controller, expected, 20230001);
    expect_same(controller, expected);

    // tail 삭제 (1명만 남아 [Alpha]만 출력)
    delete_both(controller, expected, 20230004);
    expect_same(controller, expected);

    // 마지막 1명 삭제
    delete_both(controller, expected, 20230003);
    expect_same(controller, expected);
}

TEST(StudentControllerDelete, DeleteNoGroupStudents) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20240001, "-", "Kim");
    insert_both(controller, expected, 20240002, "-", "Lee");
    insert_both(controller, expected, 20240003, "Alpha", "Park");

    delete_both(controller, expected, 20240001);
    expect_same(controller, expected);

    delete_both(controller, expected, 20240002);
    expect_same(controller, expected);
}

TEST(StudentControllerDelete, DeleteAllStudentsAndReinsertSameGroupAndYear) {
    StudentController controller;
    ReferenceController expected;

    insert_both(controller, expected, 20210001, "G1", "Alice");
    insert_both(controller, expected, 20210002, "G1", "Bob");
    insert_both(controller, expected, 20220001, "G2", "Charlie");

    delete_both(controller, expected, 20210001);
    delete_both(controller, expected, 20210002);
    delete_both(controller, expected, 20220001);
    expect_same(controller, expected);

    insert_both(controller, expected, 20210010, "G1", "Dave");
    insert_both(controller, expected, 20210020, "G1", "Eve");
    expect_same(controller, expected);
}

// ---------------------------------------------------------------------
// 용량 확장 (LookUpTable resize 시 포인터 안전성)
// ---------------------------------------------------------------------

TEST(StudentControllerGrowth, InsertBeyondDefaultCapacityYearsAndGroups) {
    StudentController controller;
    ReferenceController expected;

    // DEFAULT_CAPACITY(4) 를 넘도록 20개 연도 / 12개 그룹 삽입
    for (int i = 0; i < 20; ++i) {
        const int studentId = (2005 + i) * 10000 + i + 1;
        const std::string group = (i % 5 == 0) ? "-" : ("G" + std::to_string(i % 12));
        const std::string name = "Student" + std::to_string(i);
        insert_both(controller, expected, studentId, group, name);
    }
    expect_same(controller, expected);

    // 절반 삭제 후 검증
    for (int i = 0; i < 20; i += 2) {
        delete_both(controller, expected, (2005 + i) * 10000 + i + 1);
    }
    expect_same(controller, expected);
}

// ---------------------------------------------------------------------
// std::map 참조 모델과 무작위 연산 비교 (차분 테스트)
// ---------------------------------------------------------------------

TEST(StudentControllerDifferential, RandomInsertAndDeleteMatchReference) {
    std::mt19937 generator(20261120);
    const std::vector<std::string> groupPool = {"-", "Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta", "Iota"};

    StudentController actual;
    ReferenceController expected;
    std::vector<int> activeIds;

    for (int round = 0; round < 2000; ++round) {
        const int op = static_cast<int>(generator() % 10);
        std::ostringstream trace;

        if (op < 6 || activeIds.empty()) {
            // 삽입 (중복 학번 회피)
            const int year = 2015 + static_cast<int>(generator() % 12);
            const int num = 1 + static_cast<int>(generator() % 99);
            const int id = year * 10000 + num;
            if (expected.contains(id)) {
                continue;
            }
            const std::string &group = groupPool[generator() % groupPool.size()];
            const std::string name = "Name" + std::to_string(round);

            trace << "round: " << round << ", op: insert, id: " << id << ", group: " << group;
            SCOPED_TRACE(trace.str());

            insert_both(actual, expected, id, group, name);
            activeIds.push_back(id);
        } else if (op < 9) {
            // 존재하는 학번 삭제
            const std::size_t idx = generator() % activeIds.size();
            const int id = activeIds[idx];

            trace << "round: " << round << ", op: delete_existing, id: " << id;
            SCOPED_TRACE(trace.str());

            delete_both(actual, expected, id);
            activeIds[idx] = activeIds.back();
            activeIds.pop_back();
        } else {
            // 없는 학번 삭제 시도
            const int year = 2010 + static_cast<int>(generator() % 20);
            const int id = year * 10000 + 9999;

            trace << "round: " << round << ", op: delete_missing, id: " << id;
            SCOPED_TRACE(trace.str());

            delete_both(actual, expected, id);
        }

        expect_same(actual, expected);
        if (HasFailure()) {
            return;
        }
    }

    // 남은 학생 전부 삭제 후 빈 상태 확인
    for (const int id : activeIds) {
        delete_both(actual, expected, id);
    }
    expect_same(actual, expected);
}