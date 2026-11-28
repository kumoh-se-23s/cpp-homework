#include "StudentFile.h"
#include <fstream>
#include "StudentReader.h"
#include "StudentWriter.h"

StudentFile::StudentFile(const char *path)
    : m_path(path) {
}

void StudentFile::load(const std::function<void(AdStudent *)> &consumer) const {
    std::ifstream input(this->m_path);
    StudentReader reader;

    for (AdStudent *student = reader.load(input); student; student = reader.load(input)) {
        consumer(student);
    }
}

void StudentFile::save(const List<AdStudent *> &students) const {
    std::ofstream output(this->m_path);
    StudentWriter writer(output);

    students.for_each([&writer](AdStudent *const &student) {
        student->accept(writer);
    });
}
