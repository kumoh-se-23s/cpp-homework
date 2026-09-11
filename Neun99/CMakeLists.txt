cmake_minimum_required(VERSION 4.3)
project(Neun99_homework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(practice_ground practice_ground/testzone.cpp)

add_executable(assignment0 assignment0/main.cpp)

add_executable(assignment1_1 assignment1/1.floatDivide.cpp)
add_executable(assignment1_2 assignment1/2.findMSD.cpp)
add_executable(assignment1_3 assignment1/3.printHexagon.cpp)
add_executable(assignment1_4 assignment1/4.printTimesTable.cpp)
add_executable(assignment1_5 assignment1/5.printCompleteNum.cpp)
add_executable(assignment1_6 assignment1/6.learnRecursive.cpp)
add_executable(assignment1_7 assignment1/7.randGuess.cpp)
add_executable(assignment1_8 assignment1/8.getFactorial.cpp)

