#include <string>

#include "Comparable.h"

using namespace std;

class Student {
    string studentId;
    string name;
    string teamNum;

    string getAdmissionYear() const;

public:
    Student();

    Student(const string &studentId, const string &name, const string &teamNum);

    string getStudentId() const;

    void setStudentId(const string &studentId);

    string getName() const;

    void setName(const string &name);

    string getTeamNum() const;

    void setTeamNum(const string &teamNum);

    auto operator<=>(const Student &student) const {
        if (studentId > student.studentId) {
            return 1;
        }

        if (studentId < student.studentId) {
            return -1;
        }

        return 0;
    }

    bool operator==(const Student &student) const;

    bool operator==(const string &studentId) const;
};
