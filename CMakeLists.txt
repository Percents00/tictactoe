cmake_minimum_required(VERSION 3.25)
project(lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8")

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(lab3 main.cpp Board.cpp Bot.cpp Game.cpp TictactoeUI.cpp
        DynamicArray.h
        tests/tests.cpp
        tests/tests.h)

target_link_libraries(lab3 PRIVATE Qt6::Core Qt6::Widgets)
target_compile_options(lab3 PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)