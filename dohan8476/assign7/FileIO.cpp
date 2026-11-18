#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool FileIO::loadFile(const char* filename, StudentList& list) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    int minYear = 0, maxYear = 0;
    int totalStudents = 0, totalGroups = 0;

    // 1. 첫 두 줄 메타데이터 읽기
    if (!(fin >> minYear >> maxYear)) {
        fin.close();
        return false;
    }
    if (!(fin >> totalStudents >> totalGroups)) {
        fin.close();
        return false;
    }

    if (minYear > 0 && maxYear >= minYear) {
        list.initYearRange(minYear, maxYear);
    }

    string line;
    getline(fin, line); // 두 번째 줄 끝의 개행 문자 비우기

    // 2. 학생 데이터 파싱 (형식: 학번 팀ID 이름(공백 포함 가능))
    while (getline(fin, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Windows 개행(\r) 제거
        }
        if (line.empty()) continue;

        istringstream iss(line);
        int id;
        string teamStr;

        if (!(iss >> id >> teamStr)) {
            continue;
        }

        // 팀ID가 "-"이면 팀 없음("")으로 처리
        if (teamStr == "-") {
            teamStr = "";
        }

        // 팀ID 뒤의 공백을 건너뛰고 줄 끝까지 전체 이름 읽기 (예: "Harley Kim")
        string nameStr;
        getline(iss >> ws, nameStr);

        // 이름 뒤 불필요한 공백 제거
        while (!nameStr.empty() && (nameStr.back() == ' ' || nameStr.back() == '\t')) {
            nameStr.pop_back();
        }

        Student std(id, MyString(nameStr.c_str()), MyString(teamStr.c_str()));
        list.addStudent(std);
    }

    fin.close();
    return true;
}

bool FileIO::saveFile(const char* filename, const StudentList& list) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        return false;
    }

    int minY = list.getMinYear();
    int maxY = list.getMaxYear();

    // 1,2 번쨰 line
    fout << minY << " " << maxY << "\n";
    fout << list.getStudentCount() << " " << list.getGroupCount() << "\n";

    const YearHashTable& yearTable = list.getYearTable();
    for (int y = minY; y <= maxY; ++y) {
        const SinglyLinkedList<StudentNode>& bucket = yearTable.getBucketByYear(y);
        StudentNode* curr = bucket.getHead(); // ※ SinglyLinkedList 헤드 접근 함수명 확인
        while (curr != nullptr) {
            const Student& s = curr->data;
            fout << s.getStudentID() << " "
                 << (s.hasTeam() ? s.getTeamID() : "-") << " "
                 << s.getStudentName() << "\n";
            curr = curr->next;
        }
    }

    fout.close();
    return true;
}