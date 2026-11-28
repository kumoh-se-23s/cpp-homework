#include "GiftedFormat.h"

const char* GiftedFormat::tag() const {
    return TAG;
}

AdStudent* GiftedFormat::load(std::istream &input_stream) const {
    MyString uid;
    MyString name;
    input_stream >> uid >> name;

    return read(uid, name, input_stream);
}

AdStudent* GiftedFormat::create(std::istream &input_stream) const {
    return this->load(input_stream);
}

Gifted* GiftedFormat::modify(const Gifted &original, std::istream &input_stream) {
    return read(original.uid(), original.name(), input_stream);
}

void GiftedFormat::write(std::ostream &output_stream, const Gifted &student) {
    output_stream << TAG << ' ' << student.uid() << ' ' << student.name() << ' '
                  << student.middle_school() << ' ' << student.course() << ' ' << student.phone_number() << ' '
                  << student.parent_name() << ' ' << student.parent_phone_number();
}

Gifted* GiftedFormat::read(const MyString &uid, const MyString &name, std::istream &input_stream) {
    MyString middle_school;
    MyString course;
    MyString phone_number;
    MyString parent_name;
    MyString parent_phone_number;
    input_stream >> middle_school >> course >> phone_number >> parent_name >> parent_phone_number;

    return new Gifted(uid,
                      name, course, phone_number,
                      middle_school, parent_name, parent_phone_number);
}
