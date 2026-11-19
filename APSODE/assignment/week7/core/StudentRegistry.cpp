#include <cctype>
#include <fstream>

#include "StudentRegistry.h"

StudentRegistry::StudentRegistry(int min_year, int max_year)
    : m_list(min_year, max_year) {
}

StudentRegistry* StudentRegistry::load(const char *path) {
    std::ifstream input_stream(path);

    if (!input_stream.is_open()) {
        return nullptr;
    }

    int min_year;
    int max_year;
    int student_count;
    int team_count;

    if (!(input_stream >> min_year >> max_year >> student_count >> team_count)) {
        return nullptr;
    }

    StudentRegistry *registry = new StudentRegistry(min_year, max_year);

    Student student;

    for (int count = 0; count < student_count && StudentRegistry::read_student(input_stream, student); ++count) {
        registry->add(student);
    }

    return registry;
}

bool StudentRegistry::save(const char *path) const {
    std::ofstream output_stream(path);

    if (!output_stream.is_open()) {
        return false;
    }

    output_stream << this->m_list.min_row_key() << ' ' << this->m_list.max_row_key() << '\n';
    output_stream << this->student_count() << ' ' << this->team_count() << '\n';

    this->m_list.for_each([&output_stream](const Student &student) {
        output_stream << student.uid() << ' ' << student.gid() << ' ' << student.name() << '\n';
    });

    return true;
}

bool StudentRegistry::read_student(std::istream &input_stream, Student &student) {
    MyString uid;
    MyString gid;
    MyString name;

    if (!(input_stream >> uid >> gid)) {
        return false;
    }

    getline(input_stream, name);
    StudentRegistry::trim(name);

    student = Student(name, uid, gid);

    return true;
}

bool StudentRegistry::add(const Student &student) {
    return this->m_list.insert(student);
}

bool StudentRegistry::remove(const MyString &uid) {
    return this->m_list.remove(Student(MyString(), uid, MyString()));
}

void StudentRegistry::print(std::ostream &output_stream) const {
    output_stream << "[Year:" << this->m_list.min_row_key() << '~' << this->m_list.max_row_key() << "] "
            << "[Student Count:" << this->student_count() << "] "
            << "[Group Count: " << this->team_count() << "]\n";

    this->m_list.for_each([this, &output_stream](const Student &student) {
        this->print_line(output_stream, student);
    });
}

int StudentRegistry::student_count() const {
    return this->m_list.size();
}

int StudentRegistry::team_count() const {
    return this->m_list.column_count();
}

void StudentRegistry::print_line(std::ostream &output_stream, const Student &student) const {
    output_stream << student.uid() << ' ' << student.name() << " (";

    if (StudentTraits::has_column(student)) {
        output_stream << '[' << student.gid() << ']';

        bool first = true;

        this->m_list.for_each(student, [&output_stream, &first](const Student &mate) {
            output_stream << (first ? " " : " - ") << mate.uid() << ' ' << mate.name();
            first = false;
        });
    }

    output_stream << ")\n";
}

void StudentRegistry::trim(MyString &string) {
    while (!string.isEmpty() && std::isspace(static_cast<unsigned char>(string[0]))) {
        string.erase(0, 1);
    }

    while (!string.isEmpty() && std::isspace(static_cast<unsigned char>(string[string.length() - 1]))) {
        string.erase(string.length() - 1, 1);
    }
}
