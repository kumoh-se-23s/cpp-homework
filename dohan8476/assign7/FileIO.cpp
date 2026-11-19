#include "FileIO.h"
#include "MyString.h"
#include <fstream>

using namespace std;

static bool isWhiteSpace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static bool isAllDigits(const MyString& s) {
    if (s.isEmpty()) return false;
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

bool FileIO::loadFile(const char* filename, StudentList& list) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    int minYear = 0, maxYear = 0;
    int totalStudents = 0, totalGroups = 0;

    if (!(fin >> minYear >> maxYear) || !(fin >> totalStudents >> totalGroups)) {
        fin.close();
        return false;
    }

    // 학생이 1명 이상일 때만 연도 범위 세팅
    if (totalStudents > 0 && 1900 <= minYear && maxYear <= 2100 && minYear <= maxYear) {
        list.initYearRange(minYear, maxYear);
    }

    MyString line;
    getline(fin, line); // 두 번째 줄 끝의 개행 비우기

    //   20220987 G1 Harley Kim
    while (getline(fin, line) || !line.isEmpty()) {
        // 20220987 G1 Harley Kim 앞뒤 공백 삭제
        line.trim();
        if (line.isEmpty()) continue;

        int len = line.length();
        int pos = 0;

        // 20220987
        // 학번 추출 (pos = 8)
        while (pos < len && !isWhiteSpace(line[pos])) pos++;
        MyString idStr = line.substr(0, pos);

        // G1 Harley Kim(학번 팀 사이 공백)
        // 공백 스킵
        while (pos < len && isWhiteSpace(line[pos])) pos++;
        int teamStart = pos;

        // G1
        // 팀ID 추출(pos = 11)
        while (pos < len && !isWhiteSpace(line[pos])) pos++;
        MyString teamStr = line.substr(teamStart, pos - teamStart);

        // Harley Kim
        // 나머지 전체를 이름으로 추출 후 trim
        // pos == len이면 뒤에 이름이 없다 -> "" 반환
        MyString nameStr = (pos < len) ? line.substr(pos, len - pos) : "";
        nameStr.trim();

        if (!isAllDigits(idStr) || teamStr.isEmpty() || nameStr.isEmpty()) {
            continue;
        }

        Student std(stoi(idStr), nameStr, teamStr);
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

    // 처음에 9999 -1로 세팅해서 Student 숫자보고 저장때 0으로 초기화
    int minYear = list.getStudentCount() > 0 ? list.getMinYear() : 0;
    int maxYear = list.getStudentCount() > 0 ? list.getMaxYear() : 0;

    fout << minYear << " " << maxYear << "\n";
    fout << list.getStudentCount() << " " << list.getGroupCount() << "\n";

    if (list.getStudentCount() > 0) {
        const YearHashTable& yearTable = list.getYearTable();

        for (int year = minYear; year <= maxYear; ++year) {
            const SinglyLinkedList<StudentNode>& bucket = yearTable.getBucketByYear(year);
            StudentNode* curr = bucket.getHead();

            while (curr != nullptr) {
                const Student& s = curr->data;

                fout << s.getStudentID() << " "
                     << (s.hasTeam() ? s.getTeamID() : "-") << " "
                     << s.getStudentName() << "\n";

                curr = curr->next;
            }
        }
    }

    fout.close();
    return true;
}