#ifndef STUDENTFILE_H
#define STUDENTFILE_H
#include <functional>
#include "../factory/StudentFactory.h"
#include "../../../base/list/List.h"

class StudentFile {
    const char *m_path;
    StudentFactory m_factory;

    public:
        explicit StudentFile(const char *path);


        void load(const std::function<void(AdStudent *)> &consumer) const;

        void save(const List<AdStudent *> &students) const;
};

#endif //STUDENTFILE_H
