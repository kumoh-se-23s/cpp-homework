#include "StudentFactory.h"
#include "../Gifted.h"
#include "../LifeLong.h"
#include "../EmployeeEdu.h"
#include "../ShortTermEdu.h"

template<typename Student>
AdStudent* StudentFactory::read_with_uid(std::istream &input) {
    MyString uid;
    MyString name;
    input >> uid >> name;

    return new Student(uid, name, input);
}

template<typename Student>
AdStudent* StudentFactory::read_with_issued_uid(std::istream &input) {
    MyString name;
    input >> name;

    return new Student(name, input);
}

StudentFactory::StudentFactory()
    : m_makers{
        {"Gifted", read_with_uid<Gifted>, read_with_uid<Gifted>},
        {"LifeLong", read_with_uid<LifeLong>, read_with_uid<LifeLong>},
        {"EmployeeEdu", read_with_uid<EmployeeEdu>, read_with_issued_uid<EmployeeEdu>},
        {"ShortTermEdu", read_with_uid<ShortTermEdu>, read_with_issued_uid<ShortTermEdu>}
    } {
}

AdStudent* StudentFactory::load(std::istream &input) const {
    const Maker *maker = this->find(input);
    if (maker == nullptr) {
        return nullptr;
    }

    return maker->m_load(input);
}

AdStudent* StudentFactory::create(std::istream &input) const {
    const Maker *maker = this->find(input);
    if (maker == nullptr) {
        return nullptr;
    }

    return maker->m_create(input);
}

const StudentFactory::Maker* StudentFactory::find(std::istream &input) const {
    MyString type;
    if (!(input >> type)) {
        return nullptr;
    }

    for (const Maker &maker : this->m_makers) {
        if (maker.m_type == type) {
            return &maker;
        }
    }

    return nullptr;
}
