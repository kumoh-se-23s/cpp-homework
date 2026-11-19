#ifndef CPP_HOMEWORK_WEEK7_MANAGECIRCLEAPP_H
#define CPP_HOMEWORK_WEEK7_MANAGECIRCLEAPP_H

#include "core/StudentRegistry.h"
#include "base/MyString.h"

class ManageCircleApp {
    const char *m_file_path;
    bool m_is_running;
    StudentRegistry *m_registry;

    public:
        explicit ManageCircleApp(const char *file_path);

        ~ManageCircleApp();

        ManageCircleApp(const ManageCircleApp &other) = delete;

        ManageCircleApp& operator=(const ManageCircleApp &other) = delete;


        bool run();

    private:
        static char resolve_command(const MyString &command);

        bool input_command(MyString &command);

        static void print_error(const char *message, const MyString &detail);

        bool list() const;

        bool insert() const;

        bool remove() const;

        bool quit();
};

#endif //CPP_HOMEWORK_WEEK7_MANAGECIRCLEAPP_H
