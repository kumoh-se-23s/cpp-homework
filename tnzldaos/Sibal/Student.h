#pragma once
#include <compare>
#include <string>
using namespace std;


class Student {
    string id;
    string teamName;
    string name;

public:
    Student() = default;

    ~Student() = default;

    Student(const string& id, const string& teamName, const string& name) : id(id), teamName(teamName), name(name) {}

    // getter
    string getId() const;
    string getName() const;
    string getTeamName() const;

    // setter
    void setId(const string& id);
    void setName(const string& name);
    void setTeamName(const string& teamName);

    // 비교 연산자 (학번 기준)
    strong_ordering operator<=>(const Student& student) const;

    bool operator==(const Student& student) const;
};