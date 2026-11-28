#include "StudentReader.h"
#include "../format/GiftedFormat.h"
#include "../format/LifeLongFormat.h"
#include "../format/EmployeeEduFormat.h"
#include "../format/ShortTermEduFormat.h"

namespace {
    const GiftedFormat GIFTED_FORMAT;
    const LifeLongFormat LIFE_LONG_FORMAT;
    const EmployeeEduFormat EMPLOYEE_EDU_FORMAT;
    const ShortTermEduFormat SHORT_TERM_EDU_FORMAT;
}

StudentReader::StudentReader()
    : m_formats{&GIFTED_FORMAT, &LIFE_LONG_FORMAT, &EMPLOYEE_EDU_FORMAT, &SHORT_TERM_EDU_FORMAT} {
}

AdStudent* StudentReader::load(std::istream &input) const {
    const StudentFormat *format = this->find(input);
    if (format == nullptr) {
        return nullptr;
    }

    return format->load(input);
}

AdStudent* StudentReader::create(std::istream &input) const {
    const StudentFormat *format = this->find(input);
    if (format == nullptr) {
        return nullptr;
    }

    return format->create(input);
}

const StudentFormat* StudentReader::find(std::istream &input) const {
    MyString tag;
    if (!(input >> tag)) {
        return nullptr;
    }

    for (const StudentFormat *format : this->m_formats) {
        if (tag == format->tag()) {
            return format;
        }
    }

    return nullptr;
}
