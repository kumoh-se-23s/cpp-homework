cmake_minimum_required(VERSION 3.30)

project(student)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(student
        Main.cpp
        StudentTable.cpp
        Student.cpp
        MyString.cpp
        Student.cpp
)