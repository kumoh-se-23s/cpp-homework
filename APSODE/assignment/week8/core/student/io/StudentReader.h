#ifndef STUDENTREADER_H
#define STUDENTREADER_H
#include <istream>
#include "../format/StudentFormat.h"

class StudentReader {
    static constexpr int FORMAT_COUNT = 4;

    const StudentFormat *m_formats[FORMAT_COUNT];

    public:
        StudentReader();


        AdStudent* load(std::istream &input) const;

        AdStudent* create(std::istream &input) const;

    private:
        const StudentFormat* find(std::istream &input) const;
};

#endif //STUDENTREADER_H
