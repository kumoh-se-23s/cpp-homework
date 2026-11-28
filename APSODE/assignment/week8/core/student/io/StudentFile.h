#ifndef STUDENTFILE_H
#define STUDENTFILE_H
#include <functional>
#include "../AdStudent.h"
#include "../../../base/list/List.h"

class StudentFile {
    const char *m_path;

    public:
        explicit StudentFile(const char *path);


        void load(const std::function<void(AdStudent *)> &consumer) const;

        void save(const List<AdStudent *> &students) const;
};

#endif //STUDENTFILE_H
