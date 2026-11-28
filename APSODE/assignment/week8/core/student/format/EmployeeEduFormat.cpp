#include "EmployeeEduFormat.h"

const char* EmployeeEduFormat::tag() const {
    return TAG;
}

AdStudent* EmployeeEduFormat::load(std::istream &input_stream) const {
    MyString uid;
    MyString name;
    input_stream >> uid >> name;

    return read(uid, name, input_stream);
}

AdStudent* EmployeeEduFormat::create(std::istream &input_stream) const {
    MyString name;
    input_stream >> name;

    return read(EmployeeEdu::issue_uid(), name, input_stream);
}

EmployeeEdu* EmployeeEduFormat::modify(const EmployeeEdu &original, std::istream &input_stream) {
    return read(original.uid(), original.name(), input_stream);
}

void EmployeeEduFormat::write(std::ostream &output_stream, const EmployeeEdu &student) {
    output_stream << TAG << ' ' << student.uid() << ' ' << student.name() << ' '
                  << student.company() << ' ' << student.position() << ' ' << student.course() << ' '
                  << student.phone_number();
}

EmployeeEdu* EmployeeEduFormat::read(const MyString &uid, const MyString &name, std::istream &input_stream) {
    MyString company;
    MyString position;
    MyString course;
    MyString phone_number;
    input_stream >> company >> position >> course >> phone_number;

    return new EmployeeEdu(uid,
                           name, course, phone_number,
                           company, position);
}
