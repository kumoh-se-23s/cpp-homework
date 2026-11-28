#include "StudentModifier.h"
#include "GiftedFormat.h"
#include "LifeLongFormat.h"
#include "EmployeeEduFormat.h"
#include "ShortTermEduFormat.h"

StudentModifier::StudentModifier(std::istream &input_stream)
    : m_input_stream(input_stream), m_modified(nullptr) {
}

void StudentModifier::visit(const Gifted &student) {
    this->m_modified = GiftedFormat::modify(student, this->m_input_stream);
}

void StudentModifier::visit(const LifeLong &student) {
    this->m_modified = LifeLongFormat::modify(student, this->m_input_stream);
}

void StudentModifier::visit(const EmployeeEdu &student) {
    this->m_modified = EmployeeEduFormat::modify(student, this->m_input_stream);
}

void StudentModifier::visit(const ShortTermEdu &student) {
    this->m_modified = ShortTermEduFormat::modify(student, this->m_input_stream);
}

AdStudent* StudentModifier::modified() const {
    return this->m_modified;
}
