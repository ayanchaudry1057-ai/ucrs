cmake_minimum_required(VERSION 3.16)
project(UCRS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Sql REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(UCRS
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/Person.h
    src/Person.cpp
    src/Student.h
    src/Student.cpp
    src/Teacher.h
    src/Teacher.cpp
    src/Course.h
    src/Course.cpp
    src/Database.h
    src/Database.cpp
    src/FileHandler.h
    src/FileHandler.cpp
    src/RegistrationSystem.h
    src/RegistrationSystem.cpp
    src/Exceptions.h
)

target_link_libraries(UCRS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Sql
    SQLite3::SQLite3
)

target_include_directories(UCRS PRIVATE src)
