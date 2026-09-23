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

add_executable(assignment2_1 assignment2/1.printBinary.cpp)
add_executable(assignment2_2 assignment2/2.3digitMultiplication.cpp)
add_executable(assignment2_3 assignment2/3.findCharPosition.cpp)
add_executable(assignment2_4 assignment2/4.mainddddddddddddddd.cpp)
add_executable(assignment2_5 assignment2/5.maindddddddddddd.cpp)
add_executable(assignment2_6 assignment2/6.mainddddddddddddddddd.cpp)
add_executable(assignment2_7 assignment2/7.mergeArray/main.cpp assignment2/7.mergeArray/MyArray.cpp)
add_executable(assignment2_8 assignment2/8.runMatrix/main.cpp assignment2/8.runMatrix/Matrix.cpp)
add_executable(assignment2_9 assignment2/9.runFraction/main.cpp assignment2/9.runFraction/Fraction.cpp)