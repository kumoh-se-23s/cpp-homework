#include "StudentWriter.h"
#include "../format/GiftedFormat.h"
#include "../format/LifeLongFormat.h"
#include "../format/EmployeeEduFormat.h"
#include "../format/ShortTermEduFormat.h"

StudentWriter::StudentWriter(std::ostream &output)
    : m_output(output) {
}

void StudentWriter::visit(const Gifted &student) {
    GiftedFormat::write(this->m_output, student);
    this->m_output << '\n';
}

void StudentWriter::visit(const LifeLong &student) {
    LifeLongFormat::write(this->m_output, student);
    this->m_output << '\n';
}

void StudentWriter::visit(const EmployeeEdu &student) {
    EmployeeEduFormat::write(this->m_output, student);
    this->m_output << '\n';
}

void StudentWriter::visit(const ShortTermEdu &student) {
    ShortTermEduFormat::write(this->m_output, student);
    this->m_output << '\n';
}
