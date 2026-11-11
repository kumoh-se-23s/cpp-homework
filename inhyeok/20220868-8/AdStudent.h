//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include "MyString.h"

struct EmployeeEdu;
struct Gifted;
struct ShortTermEdu;
struct LifeLong;


struct AdStudent {
    MyString key;
    MyString name;
    MyString phone;
    MyString course;

    virtual ~AdStudent() = default;

    std::partial_ordering operator<=>(const AdStudent &v) const {
        const std::partial_ordering a1 = name <=> v.name;
        if (a1 != 0) return a1;
        return phone <=> v.phone;
    }

    virtual void input() = 0;

    [[nodiscard]] virtual MyString toString() const = 0;

    [[nodiscard]] virtual MyString toFileFormattedString() const = 0;

};


template<typename Child> requires std::is_base_of_v<AdStudent, Child>
[[nodiscard]] std::ostream &operator<<(std::ostream &out, const Child &v) {
    return out << v.toString();
}
