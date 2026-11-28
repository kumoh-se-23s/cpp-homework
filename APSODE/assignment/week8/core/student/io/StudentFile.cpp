#include "StudentFile.h"
#include <fstream>

StudentFile::StudentFile(const char *path)
    : m_path(path) {
}

void StudentFile::load(const std::function<void(AdStudent *)> &consumer) const {
    std::ifstream input(this->m_path);

    for (AdStudent *student = this->m_factory.load(input); student; student = this->m_factory.load(input)) {
        consumer(student);
    }
}

void StudentFile::save(const List<AdStudent *> &students) const {
    std::ofstream output(this->m_path);

    students.for_each([&output](AdStudent *const &student) {
        student->write(output);
        output << '\n';
    });
}
