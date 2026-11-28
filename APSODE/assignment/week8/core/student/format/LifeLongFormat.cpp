#include "LifeLongFormat.h"

const char* LifeLongFormat::tag() const {
    return TAG;
}

AdStudent* LifeLongFormat::load(std::istream &input_stream) const {
    MyString uid;
    MyString name;
    input_stream >> uid >> name;

    return read(uid, name, input_stream);
}

AdStudent* LifeLongFormat::create(std::istream &input_stream) const {
    return this->load(input_stream);
}

LifeLong* LifeLongFormat::modify(const LifeLong &original, std::istream &input_stream) {
    return read(original.uid(), original.name(), input_stream);
}

void LifeLongFormat::write(std::ostream &output_stream, const LifeLong &student) {
    output_stream << TAG << ' ' << student.uid() << ' ' << student.name() << ' '
                  << student.course() << ' ' << student.tuition() << ' ' << student.phone_number();
}

LifeLong* LifeLongFormat::read(const MyString &uid, const MyString &name, std::istream &input_stream) {
    MyString course;
    int tuition = 0;
    MyString phone_number;
    input_stream >> course >> tuition >> phone_number;

    return new LifeLong(uid,
                        name, course, phone_number,
                        tuition);
}
