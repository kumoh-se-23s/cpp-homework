#include "ShortTermEduFormat.h"

const char* ShortTermEduFormat::tag() const {
    return TAG;
}

AdStudent* ShortTermEduFormat::load(std::istream &input_stream) const {
    MyString uid;
    MyString name;
    input_stream >> uid >> name;

    return read(uid, name, input_stream);
}

AdStudent* ShortTermEduFormat::create(std::istream &input_stream) const {
    MyString name;
    input_stream >> name;

    return read(ShortTermEdu::issue_uid(), name, input_stream);
}

ShortTermEdu* ShortTermEduFormat::modify(const ShortTermEdu &original, std::istream &input_stream) {
    return read(original.uid(), original.name(), input_stream);
}

void ShortTermEduFormat::write(std::ostream &output_stream, const ShortTermEdu &student) {
    output_stream << TAG << ' ' << student.uid() << ' ' << student.name() << ' '
                  << student.course() << ' ' << student.host_depart() << ' ' << student.phone_number();
}

ShortTermEdu* ShortTermEduFormat::read(const MyString &uid, const MyString &name, std::istream &input_stream) {
    MyString course;
    MyString host_depart;
    MyString phone_number;
    input_stream >> course >> host_depart >> phone_number;

    return new ShortTermEdu(uid,
                            name, course, phone_number,
                            host_depart);
}
