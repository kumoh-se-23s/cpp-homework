#include "ManageCircleApp.h"

#include <iostream>

#include "core/Student.h"

ManageCircleApp::ManageCircleApp(const char *file_path)
    : m_file_path(file_path), m_is_running(true), m_registry(nullptr) {
}

ManageCircleApp::~ManageCircleApp() {
    if (this->m_registry != nullptr) {
        delete this->m_registry;
    }
}

bool ManageCircleApp::run() {
    this->m_registry = StudentRegistry::load(this->m_file_path);

    if (this->m_registry == nullptr) {
        std::cerr << "Cannot load file: " << this->m_file_path << '\n';
        return false;
    }

    MyString command;

    while (this->m_is_running && this->input_command(command)) {
        bool is_success = true;

        switch (resolve_command(command)) {
            case 'L':
                is_success = this->list();
                break;
            case 'I':
                is_success = this->insert();
                break;
            case 'D':
                is_success = this->remove();
                break;
            case 'Q':
                is_success = this->quit();
                break;
            default:
                this->print_error("Unknown command: ", command);
                break;
        }

        if (!is_success) {
            break;
        }
    }

    return true;
}

char ManageCircleApp::resolve_command(const MyString &command) {
    return command.length() == 1 ? command[0] : '?';
}

bool ManageCircleApp::input_command(MyString &command) {
    if (!(std::cin >> command)) {
        this->m_is_running = false;
        return false;
    }

    return true;
}

void ManageCircleApp::print_error(const char *message, const MyString &detail) {
    std::cout << message << detail << '\n';
}

bool ManageCircleApp::list() const {
    this->m_registry->print(std::cout);

    return true;
}

bool ManageCircleApp::insert() const {
    Student student;

    if (!StudentRegistry::read_student(std::cin, student)) {
        return false;
    }

    if (!this->m_registry->add(student)) {
        this->print_error("Already exists: ", student.uid());
    }

    return true;
}

bool ManageCircleApp::remove() const {
    MyString uid;

    if (!(std::cin >> uid)) {
        return false;
    }

    if (!this->m_registry->remove(uid)) {
        this->print_error("Not found: ", uid);
    }

    return true;
}

bool ManageCircleApp::quit() {
    this->m_is_running = false;

    if (!this->m_registry->save(this->m_file_path)) {
        std::cerr << "Cannot save file: " << this->m_file_path << '\n';
    }

    return true;
}
